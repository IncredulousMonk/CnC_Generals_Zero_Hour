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

// FILE: DieModule.h /////////////////////////////////////////////////////////////////////////////////
// Author: Colin Day, September 2001
// Desc:	 
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __DieModule_H_
#define __DieModule_H_

#include "Common/Module.h"
#include "GameLogic/Damage.h"
#include "GameLogic/Module/BehaviorModule.h"
#include "Common/ObjectStatusTypes.h"

//-------------------------------------------------------------------------------------------------
/** OBJECT DIE MODULE base class */
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
class DieModuleInterface
{
public:
	virtual void onDie( const DamageInfo *damageInfo ) = 0;
	virtual ~DieModuleInterface() {}
};

//-------------------------------------------------------------------------------------------------
class DieMuxData	// does NOT inherit from ModuleData.
{
public:
	DeathTypeFlags			m_deathTypes {};
	VeterancyLevelFlags		m_veterancyLevels {};
	ObjectStatusMaskType	m_exemptStatus {};						///< die module is ignored if any of these status bits are set
	ObjectStatusMaskType	m_requiredStatus {};					///< die module is ignored if any of these status bits are clear

	DieMuxData();
	static const FieldParse* getFieldParse();

	Bool isDieApplicable(const Object* obj, const DamageInfo *damageInfo) const;
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
class DieModuleData : public BehaviorModuleData
{
public:
	// MG: Cannot apply offsetof to DieModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		DieMuxData m_dieMuxData;
	};

	IniData m_ini {};

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		BehaviorModuleData::buildFieldParse(what, p);
		DieModuleData* self {static_cast<DieModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(DieMuxData::getFieldParse(), offset + offsetof( DieModuleData::IniData, m_dieMuxData ));
	}

	inline Bool isDieApplicable(const Object* obj, const DamageInfo *damageInfo) const { return m_ini.m_dieMuxData.isDieApplicable(obj, damageInfo); }
};

//-------------------------------------------------------------------------------------------------
class DieModule : public BehaviorModule, public DieModuleInterface
{

	MEMORY_POOL_GLUE_ABC( DieModule )
	MAKE_STANDARD_MODULE_MACRO_ABC( DieModule )
	MAKE_STANDARD_MODULE_DATA_MACRO_ABC(DieModule, DieModuleData)

public:

	DieModule( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype defined by MemoryPoolObject

	static Int getInterfaceMask() { return MODULEINTERFACE_DIE; }

	// BehaviorModule
	virtual DieModuleInterface* getDie() { return this; }

	void onDie( const DamageInfo *damageInfo ) = 0;

protected:
	Bool isDieApplicable(const DamageInfo *damageInfo) const { return getDieModuleData()->isDieApplicable(getObject(), damageInfo); }
	
};
inline DieModule::DieModule( Thing *thing, const ModuleData* moduleData ) : BehaviorModule( thing, moduleData ) { }
inline DieModule::~DieModule() { }

#endif
