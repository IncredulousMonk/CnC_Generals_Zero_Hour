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

// FILE: LinuxRadar.cpp ///////////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, August 2025
// Description: 
//   Linux radar implementation.
//   This has the necessary device dependent drawing necessary for the radar.
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
// #include "Common/AudioEventRTS.h"
// #include "Common/Debug.h"
// #include "Common/GlobalData.h"
// #include "Common/Player.h"
// #include "Common/PlayerList.h"

// #include "GameLogic/TerrainLogic.h"
// #include "GameLogic/GameLogic.h"
// #include "GameLogic/Object.h"

// #include "GameClient/Color.h"
// #include "GameClient/Display.h"
// #include "GameClient/GameClient.h"
// #include "GameClient/GameWindow.h"
// #include "GameClient/Image.h"
// #include "GameClient/Line2D.h"
// #include "GameClient/TerrainVisual.h"
// #include "GameClient/Water.h"
#include "LinuxDevice/Common/LinuxRadar.h"
// #include "W3DDevice/GameClient/HeightMap.h"
// #include "W3DDevice/GameClient/W3DShroud.h"
// #include "WW3D2/Texture.h"
// #include "WW3D2/DX8Caps.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxRadar::LinuxRadar()
{

   DEBUG_LOG(("Note: LinuxRadar currently is a stripped-down dummy class!\n"));

}  // end LinuxRadar

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxRadar::~LinuxRadar()
{

}  // end ~LinuxRadar

//-------------------------------------------------------------------------------------------------
/** Radar initialization */
//-------------------------------------------------------------------------------------------------
void LinuxRadar::init()
{
}  // end init

//-------------------------------------------------------------------------------------------------
/** Reset the radar to the initial empty state ready for new data */
//-------------------------------------------------------------------------------------------------
void LinuxRadar::reset()
{
}  // end reset

//-------------------------------------------------------------------------------------------------
/** Update */
//-------------------------------------------------------------------------------------------------
void LinuxRadar::update()
{
}  // end update

//-------------------------------------------------------------------------------------------------
/** Reset the radar for the new map data being given to it */
//-------------------------------------------------------------------------------------------------
void LinuxRadar::newMap(TerrainLogic *terrain)
{
(void) terrain;
}  // end newMap

// ------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxRadar::clearShroud()
{
}

// ------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxRadar::setShroudLevel(Int shroudX, Int shroudY, CellShroudStatus setting)
{
(void) shroudX;
(void) shroudY;
(void) setting;
}

//-------------------------------------------------------------------------------------------------
/** Actually draw the radar at the screen coordinates provided 
   * NOTE about how drawing works: The radar images are computed at samples across the
   * map and are built into a "square" texture area.  At the time of drawing and computing
   * radar<->world coords we consider the "ratio" of width to height of the map dimensions
   * so that when we draw we preserve the aspect ratio of the map and don't squish it in
   * any direction that would cause the map to be distorted.  Extra blank space is drawn
   * around the radar images to keep the whole radar area covered when the map displayed
   * is "long" or "tall" */
//-------------------------------------------------------------------------------------------------
void LinuxRadar::draw(Int pixelX, Int pixelY, Int width, Int height)
{
(void) pixelX;
(void) pixelY;
(void) width;
(void) height;
}  // end draw
