#include <iostream>
#include <cstdlib>

#include "KEMThreeVector.hh"
#include "KSurfaceContainer.hh"
#include "KEMConstants.hh"
#include "KEMCout.hh"

#include "KElectrostaticBoundaryIntegrator.hh"
#include "KElectrostaticRWGBoundaryIntegrator.hh"
#include "KElectrostaticNumericBoundaryIntegrator.hh"
#include "KElectrostaticBiQuadratureRectangleIntegrator.hh"

#include "KCUDASurfaceContainer.hh"
#include "KCUDAElectrostaticNumericBoundaryIntegrator.hh"

#include "TStyle.h"
#include "TApplication.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TLatex.h"

#define POW2(x) ((x)*(x))

// VALUES
#define NUMRECTANGLES 500  // number of rectangles for each Dr step
#define MINDR 2            // minimal distance ratio to be investigated
#define MAXDR 10000        // maximal distance ratio to be investigated
#define STEPSDR 500        // steps between given distance ratio range
#define SEPARATECOMP	   // if this variable has been defined potentials and fields will be computed separately,
						   // hence 'ElectricFieldAndPotential' function won't be used
						   // both options have to produce same values

// ROOT PLOTS AND COLORS (all settings apply for both field and potential)
#define PLOTNUM 1

#define COLNUM kRed

#define LINEWIDTH 1.

using namespace KEMField;

double IJKLRANDOM;
typedef KSurface<KElectrostaticBasis,KDirichletBoundary,KRectangle> KEMRectangle;
void subrn(double *u,int len);
double randomnumber();

void printVec( std::string add, KEMThreeVector input )
{
	std::cout << add.c_str() << input.X() << "\t" << input.Y() << "\t" << input.Z() << std::endl;
}

namespace KEMField{

// visitor for rectangle geometry

class RectangleVisitor :
		public KSelectiveVisitor<KShapeVisitor,
		KTYPELIST_1(KRectangle)>
{
public:
	using KSelectiveVisitor<KShapeVisitor,KTYPELIST_1(KRectangle)>::Visit;

	RectangleVisitor(){}

	void Visit(KRectangle& r) { ProcessRectangle(r); }

	void ProcessRectangle(KRectangle& r)
	{
		// get missing side length
		fAverageSideLength = 0.5*(r.GetA() + r.GetB());

		// centroid
		fShapeCentroid = r.Centroid();
	}

	double GetAverageSideLength() { return fAverageSideLength; }
	KEMThreeVector GetCentroid(){ return fShapeCentroid; }

private:
	double fAverageSideLength;
	KEMThreeVector fShapeCentroid;
};

} /* KEMField namespace*/

