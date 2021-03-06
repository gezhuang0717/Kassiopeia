#ifndef Kassiopeia_KSTrajTrajectoryExact_h_
#define Kassiopeia_KSTrajTrajectoryExact_h_

#include "KSTrajectory.h"
#include "KSTrajExactTypes.h"

#include "KSList.h"

#include "KGBall.hh"
#include "KGBallSupportSet.hh"

namespace Kassiopeia
{

    class KSTrajTrajectoryExact :
        public KSComponentTemplate< KSTrajTrajectoryExact, KSTrajectory >,
        public KSTrajExactDifferentiator
    {
        public:
            KSTrajTrajectoryExact();
            KSTrajTrajectoryExact( const KSTrajTrajectoryExact& aCopy );
            KSTrajTrajectoryExact* Clone() const;
            virtual ~KSTrajTrajectoryExact();

        public:
            void SetIntegrator( KSTrajExactIntegrator* anIntegrator );
            void ClearIntegrator( KSTrajExactIntegrator* anIntegrator );

            void SetInterpolator( KSTrajExactInterpolator* anInterpolator );
            void ClearInterpolator( KSTrajExactInterpolator* anInterpolator );

            void AddTerm( KSTrajExactDifferentiator* aTerm );
            void RemoveTerm( KSTrajExactDifferentiator* aTerm );

            void AddControl( KSTrajExactControl* aControl );
            void RemoveControl( KSTrajExactControl* aControlSize );

            void SetAttemptLimit(unsigned int n)
            {
                if(n > 1 ){fMaxAttempts = n;}
                else{fMaxAttempts = 1;};
            }

            //**********
            //trajectory
            //**********

            void SetPiecewiseTolerance(double ptol){fPiecewiseTolerance = ptol;};
            double GetPiecewiseTolerance() const {return fPiecewiseTolerance;};

            void SetMaxNumberOfSegments(double n_max){fNMaxSegments = n_max; if(fNMaxSegments < 1 ){fNMaxSegments = 1;};};
            unsigned int GetMaxNumberOfSegments() const {return fNMaxSegments;};

        public:

            void Reset();
            void CalculateTrajectory( const KSParticle& anInitialParticle, KSParticle& aFinalParticle, KThreeVector& aCenter, double& aRadius, double& aTimeStep );
            void ExecuteTrajectory( const double& aTimeStep, KSParticle& anIntermediateParticle ) const;
            void GetPiecewiseLinearApproximation(const KSParticle& anInitialParticle, const KSParticle& /*aFinalParticle*/, std::vector< KSParticle >* intermediateParticleStates) const;

            //********************
            //exact term interface
            //********************

        public:
            virtual void Differentiate(double aTime, const KSTrajExactParticle& aValue, KSTrajExactDerivative& aDerivative ) const;

        private:

            KSTrajExactParticle fInitialParticle;
            mutable KSTrajExactParticle fIntermediateParticle;
            mutable KSTrajExactParticle fFinalParticle;
            mutable KSTrajExactError fError;

            KSTrajExactIntegrator* fIntegrator;
            KSTrajExactInterpolator* fInterpolator;
            KSList< KSTrajExactDifferentiator > fTerms;
            KSList< KSTrajExactControl > fControls;

            //piecewise linear approximation
            double fPiecewiseTolerance;
            unsigned int fNMaxSegments;
            mutable KGeoBag::KGBallSupportSet<3> fBallSupport;
            mutable std::vector<KSTrajExactParticle> fIntermediateParticleStates;

            unsigned int fMaxAttempts;

    };

}

#endif
