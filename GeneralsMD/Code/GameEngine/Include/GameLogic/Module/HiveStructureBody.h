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

// FILE: HiveStructureBody.h //////////////////////////////////////////////////////////////////////////
// Author: Kris Morness, October 2002
// Desc:	 Hive structure bodies are structure bodies with the ability to propagate specified
//         damage types to slaves when available. If there are no slaves, the the structure
//         will take the damage.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __HIVE_STRUCTURE_BODY_H
#define __HIVE_STRUCTURE_BODY_H

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "GameLogic/Module/StructureBody.h"
#include "GameLogic/Damage.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class Object;

class HiveStructureBodyModuleData : public StructureBodyModuleData
{
public:
	// MG: Cannot apply offsetof to HiveStructureBodyModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		DamageTypeFlags m_damageTypesToPropagateToSlaves;
		DamageTypeFlags m_damageTypesToSwallow;							///< A subset of the damage types to propagate. Do not take them ourselves
	};

	IniData m_ini {};

	HiveStructureBodyModuleData();

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		StructureBodyModuleData::buildFieldParse(what, p);
		static const FieldParse dataFieldParse[] = 
		{
			{ "PropagateDamageTypesToSlavesWhenExisting",	INI::parseDamageTypeFlags, NULL, offsetof( HiveStructureBodyModuleData::IniData, m_damageTypesToPropagateToSlaves ) },
			{ "SwallowDamageTypesIfSlavesNotExisting",		INI::parseDamageTypeFlags, NULL, offsetof( HiveStructureBodyModuleData::IniData, m_damageTypesToSwallow ) },
			{ 0, 0, 0, 0 }
		};
		HiveStructureBodyModuleData* self {static_cast<HiveStructureBodyModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);
	}
};


//-------------------------------------------------------------------------------------------------
/** Structure body module */
//-------------------------------------------------------------------------------------------------
class HiveStructureBody : public StructureBody
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( HiveStructureBody, "HiveStructureBody" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( HiveStructureBody, HiveStructureBodyModuleData )

public:

	HiveStructureBody( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

protected:

	virtual void attemptDamage( DamageInfo *damageInfo );		///< try to damage this object
};

#endif // __HIVE_STRUCTURE_BODY_H