int main()
{
	// This program determines the accuracy of the rectangle integrators for a given distance ratio range.
	// distance ratio = distance to centroid / average side length

	// rectangle data
	double A,B;
	double P0[3];
	double N1[3];
	double N2[3];

	// assign a unique direction vector for field point to each rectangle and save into std::vector
	std::vector<KEMThreeVector> fPointDirections;

	// 'Num' rectangles will be diced in the beginning and added to a surface container
	// This values decides how much rectangles=field points will be computed for each distance ratio value

	KSurfaceContainer* container = new KSurfaceContainer();
	const unsigned int Num( NUMRECTANGLES ); /* number of rectangles */

	for( unsigned int i=0; i<Num; i++ ) {
		IJKLRANDOM = i+1;
		KEMRectangle* rectangle = new KEMRectangle();

		// dice rectangle geometry

		const double costheta = -1. + 2.*randomnumber();
		const double phi1 = 2. * M_PI * randomnumber();
		const double sintheta = sqrt( 1. - POW2(costheta) );

		N1[0] = sintheta*cos(phi1);
		N1[1] = sintheta*sin(phi1);
		N1[2] = costheta;

		const double phi2 = 2. * M_PI * randomnumber();

		N2[0] = cos(phi2)*sin(phi1) - sin(phi2)*costheta*cos(phi1);
		N2[1] = -cos(phi2)*cos(phi1) - sin(phi2)*costheta*sin(phi1);
		N2[2] = sin(phi2)*sintheta;

		A = randomnumber();
		B = randomnumber();

		P0[0] = -0.5*(B*N2[0]+A*N1[0]);
		P0[1] = -0.5*(B*N2[1]+A*N1[1]);
		P0[2] = -0.5*(B*N2[2]+A*N1[2]);

		rectangle->SetA( A );
		rectangle->SetB( B );
		rectangle->SetP0( KEMThreeVector(P0[0],P0[1],P0[2]) );
		rectangle->SetN1( KEMThreeVector(N1[0],N1[1],N1[2]) );
		rectangle->SetN2( KEMThreeVector(N2[0],N2[1],N2[2]) );

		rectangle->SetBoundaryValue( 1. );
		rectangle->SetSolution( 1. );

		container->push_back( rectangle );

		// dice direction vector of field points to be computed

		const double costhetaFP = -1.+2.*randomnumber();
		const double sinthetaFP = sqrt( 1. - POW2(costhetaFP) );
		const double phiFP = 2.*M_PI*randomnumber();

		fPointDirections.push_back( KEMThreeVector(
				sinthetaFP*cos(phiFP),
				sinthetaFP*sin(phiFP),
				costhetaFP ) );
	}

    // OpenCL surface container
    KCUDASurfaceContainer* cuContainer = new KCUDASurfaceContainer(*container);
    KCUDAInterface::GetInstance()->SetActiveData( cuContainer );

    // Bi-Quadrature and OpenCL integrator classes
    KElectrostaticBiQuadratureRectangleIntegrator fQuadIntegrator;
    KCUDAElectrostaticNumericBoundaryIntegrator intCUNum( *cuContainer );

    // ---------------------------------------------------------------------------------

    // CUDA memcopy needed here in parallel to memcopy in CUDA BI class in order
    // to guarantee that data will be copied onto GPU constant memory

    // rectangle cubature weights

    cudaMemcpyToSymbol( cuRectCub7w, gRectCub7w, sizeof(CU_TYPE)*7 );
    cudaMemcpyToSymbol( cuRectCub12w, gRectCub12w, sizeof(CU_TYPE)*12 );
    cudaMemcpyToSymbol( cuRectCub33w, gRectCub33w, sizeof(CU_TYPE)*33 );

    // ---------------------------------------------------------------------------------

	// visitor for elements
	RectangleVisitor fRectangleVisitor;

	KSurfaceContainer::iterator it;

	// distance ratios
	const double minDr( MINDR );
	const double maxDr( MAXDR );
	double Dr( 0. );
	const unsigned int kmax( STEPSDR);
	const double C = log(maxDr/minDr)/kmax;

	KEMField::cout << "Iterate from dist. ratio " << minDr << " to " << maxDr << " in " << kmax << " steps." << KEMField::endl;
	KEMField::cout << "Taking averaged relative error for " << container->size() << " rectangles for each dist. ratio value." << KEMField::endl;

	// field point

	KEMThreeVector fP;

    std::pair<KEMThreeVector,double> valQuad;
    std::pair<KEMThreeVector,double> valAna;
    std::pair<KEMThreeVector,double> valRwg;
    std::pair<KEMThreeVector,double> valNum;

	// plot

	TApplication* fAppWindow = new TApplication("fAppWindow", 0, NULL);

	gStyle->SetCanvasColor( kWhite );
	gStyle->SetLabelOffset( 0.03, "xyz" ); // values
	gStyle->SetTitleOffset( 1.6, "xyz" ); // label

	TMultiGraph *mgPot = new TMultiGraph();

	TGraph* plotDrPotNum = new TGraph( kmax+1 );
	plotDrPotNum->SetTitle( "Relative error of rectangle potential with adjusted numerical integrator" );
	plotDrPotNum->SetDrawOption( "same" );
	plotDrPotNum->SetMarkerColor( COLNUM );
	plotDrPotNum->SetLineWidth( LINEWIDTH );
	plotDrPotNum->SetLineColor( COLNUM );
	plotDrPotNum->SetMarkerSize( 0.2 );
	plotDrPotNum->SetMarkerStyle( 8 );
	if( PLOTNUM ) mgPot->Add( plotDrPotNum );

	TMultiGraph *mgField = new TMultiGraph();

	TGraph* plotDrFieldNum = new TGraph( kmax+1 );
	plotDrFieldNum->SetTitle( "Relative error of triangle field with adjusted numerical integrator" );
	plotDrFieldNum->SetDrawOption( "same" );
	plotDrFieldNum->SetMarkerColor( COLNUM );
	plotDrFieldNum->SetLineWidth( LINEWIDTH );
	plotDrFieldNum->SetLineColor( COLNUM );
	plotDrFieldNum->SetMarkerSize( 0.2 );
	plotDrFieldNum->SetMarkerStyle( 8 );
	if( PLOTNUM )mgField->Add( plotDrFieldNum );

    double relNumPot( 0. );

    double relNumField( 0. );

	// iterate over distance ratios in log steps
	for( unsigned int k=0; k<=kmax; k++ ) {

		Dr = minDr * exp(C*k);

		KEMField::cout << "Current distance ratio: " << Dr << "\t\r";
		KEMField::cout.flush();

		unsigned int directionIndex( 0);

		// iterate over container elements and dice field point distance (direction vector has already been defined)
		for( it=container->begin<KElectrostaticBasis>(); it!=container->end<KElectrostaticBasis>(); ++it ) {

			IJKLRANDOM++;

			(*it)->Accept(fRectangleVisitor);

			// assign field point value
			fP = fRectangleVisitor.GetAverageSideLength()*Dr*fPointDirections[directionIndex];

            directionIndex++;

            KEMRectangle* itRect;
            itRect = static_cast<KEMRectangle*>((*it));

#ifdef SEPARATECOMP
            valQuad = std::make_pair( fQuadIntegrator.ElectricField(itRect->GetShape(),fP), fQuadIntegrator.Potential(itRect->GetShape(),fP));
            valNum = std::make_pair(intCUNum.ElectricField(itRect->GetShape(),fP),intCUNum.Potential(itRect->GetShape(),fP));
#else
            valQuad = fQuadIntegrator.ElectricFieldAndPotential(itRect->GetShape(),fP);
            valNum = intCUNum.ElectricFieldAndPotential(itRect->GetShape(),fP);
#endif

            // sum for relative error

            relNumPot += fabs((valNum.second-valQuad.second)/valQuad.second);

            const double mag = sqrt(POW2(valQuad.first[0]) + POW2(valQuad.first[1]) + POW2(valQuad.first[2]));

            for( unsigned short i=0; i<3; i++ ) {
                relNumField += fabs(valNum.first[i]-valQuad.first[i])/(mag);
            }
		}

        relNumPot /= Num;

        relNumField /= Num;

		// save relative error of each integrator
        if( PLOTNUM ) plotDrPotNum->SetPoint( k, Dr, relNumPot );

		// reset relative error
		relNumPot = 0.;

		if( PLOTNUM ) plotDrFieldNum->SetPoint( k, Dr, relNumField );

		relNumField = 0.;
	} /* distance ratio */

    KEMField::cout << "Computation finished." << KEMField::endl;

	TCanvas cPot("cPot","Averaged relative error of rectangle potential", 0, 0, 960, 760);
	cPot.SetMargin(0.16,0.06,0.15,0.06);
	cPot.SetLogx();
	cPot.SetLogy();

	// multigraph, create plot
	cPot.cd();

	mgPot->Draw( "apl" );
	mgPot->SetTitle( "Averaged error of rectangle potential" );
	mgPot->GetXaxis()->SetTitle( "distance ratio" );
	mgPot->GetXaxis()->CenterTitle();
	mgPot->GetYaxis()->SetTitle( "relative error" );
	mgPot->GetYaxis()->CenterTitle();

	TLatex l;
	l.SetTextAlign(11);
	l.SetTextFont(62);
	l.SetTextSize(0.032);

    if( PLOTNUM ) {
        l.SetTextAngle(0);
        l.SetTextColor( COLNUM );
        l.DrawLatex(2.6,1.e-16,"Numerical (cubature + RWG)");
    }

	cPot.Update();

	TCanvas cField("cField","Averaged relative error of rectangle field", 0, 0, 960, 760);
	cField.SetMargin(0.16,0.06,0.15,0.06);
	cField.SetLogx();
	cField.SetLogy();

	// multigraph, create plot
	cField.cd();
	mgField->Draw( "apl" );
	mgField->SetTitle( "Averaged error of rectangle field" );
	mgField->GetXaxis()->SetTitle( "distance ratio" );
	mgField->GetXaxis()->CenterTitle();
	mgField->GetYaxis()->SetTitle( "relative error" );
	mgField->GetYaxis()->CenterTitle();

    if( PLOTNUM ) {
        l.SetTextAngle(0);
        l.SetTextColor( COLNUM );
        l.DrawLatex(2.6,1.e-16,"Numerical (cubature + RWG)");
    }

	cField.Update();

	fAppWindow->Run();

	return 0;
}

