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

///// SdlMouse.h /////////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __SDLMOUSE_H_
#define __SDLMOUSE_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////
#include <SDL3/SDL.h>
#include <deque>

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "GameClient/Mouse.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////

// TYPE DEFINES ///////////////////////////////////////////////////////////////

// class SdlMouse -------------------------------------------------------------
/** Class for interfacing with the mouse using SDL input as the implementation */
//-----------------------------------------------------------------------------
class SdlMouse: public Mouse
{

public:

   SdlMouse();
   virtual ~SdlMouse();

   virtual void init();                ///< init mouse, extend this functionality, do not replace
   virtual void reset();               ///< reset the system
   virtual void update();              ///< update
   virtual void initCursorResources(); ///< load windows resources needed for 2d cursors.

   virtual void setCursor( MouseCursor cursor );   ///< set mouse cursor
   virtual void capture();                         ///< capture the mouse
   virtual void releaseCapture();                  ///< release mouse capture

   virtual void setVisibility(Bool visible);

   void addEvent(const SDL_Event& event);
   void lostFocus (Bool state) { m_lostFocus = state;}

protected:

   /// get the next event available in the buffer
   virtual UnsignedByte getMouseEvent(MouseIO *result);

   /// translate a win32 mouse event to our own info
   void translateEvent(const SDL_Event& event, MouseIO *result);

   Int m_directionFrame {};   ///< current frame of directional cursor (frome 0 points up).
   Bool m_lostFocus {};       ///< flag if window has lost focus and mouse should stop being updated.

   private:
   std::deque<SDL_Event> m_eventBuffer {};  ///< queue of pending SDL keyboard events
};  // end SdlMouse 

// INLINING ///////////////////////////////////////////////////////////////////

// EXTERNALS //////////////////////////////////////////////////////////////////
extern SdlMouse* TheSdlMouse;

#endif // __SDLMOUSE_H_
