#ifndef KGROOTGEOMETRYPAINTER_HH_
#define KGROOTGEOMETRYPAINTER_HH_

#include "KROOTWindow.h"
using katrin::KROOTWindow;

#include "KROOTPainter.h"
using katrin::KROOTPainter;

#include "KGCore.hh"
#include "KGAppearance.hh"
#include "KGFlattenedCircleSurface.hh"
#include "KGFlattenedPolyLoopSurface.hh"
#include "KGRotatedLineSegmentSurface.hh"
#include "KGRotatedArcSegmentSurface.hh"
#include "KGRotatedPolyLineSurface.hh"
#include "KGRotatedCircleSurface.hh"
#include "KGRotatedPolyLoopSurface.hh"
#include "KGShellLineSegmentSurface.hh"
#include "KGShellArcSegmentSurface.hh"
#include "KGShellPolyLineSurface.hh"
#include "KGShellPolyLoopSurface.hh"
#include "KGShellCircleSurface.hh"
#include "KGExtrudedLineSegmentSurface.hh"
#include "KGExtrudedArcSegmentSurface.hh"
#include "KGExtrudedPolyLineSurface.hh"
#include "KGExtrudedCircleSurface.hh"
#include "KGExtrudedPolyLoopSurface.hh"
#include "KGRotatedLineSegmentSpace.hh"
#include "KGRotatedArcSegmentSpace.hh"
#include "KGRotatedPolyLineSpace.hh"
#include "KGRotatedCircleSpace.hh"
#include "KGRotatedPolyLoopSpace.hh"
#include "KGExtrudedCircleSpace.hh"
#include "KGExtrudedPolyLoopSpace.hh"

//include root stuff
#include "TPolyLine.h"

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <utility>
using std::pair;

#include <algorithm>
using std::reverse;
using std::swap;

#include "KField.h"

namespace KGeoBag
{

