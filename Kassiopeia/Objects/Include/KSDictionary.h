#ifndef Kassiopeia_KSDictionary_h_
#define Kassiopeia_KSDictionary_h_

#ifndef STATICINT
#define STATICINT static const int __attribute__((__unused__))
#endif

#include "KSComponent.h"
#include "KSCommand.h"

#include <map>

namespace Kassiopeia
{

    class KSCommandFactory
    {
        public:
            KSCommandFactory();
            virtual ~KSCommandFactory();

            virtual KSCommand* CreateCommand( KSComponent* aParent, KSComponent* aChild ) const = 0;
    };

    class KSComponentFactory
    {
        public:
            KSComponentFactory();
            virtual ~KSComponentFactory();

            virtual KSComponent* CreateComponent( KSComponent* aParent ) const = 0;
    };

    template< class XType >
    class KSDictionary
    {
        private:
            KSDictionary();
            ~KSDictionary();

            typedef std::multimap< std::string, const KSCommandFactory* > CommandFactoryMap;
            typedef CommandFactoryMap::iterator CommandFactoryIt;
            typedef CommandFactoryMap::const_iterator CommandFactoryCIt;
            typedef CommandFactoryMap::value_type CommandFactoryEntry;

            typedef std::multimap< std::string, const KSComponentFactory* > ComponentFactoryMap;
            typedef ComponentFactoryMap::iterator ComponentFactoryIt;
            typedef ComponentFactoryMap::const_iterator ComponentFactoryCIt;
            typedef ComponentFactoryMap::value_type ComponentFactoryEntry;

            static CommandFactoryMap* fCommandFactories;
            static ComponentFactoryMap* fComponentFactories;

        public:
            static KSCommand* GetCommand( KSComponent* aParent, KSComponent* aChild, const std::string& aField );
            template< class XParentType, class XChildType >
            static int AddCommand( void (XParentType::*anAddMember)( XChildType* ), void (XParentType::*aRemoveMember)( XChildType* ), const std::string& anAddField, const std::string& aRemoveField );
            template< class XParentType, class XChildType >
            static int AddCommand( void (XParentType::*aSetMember)( const XChildType& ), const XChildType& (XParentType::*aGetMember)() const, const std::string& aParameterField );

            static KSComponent* GetComponent( KSComponent* aParent, const std::string& aField );
            template< class XMemberType >
            static int AddComponent( XMemberType aMember, const std::string& aField );
    };

    template< class XType >
    typename KSDictionary< XType >::CommandFactoryMap* KSDictionary< XType >::fCommandFactories = NULL;

    template< class XType >
    KSCommand* KSDictionary< XType >::GetCommand( KSComponent* aParent, KSComponent* aChild, const std::string& aField )
    {
        if( fCommandFactories == NULL )
        {
            fCommandFactories = new CommandFactoryMap();
        }

        KSCommand* tCommand = NULL;
        CommandFactoryIt tLowIt = fCommandFactories->lower_bound( aField );
        CommandFactoryIt tUpIt = fCommandFactories->upper_bound( aField );

        if( tLowIt != tUpIt )
        {
            objctmsg_debug( "  found command <" << aField << "> for parent <" << aParent->GetName() << ">" << eom );
            for( CommandFactoryIt tIt = tLowIt; tIt != tUpIt; tIt++ )
            {
                tCommand = tIt->second->CreateCommand( aParent, aChild );
                if( tCommand != NULL )
                {
                    tCommand->SetName( aParent->GetName() + std::string( "/" ) + aField + std::string( "/" ) + aChild->GetName() );
                    return tCommand;
                }
            }
        }

        objctmsg_debug( "  no command <" << aField << "> for parent <" << aParent->GetName() << ">" << eom );
        return tCommand;
    }

    template< class XType >
    typename KSDictionary< XType >::ComponentFactoryMap* KSDictionary< XType >::fComponentFactories = NULL;

    template< class XType >
    KSComponent* KSDictionary< XType >::GetComponent( KSComponent* aParent, const std::string& aField )
    {
        if( fComponentFactories == NULL )
        {
            fComponentFactories = new ComponentFactoryMap();
        }

        KSComponent* tComponent = NULL;
        ComponentFactoryIt tLowIt = fComponentFactories->lower_bound( aField );
        ComponentFactoryIt tUpIt = fComponentFactories->upper_bound( aField );

        if( tLowIt != tUpIt )
        {
            objctmsg_debug( "  found output <" << aField << "> for parent <" << aParent->GetName() << ">" << eom );
            for( ComponentFactoryIt tIt = tLowIt; tIt != tUpIt; tIt++ )
            {
                tComponent = tIt->second->CreateComponent( aParent );
                if( tComponent != NULL )
                {
                    tComponent->SetName( aParent->GetName() + std::string( "/" ) + aField );
                    return tComponent;
                }
            }
        }

        objctmsg_debug( "  no output <" << aField << "> for parent <" << aParent->GetName() << ">" << eom );
        return tComponent;
    }

}

#endif
