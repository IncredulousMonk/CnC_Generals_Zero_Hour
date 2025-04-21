/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: SdlMouse.cpp ///////////////////////////////////////////////////////////////////////////
// Created:    Colin Day, July 2001
// Desc:       Interface for the mouse using only the Win32 messages
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "Common/Debug.h"
#include "GameClient/GameClient.h"
#include "LinuxDevice/GameClient/SdlMouse.h"
#include <cassert>

/// The SdlMouse singleton instance
SdlMouse* TheSdlMouse {nullptr};

SDL_Cursor* cursorResources[Mouse::NUM_MOUSE_CURSORS][MAX_2D_CURSOR_DIRECTIONS];
///////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
/** Get a mouse event from the buffer if available, we need to translate
   * from the windows message meanings to our own internal mouse 
   * structure */
//-------------------------------------------------------------------------------------------------
UnsignedByte SdlMouse::getMouseEvent(MouseIO *result)
{
   assert(result);

   // if there is nothing here there is no event data to do
   if (m_eventBuffer.empty()) {
      return MOUSE_NONE;
   }

   // translate the win32 mouse message to our own system
   translateEvent(m_eventBuffer.front(), result);

   // remove this event from the buffer
   m_eventBuffer.pop_back();

   // got event OK and all done with this one
   return MOUSE_OK;

}  // end getMouseEvent

//-------------------------------------------------------------------------------------------------
/** Translate a win32 mouse event to our own event info */
//-------------------------------------------------------------------------------------------------
void SdlMouse::translateEvent(const SDL_Event& event, MouseIO *result)
{
   UnsignedInt frame;

   //
   // get the current input frame from the client, if we don't have
   // a client (like in the GUI editor) we just use frame 1 so it
   // registers with the system
   //
   if (TheGameClient) {
      frame = TheGameClient->getFrame();
   } else {
      frame = 1;
   }

   // set these to defaults
   result->leftState = result->middleState = result->rightState = MBS_Up;
   result->leftFrame = result->middleFrame = result->rightFrame = 0;
   result->pos.x = result->pos.y = result->wheelPos = 0;

   // Time is the same for all events
   result->time = event.common.timestamp;
   
   switch(event.type) {

   // ------------------------------------------------------------------------
   case SDL_EVENT_MOUSE_BUTTON_DOWN:
   case SDL_EVENT_MOUSE_BUTTON_UP:
   {
      const SDL_MouseButtonEvent& m {event.button};
      result->pos.x = m.x;
      result->pos.y = m.y;
      MouseButtonState state {};
      if (m.down && m.clicks > 1) {
         state = MBS_DoubleClick;
      } else {
         state = m.down ? MBS_Down : MBS_Up;
      }
      if (m.button == 1) {
         result->leftState = state;
         result->leftFrame = static_cast<Int>(frame);
      } else if (m.button == 2) {
         result->middleState = state;
         result->middleFrame = static_cast<Int>(frame);
      } else if (m.button == 3) {
         result->rightState = state;
         result->rightFrame = static_cast<Int>(frame);
      }
      break;
   }  // end button events

   // ------------------------------------------------------------------------
   case SDL_EVENT_MOUSE_MOTION:
   {
      const SDL_MouseMotionEvent& m {event.motion};
      result->pos.x = m.x;
      result->pos.y = m.y;
      break;
   }  // end mouse move

   // ------------------------------------------------------------------------
   case SDL_EVENT_MOUSE_WHEEL:
   {
      const SDL_MouseWheelEvent& m {event.wheel};
      result->pos.x = m.mouse_x;
      result->pos.y = m.mouse_y;
      result->wheelPos = m.y;
      break;
   }  // end mouse wheel

   // ------------------------------------------------------------------------
   default:
   {

      DEBUG_CRASH(("translateEvent: Unknown SDL mouse event [%d]\n", event.type));
      return;

   }  // end default

   }  // end switch on message at event index in buffer

}  // end translateEvent

