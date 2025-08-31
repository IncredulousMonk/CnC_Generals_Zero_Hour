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

// FILE: LinuxGameEngine.cpp ////////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, March 2025
// Description:
//   Implementation of the Linux game engine, this is the highest level of 
//   the game application, it creates all the devices we will use for the game
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "LinuxDevice/Common/LinuxGameEngine.h"
#include "LinuxDevice/GameClient/SdlKeyboard.h"
#include "LinuxDevice/GameClient/SdlMouse.h"
#include "Common/PerfTimer.h"
#include "OpenGLRenderer.h"
#include <SDL3/SDL.h>
#include <iostream>

// #include "GameNetwork/LANAPICallbacks.h"

// extern DWORD TheMessageTime;

//-------------------------------------------------------------------------------------------------
/** Constructor for LinuxGameEngine */
//-------------------------------------------------------------------------------------------------
LinuxGameEngine::LinuxGameEngine() {
}

//-------------------------------------------------------------------------------------------------
/** Destructor for LinuxGameEngine */
//-------------------------------------------------------------------------------------------------
LinuxGameEngine::~LinuxGameEngine() {
   delete TheOpenGLRenderer;
   TheOpenGLRenderer = nullptr;
}


//-------------------------------------------------------------------------------------------------
/** Initialize the game engine */
//-------------------------------------------------------------------------------------------------
void LinuxGameEngine::init(int argc, char* argv[]) {

   TheOpenGLRenderer = NEW OpenGLRenderer();
   TheOpenGLRenderer->init();

   TheOpenGLRenderer->drawSplashImage();

   // extending functionality
   GameEngine::init(argc, argv);

}  // end init

//-------------------------------------------------------------------------------------------------
/** Reset the system */
//-------------------------------------------------------------------------------------------------
void LinuxGameEngine::reset(void) {

   // extending functionality
   GameEngine::reset();
   std::cout << "LinuxGameEngine::reset\n";

}  // end reset

//-------------------------------------------------------------------------------------------------
/** Update the game engine by updating the GameClient and 
    * GameLogic singletons. */
//-------------------------------------------------------------------------------------------------
void LinuxGameEngine::update(void) {
   // call the engine normal update
   GameEngine::update();

   //  extern HWND ApplicationHWnd;
   //  if (ApplicationHWnd && ::IsIconic(ApplicationHWnd)) {
   //      while (ApplicationHWnd && ::IsIconic(ApplicationHWnd)) {
   //          // We are alt-tabbed out here.  Sleep a bit, & process windows
   //          // so that we can become un-alt-tabbed out.
   //          Sleep(5);
   //          serviceWindowsOS();

   //          if (TheLAN != NULL) {
   //              // BGC - need to update TheLAN so we can process and respond to other
   //              // people's messages who may not be alt-tabbed out like we are.
   //              TheLAN->setIsActive(isActive());
   //              TheLAN->update();
   //          }

   //          // If we are running a multiplayer game, keep running the logic.
   //          // There is code in the client to skip client redraw if we are 
   //          // iconic.  jba.
   //          if (TheGameEngine->getQuitting() || TheGameLogic->isInInternetGame() || TheGameLogic->isInLanGame()) {
   //              break; // keep running.
   //          }
   //      }

   // When we are alt-tabbed out... the MilesAudioManager seems to go into a coma sometimes
   // and not regain focus properly when we come back. This seems to wake it up nicely.
   // AudioAffect aa = (AudioAffect)0x10;
   //    TheAudio->setVolume(TheAudio->getVolume( aa ), aa );

   // }

   // Check for SDL events.
   checkForEvents();

}  // end update

//-------------------------------------------------------------------------------------------------
/** This function may be called from within this application to check
  * for SDL events.  Presumeably we would call this at least once each time
  * around the game loop to keep events from backing up. */
//-------------------------------------------------------------------------------------------------
void LinuxGameEngine::checkForEvents(void) {

   SDL_Event event;
   while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_KEY_DOWN:
      case SDL_EVENT_KEY_UP:
         if (TheSdlKeyboard) {
            TheSdlKeyboard->addEvent(event.key);
         }
         break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
      case SDL_EVENT_MOUSE_BUTTON_UP:
      case SDL_EVENT_MOUSE_MOTION:
      case SDL_EVENT_MOUSE_WHEEL:
         if (TheSdlMouse) {
            TheSdlMouse->addEvent(event);
         }
         break;
      case SDL_EVENT_QUIT:
         setQuitting(true);
         break;
      default:
         break;
      }
   }

//    MSG msg;
//    Int returnValue;

//    //
//    // see if we have any messages to process, a NULL window handle tells the
//    // OS to look at the main window associated with the calling thread, us!
//    //
//    while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
//    {

//       // get the message
//       returnValue = GetMessage( &msg, NULL, 0, 0 );

//       // this is one possible way to check for quitting conditions as a message
//       // of WM_QUIT will cause GetMessage() to return 0
// /*
//       if( returnValue == 0 )
//       {

//          setQuitting( true );
//          break;

//       }
// */

//       TheMessageTime = msg.time;
//       // translate and dispatch the message
//       TranslateMessage( &msg );
//       DispatchMessage( &msg );
//       TheMessageTime = 0;
            
//    }  // end while

}  // end checkForEvents

