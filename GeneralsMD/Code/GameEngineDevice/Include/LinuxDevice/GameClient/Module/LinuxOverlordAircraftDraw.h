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

// FILE: LinuxOverlordAircraftDraw.h //////////////////////////////////////////////////////////////
// Author: Matthew Gill, February 2026
// Description: 
//  Units that receive portable structure upgrades (like the Overlord Tank) have a super specific special need.
//  He needs his rider to draw explicitly after him,
//  and he needs direct access to get that rider when everyone else can't see it because of the OverlordContain.
//  In the case of aircraft (Helix, SpectreGunship, etc.) we need this draw module which mimics the OverlordTankDraw
//  but does not draw treads, trackmarks, turrets, etc. Whee!
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _LINUX_OVERLORD_AIRCRAFT_DRAW_H_
#define _LINUX_OVERLORD_AIRCRAFT_DRAW_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "LinuxDevice/GameClient/Module/LinuxModelDraw.h"

//-------------------------------------------------------------------------------------------------
class LinuxOverlordAircraftDrawModuleData: public LinuxModelDrawModuleData
{
public:
   LinuxOverlordAircraftDrawModuleData();
   ~LinuxOverlordAircraftDrawModuleData();
   static void buildFieldParse(void* what, MultiIniFieldParse& p);
};

//-------------------------------------------------------------------------------------------------
class LinuxOverlordAircraftDraw: public LinuxModelDraw
{

   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( LinuxOverlordAircraftDraw, "LinuxOverlordAircraftDraw" )
   MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( LinuxOverlordAircraftDraw, LinuxOverlordAircraftDrawModuleData )

public:

   LinuxOverlordAircraftDraw( Thing *thing, const ModuleData* moduleData );
   // virtual destructor prototype provided by memory pool declaration

   virtual void setHidden(Bool h);
   virtual void doDrawModule(const Matrix3D* transformMtx);

protected:

};

#endif
