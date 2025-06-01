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

// FILE: LinuxGameEngine.h ////////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, March 2025
// Description: 
//   Device implementation of the game engine ... this is, of course, the 
//   highest level of the game that creates the necessary interfaces to the 
//   devices we need
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LINUXGAMEENGINE_H_
#define __LINUXGAMEENGINE_H_

#include "Common/GameEngine.h"
// #include "GameLogic/GameLogic.h"
// #include "GameNetwork/NetworkInterface.h"
#include "LinuxDevice/Audio/SdlAudioManager.h"
#include "LinuxDevice/Common/LinuxBIGFileSystem.h"
#include "LinuxDevice/Common/LinuxLocalFileSystem.h"
// #include "W3DDevice/Common/W3DModuleFactory.h"
// #include "W3DDevice/GameLogic/W3DGameLogic.h"
#include "LinuxDevice/GameClient/LinuxGameClient.h"
// #include "W3DDevice/GameClient/W3DWebBrowser.h"
#include "LinuxDevice/Common/LinuxFunctionLexicon.h"
// #include "W3DDevice/Common/W3DRadar.h"
// #include "W3DDevice/Common/W3DFunctionLexicon.h"
// #include "W3DDevice/Common/W3DThingFactory.h"




//-------------------------------------------------------------------------------------------------
/** Class declaration for the Linux game engine */
//-------------------------------------------------------------------------------------------------
class LinuxGameEngine: public GameEngine {

public:

   LinuxGameEngine();
   virtual ~LinuxGameEngine();

   virtual void init(int argc, char* argv[]);                  ///< initialization 
   virtual void reset(void);                                   ///< reset engine
   virtual void update(void);                                  ///< update the game engine
   virtual void checkForEvents(void);                          ///< check for SDL events

protected:

   virtual GameLogic* createGameLogic(void);                   ///< factory for game logic
   virtual GameClient* createGameClient(void);                 ///< factory for game client
   virtual ModuleFactory* createModuleFactory(void);           ///< factory for creating modules
   virtual ThingFactory* createThingFactory(void);             ///< factory for the thing factory
   virtual FunctionLexicon* createFunctionLexicon(void);       ///< factory for function lexicon
   virtual LocalFileSystem* createLocalFileSystem(void);       ///< factory for local file system
   virtual ArchiveFileSystem* createArchiveFileSystem(void);   ///< factory for archive file system
   virtual NetworkInterface* createNetwork(void);              ///< Factory for the network
   virtual Radar* createRadar(void);                           ///< Factory for radar
   virtual WebBrowser* createWebBrowser(void);                 ///< Factory for embedded browser
   virtual AudioManager* createAudioManager(void);             ///< Factory for audio device
   virtual ParticleSystemManager* createParticleSystemManager(void);


protected:
   // UINT m_previousErrorMode;
};  // end LinuxGameEngine

// INLINE -----------------------------------------------------------------------------------------
// inline GameLogic* LinuxGameEngine::createGameLogic( void ) { return NEW W3DGameLogic; }
inline GameLogic* LinuxGameEngine::createGameLogic( void ) { printf("Creating NULL GameLogic!\n"); return nullptr; }
inline GameClient* LinuxGameEngine::createGameClient( void ) { return NEW LinuxGameClient; }
// inline ModuleFactory* LinuxGameEngine::createModuleFactory( void ) { return NEW W3DModuleFactory; }
inline ModuleFactory* LinuxGameEngine::createModuleFactory( void ) { printf("Creating NULL ModuleFactory!\n"); return nullptr; }
// inline ThingFactory* LinuxGameEngine::createThingFactory( void ) { return NEW W3DThingFactory; }
inline ThingFactory* LinuxGameEngine::createThingFactory( void ) { printf("Creating NULL ThingFactory!\n"); return nullptr; }
inline FunctionLexicon* LinuxGameEngine::createFunctionLexicon( void ) { return NEW LinuxFunctionLexicon; }
inline LocalFileSystem* LinuxGameEngine::createLocalFileSystem( void ) { return NEW LinuxLocalFileSystem; }
inline ArchiveFileSystem* LinuxGameEngine::createArchiveFileSystem( void ) { return NEW LinuxBIGFileSystem; }
// inline ParticleSystemManager* LinuxGameEngine::createParticleSystemManager( void ) { return NEW W3DParticleSystemManager; }
inline ParticleSystemManager* LinuxGameEngine::createParticleSystemManager( void ) { printf("Creating NULL ParticleSystemManager!\n"); return nullptr; }

// inline NetworkInterface* LinuxGameEngine::createNetwork( void ) { return NetworkInterface::createNetwork(); }
inline NetworkInterface* LinuxGameEngine::createNetwork( void ) { printf("Creating NULL Network!\n"); return nullptr; }
// inline Radar* LinuxGameEngine::createRadar( void ) { return NEW W3DRadar; }
inline Radar* LinuxGameEngine::createRadar( void ) { printf("Creating NULL Radar!\n"); return nullptr; }
// inline WebBrowser* LinuxGameEngine::createWebBrowser( void ) { return NEW CComObject<W3DWebBrowser>; }
inline WebBrowser* LinuxGameEngine::createWebBrowser( void ) { printf("Creating NULL WebBrowser!\n"); return nullptr; }
inline AudioManager* LinuxGameEngine::createAudioManager( void ) { return NEW SdlAudioManager; }

#endif  // end __LINUXGAMEENGINE_H_