    class KGROOTGeometryPainter :
        public KROOTPainter,
        public KGVisitor,
        public KGSurface::Visitor,
        public KGFlattenedCircleSurface::Visitor,
        public KGFlattenedPolyLoopSurface::Visitor,
        public KGRotatedLineSegmentSurface::Visitor,
        public KGRotatedArcSegmentSurface::Visitor,
        public KGRotatedPolyLineSurface::Visitor,
        public KGRotatedCircleSurface::Visitor,
        public KGRotatedPolyLoopSurface::Visitor,
        public KGShellLineSegmentSurface::Visitor,
        public KGShellArcSegmentSurface::Visitor,
        public KGShellPolyLineSurface::Visitor,
        public KGShellPolyLoopSurface::Visitor,
        public KGShellCircleSurface::Visitor,
        public KGExtrudedLineSegmentSurface::Visitor,
        public KGExtrudedArcSegmentSurface::Visitor,
        public KGExtrudedPolyLineSurface::Visitor,
        public KGExtrudedCircleSurface::Visitor,
        public KGExtrudedPolyLoopSurface::Visitor,
        public KGSpace::Visitor,
        public KGRotatedLineSegmentSpace::Visitor,
        public KGRotatedArcSegmentSpace::Visitor,
        public KGRotatedPolyLineSpace::Visitor,
        public KGRotatedCircleSpace::Visitor,
        public KGRotatedPolyLoopSpace::Visitor,
        public KGExtrudedCircleSpace::Visitor,
        public KGExtrudedPolyLoopSpace::Visitor
    {
        public:
    		KGROOTGeometryPainter();
            virtual ~KGROOTGeometryPainter();

        public:
            void Render();
            void Display();
            void Write();

            void AddSurface( KGSurface* aSurface );
            void AddSpace( KGSpace* aSpace );

            virtual double GetXMin();
            virtual double GetXMax();
            virtual double GetYMin();
            virtual double GetYMax();

        private:
            vector< KGSurface* > fSurfaces;
            vector< KGSpace* > fSpaces;

            KGAppearanceData fDefaultData;

            //****************
            //Plane settings
            //****************

            ;K_SET( KThreeVector, PlaneNormal)
            ;K_SET( KThreeVector, PlanePoint)
            ;K_SET( bool, SwapAxis)
            ;K_GET( KThreeVector, PlaneVectorA)
            ;K_GET( KThreeVector, PlaneVectorB)

        public:
            virtual std::string GetXAxisLabel();
            virtual std::string GetYAxisLabel();
        private:
            std::string GetAxisLabel( KThreeVector anAxis );

        public:
            void CalculatePlaneCoordinateSystem();

            K_SET( double, Epsilon );


            //****************
            //surface visitors
            //****************

        protected:
            virtual void VisitSurface( KGSurface* aSurface );
            virtual void VisitFlattenedClosedPathSurface( KGFlattenedCircleSurface* aFlattenedCircleSurface );
            virtual void VisitFlattenedClosedPathSurface( KGFlattenedPolyLoopSurface* aFlattenedPolyLoopSurface );
            virtual void VisitRotatedPathSurface( KGRotatedLineSegmentSurface* aRotatedLineSegmentSurface );
            virtual void VisitRotatedPathSurface( KGRotatedArcSegmentSurface* aRotatedArcSegmentSurface );
            virtual void VisitRotatedPathSurface( KGRotatedPolyLineSurface* aRotatedPolyLineSurface );
            virtual void VisitRotatedPathSurface( KGRotatedCircleSurface* aRotatedCircleSurface );
            virtual void VisitRotatedPathSurface( KGRotatedPolyLoopSurface* aRotatedPolyLoopSurface );
            virtual void VisitShellPathSurface( KGShellLineSegmentSurface* aShellLineSegmentSurface );
            virtual void VisitShellPathSurface( KGShellArcSegmentSurface* aShellArcSegmentSurface );
            virtual void VisitShellPathSurface( KGShellPolyLineSurface* aShellPolyLineSurface );
            virtual void VisitShellPathSurface( KGShellPolyLoopSurface* aShellPolyLoopSurface );
            virtual void VisitShellPathSurface( KGShellCircleSurface* aShellCircleSurface );
            virtual void VisitExtrudedPathSurface( KGExtrudedLineSegmentSurface* aExtrudedLineSegmentSurface );
            virtual void VisitExtrudedPathSurface( KGExtrudedArcSegmentSurface* aExtrudedArcSegmentSurface );
            virtual void VisitExtrudedPathSurface( KGExtrudedPolyLineSurface* aExtrudedPolyLineSurface );
            virtual void VisitExtrudedPathSurface( KGExtrudedCircleSurface* aExtrudedCircleSurface );
            virtual void VisitExtrudedPathSurface( KGExtrudedPolyLoopSurface* aExtrudedPolyLoopSurface );

            //**************
            //space visitors
            //**************

        protected:
            virtual void VisitSpace( KGSpace* aSpace );
            virtual void VisitRotatedOpenPathSpace( KGRotatedLineSegmentSpace* aRotatedLineSegmentSpace );
            virtual void VisitRotatedOpenPathSpace( KGRotatedArcSegmentSpace* aRotatedArcSegmentSpace );
            virtual void VisitRotatedOpenPathSpace( KGRotatedPolyLineSpace* aRotatedPolyLineSpace );
            virtual void VisitRotatedClosedPathSpace( KGRotatedCircleSpace* aRotatedCircleSpace );
            virtual void VisitRotatedClosedPathSpace( KGRotatedPolyLoopSpace* aRotatedPolyLoopSpace );
            virtual void VisitExtrudedClosedPathSpace( KGExtrudedCircleSpace* aExtrudedCircleSpace );
            virtual void VisitExtrudedClosedPathSpace( KGExtrudedPolyLoopSpace* aExtrudedPolyLoopSpace );

        private:
            void LocalToGlobal( const KThreeVector& aLocal, KThreeVector& aGlobal );

            //**********
            //data types
            //**********

            class Points
            {
                public:
                    typedef KTwoVector Element;
                    typedef deque< Element > Set;
                    typedef Set::iterator It;
                    typedef Set::const_iterator CIt;

                public:
                    Set fData;
            };

            class OpenPoints :
                public Points
            {
            };

            class ClosedPoints :
                public Points
            {
            };

            class Mesh
            {
                public:
                    typedef KThreeVector Element;
                    typedef deque< Element > Group;
                    typedef Group::iterator GroupIt;
                    typedef Group::const_iterator GroupCIt;
                    typedef deque< Group > Set;
                    typedef Set::iterator SetIt;
                    typedef Set::const_iterator SetCIt;

                public:
                    Set fData;
            };

            class FlatMesh :
                public Mesh
            {
            };

            class TubeMesh :
                public Mesh
            {
            };

            class TorusMesh :
                public Mesh
            {
            };

            class ShellMesh : 
                public Mesh
            {
            };

            class Lines
            {
                public:
                    typedef KThreeVector Element;
                    typedef pair< Element, Element > Line;
                    typedef deque< Line > Group;
                    typedef Group::iterator GroupIt;
                    typedef Group::const_iterator GroupCIt;
                    typedef deque< Group > Set;
                    typedef Set::iterator SetIt;
                    typedef Set::const_iterator SetCIt;

                public:
                    Set fData;
            };

            class CircleLines :
				public Lines
            {
            };

            class ParallelLines :
				public Lines
            {
            };

            class IntersectionPoints
            {
                public:
                    typedef KTwoVector Element;
                    typedef deque< Element > Group;
                    typedef Group::iterator GroupIt;
                    typedef Group::const_iterator GroupCIt;
                    typedef enum
                    {
                    	eUndefined, eParallel, eCircle
                    } Origin;
                    typedef pair< Group, Origin> NamedGroup;
                    typedef deque< NamedGroup > Set;
                    typedef Set::iterator SetIt;
                    typedef Set::const_iterator SetCIt;

                public:
                    Set fData;
            };

            class OrderedPoints
            {
				public:
					typedef Points Element;
					typedef deque< Element > Set;
					typedef Set::iterator SetIt;
					typedef Set::const_iterator SetCIt;

                public:
					Set fData;
            };

            //****************
            //points functions
            //****************

            void LineSegmentToOpenPoints( const KGPlanarLineSegment* aLineSegment, OpenPoints& aPoints );
            void ArcSegmentToOpenPoints( const KGPlanarArcSegment* anArcSegment, OpenPoints& aPoints );
            void PolyLineToOpenPoints( const KGPlanarPolyLine* aPolyLine, OpenPoints& aPoints );
            void CircleToClosedPoints( const KGPlanarCircle* aCircle, ClosedPoints& aPoints );
            void PolyLoopToClosedPoints( const KGPlanarPolyLoop* aPolyLoop, ClosedPoints& aPoints );

            //**************
            //mesh functions
            //**************

            void ClosedPointsFlattenedToTubeMeshAndApex( const ClosedPoints& aPoints, const KTwoVector& aCentroid, const double& aZ, TubeMesh& aMesh, KThreeVector& anApex );
            void OpenPointsRotatedToTubeMesh( const OpenPoints& aPoints, TubeMesh& aMesh );
            void ClosedPointsRotatedToTorusMesh( const ClosedPoints& aPoints, TorusMesh& aMesh );
            void OpenPointsExtrudedToFlatMesh( const OpenPoints& aPoints, const double& aZMin, const double& aZMax, FlatMesh& aMesh );
            void ClosedPointsExtrudedToTubeMesh( const ClosedPoints& aPoints, const double& aZMin, const double& aZMax, TubeMesh& aMesh );

            //**************
            //line functions
            //**************

            void TubeMeshToCircleLines( const TubeMesh aMesh, CircleLines& aCircleLines );
            void TubeMeshToParallelLines( const TubeMesh aMesh, ParallelLines& aParallelLines );
            void TorusMeshToCircleLines( const TorusMesh aMesh, CircleLines& aCircleLines );
            void TorusMeshToParallelLines( const TorusMesh aMesh, ParallelLines& aParallelLines );

            //**********************
            //intersection functions
            //**********************

            void LinesToIntersections( const CircleLines aCircleLinesSet, IntersectionPoints& anIntersectionPoints );
            void LinesToIntersections( const ParallelLines aCircleLinesSet, IntersectionPoints& anIntersectionPoints );
            void CalculatePlaneIntersection( const KThreeVector aStartPoint, const KThreeVector anEndPoint, KThreeVector& anIntersectionPoint, bool& anIntersection );
            void TransformToPlaneSystem( const KThreeVector aPoint, KTwoVector& aPlanePoint );

            void IntersectionPointsToOrderedPoints( const IntersectionPoints anIntersectionPoints, OrderedPoints& anOrderedPoints );

            void CreateClosedOrderedPoints( const IntersectionPoints anIntersectionPoints, OrderedPoints& anOrderedPoints );
            void CreateOpenOrderedPoints( const IntersectionPoints anIntersectionPoints, OrderedPoints& anOrderedPoints );
            void CreateDualOrderedPoints( const IntersectionPoints anIntersectionPoints, OrderedPoints& anOrderedPoints );

            void CombineOrderedPoints( OrderedPoints& anOrderedPoints );


            //*******************
            //rendering functions
            //*******************

            void OrderedPointsToROOTSurface( const OrderedPoints anOrderedPoints);
            void OrderedPointsToROOTSpace( const OrderedPoints anOrderedPoints);


        private:
            //root stuff
            vector< TPolyLine* > fROOTSpaces;
            vector< TPolyLine* > fROOTSurfaces;

            KGSpace* fCurrentSpace;
            KGSurface* fCurrentSurface;
            KGAppearanceData* fCurrentData;
            KThreeVector fCurrentOrigin;
            KThreeVector fCurrentXAxis;
            KThreeVector fCurrentYAxis;
            KThreeVector fCurrentZAxis;
            bool fIgnore;
    };

}

#endif
