#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cmath>

#include "KFMPoint.hh"
#include "KFMPointCloud.hh"

#include "KEMThreeVector.hh"

#include "KVMPathIntegral.hh"
#include "KVMLineIntegral.hh"
#include "KVMSurfaceIntegral.hh"
#include "KVMFluxIntegral.hh"

#include "KVMField.hh"
#include "KVMFieldWrapper.hh"

#include "KVMLineSegment.hh"
#include "KVMTriangularSurface.hh"
#include "KVMRectangularSurface.hh"

#include "KFMElectrostaticMultipoleCalculatorAnalytic.hh"
#include "KFMElectrostaticMultipoleCalculatorNumeric.hh"


#include <cstdlib>
#include <iomanip>

using namespace KEMField;


int main()
{
    //std::cout << std::setprecision(15);

    int degree = 3;
    unsigned int size = (degree + 1)*(degree + 1);

    //now lets make the multipole calculators
    KFMElectrostaticMultipoleCalculatorAnalytic* aCalc = new KFMElectrostaticMultipoleCalculatorAnalytic();
    aCalc->SetDegree(degree);


    KEMThreeVector origin1(0.,0.,0.);
    KEMThreeVector origin2(1.32, 0.434231, 0.34534);
    KEMThreeVector point(1.4, 2.1454, 1.343432);
    KEMThreeVector del1 = point - origin1;
    KEMThreeVector del2 = point - origin2;
    KEMThreeVector del_origin = origin2 - origin1;
    KEMThreeVector del_origin_par = origin1 - origin2;

    std::vector< std::complex<double> > source;
    std::vector< std::complex<double> > source1;
    std::vector< std::complex<double> > source2;
    std::vector< std::complex<double> > target;
    std::vector< std::complex<double> > true_value;

    source.resize(size);
    source1.resize(size);
    source2.resize(size);
    target.resize(size);
    true_value.resize(size);

    int psi, nsi;
    // double real;
    // double imag;
    for(int n=0; n<=degree; n++)
    {
        for(int m=0; m<=n; m++)
        {
            psi = n*(n+1) + m;
            nsi = n*(n+1) - m;

            source[psi] = KFMMath::RegularSolidHarmonic_Cart(n,m,del1);
            source[nsi] = std::conj(source[psi]);

            source1[psi] = source[psi];
            source1[nsi] = source[nsi];

            source2[nsi] = source[psi];
            source2[psi] = source[nsi];

            true_value[psi] = KFMMath::RegularSolidHarmonic_Cart(n,m,del2);
            true_value[nsi] = std::conj(true_value[psi]);
        }
    }

    std::cout<<"--------------------------------"<<std::endl;


    aCalc->TranslateMoments(del_origin, source1, target);

    for(int n=0; n<=degree; n++)
    {
        for(int m=0; m<=n; m++)
        {
            psi = n*(n+1) + m;
            std::cout<<"target("<<n<<","<<m<<") = "<<target[psi]<<std::endl;
        }
    }


    std::cout<<"--------------------------------"<<std::endl;

    aCalc->TranslateMomentsFast(del_origin, source2, target);

    for(int n=0; n<=degree; n++)
    {
        for(int m=0; m<=n; m++)
        {
            psi = n*(n+1) + m;
            std::cout<<"f_target("<<n<<","<<m<<") = "<<target[psi]<<std::endl;
        }
    }

    std::cout<<"--------------------------------"<<std::endl;


    return 0;
}
