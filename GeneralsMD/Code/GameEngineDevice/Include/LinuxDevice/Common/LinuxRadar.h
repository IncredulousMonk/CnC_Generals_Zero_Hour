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

// FILE: LinuxRadar.h /////////////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, August 2025
// Description: 
//   Linux radar implementation.
//   This has the necessary device dependent drawing necessary for the radar.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LINUXRADAR_H_
#define __LINUXRADAR_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/Radar.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class TerrainLogic;

class LinuxRadar: public Radar
{

public:

   LinuxRadar();
   ~LinuxRadar();

   virtual void init();                                           ///< subsystem init
   virtual void update();                                         ///< subsystem update
   virtual void reset();                                          ///< subsystem reset

   virtual void newMap(TerrainLogic *terrain);                    ///< reset radar for new map

   void draw(Int pixelX, Int pixelY, Int width, Int height);      ///< draw the radar

   virtual void clearShroud();
   virtual void setShroudLevel(Int x, Int y, CellShroudStatus setting);

};


#endif // __LINUXRADAR_H_
