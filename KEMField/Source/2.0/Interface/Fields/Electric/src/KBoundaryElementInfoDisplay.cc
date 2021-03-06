/*
 * KBoundaryElementInfoDisplay.cc
 *
 *  Created on: 24 Sep 2015
 *      Author: wolfgang
 */

#include "KBoundaryElementInfoDisplay.hh"
#include "KSurfaceContainer.hh"
#include "KEMCout.hh"

namespace KEMField {

KBoundaryElementInfoDisplay::KBoundaryElementInfoDisplay()
{
    Preprocessing(true);
}

KBoundaryElementInfoDisplay::~KBoundaryElementInfoDisplay()
{

}

void KBoundaryElementInfoDisplay::PreVisit(KElectrostaticBoundaryField& field)
{
    KSmartPointer<KSurfaceContainer> container = field.GetContainer();
    container->size();
    cout << "Discretized geometry elements: " << container->size();
    cout << endl;
}

} /* namespace KEMField */
