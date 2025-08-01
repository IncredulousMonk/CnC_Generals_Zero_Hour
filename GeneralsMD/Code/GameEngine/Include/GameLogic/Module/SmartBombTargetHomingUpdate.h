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

// FILE: SmartBombTargetHomingUpdate.h /////////////////////////////////////////////////////////////////////////
// Author: Mark Lorenzen, July 2003
// Desc:   Update that will fudge a falling object's position just slightly, to make it find its target better
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __SMARTBOMB_UPDATE_H_
#define __SMARTBOMB_UPDATE_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "GameLogic/Module/UpdateModule.h"

//-------------------------------------------------------------------------------------------------
class SmartBombTargetHomingUpdateModuleData : public UpdateModuleData
{
public:
	// MG: Cannot apply offsetof to SmartBombTargetHomingUpdateModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		Real m_courseCorrectionScalar;
	};

	IniData m_ini {};

	SmartBombTargetHomingUpdateModuleData()
	{
		m_ini.m_courseCorrectionScalar = 0.99f;
	}

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		UpdateModuleData::buildFieldParse(what, p);
		static const FieldParse dataFieldParse[] = 
		{
			{ "CourseCorrectionScalar", INI::parseReal, NULL, offsetof( SmartBombTargetHomingUpdateModuleData::IniData, m_courseCorrectionScalar ) },
			{ 0, 0, 0, 0 }
		};
		SmartBombTargetHomingUpdateModuleData* self {static_cast<SmartBombTargetHomingUpdateModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);
	}
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class SmartBombTargetHomingUpdate : public UpdateModule
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( SmartBombTargetHomingUpdate, "SmartBombTargetHomingUpdate" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( SmartBombTargetHomingUpdate, SmartBombTargetHomingUpdateModuleData )

public:

	SmartBombTargetHomingUpdate( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	void SetTargetPosition( const Coord3D& target );

	virtual UpdateSleepTime update( void );

protected:


	Bool	m_targetReceived {};
	Coord3D	m_target {};

};

#endif // __SMARTBOMB_UPDATE_H_
