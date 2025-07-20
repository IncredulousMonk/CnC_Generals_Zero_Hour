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

// FILE: MoneyCrateCollide.h /////////////////////////////////////////////////////////////////////////
// Author: Graham Smallwood, March 2002
// Desc:   A crate that gives x money to the collider
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef MONEY_CRATE_COLLIDE_H_
#define MONEY_CRATE_COLLIDE_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/Module.h"
#include "GameLogic/Module/CrateCollide.h"
#include "GameLogic/Module/AutoDepositUpdate.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class Thing;
void parseUpgradePair( INI *ini, void *instance, void *store, const void *userData );

//-------------------------------------------------------------------------------------------------
class MoneyCrateCollideModuleData : public CrateCollideModuleData
{
public:
	// MG: Cannot apply offsetof to MoneyCrateCollideModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		UnsignedInt m_moneyProvided;
	};

	IniData m_ini {};

	std::list<upgradePair> m_upgradeBoost {};

	MoneyCrateCollideModuleData()
	{
		m_ini.m_moneyProvided = 0;
		m_upgradeBoost.clear();
	}

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		CrateCollideModuleData::buildFieldParse(what, p);

		static const FieldParse dataFieldParse[] = 
		{
			{ "MoneyProvided",	INI::parseUnsignedInt,	NULL, offsetof( MoneyCrateCollideModuleData::IniData, m_moneyProvided ) },
			{ "UpgradedBoost",	parseUpgradePair,		NULL, 0 },

			{ 0, 0, 0, 0 }
		};
		MoneyCrateCollideModuleData* self {static_cast<MoneyCrateCollideModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);

	}
};

//-------------------------------------------------------------------------------------------------
class MoneyCrateCollide : public CrateCollide
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( MoneyCrateCollide, "MoneyCrateCollide" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( MoneyCrateCollide, MoneyCrateCollideModuleData )

public:

	MoneyCrateCollide( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

protected:

	/// This is the game logic execution function that all real CrateCollides will implement
	virtual Bool executeCrateBehavior( Object *other );

	Int getUpgradedSupplyBoost( Object *other ) const;

};

#endif
