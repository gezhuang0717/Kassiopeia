#ifndef Kassiopeia_KSReadFileROOT_h_
#define Kassiopeia_KSReadFileROOT_h_

#include "KSReadFile.h"

#include "KSReadRunROOT.h"
#include "KSReadEventROOT.h"
#include "KSReadTrackROOT.h"
#include "KSReadStepROOT.h"

#include "KRootFile.h"
using katrin::KFile;
using katrin::KRootFile;

namespace Kassiopeia
{

    class KSReadFileROOT :
        public KSReadFile
    {
        public:
            typedef map< std::string, KSReadObjectROOT* > ObjectMap;
            typedef ObjectMap::iterator ObjectIt;
            typedef ObjectMap::const_iterator ObjectCIt;
            typedef ObjectMap::value_type ObjectEntry;

        public:
            KSReadFileROOT();
            ~KSReadFileROOT();

            bool TryFile( KRootFile* aFile );
            void OpenFile( KRootFile* aFile );
            void CloseFile();

            KSReadRunROOT& GetRun();
            KSReadEventROOT& GetEvent();
            KSReadTrackROOT& GetTrack();
            KSReadStepROOT& GetStep();

        protected:
            KRootFile* fRootFile;

            KSReadRunROOT* fRun;
            KSReadEventROOT* fEvent;
            KSReadTrackROOT* fTrack;
            KSReadStepROOT* fStep;
    };

}

#endif
