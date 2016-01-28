#ifdef KEMFIELD_USE_CL_VECTOR
    #define __NO_STD_VECTOR // Use cl::vector instead of STL version
    #define CL_VECTOR_TYPE cl::vector
#else
    #define CL_VECTOR_TYPE std::vector
#endif

#define __CL_ENABLE_EXCEPTIONS
#if defined __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif
#include <utility>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <limits>
#include <exception>

#define DATA_SIZE (10240)

const char *KernelSource = "\n" \
  "#ifdef AMD                                                              \n" \
  "#pragma OPENCL EXTENSION cl_amd_fp64 : enable                           \n" \
  "#else                                                                   \n" \
  "#pragma OPENCL EXTENSION cl_khr_fp64 : enable                           \n" \
  "#endif /* AMD */                                                        \n" \
  "                                                                        \n" \
  "double ArcSinh(double x)                                                \n" \
  "{                                                                       \n" \
  "  return log(x + sqrt(1. + x*x));                                       \n" \
  "}                                                                       \n" \
  "                                                                        \n" \
  "__kernel void vector_ArcSinh(                                           \n" \
  "   __global double* input,                                              \n" \
  "   __global double* output,                                             \n" \
  "   __global int* count)                                                 \n" \
  "{                                                                       \n" \
  "   int i = get_global_id(0);                                            \n" \
  "   if (i<count[0])                                                      \n" \
  "     output[i] = ArcSinh(input[i]);                                     \n" \
  "}                                                                       \n" \
  "\n";

const char *KernelSource_f = "\n" \
  "                                                                       \n" \
  "__kernel void vector_ArcSinh(                                          \n" \
  "   __global float* input,                                              \n" \
  "   __global float* output,                                             \n" \
  "   __global int* count)                                                \n" \
  "{                                                                      \n" \
  "   int i = get_global_id(0);                                           \n" \
  "   if(i < count[0])                                                    \n" \
  "       output[i] = 1./fabs(input[i]);                                  \n" \
  "}                                                                      \n" \
  "\n";

