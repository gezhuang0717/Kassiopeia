/*
 * KEMElectricFieldPointsRootFile.hh
 *
 *  Created on: 04.05.2015
 *      Author: gosda
 */

#ifndef KEMELECTRICFIELDPOINTSROOTFILE_HH_
#define KEMELECTRICFIELDPOINTSROOTFILE_HH_

#include <string>
#include "TFile.h"
#include "TTree.h"
#include "KEMThreeVector.hh"

namespace KEMField {

class KEMElectricFieldPointsRootFile {
public:
	KEMElectricFieldPointsRootFile(std::string fullPath);
	~KEMElectricFieldPointsRootFile();

	void append(KPosition position,KDirection eField,double potential);
	void Write();
private:
	TFile fFile;
	TTree fTree;
	KPosition fPosition;
	double fPotential;
	KDirection fElectricField;
	double fElectricFieldAbs;
};

} //KEMField

#endif /* KEMELECTRICFIELDPOINTSROOTFILE_HH_ */
