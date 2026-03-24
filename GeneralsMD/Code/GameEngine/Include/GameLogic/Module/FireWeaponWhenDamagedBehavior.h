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

// FILE: FireWeaponWhenDamagedBehavior.h /////////////////////////////////////////////////////////////////////////
// Author: Steven Johnson, June 2002
// Desc:   Update that will count down a lifetime and destroy object when it reaches zero
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __FireWeaponWhenDamagedBehavior_H_
#define __FireWeaponWhenDamagedBehavior_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "GameLogic/Module/BehaviorModule.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/DamageModule.h"
#include "GameLogic/Weapon.h"

//-------------------------------------------------------------------------------------------------
class FireWeaponWhenDamagedBehaviorModuleData : public UpdateModuleData
{
public:
	// MG: Cannot apply offsetof to FireWeaponWhenDamagedBehaviorModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		UpgradeMuxData			m_upgradeMuxData;
		Bool					m_initiallyActive;
		DamageTypeFlags			m_damageTypes;
		Real					m_damageAmount;
		const WeaponTemplate*	m_reactionWeaponPristine;	///< fire these weapons only when damage is received
		const WeaponTemplate*	m_reactionWeaponDamaged;
		const WeaponTemplate*	m_reactionWeaponReallyDamaged;
		const WeaponTemplate*	m_reactionWeaponRubble;
		const WeaponTemplate*	m_continuousWeaponPristine;	///< fire these weapons continuously, versus just onDamage 
		const WeaponTemplate*	m_continuousWeaponDamaged;
		const WeaponTemplate*	m_continuousWeaponReallyDamaged;
		const WeaponTemplate*	m_continuousWeaponRubble;
	};

	IniData m_ini {};

	FireWeaponWhenDamagedBehaviorModuleData()
	{
		m_ini.m_initiallyActive = false;
		m_ini.m_reactionWeaponPristine = NULL;
		m_ini.m_reactionWeaponDamaged = NULL;
		m_ini.m_reactionWeaponReallyDamaged = NULL;
		m_ini.m_reactionWeaponRubble = NULL;
		m_ini.m_continuousWeaponPristine = NULL; 
		m_ini.m_continuousWeaponDamaged = NULL;	
		m_ini.m_continuousWeaponReallyDamaged = NULL;	
		m_ini.m_continuousWeaponRubble = NULL;	
		m_ini.m_damageTypes = DAMAGE_TYPE_FLAGS_ALL;
		m_ini.m_damageAmount = 0;
	}


	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		static const FieldParse dataFieldParse[] = 
		{
			{ "StartsActive",					INI::parseBool,				NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_initiallyActive ) },
			{ "ReactionWeaponPristine",			INI::parseWeaponTemplate,	NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_reactionWeaponPristine) },
			{ "ReactionWeaponDamaged",			INI::parseWeaponTemplate,	NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_reactionWeaponDamaged) },
			{ "ReactionWeaponReallyDamaged",	INI::parseWeaponTemplate,	NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_reactionWeaponReallyDamaged) },
			{ "ReactionWeaponRubble",			INI::parseWeaponTemplate,	NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_reactionWeaponRubble) },
			{ "ContinuousWeaponPristine",		INI::parseWeaponTemplate,	NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_continuousWeaponPristine) },
			{ "ContinuousWeaponDamaged",		INI::parseWeaponTemplate,	NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_continuousWeaponDamaged) },
			{ "ContinuousWeaponReallyDamaged",	INI::parseWeaponTemplate,	NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_continuousWeaponReallyDamaged) },
			{ "ContinuousWeaponRubble",			INI::parseWeaponTemplate,	NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_continuousWeaponRubble) },
			{ "DamageTypes",					INI::parseDamageTypeFlags,	NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_damageTypes ) },
			{ "DamageAmount",					INI::parseReal,				NULL, offsetof(FireWeaponWhenDamagedBehaviorModuleData::IniData, m_damageAmount ) },
			{ 0, 0, 0, 0 }
		};

		UpdateModuleData::buildFieldParse(what, p);
		FireWeaponWhenDamagedBehaviorModuleData* self {static_cast<FireWeaponWhenDamagedBehaviorModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);
		p.add(UpgradeMuxData::getFieldParse(), offset + offsetof( FireWeaponWhenDamagedBehaviorModuleData::IniData, m_upgradeMuxData ));
	}


private:

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class FireWeaponWhenDamagedBehavior : public UpdateModule, 
																			public UpgradeMux, 
																			public DamageModuleInterface
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( FireWeaponWhenDamagedBehavior, "FireWeaponWhenDamagedBehavior" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( FireWeaponWhenDamagedBehavior, FireWeaponWhenDamagedBehaviorModuleData )

public:

	FireWeaponWhenDamagedBehavior( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	// No copies allowed!
	FireWeaponWhenDamagedBehavior(const FireWeaponWhenDamagedBehavior&) = delete;
	FireWeaponWhenDamagedBehavior& operator=(const FireWeaponWhenDamagedBehavior&) = delete;

	// module methids
	static Int getInterfaceMask() { return UpdateModule::getInterfaceMask() | (MODULEINTERFACE_UPGRADE) | (MODULEINTERFACE_DAMAGE); }

	// BehaviorModule
	virtual UpgradeModuleInterface* getUpgrade() { return this; }
	virtual DamageModuleInterface* getDamage() { return this; }

	// DamageModuleInterface
	virtual void onDamage( DamageInfo* damageInfo );
	virtual void onHealing( DamageInfo* /* damageInfo */ ) { }
	virtual void onBodyDamageStateChange(const DamageInfo* /* damageInfo */, BodyDamageType /* oldState */, BodyDamageType /* newState */) { }

	// UpdateModuleInterface
	virtual UpdateSleepTime update();

protected:

	virtual void upgradeImplementation() 
	{ 
		setWakeFrame(getObject(), UPDATE_SLEEP_NONE);
	}

	virtual void getUpgradeActivationMasks(UpgradeMaskType& activation, UpgradeMaskType& conflicting) const
	{
		getFireWeaponWhenDamagedBehaviorModuleData()->m_ini.m_upgradeMuxData.getUpgradeActivationMasks(activation, conflicting);
	}

	virtual void performUpgradeFX()
	{
		getFireWeaponWhenDamagedBehaviorModuleData()->m_ini.m_upgradeMuxData.performUpgradeFX(getObject());
	}

	virtual void processUpgradeRemoval()
	{
		// I can't take it any more.  Let the record show that I think the UpgradeMux multiple inheritence is CRAP.
		getFireWeaponWhenDamagedBehaviorModuleData()->m_ini.m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject());
	}

	virtual Bool requiresAllActivationUpgrades() const
	{
		return getFireWeaponWhenDamagedBehaviorModuleData()->m_ini.m_upgradeMuxData.m_requiresAllTriggers;
	}

	inline Bool isUpgradeActive() const { return isAlreadyUpgraded(); }
	
	virtual Bool isSubObjectsUpgrade() { return false; }

private:
	Weapon *m_reactionWeaponPristine {};
	Weapon *m_reactionWeaponDamaged {};
	Weapon *m_reactionWeaponReallyDamaged {};
	Weapon *m_reactionWeaponRubble {};
	Weapon *m_continuousWeaponPristine {};
	Weapon *m_continuousWeaponDamaged {};
	Weapon *m_continuousWeaponReallyDamaged {};
	Weapon *m_continuousWeaponRubble {};

};

#endif // __FireWeaponWhenDamagedBehavior_H_