void subrn(double *u,int len)
{
	// This subroutine computes random numbers u[1],...,u[len]
	// in the (0,1) interval. It uses the 0<IJKLRANDOM<900000000
	// integer as initialization seed.
	//  In the calling program the dimension
	// of the u[] vector should be larger than len (the u[0] value is
	// not used).
	// For each IJKLRANDOM
	// numbers the program computes completely independent random number
	// sequences (see: F. James, Comp. Phys. Comm. 60 (1990) 329, sec. 3.3).

	static int iff=0;
	static long ijkl,ij,kl,i,j,k,l,ii,jj,m,i97,j97,ivec;
	static float s,t,uu[98],c,cd,cm,uni;
	if(iff==0)
	{
		if(IJKLRANDOM==0)
		{
			std::cout << "Message from subroutine subrn:\n";
			std::cout << "the global integer IJKLRANDOM should be larger than 0 !!!\n";
			std::cout << "Computation is  stopped !!! \n";
			exit(0);
		}
		ijkl=IJKLRANDOM;
		if(ijkl<1 || ijkl>=900000000) ijkl=1;
		ij=ijkl/30082;
		kl=ijkl-30082*ij;
		i=((ij/177)%177)+2;
		j=(ij%177)+2;
		k=((kl/169)%178)+1;
		l=kl%169;
		for(ii=1;ii<=97;ii++)
		{ s=0; t=0.5;
		for(jj=1;jj<=24;jj++)
		{ m=(((i*j)%179)*k)%179;
		i=j; j=k; k=m;
		l=(53*l+1)%169;
		if((l*m)%64 >= 32) s=s+t;
		t=0.5*t;
		}
		uu[ii]=s;
		}
		c=362436./16777216.;
		cd=7654321./16777216.;
		cm=16777213./16777216.;
		i97=97;
		j97=33;
		iff=1;
	}
	for(ivec=1;ivec<=len;ivec++)
	{ uni=uu[i97]-uu[j97];
	if(uni<0.) uni=uni+1.;
	uu[i97]=uni;
	i97=i97-1;
	if(i97==0) i97=97;
	j97=j97-1;
	if(j97==0) j97=97;
	c=c-cd;
	if(c<0.) c=c+cm;
	uni=uni-c;
	if(uni<0.) uni=uni+1.;
	if(uni==0.)
	{ uni=uu[j97]*0.59604644775391e-07;
	if(uni==0.) uni=0.35527136788005e-14;
	}
	u[ivec]=uni;
	}
	return;
}

////////////////////////////////////////////////////////////////

double randomnumber()
{
	// This function computes 1 random number in the (0,1) interval,
	// using the subrn subroutine.

	double u[2];
	subrn(u,1);
	return u[1];
}
