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

// FILE: MissileLauncherBuildingUpdate.h /////////////////////////////////////////////////////////////////////////
// Author: Matthew D. Campbell, April 2002
// Desc:   Update will change model state conditions based on special power state
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _MissileLauncherBuildingUpdate_H_
#define _MissileLauncherBuildingUpdate_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/AudioEventRTS.h"
#include "Common/INI.h"
#include "GameLogic/Module/SpecialPowerUpdateModule.h"
class DamageInfo;
class SpecialPowerTemplate;
class SpecialPowerModule;
class FXList;

//-------------------------------------------------------------------------------------------------
class MissileLauncherBuildingUpdateModuleData : public UpdateModuleData
{
public:
	// MG: Cannot apply offsetof to MissileLauncherBuildingUpdateModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		SpecialPowerTemplate *m_specialPowerTemplate;		///< pointer to the special power template
		UnsignedInt m_doorOpenTime;							///< in frames, time we should take to open the door
		UnsignedInt m_doorWaitOpenTime;					///< in frames, time we should leave the door open after firing the superweapon
		UnsignedInt m_doorClosingTime;					///< in frames, time it takes to close the door

		const FXList *m_openingFX;
		const FXList *m_openFX;
		const FXList *m_waitingToCloseFX;
		const FXList *m_closingFX;
		const FXList *m_closedFX;
	};

	IniData m_ini {};

	AudioEventRTS m_openIdleAudio {};

	MissileLauncherBuildingUpdateModuleData()
	{
		m_ini.m_specialPowerTemplate = NULL;
		m_ini.m_doorOpenTime = 0;
		m_ini.m_doorWaitOpenTime = 0;
		m_ini.m_doorClosingTime = 0;

		m_ini.m_openingFX = m_ini.m_openFX = m_ini.m_waitingToCloseFX = m_ini.m_closingFX = m_ini.m_closedFX = NULL;
	}

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		UpdateModuleData::buildFieldParse(what, p);

		static const FieldParse dataFieldParse[] = 
		{
			{ "SpecialPowerTemplate",	INI::parseSpecialPowerTemplate,										NULL, offsetof( MissileLauncherBuildingUpdateModuleData::IniData, m_specialPowerTemplate ) },
			{ "DoorOpenTime",			INI::parseDurationUnsignedInt,										NULL, offsetof( MissileLauncherBuildingUpdateModuleData::IniData, m_doorOpenTime ) },
			{ "DoorWaitOpenTime",		INI::parseDurationUnsignedInt,										NULL, offsetof( MissileLauncherBuildingUpdateModuleData::IniData, m_doorWaitOpenTime ) },
			{ "DoorCloseTime",			INI::parseDurationUnsignedInt,										NULL, offsetof( MissileLauncherBuildingUpdateModuleData::IniData, m_doorClosingTime ) },
			{ "DoorOpeningFX",			INI::parseFXList,													NULL, offsetof( MissileLauncherBuildingUpdateModuleData::IniData, m_openingFX ) },
			{ "DoorOpenFX",				INI::parseFXList,													NULL, offsetof( MissileLauncherBuildingUpdateModuleData::IniData, m_openFX ) },
			{ "DoorWaitingToCloseFX",	INI::parseFXList,													NULL, offsetof( MissileLauncherBuildingUpdateModuleData::IniData, m_waitingToCloseFX ) },
			{ "DoorClosingFX",			INI::parseFXList,													NULL, offsetof( MissileLauncherBuildingUpdateModuleData::IniData, m_closingFX ) },
			{ "DoorClosedFX",			INI::parseFXList,													NULL, offsetof( MissileLauncherBuildingUpdateModuleData::IniData, m_closedFX ) },
			{ "DoorOpenIdleAudio",		MissileLauncherBuildingUpdateModuleData::parseAudioEventRTS,		NULL, 0 },
			{ 0, 0, 0, 0 }
		};
		MissileLauncherBuildingUpdateModuleData* self {static_cast<MissileLauncherBuildingUpdateModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);
	}
private:
	// Proxy parse function to avoid offset problems:
	static void parseAudioEventRTS(INI* ini, void *instance, void* store, const void* userData);
};

//-------------------------------------------------------------------------------------------------
class MissileLauncherBuildingUpdate : public SpecialPowerUpdateModule
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( MissileLauncherBuildingUpdate, "MissileLauncherBuildingUpdate" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( MissileLauncherBuildingUpdate, MissileLauncherBuildingUpdateModuleData )

public:

	MissileLauncherBuildingUpdate( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	// No copies allowed!
	MissileLauncherBuildingUpdate(const MissileLauncherBuildingUpdate&) = delete;
	MissileLauncherBuildingUpdate& operator=(const MissileLauncherBuildingUpdate&) = delete;

	//SpecialPowerUpdateInterface pure virtual implementations
	virtual Bool initiateIntentToDoSpecialPower(const SpecialPowerTemplate *specialPowerTemplate, const Object *targetObj, const Coord3D *targetPos, const Waypoint *way, UnsignedInt commandOptions );
	virtual Bool isSpecialAbility() const { return false; }
	virtual Bool isSpecialPower() const { return true; }
	virtual Bool isActive() const { return m_doorState != m_timeoutState; }
	SpecialPowerTemplate* getTemplate() const;
	virtual Bool doesSpecialPowerHaveOverridableDestinationActive() const { return false; } //Is it active now?
	virtual Bool doesSpecialPowerHaveOverridableDestination() const { return false; }	//Does it have it, even if it's not active?
	virtual void setSpecialPowerOverridableDestination( const Coord3D* /* loc */ ) {}

	virtual SpecialPowerUpdateInterface* getSpecialPowerUpdateInterface() { return this; }
	virtual CommandOption getCommandOption() const { return (CommandOption)0; }

	virtual UpdateSleepTime update();	///< Deciding whether or not to make new guys
	virtual Bool isPowerCurrentlyInUse( const CommandButton *command = NULL ) const;

private:
	enum DoorStateType
	{
		DOOR_CLOSED,
		DOOR_OPENING,
		DOOR_OPEN,
		DOOR_WAITING_TO_CLOSE,
		DOOR_CLOSING,
	};
	
	void switchToState(DoorStateType dst);

	const SpecialPowerModuleInterface*	m_specialPowerModule {};
	DoorStateType						m_doorState {};
	DoorStateType						m_timeoutState {};
	UnsignedInt							m_timeoutFrame {};
	AudioEventRTS						m_openIdleAudio {};
};

#endif // _MissileLauncherBuildingUpdate_H_
