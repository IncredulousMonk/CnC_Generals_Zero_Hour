/*
** Command & Conquer Generals Zero Hour(tm)
** Copyright 2025 Electronic Arts Inc.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// FILE: LinuxScienceModelDraw.h //////////////////////////////////////////////////////////////////
// Author: Matthew Gill, July 2025
// Description: 
//   Draw module just like Model, except it only draws if the local player has the specified
//   science.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _LINUX_SCIENCE_MODEL_DRAW_H_
#define _LINUX_SCIENCE_MODEL_DRAW_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "LinuxDevice/GameClient/Module/LinuxModelDraw.h"

enum ScienceType;

//-------------------------------------------------------------------------------------------------
class LinuxScienceModelDrawModuleData: public LinuxModelDrawModuleData
{
public:
	// MG: Cannot apply offsetof to LinuxScienceModelDrawModuleData, so had to move data into an embedded struct.
	struct IniData
	{
      ScienceType m_requiredScience {}; ///< Local player must have this science for me to ever draw
	};

	IniData m_ini {};

   LinuxScienceModelDrawModuleData();
   ~LinuxScienceModelDrawModuleData();
   static void buildFieldParse(void* what, MultiIniFieldParse& p);
};

//-------------------------------------------------------------------------------------------------
class LinuxScienceModelDraw: public LinuxModelDraw
{

   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(LinuxScienceModelDraw, "LinuxScienceModelDraw")
   MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA(LinuxScienceModelDraw, LinuxScienceModelDrawModuleData)

public:

   LinuxScienceModelDraw(Thing* thing, const ModuleData* moduleData);
   // virtual destructor prototype provided by memory pool declaration
   virtual void doDrawModule(const Matrix3D* transformMtx);///< checks a property on the local player before passing this up

protected:
};

#endif

