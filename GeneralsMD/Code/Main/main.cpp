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

// FIXME: Remove this once WorldHeightMap.cpp is implemented.  It defines a bunch of global variables.
#define INSTANTIATE_WELL_KNOWN_KEYS
#include "Common/WellKnownKeys.h"

SDL_Window* window {};
SDL_Renderer* renderer {};
SDL_Surface* surface {};
SDL_Texture* splashTexture {};
static SDL_GLContext glContext {};
static CriticalSection critSec4 {};

// What are these for?
const Char *g_strFile = "data\\Generals.str";
const Char *g_csfFile = "data\\%s\\Generals.csf";

void initialiseSdl(void) {
   if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialise SDL: %s", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

   window = SDL_CreateWindow("Command and Conquer Generals", 800, 600, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
   if (!window) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   glContext = SDL_GL_CreateContext(window);
   if (!glContext) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create OpenGL context: %s", SDL_GetError());
      SDL_DestroyWindow(window);
      SDL_Quit();
      exit(1);
   }

   if (!SDL_GL_SetSwapInterval(1)) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Warning: Unable to set VSync!: %s", SDL_GetError());
      SDL_GL_DestroyContext(glContext);
      SDL_DestroyWindow(window);
      SDL_Quit();
      exit(1);
   }

   surface = SDL_CreateSurface(1024, 1024, SDL_PIXELFORMAT_ABGR8888); // SDL pixel format is backwards!
   if (!surface) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create surface: %s", SDL_GetError());
      SDL_GL_DestroyContext(glContext);
      SDL_DestroyWindow(window);
      SDL_Quit();
      exit(1);
   }
   SDL_ClearSurface(surface, 0.0f, 0.0f, 0.0f, 1.0f);

   renderer = SDL_CreateSoftwareRenderer(surface);
   if (!renderer) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s", SDL_GetError());
      SDL_DestroySurface(surface);
      SDL_GL_DestroyContext(glContext);
      SDL_DestroyWindow(window);
      SDL_Quit();
      exit(1);
   }

   // renderer = SDL_CreateRenderer(window, NULL);
   // if (!renderer) {
   //    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s", SDL_GetError());
   //    SDL_GL_DestroyContext(glContext);
   //    SDL_DestroyWindow(window);
   //    SDL_Quit();
   //    exit(1);
   // }

   SDL_Surface* bmp = SDL_LoadBMP("../assets/Install_Final.bmp");
   if (!bmp) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load bitmap: %s", SDL_GetError());
      SDL_DestroyRenderer(renderer);
      SDL_GL_DestroyContext(glContext);
      SDL_DestroyWindow(window);
      SDL_Quit();
      exit(1);
   }

   splashTexture = SDL_CreateTextureFromSurface(renderer, bmp);
   SDL_DestroySurface(bmp);
   if (!splashTexture) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create texture: %s", SDL_GetError());
      SDL_DestroyRenderer(renderer);
      SDL_GL_DestroyContext(glContext);
      SDL_DestroyWindow(window);
      SDL_Quit();
      exit(1);
   }
}

void cleanupSdl(void) {
   SDL_DestroyTexture(splashTexture);
   SDL_DestroySurface(surface);
   SDL_DestroyRenderer(renderer);
   SDL_GL_DestroyContext(glContext);
   SDL_DestroyWindow(window);
}

int main(int argc, char* argv[]) {
   TheMemoryPoolCriticalSection = &critSec4;

   initialiseSdl();

   SDL_ShowWindow(window);

   int major {};
   int minor {};
   SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
   SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
   SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "OpenGL version: %d.%d", major, minor);

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
   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, caption, text, window);
   return 1;
}

void SetWindowText(const char* text) {
   SDL_SetWindowTitle(window, text);
}

const char *gAppPrefix = ""; /// So WB can have a different debug log file name.
