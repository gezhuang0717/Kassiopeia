#ifndef Kassiopeia_KSNavSpace_h_
#define Kassiopeia_KSNavSpace_h_

#include "KSSpaceNavigator.h"

#include "KMathBracketingSolver.h"
using namespace katrin;

namespace Kassiopeia
{

    class KSNavSpace :
        public KSComponentTemplate< KSNavSpace, KSSpaceNavigator >
    {
        public:
            KSNavSpace();
            KSNavSpace( const KSNavSpace& aCopy );
            KSNavSpace* Clone() const;
            virtual ~KSNavSpace();

        public:
            void SetEnterSplit( const bool& aEnterSplit );
            const bool& GetEnterSplit() const;

            void SetExitSplit( const bool& aExitSplit );
            const bool& GetExitSplit() const;

            void SetFailCheck( const bool& aValue );
            const bool& GetFailCheck() const;

        private:
            bool fEnterSplit;
            bool fExitSplit;
            bool fFailCheck;

        public:
            void CalculateNavigation( const KSTrajectory& aTrajectory, const KSParticle& aTrajectoryInitialParticle, const KSParticle& aTrajectoryFinalParticle, const KThreeVector& aTrajectoryCenter, const double& aTrajectoryRadius, const double& aTrajectoryStep, KSParticle& aNavigationParticle, double& aNavigationStep, bool& aNavigationFlag );
            void ExecuteNavigation( const KSParticle& anInitialParticle, KSParticle& aFinalParticle, KSParticleQueue& aSecondaries ) const;
            void FinalizeNavigation( KSParticle& aFinalParticle ) const;
            void StartNavigation( KSParticle& aParticle, KSSpace* aRoot );
            void StopNavigation( KSParticle& aParticle, KSSpace* aRoot );

        private:
            const KSTrajectory* fCurrentTrajectory;
            const KSSpace* fCurrentSpace;

            mutable KSSpace* fParentSpace;
            KThreeVector fParentSpaceAnchor;
            double fParentSpaceDistance;
            bool fParentSpaceRecalculate;

            mutable KSSpace* fChildSpace;
            KThreeVector fChildSpaceAnchor;
            double fChildSpaceDistance;
            bool fChildSpaceRecalculate;

            mutable KSSide* fParentSide;
            KThreeVector fParentSideAnchor;
            double fParentSideDistance;
            bool fParentSideRecalculate;

            mutable KSSide* fChildSide;
            KThreeVector fChildSideAnchor;
            double fChildSideDistance;
            bool fChildSideRecalculate;

            mutable KSSurface* fChildSurface;
            mutable KSSurface* fLastStepSurface;
            KThreeVector fChildSurfaceAnchor;
            double fChildSurfaceDistance;
            bool fChildSurfaceRecalculate;

            mutable bool fSpaceInsideCheck;
            mutable bool fNavigationFail;

            double SpaceIntersectionFunction( const double& anIntersection );
            double SurfaceIntersectionFunction( const double& anIntersection );
            double SideIntersectionFunction( const double& anIntersection );
            KMathBracketingSolver fSolver;
            KSParticle fIntermediateParticle;
    };

}

#endif
