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

///// SdlKeyboard.cpp ////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#include "Common/Debug.h"
#include "Common/Language.h"
#include "GameClient/KeyDefs.h"
#include "GameClient/Keyboard.h"
#include "LinuxDevice/GameClient/SdlKeyboard.h"
#include <cassert>

/// The SdlKeyboard singleton instance
SdlKeyboard* TheSdlKeyboard {nullptr};

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
/** Add an SDL keyboard event to the buffer */
//-------------------------------------------------------------------------------------------------
void SdlKeyboard::addEvent(const SDL_KeyboardEvent& event)
{
   m_eventBuffer.push_back(event);
}

//-------------------------------------------------------------------------------------------------
/** Get a single keyboard event from direct input */
//-------------------------------------------------------------------------------------------------
void SdlKeyboard::getKey(KeyboardIO* key)
{
   assert(key);
   key->sequence = 0;
   key->key = KEY_NONE;

   if (m_eventBuffer.empty()) {
      return;
   }

   SDL_KeyboardEvent& event {m_eventBuffer.front()};

   // set the key
   key->key = static_cast<UnsignedShort>(event.scancode);

   //
   // state of key, note we are setting the key state here with an assignment
   // and not a bit set of the up/down state, this is the "start"
   // of building this "key"
   //
   key->state = ((event.type == SDL_EVENT_KEY_DOWN) ? KEY_STATE_DOWN : KEY_STATE_UP);

   // set status as unused (unprocessed)
   key->status = KeyboardIO::STATUS_UNUSED;

   m_eventBuffer.pop_back();
}  // end getKey

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
SdlKeyboard::SdlKeyboard()
{

   if ((SDL_GetModState() & SDL_KMOD_CAPS) == SDL_KMOD_CAPS) {
      m_modifiers |= KEY_STATE_CAPSLOCK;
      DEBUG_LOG(("Caps lock is on!\n"));
   } else {
      m_modifiers &= static_cast<UnsignedShort>(~KEY_STATE_CAPSLOCK);
   }

}  // end SdlKeyboard

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
SdlKeyboard::~SdlKeyboard()
{

}  // end ~SdlKeyboard

//-------------------------------------------------------------------------------------------------
/** initialize the keyboard */
//-------------------------------------------------------------------------------------------------
void SdlKeyboard::init()
{

   // extending functionality
   Keyboard::init();

}  // end init

//-------------------------------------------------------------------------------------------------
/** Reset keyboard system */
//-------------------------------------------------------------------------------------------------
void SdlKeyboard::reset()
{

   // extend functionality
   Keyboard::reset();

}  // end reset

//-------------------------------------------------------------------------------------------------
/** called once per frame to update the keyboard state */
//-------------------------------------------------------------------------------------------------
void SdlKeyboard::update()
{

   // extending functionality
   Keyboard::update();

}  // end update

//-------------------------------------------------------------------------------------------------
/** Return TRUE if the caps lock key is down/hilighted */
//-------------------------------------------------------------------------------------------------
Bool SdlKeyboard::getCapsState()
{

   return BitTest(SDL_GetModState(), SDL_KMOD_CAPS);

}  // end getCapsState
