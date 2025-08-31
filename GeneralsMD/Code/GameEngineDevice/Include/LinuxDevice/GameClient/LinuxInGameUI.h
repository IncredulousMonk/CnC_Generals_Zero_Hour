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

///// LinuxInGameUI.h ////////////////////////////
// Matthew Gill, August 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXINGAMEUI_H_
#define __LINUXINGAMEUI_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "GameClient/InGameUI.h"
#include "GameClient/View.h"
#include "LinuxDevice/GameClient/LinuxView.h"
// #include "WW3D2/Render2D.h"
// #include "WW3D2/RendObj.h"
// #include "WW3D2/Line3D.h"

class HAnimClass;

// LinuxInGameUI --------------------------------------------------------------
/** Implementation for the Linux game user interface.  This singleton is
  * responsible for all user interaction and feedback as part of the
  * user interface */
//-----------------------------------------------------------------------------
class LinuxInGameUI: public InGameUI {

public:

   LinuxInGameUI();
   virtual ~LinuxInGameUI();

   // Inherited from subsystem interface -----------------------------------------------------------
   virtual void init();    ///< Initialize the in-game user interface
   virtual void update();  ///< Update the UI by calling preDraw(), draw(), and postDraw()
   virtual void reset();   ///< Reset
   //-----------------------------------------------------------------------------------------------

   virtual void draw();    ///< Render the in-game user interface

protected:

   /// factory for views
   virtual View *createView( void ) { return NEW LinuxView; }

   // virtual void drawSelectionRegion();          ///< draw the selection region on screen
   // virtual void drawMoveHints(View* view);      ///< draw move hint visual feedback
   // virtual void drawAttackHints(View* view);    ///< draw attack hint visual feedback
   // virtual void drawPlaceAngle(View* view);     ///< draw place building angle if needed

   // RenderObjClass *m_moveHintRenderObj[ MAX_MOVE_HINTS ];
   // HAnimClass *m_moveHintAnim[ MAX_MOVE_HINTS ];
   // RenderObjClass *m_buildingPlacementAnchor;
   // RenderObjClass *m_buildingPlacementArrow;

};  // end class LinuxInGameUI

#endif  // end __LINUXINGAMEUI_H_
