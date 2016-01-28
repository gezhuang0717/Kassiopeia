#ifndef KGVTKGEOMETRYPAINTERBUILDER_HH_
#define KGVTKGEOMETRYPAINTERBUILDER_HH_

#include "KComplexElement.hh"
#include "KGVTKGeometryPainter.hh"
#include "KGVisualizationMessage.hh"

using namespace KGeoBag;
namespace katrin
{

    typedef KComplexElement< KGVTKGeometryPainter > KGVTKGeometryPainterBuilder;

    template< >
    inline bool KGVTKGeometryPainterBuilder::AddAttribute( KContainer* aContainer )
    {
        if( aContainer->GetName() == "name" )
        {
            aContainer->CopyTo( fObject, &KNamed::SetName );
            return true;
        }
        if( aContainer->GetName() == "file" )
        {
            aContainer->CopyTo( fObject, &KGVTKGeometryPainter::SetFile );
            return true;
        }
        if( aContainer->GetName() == "path" )
        {
            aContainer->CopyTo( fObject, &KGVTKGeometryPainter::SetPath );
            return true;
        }
        if( aContainer->GetName() == "write_stl" )
        {
            aContainer->CopyTo( fObject, &KGVTKGeometryPainter::SetWriteSTL );
            return true;
        }
        if( aContainer->GetName() == "surfaces" )
        {
            if( aContainer->AsReference< string >().size() == 0 )
            {
                return true;
            }

            vector< KGSurface* > tSurfaces = KGInterface::GetInstance()->RetrieveSurfaces( aContainer->AsReference< string >() );
            vector< KGSurface* >::const_iterator tSurfaceIt;
            KGSurface* tSurface;

            if( tSurfaces.size() == 0 )
            {
                coremsg( eWarning ) << "no surfaces found for specifier <" << aContainer->AsReference< string >() << ">" << eom;
                return false;
            }

            for( tSurfaceIt = tSurfaces.begin(); tSurfaceIt != tSurfaces.end(); tSurfaceIt++ )
            {
                tSurface = *tSurfaceIt;
                fObject->AddSurface( tSurface );
            }
            return true;
        }
        if( aContainer->GetName() == "spaces" )
        {
            if( aContainer->AsReference< string >().size() == 0 )
            {
                return true;
            }

            vector< KGSpace* > tSpaces = KGInterface::GetInstance()->RetrieveSpaces( aContainer->AsReference< string >() );
            vector< KGSpace* >::const_iterator tSpaceIt;
            KGSpace* tSpace;

            if( tSpaces.size() == 0 )
            {
                coremsg( eWarning ) << "no spaces found for specifier <" << aContainer->AsReference< string >() << ">" << eom;
                return false;
            }

            for( tSpaceIt = tSpaces.begin(); tSpaceIt != tSpaces.end(); tSpaceIt++ )
            {
                tSpace = *tSpaceIt;
                fObject->AddSpace( tSpace );
            }
            return true;
        }

        return false;
    }

}

#endif
