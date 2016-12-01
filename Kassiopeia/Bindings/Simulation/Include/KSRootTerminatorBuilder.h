#ifndef Kassiopeia_KSRootTerminatorBuilder_h_
#define Kassiopeia_KSRootTerminatorBuilder_h_

#include "KComplexElement.hh"
#include "KSRootTerminator.h"
#include "KSToolbox.h"

using namespace Kassiopeia;
namespace katrin
{

    typedef KComplexElement< KSRootTerminator > KSRootTerminatorBuilder;

    template< >
    inline bool KSRootTerminatorBuilder::AddAttribute( KContainer* aContainer )
    {
        if( aContainer->GetName() == "name" )
        {
            aContainer->CopyTo( fObject, &KNamed::SetName );
            return true;
        }
        if( aContainer->GetName() == "add_terminator" )
        {
            fObject->AddTerminator( KSToolbox::GetInstance()->GetObjectAs< KSTerminator >( aContainer->AsReference< string >() ) );
            return true;
        }
        return false;
    }

}

#endif