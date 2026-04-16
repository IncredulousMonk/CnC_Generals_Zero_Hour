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

// FILE: DefaultProductionExitUpdate.h /////////////////////////////////////////////////////////////////////////
// Author: Graham Smallwood, January, 2002
// Desc:		Hand off produced Units to me so I can Exit them into the world with my specific style
//					This instance simply spits the guy out at a point.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _DEFAULT_PRODUCTION_EXIT_UPDATE_H
#define _DEFAULT_PRODUCTION_EXIT_UPDATE_H

#include "GameLogic/Module/UpdateModule.h"
#include "Common/INI.h"
#include "Lib/BaseType.h"

class Object;

//-------------------------------------------------------------------------------------------------
class DefaultProductionExitUpdateModuleData : public UpdateModuleData
{
public:
	// MG: Cannot apply offsetof to DefaultProductionExitUpdateModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		Coord3D m_unitCreatePoint;
		Coord3D m_naturalRallyPoint;
		Bool	m_useSpawnRallyPoint;
	};

	IniData m_ini {};

	DefaultProductionExitUpdateModuleData()
	{
		m_ini.m_unitCreatePoint.zero();
		m_ini.m_naturalRallyPoint.zero();
		m_ini.m_useSpawnRallyPoint = false;
	}

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		UpdateModuleData::buildFieldParse(what, p);
		static const FieldParse dataFieldParse[] = 
		{
			{ "UnitCreatePoint",	INI::parseCoord3D,	NULL, offsetof( DefaultProductionExitUpdateModuleData::IniData, m_unitCreatePoint ) },
			{ "NaturalRallyPoint",	INI::parseCoord3D,	NULL, offsetof( DefaultProductionExitUpdateModuleData::IniData, m_naturalRallyPoint ) },
			{ "UseSpawnRallyPoint",	INI::parseBool,		NULL, offsetof( DefaultProductionExitUpdateModuleData::IniData, m_useSpawnRallyPoint ) },
			{ 0, 0, 0, 0 }
		};
		DefaultProductionExitUpdateModuleData* self {static_cast<DefaultProductionExitUpdateModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);
	}
};

//-------------------------------------------------------------------------------------------------
class DefaultProductionExitUpdate : public UpdateModule, public ExitInterface
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( DefaultProductionExitUpdate, "DefaultProductionExitUpdate" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( DefaultProductionExitUpdate, DefaultProductionExitUpdateModuleData )

public:

	virtual ExitInterface* getUpdateExitInterface() { return this; }

	DefaultProductionExitUpdate( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	// Required funcs to fufill interface requirements
	virtual Bool isExitBusy() const {return FALSE;}	///< Contain style exiters are getting the ability to space out exits, so ask this before reserveDoor as a kind of no-commitment check.
	virtual ExitDoorType reserveDoorForExit( const ThingTemplate* /* objType */, Object* /* specificObject */ ) { return DOOR_1; }
	virtual void exitObjectViaDoor( Object *newObj, ExitDoorType exitDoor );
	virtual void unreserveDoorForExit( ExitDoorType /* exitDoor */ ) { /* nothing */ }
	virtual void exitObjectByBudding( Object* /* newObj */, Object* /* budHost */ ) { return; }

	virtual void setRallyPoint( const Coord3D *pos );				///< define a "rally point" for units to move towards
	virtual const Coord3D *getRallyPoint( void ) const;			///< define a "rally point" for units to move towards
	virtual Bool useSpawnRallyPoint( void ) const;
	virtual Bool getNaturalRallyPoint( Coord3D& rallyPoint, Bool offset = TRUE ) const;			///< get the natural "rally point" for units to move towards
	virtual Bool getExitPosition( Coord3D& exitPosition ) const;					///< access to the "Door" position of the production object
	virtual UpdateSleepTime update()										{ return UPDATE_SLEEP_FOREVER; }

protected:

	Coord3D m_rallyPoint {};				///< Where units should move to after they have reached the "natural" rally point
	Bool m_rallyPointExists {};				///< Only move to the rally point if this is true

};

//-------------------------------------------------------------------------------------------------
inline void DefaultProductionExitUpdate::setRallyPoint( const Coord3D *pos )
{
	m_rallyPoint = *pos;
	m_rallyPointExists = true;
}

//-------------------------------------------------------------------------------------------------
inline const Coord3D *DefaultProductionExitUpdate::getRallyPoint( void ) const
{
	if (m_rallyPointExists)
		return &m_rallyPoint;

	return NULL;
}

//-------------------------------------------------------------------------------------------------
inline Bool DefaultProductionExitUpdate::useSpawnRallyPoint( void ) const
{
	// Check if the building has requested spawn units (like those that are airdropped)
	// to use the rally points of the building.
	if (getDefaultProductionExitUpdateModuleData()->m_ini.m_useSpawnRallyPoint)
		return TRUE;
	else
		return FALSE;
}

#endif
