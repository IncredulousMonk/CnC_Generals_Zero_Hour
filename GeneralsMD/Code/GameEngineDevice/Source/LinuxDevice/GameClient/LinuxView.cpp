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

///// LinuxView.cpp //////////////////////////////
// Matthew Gill, August 2025
//////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Linux implementation of the game view window.  View windows are literally
// a window into the game world that have width, height, and camera 
// controls for what to look at
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

// USER INCLUDES //////////////////////////////////////////////////////////////////////////////////
#include "LinuxDevice/GameClient/LinuxDisplay.h"
#include "LinuxDevice/GameClient/LinuxView.h"

// 30 fps
Int TheLinuxFrameLengthInMsec = 1000 / LOGICFRAMES_PER_SECOND; // default is 33msec/frame == 30fps. but we may change it depending on sys config.



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxView::LinuxView()
{
}  // end LinuxView

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxView::~LinuxView()
{
}  // end ~LinuxView

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::init()
{
   // extend View functionality
   View::init();
   setName("LinuxView");
}  // end init

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::reset()
{
   View::reset();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::updateView()
{
   UPDATE();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::update()
{
}

//-------------------------------------------------------------------------------------------------
/** Draw member for the Linux window, this will literally draw the window 
  * for this view */
//-------------------------------------------------------------------------------------------------
void LinuxView::drawView()
{
   DRAW();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::draw()
{
}
