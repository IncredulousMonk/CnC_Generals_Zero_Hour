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


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE: HijackerUpdate.h /////////////////////////////////////////////////////////////////////////
// Author: Mark Lorenzen, July 2002
// Desc:   Allows hijacker to keep with his hijacked vehicle (though hidden) until it dies, then 
// to become a hijacker once more
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
 
#ifndef __HIJACKER_UPDATE_H
#define __HIJACKER_UPDATE_H

#include "GameLogic/Module/UpdateModule.h"

//-------------------------------------------------------------------------------------------------
class HijackerUpdateModuleData : public UpdateModuleData
{
public:
	// MG: Cannot apply offsetof to HijackerUpdateModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		AsciiString m_attachToBone;
		AsciiString m_parachuteName;
	};

	IniData m_ini {};

	//StickBombUpdateModuleData();

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		UpdateModuleData::buildFieldParse(what, p);
		static const FieldParse dataFieldParse[] = 
		{
			{ "AttachToTargetBone",	INI::parseAsciiString,		NULL, offsetof( HijackerUpdateModuleData::IniData, m_attachToBone ) },
			{ "ParachuteName",		INI::parseAsciiString,		NULL, offsetof( HijackerUpdateModuleData::IniData, m_parachuteName ) },
			{ 0, 0, 0, 0 }
		};
		HijackerUpdateModuleData* self {static_cast<HijackerUpdateModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);
	}
};

//-------------------------------------------------------------------------------------------------
class HijackerUpdate : public UpdateModule
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( HijackerUpdate, "HijackerUpdate" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( HijackerUpdate, HijackerUpdateModuleData )

public:

	HijackerUpdate( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	virtual UpdateSleepTime update();							///< called once per frame

	void setTargetObject( const Object *object );
	Object* getTargetObject() const;
	void setUpdate(Bool u ) {m_update = u;} 
	void setIsInVehicle(Bool i ) {m_isInVehicle = i;} 

private:

	ObjectID m_targetID;
	Coord3D	 m_ejectPos;
	Bool     m_update;
	Bool		 m_isInVehicle;
	Bool		 m_wasTargetAirborne;

//	DieModuleInterface *m_ejectPilotDMI; // point to ejectpilotdiemodule 
																			 // of target vehicle if it has one

};

#endif // __HIJACKER_UPDATE_H

