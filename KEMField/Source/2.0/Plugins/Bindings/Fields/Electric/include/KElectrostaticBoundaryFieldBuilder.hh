/*
 * KElectrostaticBoundaryFieldBuilder.hh
 *
 *  Created on: 17 Jun 2015
 *      Author: wolfgang
 */

#ifndef KEMFIELD_SOURCE_2_0_PLUGINS_BINDINGS_FIELDS_ELECTRIC_INCLUDE_KELECTROSTATICBOUNDARYFIELDBUILDER_HH_
#define KEMFIELD_SOURCE_2_0_PLUGINS_BINDINGS_FIELDS_ELECTRIC_INCLUDE_KELECTROSTATICBOUNDARYFIELDBUILDER_HH_

#include "KEMBindingsMessage.hh"
#include "KSmartPointerRelease.hh"
#include "KComplexElement.hh"
#include "KElectrostaticBoundaryFieldWithKGeoBag.hh"
#include "KSmartPointerRelease.hh"

namespace katrin {

typedef KComplexElement< KEMField::KElectrostaticBoundaryFieldWithKGeoBag>
KElectrostaticBoundaryFieldBuilder;

template< >
inline bool KElectrostaticBoundaryFieldBuilder::AddAttribute( KContainer* aContainer )
{
	if( aContainer->GetName() == "name" )
	{
		std::string name;
		aContainer->CopyTo(name);
		fObject->SetName(name);
		SetName(name);
	}
	else if( aContainer->GetName() == "directory" )
	{
		aContainer->CopyTo( fObject, &KEMField::KElectrostaticBoundaryFieldWithKGeoBag::SetDirectory );
	}
	else if( aContainer->GetName() == "file" )
	{
		aContainer->CopyTo( fObject, &KEMField::KElectrostaticBoundaryFieldWithKGeoBag::SetFile );
	}
	else if( aContainer->GetName() == "system" )
	{
		KGeoBag::KGSpace* tSpace = KGeoBag::KGInterface::GetInstance()->RetrieveSpace( aContainer->AsReference< string >() );

		if( tSpace == NULL )
		{
			BINDINGMSG( eWarning ) << "no spaces found for specifier <" << aContainer->AsReference< string >() << ">" << eom;
			return false;
		}

		fObject->SetSystem( tSpace );
	}
	else if( aContainer->GetName() == "surfaces" )
	{
		std::vector< KGeoBag::KGSurface* > tSurfaces = KGeoBag::KGInterface::GetInstance()->RetrieveSurfaces( aContainer->AsReference< string >() );
		std::vector< KGeoBag::KGSurface* >::const_iterator tSurfaceIt;
		KGeoBag::KGSurface* tSurface;

		if( tSurfaces.size() == 0 )
		{
			BINDINGMSG( eWarning ) << "no surfaces found for specifier <" << aContainer->AsReference< string >() << ">" << eom;
			return false;
		}

		for( tSurfaceIt = tSurfaces.begin(); tSurfaceIt != tSurfaces.end(); tSurfaceIt++ )
		{
			tSurface = *tSurfaceIt;
			fObject->AddSurface( tSurface );
		}
	}
	else if( aContainer->GetName() == "spaces" )
	{
		std::vector< KGeoBag::KGSpace* > tSpaces = KGeoBag::KGInterface::GetInstance()->RetrieveSpaces( aContainer->AsReference< string >() );
		std::vector< KGeoBag::KGSpace* >::const_iterator tSpaceIt;
		KGeoBag::KGSpace* tSpace;

		if( tSpaces.size() == 0 )
		{
			BINDINGMSG( eWarning ) << "no spaces found for specifier <" << aContainer->AsReference< string >() << ">" << eom;
			return false;
		}

		for( tSpaceIt = tSpaces.begin(); tSpaceIt != tSpaces.end(); tSpaceIt++ )
		{
			tSpace = *tSpaceIt;
			fObject->AddSpace( tSpace );
		}
	}
	else if( aContainer->GetName() == "symmetry" )
	{
		if( aContainer->AsReference< string >() == "none" )
		{
			fObject->SetSymmetry( KEMField::KElectrostaticBoundaryFieldWithKGeoBag::NoSymmetry );
			return true;
		}
		if( aContainer->AsReference< string >() == "axial" )
		{
			fObject->SetSymmetry( KEMField::KElectrostaticBoundaryFieldWithKGeoBag::AxialSymmetry );
			return true;
		}
		if( aContainer->AsReference< string >() == "discrete_axial" )
		{
			fObject->SetSymmetry( KEMField::KElectrostaticBoundaryFieldWithKGeoBag::DiscreteAxialSymmetry );
			return true;
		}
		BINDINGMSG( eWarning ) << "symmetry must be <none>, <axial>, or <discrete_axial>" << eom;
		return false;
	}
	else if( aContainer->GetName() == "hash_masked_bits" )
	{
		aContainer->CopyTo( fObject, &KEMField::KElectrostaticBoundaryFieldWithKGeoBag::SetHashMaskedBits );
	}
	else if( aContainer->GetName() == "hash_threshold" )
	{
		aContainer->CopyTo( fObject, &KEMField::KElectrostaticBoundaryFieldWithKGeoBag::SetHashThreshold );
	}
	else if( aContainer->GetName() == "minimum_element_area" )
	{
		aContainer->CopyTo( fObject, &KEMField::KElectrostaticBoundaryFieldWithKGeoBag::SetMinimumElementArea);
	}
	else if( aContainer->GetName() == "maximum_element_aspect_ratio" )
	{
		aContainer->CopyTo( fObject, &KEMField::KElectrostaticBoundaryFieldWithKGeoBag::SetMaximumElementAspectRatio);
	}
	else return false;
	return true;
}

template< >
inline bool KElectrostaticBoundaryFieldBuilder::AddElement( KContainer* anElement )
{
	if(anElement->Is<KEMField::KChargeDensitySolver>())
	{
		if(!(fObject->GetChargeDensitySolver()) ){
			KEMField::KSmartPointer<KEMField::KChargeDensitySolver> solver =
					ReleaseToSmartPtr<KEMField::KChargeDensitySolver>(anElement);
			fObject->SetChargeDensitySolver(solver);
		}
		else{
			BINDINGMSG(eError) << "Cannot set more than one charge density solver for field " << GetName() << "!"<< eom;
		}
	}
	else if(anElement->Is<KEMField::KElectricFieldSolver>())
	{
		if(!(fObject->GetFieldSolver()) ){
			KEMField::KSmartPointer<KEMField::KElectricFieldSolver> solver =
					ReleaseToSmartPtr<KEMField::KElectricFieldSolver>(anElement);
			fObject->SetFieldSolver(solver);
		}
		else{
			BINDINGMSG(eError) << "Cannot set more than one field solver for field " << GetName() << "!"<< eom;
		}
	}
	else if(anElement->Is<KEMField::KElectrostaticBoundaryField::Visitor>())
	{
		KEMField::KSmartPointer<KEMField::KElectrostaticBoundaryField::Visitor> visitor =
				ReleaseToSmartPtr<KEMField::KElectrostaticBoundaryField::Visitor>(anElement);
		fObject->AddVisitor(visitor);
	}
	else return false;
	return true;
}

template< >
inline bool KElectrostaticBoundaryFieldBuilder::End() {
	if(!fObject->GetChargeDensitySolver() && !fObject->GetFieldSolver())
	{
		BINDINGMSG( eError ) << " No charge density solver and no field solver"
				" set in field " << GetName() << "!" << eom;
	}
	else if(!fObject->GetChargeDensitySolver())
	{
		BINDINGMSG( eError ) << " No charge density solver"
				" set in field " << GetName() << "!" << eom;
	}
	else if(!fObject->GetFieldSolver())
	{
		BINDINGMSG( eError ) << " No field solver"
				" set in field " << GetName() << "!" << eom;
	}
	else return true;
	return false;
}

} // katrin

#endif /* KEMFIELD_SOURCE_2_0_PLUGINS_BINDINGS_FIELDS_ELECTRIC_INCLUDE_KELECTROSTATICBOUNDARYFIELDBUILDER_HH_ */
