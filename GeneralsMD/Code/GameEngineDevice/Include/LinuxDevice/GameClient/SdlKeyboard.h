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

///// SdlKeyboard.h //////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __SDLKEYBOARD_H_
#define __SDLKEYBOARD_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////
#include <SDL3/SDL.h>
#include <deque>

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "GameClient/Keyboard.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////

// TYPE DEFINES ///////////////////////////////////////////////////////////////

// class SdlKeyboard ----------------------------------------------------------
/** Class for interfacing with the keyboard using SDL input as the
  * implementation */
//-----------------------------------------------------------------------------
class SdlKeyboard: public Keyboard {
public:

   SdlKeyboard();
   virtual ~SdlKeyboard();

   // extend methods from the base class
   virtual void init();          ///< initialize the keyboard, extending init functionality
   virtual void reset();         ///< Reset the keybaord system
   virtual void update();        ///< update call, extending update functionality
   virtual Bool getCapsState();  ///< get state of caps lock key, return TRUE if down

   void addEvent(const SDL_KeyboardEvent& event);
protected:

   // extended methods from the base class
   virtual void getKey(KeyboardIO* key);  ///< get a single key event

   //-----------------------------------------------------------------------------------------------

private:
   std::deque<SDL_KeyboardEvent> m_eventBuffer {};  ///< queue of pending SDL keyboard events

};  // end class SdlKeyboard

// INLINING ///////////////////////////////////////////////////////////////////

// EXTERNALS //////////////////////////////////////////////////////////////////
extern SdlKeyboard* TheSdlKeyboard;

#endif // __SDLKEYBOARD_H_
