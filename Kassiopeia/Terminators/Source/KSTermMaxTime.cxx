#include "KSTermMaxTime.h"

#include "KSTerminatorsMessage.h"

#include "KSParticle.h"

namespace Kassiopeia
{

    KSTermMaxTime::KSTermMaxTime() :
            fTime( 0. )
    {
    }
    KSTermMaxTime::KSTermMaxTime( const KSTermMaxTime& aCopy ) :
            KSComponent(),
            fTime( aCopy.fTime )
    {
    }
    KSTermMaxTime* KSTermMaxTime::Clone() const
    {
        return new KSTermMaxTime( *this );
    }
    KSTermMaxTime::~KSTermMaxTime()
    {
    }

    void KSTermMaxTime::CalculateTermination( const KSParticle& anInitialParticle, bool& aFlag )
    {
        if (fTime < 0.)
            termmsg( eError ) << "negative time defined in MaxTime terminator <" << this->GetName() << ">" << eom;

        if( anInitialParticle.GetTime() > fTime )
        {
            aFlag = true;
            return;
        }
        aFlag = false;
        return;
    }
    void KSTermMaxTime::ExecuteTermination( const KSParticle&, KSParticle& aFinalParticle, KSParticleQueue& ) const
    {
        aFinalParticle.SetActive( false );
        aFinalParticle.SetLabel(  GetName() );
        return;
    }

}
