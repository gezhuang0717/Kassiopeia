//
// Created by trost on 27.05.15.
//

#include "KSIntDecayCalculatorFerencBBRTransitionBuilder.h"
#include "KSGenGeneratorCompositeBuilder.h"
#include "KSRootBuilder.h"

using namespace Kassiopeia;
namespace katrin
{

    template< >
    KSIntDecayCalculatorFerencBBRTransitionBuilder::~KComplexElement()
    {
    }

    STATICINT sKSIntDecayCalculatorFerencBBRTransitionBuilderStructure =
            KSIntDecayCalculatorFerencBBRTransitionBuilder::Attribute< string >( "name" ) +
            KSIntDecayCalculatorFerencBBRTransitionBuilder::Attribute< long long >( "target_pid" ) +
            KSIntDecayCalculatorFerencBBRTransitionBuilder::Attribute< long long >( "min_pid" ) +
            KSIntDecayCalculatorFerencBBRTransitionBuilder::Attribute< long long >( "max_pid" ) +
            KSIntDecayCalculatorFerencBBRTransitionBuilder::Attribute< double >( "temperature" ) +
            KSIntDecayCalculatorFerencBBRTransitionBuilder::ComplexElement< KSGenGeneratorComposite >( "decay_product_generator");

    STATICINT sToolboxKSIntDecayCalculatorFerencBBRTransition =
            KSRootBuilder::ComplexElement< KSIntDecayCalculatorFerencBBRTransition >( "ksint_decay_calculator_ferenc_bbr_transition" );
}
