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

// FILE: DeletionUpdate.h /////////////////////////////////////////////////////////////////////////
// Author: Graham Smallwood, August 2002
// Desc:   Update that will count down a lifetime and destroy object when it reaches zero
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __DELETION_UPDATE_H_
#define __DELETION_UPDATE_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "GameLogic/Module/UpdateModule.h"

//-------------------------------------------------------------------------------------------------
class DeletionUpdateModuleData : public UpdateModuleData
{
public:
	// MG: Cannot apply offsetof to DeletionUpdateModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		UnsignedInt m_minFrames;
		UnsignedInt m_maxFrames;
	};

	IniData m_ini {};

	DeletionUpdateModuleData()
	{
		m_ini.m_minFrames = 0.0f;
		m_ini.m_maxFrames = 0.0f;
	}

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		UpdateModuleData::buildFieldParse(what, p);
		static const FieldParse dataFieldParse[] = 
		{
			{ "MinLifetime",					INI::parseDurationUnsignedInt,		NULL, offsetof( DeletionUpdateModuleData::IniData, m_minFrames ) },
			{ "MaxLifetime",					INI::parseDurationUnsignedInt,		NULL, offsetof( DeletionUpdateModuleData::IniData, m_maxFrames ) },
			{ 0, 0, 0, 0 }
		};
		DeletionUpdateModuleData* self {static_cast<DeletionUpdateModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);
	}
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class DeletionUpdate : public UpdateModule
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( DeletionUpdate, "DeletionUpdate" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( DeletionUpdate, DeletionUpdateModuleData )

public:

	DeletionUpdate( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	void setLifetimeRange( UnsignedInt minFrames, UnsignedInt maxFrames );
	UnsignedInt getDieFrame() { return m_dieFrame; }

	virtual UpdateSleepTime update( void );

protected:

	UnsignedInt calcSleepDelay(UnsignedInt minFrames, UnsignedInt maxFrames);

	UnsignedInt m_dieFrame {};			///< frame we die on

};

#endif

