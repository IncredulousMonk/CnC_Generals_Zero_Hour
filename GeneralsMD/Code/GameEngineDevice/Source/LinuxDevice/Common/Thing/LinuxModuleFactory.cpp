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

// FILE: LinuxModuleFactory.cpp ////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, June 2025
// Description: 
//   Linux game logic class, there shouldn't be a lot of new functionality
//   in this class, but there are certain things that need to have close
//   knowledge of each other like the logical and visual terrain.
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "LinuxDevice/Common/LinuxModuleFactory.h"
// #include "LinuxDevice/GameClient/Module/LinuxDebrisDraw.h"
#include "LinuxDevice/GameClient/Module/LinuxDefaultDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxDependencyModelDraw.h"
#include "LinuxDevice/GameClient/Module/LinuxModelDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxLaserDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxOverlordTankDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxOverlordTruckDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxOverlordAircraftDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxPoliceCarDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxProjectileStreamDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxRopeDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxSupplyDraw.h"
#include "LinuxDevice/GameClient/Module/LinuxScienceModelDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxTankDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxTruckDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxTankTruckDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxTracerDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxTreeDraw.h"
// #include "LinuxDevice/GameClient/Module/LinuxPropDraw.h"

//-------------------------------------------------------------------------------------------------
/** Initialize method */
//-------------------------------------------------------------------------------------------------
void LinuxModuleFactory::init()
{

   // extending functionality
   ModuleFactory::init();

   // add the specific module templates we need for the draw methods
   addModuleRename(LinuxDefaultDraw, W3DDefaultDraw);
   // addModule( LinuxDebrisDraw );
   addModuleRename(LinuxModelDraw, W3DModelDraw);
   // addModule( LinuxLaserDraw );
   // addModule( LinuxOverlordTankDraw );
   // addModule( LinuxOverlordTruckDraw );
   // addModule( LinuxOverlordAircraftDraw );
   // addModule( LinuxProjectileStreamDraw );
   // addModule( LinuxPoliceCarDraw );
   // addModule( LinuxRopeDraw );
   addModuleRename(LinuxScienceModelDraw, W3DScienceModelDraw);
   // addModule( LinuxSupplyDraw );
   // addModule( LinuxDependencyModelDraw );
   // addModule( LinuxTankDraw );
   // addModule( LinuxTruckDraw );
   // addModule( LinuxTracerDraw );
   // addModule( LinuxTankTruckDraw );
   // addModule( LinuxTreeDraw );
   // addModule( LinuxPropDraw );

}  // end init
