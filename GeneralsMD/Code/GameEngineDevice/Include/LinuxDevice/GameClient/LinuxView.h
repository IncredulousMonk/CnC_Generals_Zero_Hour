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

///// LinuxView.h ////////////////////////////////
// Matthew Gill, August 2025
//////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Linux implementation of the game view window.  View windows are literally
// a window into the game world that have width, height, and camera 
// controls for what to look at
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LINUXVIEW_H_
#define __LINUXVIEW_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////////////////////////
#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>

// USER INCLUDES //////////////////////////////////////////////////////////////////////////////////
#include "Common/STLTypedefs.h"
#include "Common/SubsystemInterface.h"
#include "GameClient/View.h"

// ------------------------------------------------------------------------------------------------
/** LinuxView implementation of the game view class.  This allows us to create
  * a "window" into the game world that can be sized and contains controls
  * for manipulating the camera */
// ------------------------------------------------------------------------------------------------
class LinuxView: public View, public SubsystemInterface
{
private:
   GLuint m_vaoTri {};
   GLuint m_vboTri {};
   GLuint m_progTri {};
public:
   LinuxView();
   ~LinuxView();

   virtual void init();       ///< init/re-init the LinuxView
   virtual void reset();
   virtual void drawView();   ///< draw this view
   virtual void updateView(); ///<called once per frame to determine the final camera and object transforms

   virtual void draw();       ///< draw this view
   virtual void update();     ///<called once per frame to determine the final camera and object transforms
};  // end class LinuxView

// EXTERNALS //////////////////////////////////////////////////////////////////////////////////////
extern Int TheLinuxFrameLengthInMsec;  // default is 33msec/frame == 30fps. but we may change it depending on sys config.

#endif  // end __LINUXVIEW_H_
