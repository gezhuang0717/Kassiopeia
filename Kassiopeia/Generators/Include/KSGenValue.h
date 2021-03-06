#ifndef _Kassiopeia_KSGenValue_h_
#define _Kassiopeia_KSGenValue_h_

#include "KSComponentTemplate.h"

#include <vector>
using std::vector;

namespace Kassiopeia
{

    class KSGenValue :
        public KSComponentTemplate< KSGenValue >
    {
        public:
            KSGenValue();
            virtual ~KSGenValue();

        public:
            virtual void DiceValue( std::vector< double >& aDicedValue ) = 0;
    };

}

#endif
