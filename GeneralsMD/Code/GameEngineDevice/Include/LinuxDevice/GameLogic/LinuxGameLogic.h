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

// FILE: LinuxGameLogic.h /////////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, June 2025
//
// Linux game logic class, there shouldn't be a lot of new functionality
// in this class, but there are certain things that need to have close 
// knowledge of each other like the logical and visual terrain
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LINUXGAMELOGIC_H_
#define __LINUXGAMELOGIC_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "GameLogic/GameLogic.h"
#include "LinuxDevice/GameLogic/LinuxTerrainLogic.h"
// #include "LinuxDevice/GameLogic/LinuxGhostObject.h"

// class LinuxGhostObjectManager;
///////////////////////////////////////////////////////////////////////////////
// PROTOTYPES /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// LinuxGameLogic
//-----------------------------------------------------------------------------
/** Linux-specific functionality for game logic */
//-----------------------------------------------------------------------------
class LinuxGameLogic: public GameLogic
{

public:

protected:

   /// factory for TheTerrainLogic, called from init()
   virtual TerrainLogic *createTerrainLogic() { return NEW LinuxTerrainLogic; };
   // virtual GhostObjectManager *createGhostObjectManager(void) { return NEW LinuxGhostObjectManager; }

};  // end class LinuxGameLogic

#endif  // end __LINUXGAMELOGIC_H_
