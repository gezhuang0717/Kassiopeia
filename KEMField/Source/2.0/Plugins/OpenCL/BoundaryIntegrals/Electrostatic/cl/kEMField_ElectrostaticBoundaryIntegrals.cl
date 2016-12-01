#ifndef KEMFIELD_ELECTROSTATICBOUNDARYINTEGRALS_CL
#define KEMFIELD_ELECTROSTATICBOUNDARYINTEGRALS_CL

#include "kEMField_defines.h"

#include "kEMField_BoundaryIntegrals.cl"
#include "kEMField_ElectrostaticRectangle.cl"
#include "kEMField_ElectrostaticTriangle.cl"
#include "kEMField_ElectrostaticLineSegment.cl"
#include "kEMField_ElectrostaticConicSection.cl"

//______________________________________________________________________________

CL_TYPE EBI_Potential(const CL_TYPE* P,
		     __global const short* shapeType,
		     __global const CL_TYPE* data)
{
#ifdef TRIANGLE
  if (shapeType[0] == TRIANGLE)
    return ET_Potential(P,data);
#else
  if (false) {}
#endif
#ifdef RECTANGLE
  else if (shapeType[0] == RECTANGLE)
    return ER_Potential(P,data);
#endif
#ifdef LINESEGMENT
  else if (shapeType[0] == LINESEGMENT)
    return EL_Potential(P,data);
#endif
#ifdef CONICSECTION
  else if (shapeType[0] == CONICSECTION)
    return EC_Potential(P,data);
#endif

  return 0.;
}

//______________________________________________________________________________

CL_TYPE4 EBI_EField(const CL_TYPE* P,
		   __global const short* shapeType,
		   __global const CL_TYPE* data)
{
#ifdef TRIANGLE
  if (shapeType[0] == TRIANGLE)
    return ET_EField(P,data);
#else
  if (false) {}
#endif
#ifdef RECTANGLE
  else if (shapeType[0] == RECTANGLE)
    return ER_EField(P,data);
#endif
#ifdef LINESEGMENT
  else if (shapeType[0] == LINESEGMENT)
    return EL_EField(P,data);
#endif
#ifdef CONICSECTION
  else if (shapeType[0] == CONICSECTION)
    return EC_EField(P,data);
#endif

    return (CL_TYPE4)(0.,0.,0.,0.);
}

//______________________________________________________________________________

CL_TYPE BI_BoundaryIntegral(int iBoundary,
			    __global const int* boundaryInfo,
			    __global const CL_TYPE* boundaryData,
			    __global const short* shapeType_target,
			    __global const short* shapeType_source,
			    __global const CL_TYPE* data_target,
			    __global const CL_TYPE* data_source)
{
  CL_TYPE P_target[3];
  BI_Centroid(&P_target[0],shapeType_target,data_target);

  CL_TYPE4 eField = (CL_TYPE4)(0.,0.,0.,0.);
  CL_TYPE P_source[3];
  CL_TYPE N_target[3];
  CL_TYPE val;
  CL_TYPE dist2;

#ifdef DIRICHLETBOUNDARY
  if (BI_GetBoundaryType(iBoundary,boundaryInfo) == DIRICHLETBOUNDARY)
  {
    val = EBI_Potential(P_target,shapeType_source,data_source);
  }
#else
  if (false)
  {

  }
#endif
#ifdef NEUMANNBOUNDARY
  else // NEUMANN
  {
    eField = EBI_EField(P_target,shapeType_source,data_source);
    BI_Centroid(&P_source[0],shapeType_source,data_source);
    BI_Normal(&N_target[0],shapeType_target,data_target);

    val = eField.x*N_target[0] + eField.y*N_target[1] + eField.z*N_target[2];
    dist2 = ((P_target[0]-P_source[0])*(P_target[0]-P_source[0]) +
	     (P_target[1]-P_source[1])*(P_target[1]-P_source[1]) +
	     (P_target[2]-P_source[2])*(P_target[2]-P_source[2]));
    if (dist2<1.e-24)
    {
      val = val*((1. + boundaryData[iBoundary*BOUNDARYSIZE])/
		 (1. - boundaryData[iBoundary*BOUNDARYSIZE]));
    }
  }
#endif
  return val;
}

#endif /* KEMFIELD_ELECTROSTATICBOUNDARYINTEGRALS_CL */