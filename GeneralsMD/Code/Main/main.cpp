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

// FILE: Main.cpp /////////////////////////////////////////////////////////////
// 
// Linux entry point for game application
//
// Copyright Matthew Gill, March 2025
//
///////////////////////////////////////////////////////////////////////////////

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "LinuxDevice/Common/LinuxGameEngine.h"
#include "Common/CriticalSection.h"
#include "Common/GameMemory.h"
#include "Common/Version.h"
#include "BuildVersion.h"

SDL_Window* win {};
SDL_Renderer* ren {};
SDL_Texture* tex {};
static CriticalSection critSec4 {};

void initialiseSdl(void) {
   if (!SDL_Init(SDL_INIT_VIDEO)) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialise SDL: %s", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   win = SDL_CreateWindow("Command and Conquer Generals", 800, 600, SDL_WINDOW_HIDDEN);
   if (!win) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   ren = SDL_CreateRenderer(win, NULL);
   if (!ren) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s", SDL_GetError());
      SDL_DestroyWindow(win);
      SDL_Quit();
      exit(1);
   }

   SDL_Surface* bmp = SDL_LoadBMP("../assets/Install_Final.bmp");
   if (!bmp) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load bitmap: %s", SDL_GetError());
      SDL_DestroyRenderer(ren);
      SDL_DestroyWindow(win);
      SDL_Quit();
      exit(1);
   }

   tex = SDL_CreateTextureFromSurface(ren, bmp);
   SDL_DestroySurface(bmp);
   if (!tex) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create texture: %s", SDL_GetError());
      SDL_DestroyRenderer(ren);
      SDL_DestroyWindow(win);
      SDL_Quit();
      exit(1);
   }
}

void drawSplashImage(void) {
   SDL_RenderClear(ren);
   SDL_RenderTexture(ren, tex, NULL, NULL);
   SDL_RenderPresent(ren);
}

void cleanupSdl(void) {
   SDL_DestroyTexture(tex);
   SDL_DestroyRenderer(ren);
   SDL_DestroyWindow(win);
}

int main(int argc, char* argv[]) {
   TheMemoryPoolCriticalSection = &critSec4;

   initialiseSdl();

   SDL_ShowWindow(win);

   DEBUG_INIT(DEBUG_FLAGS_DEFAULT);
   SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Initialising memory manager.");
   initMemoryManager();

   // Set up version info
   TheVersion = NEW Version;
   TheVersion->setVersion(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILDNUM, VERSION_LOCALBUILDNUM,
      AsciiString(VERSION_BUILDUSER), AsciiString(VERSION_BUILDLOC),
      AsciiString(__TIME__), AsciiString(__DATE__));

   SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Entering game main loop.");
   GameMain(argc, argv);
   SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Exited game main loop.");

   delete TheVersion;
   TheVersion = NULL;

#ifdef MEMORYPOOL_DEBUG
   TheMemoryPoolFactory->debugMemoryReport(REPORT_POOLINFO | REPORT_POOL_OVERFLOW | REPORT_SIMPLE_LEAKS, 0, 0);
#endif
#if defined(_DEBUG) || defined(_INTERNAL)
   // TheMemoryPoolFactory->memoryPoolUsageReport("AAAMemStats");
#endif

   shutdownMemoryManager();
   DEBUG_SHUTDOWN();

   cleanupSdl();
   SDL_Quit();

   return 0;
}

// CreateGameEngine ===========================================================
/** Create the Linux game engine we're going to use */
//=============================================================================
GameEngine* CreateGameEngine(void) {
   LinuxGameEngine *engine;

   engine = NEW LinuxGameEngine;
   //game engine may not have existed when app got focus so make sure it
   //knows about current focus state.
   // engine->setIsActive(isWinMainActive);

   return engine;

}  // end CreateGameEngine

int MessageBox(const char* text, const char* caption, UnsignedInt) {
   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, caption, text, win);
   return 1;
}

void SetWindowText(const char* text) {
   SDL_SetWindowTitle(win, text);
}

void* TheWritableGlobalData = NULL;
const char *gAppPrefix = ""; /// So WB can have a different debug log file name.
