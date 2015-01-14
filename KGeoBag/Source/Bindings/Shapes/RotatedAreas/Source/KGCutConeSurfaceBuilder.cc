#include "KGCutConeSurfaceBuilder.hh"
#include "KGInterfaceBuilder.hh"

namespace katrin
{

    static const int sKGCutConeSurfaceBuilder =
        KGInterfaceBuilder::ComplexElement< KGCutConeSurface >( "cut_cone_surface" );

    static const int sKGCutConeSurfaceBuilderStructure =
        KGCutConeSurfaceBuilder::Attribute< string >( "name" ) +
        KGCutConeSurfaceBuilder::Attribute< double >( "z1" ) +
        KGCutConeSurfaceBuilder::Attribute< double >( "r1" ) +
        KGCutConeSurfaceBuilder::Attribute< double >( "z2" ) +
        KGCutConeSurfaceBuilder::Attribute< double >( "r2" ) +
        KGCutConeSurfaceBuilder::Attribute< unsigned int >( "longitudinal_mesh_count" ) +
        KGCutConeSurfaceBuilder::Attribute< double >( "longitudinal_mesh_power" ) +
        KGCutConeSurfaceBuilder::Attribute< unsigned int >( "axial_mesh_count" );

}