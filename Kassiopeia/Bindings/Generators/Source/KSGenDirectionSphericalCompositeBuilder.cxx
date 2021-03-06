#include "KSGenDirectionSphericalCompositeBuilder.h"
#include "KSGenValueFixBuilder.h"
#include "KSGenValueSetBuilder.h"
#include "KSGenValueListBuilder.h"
#include "KSGenValueUniformBuilder.h"
#include "KSGenValueGaussBuilder.h"
#include "KSGenValueGeneralizedGaussBuilder.h"
#include "KSGenValueAngleCosineBuilder.h"
#include "KSGenValueAngleSphericalBuilder.h"
#include "KSRootBuilder.h"

#ifdef Kassiopeia_USE_ROOT
#include "KSGenValueFormulaBuilder.h"
#include "KSGenValueHistogramBuilder.h"
#endif

using namespace Kassiopeia;
using namespace std;

namespace katrin
{

    template< >
    KSGenDirectionSphericalCompositeBuilder::~KComplexElement()
    {
    }

    STATICINT sKSGenDirectionSphericalCompositeStructure =
        KSGenDirectionSphericalCompositeBuilder::Attribute< string >( "name" ) +
        KSGenDirectionSphericalCompositeBuilder::Attribute< string >( "theta" ) +
        KSGenDirectionSphericalCompositeBuilder::Attribute< string >( "phi" ) +
        KSGenDirectionSphericalCompositeBuilder::Attribute< string >( "surface" ) +
        KSGenDirectionSphericalCompositeBuilder::Attribute< string >( "space" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueFix >( "theta_fix" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueSet >( "theta_set" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueList >( "theta_list" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueUniform >( "theta_uniform" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueGauss >( "theta_gauss" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueGeneralizedGauss >( "theta_generalized_gauss" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueAngleCosine >( "theta_cosine" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueAngleSpherical >( "theta_spherical" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueFix >( "phi_fix" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueSet >( "phi_set" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueList >( "phi_list" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueUniform >( "phi_uniform" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueGauss >( "phi_gauss" ) +
        KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueGeneralizedGauss >( "phi_generalized_gauss" );

    STATICINT sKSGenDirectionSphericalComposite =
        KSRootBuilder::ComplexElement< KSGenDirectionSphericalComposite >( "ksgen_direction_spherical_composite" );

#ifdef Kassiopeia_USE_ROOT
    STATICINT sKSGenDirectionSphericalCompositeStructureROOT =
            KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueFormula >( "theta_formula" ) +
            KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueHistogram >( "theta_histogram" ) +
            KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueFormula >( "phi_formula" ) +
            KSGenDirectionSphericalCompositeBuilder::ComplexElement< KSGenValueHistogram >( "phi_histogram" );
#endif

}
