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

// FILE: AutoDepositUpdate.h /////////////////////////////////////////////////
//-----------------------------------------------------------------------------
//                                                                          
//                       Electronic Arts Pacific.                          
//                                                                          
//                       Confidential Information                           
//                Copyright (C) 2002 - All Rights Reserved                  
//                                                                          
//-----------------------------------------------------------------------------
//
//	created:	Aug 2002
//
//	Filename: 	AutoDepositUpdate.h
//
//	author:		Chris Huybregts
//	
//	purpose:	Auto Deposit Update Module
//
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __AUTO_DEPOSIT_UPDATE_H_
#define __AUTO_DEPOSIT_UPDATE_H_

//-----------------------------------------------------------------------------
// SYSTEM INCLUDES ////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// USER INCLUDES //////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
#include "GameLogic/Module/UpdateModule.h"
//-----------------------------------------------------------------------------
// FORWARD REFERENCES /////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
class Player;
class Thing;
void parseUpgradeBoost( INI *ini, void *instance, void *store, const void *userData );
struct upgradePair
{
	std::string type {};
	Int         amount {};
};

//-----------------------------------------------------------------------------
// TYPE DEFINES ///////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
class AutoDepositUpdateModuleData : public UpdateModuleData
{
public:

	// MG: Cannot apply offsetof to AutoDepositUpdateModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		UnsignedInt m_depositFrame;
		Int m_depositAmount;
		Int m_initialCaptureBonus;
		Bool m_isActualMoney;
		AutoDepositUpdateModuleData* m_obj {}; ///< pointer to the parent object
	};

	IniData m_ini {};

	std::list<upgradePair> m_upgradeBoost {};

	AutoDepositUpdateModuleData()
	{
		m_ini.m_depositFrame = 0;
		m_ini.m_depositAmount = 0;
		m_ini.m_initialCaptureBonus = 0;
		m_ini.m_isActualMoney = TRUE;
		m_ini.m_obj = this;
		m_upgradeBoost.clear();
	}

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		UpdateModuleData::buildFieldParse(what, p);
		static const FieldParse dataFieldParse[] = 
		{
			{ "DepositTiming",			INI::parseDurationUnsignedInt,	NULL, offsetof( AutoDepositUpdateModuleData::IniData, m_depositFrame ) },
			{ "DepositAmount",			INI::parseInt,					NULL, offsetof( AutoDepositUpdateModuleData::IniData, m_depositAmount ) },
			{ "InitialCaptureBonus",	INI::parseInt,					NULL, offsetof( AutoDepositUpdateModuleData::IniData, m_initialCaptureBonus ) },
			{ "ActualMoney",			INI::parseBool,					NULL, offsetof( AutoDepositUpdateModuleData::IniData, m_isActualMoney ) },
			{ "UpgradedBoost",			parseUpgradeBoost,				NULL, 0 },
			{ 0, 0, 0, 0 }
		};
		AutoDepositUpdateModuleData* self {static_cast<AutoDepositUpdateModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);
	}
};


//-------------------------------------------------------------------------------------------------
class AutoDepositUpdate : public UpdateModule
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( AutoDepositUpdate, "AutoDepositUpdate" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( AutoDepositUpdate, AutoDepositUpdateModuleData )

public:

	AutoDepositUpdate( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	void awardInitialCaptureBonus( Player *player );	// Test and award the initial capture bonus
	virtual UpdateSleepTime update( void );

protected:

	Int getUpgradedSupplyBoost() const;

	UnsignedInt m_depositOnFrame;
	Bool m_awardInitialCaptureBonus;
	Bool m_initialized;

};


//-----------------------------------------------------------------------------
// INLINING ///////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EXTERNALS //////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------

#endif // __AUTO_DEPOSIT_UPDATE_H_
