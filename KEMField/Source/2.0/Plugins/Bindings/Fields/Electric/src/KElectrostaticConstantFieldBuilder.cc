#include "KElectrostaticConstantFieldBuilder.hh"

#include "KEMToolboxBuilder.hh"


using namespace KEMField;
namespace katrin {

template< >
KElectrostaticConstantFieldBuilder::~KComplexElement()
{
}

STATICINT sKEMToolBoxBuilder =
        KEMToolboxBuilder::ComplexElement< KElectrostaticConstantField >( "constant_electric_field" );

STATICINT sKElectrostaticConstantFieldBuilder =
		KElectrostaticConstantFieldBuilder::Attribute<std::string>( "name" ) +
		KElectrostaticConstantFieldBuilder::Attribute<KEMStreamableThreeVector>( "field" );

} //katrin