///////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
SdlMouse::SdlMouse()
{
   for (Int i=0; i<NUM_MOUSE_CURSORS; i++) {
      for (Int j=0; j<MAX_2D_CURSOR_DIRECTIONS; j++) {
         cursorResources[i][j] = nullptr;
      }
   }
   m_directionFrame = 0; //points up.
   m_lostFocus = FALSE;
}  // end SdlMouse

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
SdlMouse::~SdlMouse()
{

   // remove our global reference that was for the WndProc() only
   TheSdlMouse = NULL;

}  // end ~SdlMouse

//-------------------------------------------------------------------------------------------------
/** Initialize our device */
//-------------------------------------------------------------------------------------------------
void SdlMouse::init()
{

   // extending functionality
   Mouse::init();

   //
   // when we receive messages from a Windows message procedure, the mouse
   // moves report the current cursor position and not deltas, our mouse
   // needs to process those positions as absolute and not relative
   //
   m_inputMovesAbsolute = TRUE;

}  // end int

//-------------------------------------------------------------------------------------------------
/** Reset */
//-------------------------------------------------------------------------------------------------
void SdlMouse::reset()
{

   // extend
   Mouse::reset();

}  // end reset

//-------------------------------------------------------------------------------------------------
/** Update, called once per frame */
//-------------------------------------------------------------------------------------------------
void SdlMouse::update()
{

   // extend 
   Mouse::update();

}  // end update

//-------------------------------------------------------------------------------------------------
/** Add an SDL mouse event to the buffer */
//-------------------------------------------------------------------------------------------------
void SdlMouse::addEvent(const SDL_Event& event)
{
   m_eventBuffer.push_back(event);
}

void SdlMouse::setVisibility(Bool visible)
{
   //Extend
   Mouse::setVisibility(visible);
   //Maybe need to set cursor to force hiding of some cursors.
   SdlMouse::setCursor(getMouseCursor());
}

/**Preload all the cursors we may need during the game.  This must be done before the D3D device
is created to avoid cursor corruption on buggy ATI Radeon cards. */
void SdlMouse::initCursorResources(void)
{
//    for (Int cursor=FIRST_CURSOR; cursor<NUM_MOUSE_CURSORS; cursor++)
//    {
//       for (Int direction=0; direction<m_cursorInfo[cursor].numDirections; direction++)
//       {	if (!cursorResources[cursor][direction] && !m_cursorInfo[cursor].textureName.isEmpty())
//          {	//this cursor has never been loaded before.
//             char resourcePath[256];
//             //Check if this is a directional cursor
//             if (m_cursorInfo[cursor].numDirections > 1)
//                sprintf(resourcePath,"data\\cursors\\%s%d.ANI",m_cursorInfo[cursor].textureName.str(),direction);
//             else
//                sprintf(resourcePath,"data\\cursors\\%s.ANI",m_cursorInfo[cursor].textureName.str());

//             cursorResources[cursor][direction]=LoadCursorFromFile(resourcePath);
//             DEBUG_ASSERTCRASH(cursorResources[cursor][direction], ("MissingCursor %s\n",resourcePath));
//          }
//       }
// //    SetCursor(cursorResources[cursor][m_directionFrame]);
//    }
}

//-------------------------------------------------------------------------------------------------
/** Super basic simplistic cursor */
//-------------------------------------------------------------------------------------------------
void SdlMouse::setCursor(MouseCursor cursor)
{
   DEBUG_LOG(("SdlMouse::setCursor not fully implemented yet\n"));

   // extend
   Mouse::setCursor(cursor);

   if (m_lostFocus) {
      return;  //stop messing with mouse cursor if we don't have focus.
   }

   if (cursor == NONE || !m_visible) {
      SDL_SetCursor(NULL);
   } else {
      SDL_SetCursor(cursorResources[cursor][m_directionFrame]);
   }  // end switch

   // save current cursor
   m_currentCursor = cursor;

}  // end setCursor

//-------------------------------------------------------------------------------------------------
/** Capture the mouse to our application */
//-------------------------------------------------------------------------------------------------
void SdlMouse::capture()
{

//	SetCapture( ApplicationHWnd );

}  // end capture

//-------------------------------------------------------------------------------------------------
/** Release the mouse capture for our app window */
//-------------------------------------------------------------------------------------------------
void SdlMouse::releaseCapture()
{

//	ReleaseCapture();

}  // end releaseCapture
