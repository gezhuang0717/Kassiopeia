#ifndef KFMBallCloud_HH__
#define KFMBallCloud_HH__

#include <vector>

#include "KFMBall.hh"

namespace KEMField
{

/*
*
*@file KFMBallCloud.hh
*@class KFMBallCloud
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Sat Aug 24 14:27:45 CEST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/


template<unsigned int NDIM>
class KFMBallCloud
{
    public:
        KFMBallCloud(){};
        virtual ~KFMBallCloud(){};

        unsigned int GetNBalls() const
        {
            return fBalls.size();
        }

        void AddBall(const KFMBall<NDIM>& Ball)
        {
            fBalls.push_back(Ball);
        }

        void Clear()
        {
            fBalls.clear();
        }

        KFMBall<NDIM> GetBall(unsigned int i) const {return fBalls[i];}; //no check performed

        void GetBalls( std::vector< KFMBall<NDIM> >* Balls) const
        {
            *Balls = fBalls;
        }

        std::vector< KFMBall<NDIM> >* GetBalls()
        {
            return &fBalls;
        }


    private:

        unsigned int fID;
        std::vector< KFMBall<NDIM> > fBalls;


};


}//end of KEMField



#endif /* KFMBallCloud_H__ */
