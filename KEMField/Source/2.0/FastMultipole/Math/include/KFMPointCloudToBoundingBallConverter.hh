#ifndef KFMPointCloudToBoundingBallConverter_HH__
#define KFMPointCloudToBoundingBallConverter_HH__

#include "KFMPointCloud.hh"
#include "KFMBall.hh"
#include "KFMObjectContainer.hh"

#include "KFMBoundaryCalculator.hh"


namespace KEMField
{

/*
*
*@file KFMPointCloudToBoundingBallConverter.hh
*@class KFMPointCloudToBoundingBallConverter
*@brief
*@details
*
*<b>Revision History:<b>
*Date Name Brief Description
*Mon Aug 26 16:08:07 CEST 2013 J. Barrett (barrettj@mit.edu) First Version
*
*/


template<unsigned int NDIM>
class KFMPointCloudToBoundingBallConverter
{
    public:
        KFMPointCloudToBoundingBallConverter(){};
        virtual ~KFMPointCloudToBoundingBallConverter(){};

        void Convert(const KFMObjectContainer< KFMPointCloud<NDIM> >* cloud_container,
                     KFMObjectContainer< KFMBall<NDIM> >* ball_container) const
        {
            ball_container->DeleteAllObjects();

            unsigned int n_clouds =  cloud_container->GetNObjects();

            for(unsigned int i=0; i<n_clouds; i++)
            {
                ball_container->AddObject( Convert(cloud_container->GetObjectWithID(i) ) );
            }
        }

        KFMBall<NDIM> Convert(const KFMPointCloud<NDIM>* cloud) const
        {
            fCalculator.Reset();
            fCalculator.AddPointCloud(cloud);
            return fCalculator.GetMinimalBoundingBall();
        }

    private:

        mutable KFMBoundaryCalculator<NDIM> fCalculator;

};


}


#endif /* KFMPointCloudToBoundingBallConverter_H__ */
