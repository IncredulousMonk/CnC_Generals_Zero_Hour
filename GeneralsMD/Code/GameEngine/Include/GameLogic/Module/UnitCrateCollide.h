/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: UnitCrateCollide.h /////////////////////////////////////////////////////////////////////////
// Author: Graham Smallwood, March 2002
// Desc:   A crate that gives n units of type m the the pickerupper
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef UNIT_CRATE_COLLIDE_H_
#define UNIT_CRATE_COLLIDE_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/Module.h"
#include "GameLogic/Module/CrateCollide.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class Thing;

//-------------------------------------------------------------------------------------------------
class UnitCrateCollideModuleData : public CrateCollideModuleData
{
public:
	// MG: Cannot apply offsetof to UnitCrateCollideModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		UnsignedInt m_unitCount;
		AsciiString m_unitType;
	};

	IniData m_ini {};

	UnitCrateCollideModuleData()
	{
		m_ini.m_unitCount = 0;
		m_ini.m_unitType = "";
	}

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		CrateCollideModuleData::buildFieldParse(what, p);

		static const FieldParse dataFieldParse[] = 
		{
			{ "UnitCount",	INI::parseUnsignedInt,	NULL, offsetof( UnitCrateCollideModuleData::IniData, m_unitCount ) },
			{ "UnitName",	INI::parseAsciiString,	NULL, offsetof( UnitCrateCollideModuleData::IniData, m_unitType ) },
			{ 0, 0, 0, 0 }
		};
		UnitCrateCollideModuleData* self {static_cast<UnitCrateCollideModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);

	}
};

//-------------------------------------------------------------------------------------------------
class UnitCrateCollide : public CrateCollide
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( UnitCrateCollide, "UnitCrateCollide" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( UnitCrateCollide, UnitCrateCollideModuleData )

public:

	UnitCrateCollide( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	/// This is the game logic execution function that all real CrateCollides will implement
	virtual Bool executeCrateBehavior( Object *other );
protected:

};

#endif