int main()
{
  // ifdef-guard header
  std::cout<<"#ifndef KEMFIELD_DEFINES_H"<<std::endl;
  std::cout<<"#define KEMFIELD_DEFINES_H"<<std::endl;
  std::cout<<""<<std::endl;
  std::cout<<"/* NOTE: this file has been autogenerated by the program \"GenerateOpenCLHeader\""<<std::endl;
  std::cout<<"         at build time.  Any changes to this file will be erased when KEMField"<<std::endl;
  std::cout<<"         is re-installed!                                                    */"<<std::endl;
  std::cout<<""<<std::endl;

  // Set up list of desired extensions
  std::vector<std::string> desired_extensions;
  desired_extensions.push_back(((std::string)"cl_khr_fp64"));
  desired_extensions.push_back(((std::string)"cl_amd_fp64"));

  // Create flag for double precision support
  bool double_precision = false;

  // Default GPU to use (if more than one are present)
  unsigned int defaultDeviceID = 0;

  // Get available platforms
  CL_VECTOR_TYPE<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  std::cout<<"/* Generated using platform #"<<KEMFIELD_OPENCL_PLATFORM<<" out of "<<platforms.size()<<" platforms. */"<<std::endl;
  std::cout<<std::endl;

  // Select the default platform and create a context using this platform and the GPU
  cl_context_properties cps[3] = {
    CL_CONTEXT_PLATFORM,
    (cl_context_properties)(platforms[KEMFIELD_OPENCL_PLATFORM])(),
    0
  };

  unsigned int deviceType = KEMFIELD_OPENCL_DEVICE_TYPE;

  cl::Context* context = NULL;

  if(deviceType == 0) //we have a GPU
  {
    context = new cl::Context( CL_DEVICE_TYPE_GPU, cps);
  }

  if(deviceType == 1) //we have a CPU
  {
    context = new cl::Context( CL_DEVICE_TYPE_CPU, cps);
  }

  if(deviceType == 2) //we have an accelerator device
  {
    context = new cl::Context( CL_DEVICE_TYPE_ACCELERATOR, cps);
  }

  // Get a list of devices on this platform
  CL_VECTOR_TYPE<cl::Device> devices = context->getInfo<CL_CONTEXT_DEVICES>();

  std::cout<<"/*"<<std::endl;
  std::cout<<" Number of available platforms on this machine: "<<platforms.size()<<std::endl;
  std::cout<<" Available devices on platform #"<<KEMFIELD_OPENCL_PLATFORM<<std::endl;
  for (unsigned int i=0;i<devices.size();i++)
  {
    std::cout<<" Device # "<<i<<std::endl;
    std::cout<<"   Name:           "<<devices[i].getInfo<CL_DEVICE_NAME>()<<std::endl;
    std::cout<<"   Vendor:         "<<devices[i].getInfo<CL_DEVICE_VENDOR>()<<std::endl;
    std::cout<<"   Profile:        "<<devices[i].getInfo<CL_DEVICE_PROFILE>()<<std::endl;
    std::cout<<"   Version:        "<<devices[i].getInfo<CL_DEVICE_VERSION>()<<std::endl;
    std::cout<<"   Driver Version: "<<devices[i].getInfo<CL_DRIVER_VERSION>()<<std::endl;
    // std::cout<<"   "<<devices[i].getInfo<CL_DEVICE_OPENCL_C_VERSION>()<<std::endl;
    std::cout<<"   Extensions:     "<<devices[i].getInfo<CL_DEVICE_EXTENSIONS>()<<std::endl;
    std::cout<<""<<std::endl;
  }
  std::cout<<"                                                                           */"<<std::endl;

  std::stringstream options;

  for (unsigned int i=0;i<devices.size();i++)
  {
    std::string device_ext_list = devices[i].getInfo<CL_DEVICE_EXTENSIONS>();

    std::istringstream iss(device_ext_list);
    std::vector<std::string> device_extensions;
    std::copy(std::istream_iterator<std::string>(iss),
             std::istream_iterator<std::string>(),
             std::back_inserter<std::vector<std::string> >(device_extensions) );

    for (unsigned int j=0;j<device_extensions.size();j++)
    {
      for (unsigned int k=0;k<desired_extensions.size();k++)
      {
        if (device_extensions[j] == desired_extensions[k])
        {
          std::cout<<"#pragma OPENCL EXTENSION "
              <<device_extensions[j]
              <<" : enable"<<std::endl;
          desired_extensions[k] = "NULL";
          if (k==0 || k==1)
          {
            if (k==1)
              options << "-DAMD";
            double_precision = true;
            defaultDeviceID = i;
          }
        }
      }
    }
  }

  // Create a new devices vector with only the default one in it
  cl::Device defaultDevice = devices[defaultDeviceID];
  devices.clear();
  devices.push_back(defaultDevice);

  if (double_precision)
  {
    std::cout<<"#define CL_TYPE double"<<std::endl;
    std::cout<<"#define CL_TYPE2 double2"<<std::endl;
    std::cout<<"#define CL_TYPE4 double4"<<std::endl;
    std::cout<<"#define CL_TYPE8 double8"<<std::endl;
    std::cout<<"#define CL_TYPE16 double16"<<std::endl;
  }
  else
  {
    std::cout<<"#define CL_TYPE float"<<std::endl;
    std::cout<<"#define CL_TYPE2 float2"<<std::endl;
    std::cout<<"#define CL_TYPE4 float4"<<std::endl;
    std::cout<<"#define CL_TYPE8 float8"<<std::endl;
    std::cout<<"#define CL_TYPE16 float16"<<std::endl;
  }

#ifndef KEMFIELD_OPENCL_CPU
  if (!double_precision && platforms[0].getInfo<CL_PLATFORM_NAME>()=="Apple")
  {
    // we must recast sqrt() to take only floats
    std::cout<<"#define SQRT(x) sqrt((float)(x))"<<std::endl;
    std::cout<<"#define LOG(x) log((float)(x))"<<std::endl;
  }
  else
#endif
  {
    std::cout<<"#define SQRT(x) sqrt(x)"<<std::endl;
    std::cout<<"#define LOG(x) log(x)"<<std::endl;
  }

  int* count = new int[1];
  count[0] = DATA_SIZE;

  double arcSinhInputs[DATA_SIZE];
  double arcSinhOutputs[DATA_SIZE];
  float arcSinhInputs_f[DATA_SIZE];
  float arcSinhOutputs_f[DATA_SIZE];

  for (unsigned int i=0;i<DATA_SIZE;i++)
  {
    arcSinhInputs[i]   = -pow(2,i);
    arcSinhInputs_f[i] = exp((double)i);
  }

  // Create a command queue and use the first device
  cl::CommandQueue queue = cl::CommandQueue(*context, devices[0]);

  // Source file
  std::string sourceCode;
  if (double_precision)
    sourceCode = KernelSource;
  else
    sourceCode = KernelSource_f;

  cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(),
						sourceCode.length()+1));

  // Make program of the source code in the context
  cl::Program program = cl::Program(*context, source);

  // Build program for these specific devices
  try
  {
    program.build(devices, options.str().c_str());
  }
  catch (cl::Error error)
  {
    std::cerr<<"There was an error compiling the kernels.  Here is the information from the OpenCL C++ API:"<<std::endl;
    std::cerr<<error.what()<<"("<<error.err()<<")"<<std::endl;
    std::cerr<<"Build Status: "<<program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(devices[0])<<std::endl;
    std::cerr<<"Build Options:\t"<<program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(devices[0])<<std::endl;
    std::cerr<<"Build Log:\t "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
    return 0;
  }

  // Make kernel
  cl::Kernel kernel(program, "vector_ArcSinh");

  // Create memory buffers
  cl::Buffer* bufferIn;
  cl::Buffer* bufferOut;
  if (double_precision)
  {
    bufferIn  = new cl::Buffer(*context, CL_MEM_READ_ONLY,DATA_SIZE*sizeof(double));
    bufferOut = new cl::Buffer(*context, CL_MEM_WRITE_ONLY,DATA_SIZE*sizeof(double));
  }
  else
  {
    bufferIn  = new cl::Buffer(*context, CL_MEM_READ_ONLY,DATA_SIZE*sizeof(cl_mem));
    bufferOut = new cl::Buffer(*context, CL_MEM_WRITE_ONLY,DATA_SIZE*sizeof(cl_mem));
  }

  cl::Buffer bufferCount  = cl::Buffer(*context, CL_MEM_READ_ONLY,sizeof(int));

  // Copy in buffer to the memory buffer
  if (double_precision)
    queue.enqueueWriteBuffer(*bufferIn,CL_TRUE,0,DATA_SIZE*sizeof(double),arcSinhInputs);
  else
    queue.enqueueWriteBuffer(*bufferIn,CL_TRUE,0,DATA_SIZE*sizeof(cl_mem),arcSinhInputs_f);

  queue.enqueueWriteBuffer(bufferCount,CL_TRUE,0,sizeof(int),count);

  // Set arguments to kernel
  kernel.setArg(0, *bufferIn);
  kernel.setArg(1, *bufferOut);
  kernel.setArg(2, bufferCount);

  // Run the kernel on specific ND range
  cl::NDRange global(DATA_SIZE);
  cl::NDRange local(1);
  queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

  // Read out buffer into a local list
  if (double_precision)
    queue.enqueueReadBuffer(*bufferOut,CL_TRUE,0,DATA_SIZE*sizeof(double),arcSinhOutputs);
  else
  {
    queue.enqueueReadBuffer(*bufferOut,CL_TRUE,0,DATA_SIZE*sizeof(cl_mem),arcSinhOutputs_f);
    for(int i=0;i<DATA_SIZE;i++)
      arcSinhOutputs[i] = arcSinhOutputs_f[i];
  }

  // we still don't need this type of functionality yet, but eventually it would
  // be great to experimentally query the card for its limits for certain
  // math functions, and insert them into the OpenCL code
  for (unsigned int i=0;i<DATA_SIZE;i++)
    if (arcSinhOutputs[i]!=arcSinhOutputs[i] ||
    	fabs(arcSinhOutputs[i]) == std::numeric_limits<double>::infinity() ||
	arcSinhOutputs[i]==0.)
    {
      // std::cout<<"#define MAX_ASINH_ARG "<<arcSinhInputs[i-1]<<std::endl;
      break;
    }

  queue.finish();

  delete bufferIn;
  delete bufferOut;

  // complete ifdef-guard header
  std::cout<<""<<std::endl;
  std::cout<<"#endif /* KEMFIELD_DEFINES_H */"<<std::endl;

  delete context;

  if (double_precision)
    return defaultDeviceID;
  else
    return 255;
}
