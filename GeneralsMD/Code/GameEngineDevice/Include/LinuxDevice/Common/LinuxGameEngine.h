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
#include "GameLogic/GameLogic.h"
// #include "GameNetwork/NetworkInterface.h"
#include "LinuxDevice/Audio/SdlAudioManager.h"
#include "LinuxDevice/Common/LinuxBIGFileSystem.h"
#include "LinuxDevice/Common/LinuxLocalFileSystem.h"
#include "LinuxDevice/Common/LinuxModuleFactory.h"
#include "LinuxDevice/GameLogic/LinuxGameLogic.h"
#include "LinuxDevice/GameClient/LinuxGameClient.h"
#include "LinuxDevice/GameClient/LinuxParticleSys.h"
// #include "W3DDevice/GameClient/W3DWebBrowser.h"
#include "LinuxDevice/Common/LinuxFunctionLexicon.h"
#include "LinuxDevice/Common/LinuxRadar.h"
#include "Common/ThingFactory.h"

//-------------------------------------------------------------------------------------------------
/** Class declaration for the Linux game engine */
//-------------------------------------------------------------------------------------------------
class LinuxGameEngine: public GameEngine {

public:

   LinuxGameEngine();
   virtual ~LinuxGameEngine();

   virtual void init(int argc, char* argv[]);                  ///< initialization 
   virtual void reset();                                   ///< reset engine
   virtual void update();                                  ///< update the game engine
   virtual void checkForEvents();                          ///< check for SDL events

protected:

   virtual GameLogic* createGameLogic();                   ///< factory for game logic
   virtual GameClient* createGameClient();                 ///< factory for game client
   virtual ModuleFactory* createModuleFactory();           ///< factory for creating modules
   virtual ThingFactory* createThingFactory();             ///< factory for the thing factory
   virtual FunctionLexicon* createFunctionLexicon();       ///< factory for function lexicon
   virtual LocalFileSystem* createLocalFileSystem();       ///< factory for local file system
   virtual ArchiveFileSystem* createArchiveFileSystem();   ///< factory for archive file system
   virtual NetworkInterface* createNetwork();              ///< Factory for the network
   virtual Radar* createRadar();                           ///< Factory for radar
   virtual WebBrowser* createWebBrowser();                 ///< Factory for embedded browser
   virtual AudioManager* createAudioManager();             ///< Factory for audio device
   virtual ParticleSystemManager* createParticleSystemManager();


protected:
   // UINT m_previousErrorMode;
};  // end LinuxGameEngine

// INLINE -----------------------------------------------------------------------------------------
inline GameLogic* LinuxGameEngine::createGameLogic() { return NEW LinuxGameLogic; }
inline GameClient* LinuxGameEngine::createGameClient() { return NEW LinuxGameClient; }
inline ModuleFactory* LinuxGameEngine::createModuleFactory() { return NEW LinuxModuleFactory; }
inline ThingFactory* LinuxGameEngine::createThingFactory() { return NEW ThingFactory; }
inline FunctionLexicon* LinuxGameEngine::createFunctionLexicon() { return NEW LinuxFunctionLexicon; }
inline LocalFileSystem* LinuxGameEngine::createLocalFileSystem() { return NEW LinuxLocalFileSystem; }
inline ArchiveFileSystem* LinuxGameEngine::createArchiveFileSystem() { return NEW LinuxBIGFileSystem; }
inline ParticleSystemManager* LinuxGameEngine::createParticleSystemManager() { return NEW LinuxParticleSystemManager; }

// inline NetworkInterface* LinuxGameEngine::createNetwork() { return NetworkInterface::createNetwork(); }
inline NetworkInterface* LinuxGameEngine::createNetwork() { printf("Creating NULL Network!\n"); return nullptr; }
inline Radar* LinuxGameEngine::createRadar() { return NEW LinuxRadar; }
// inline WebBrowser* LinuxGameEngine::createWebBrowser() { return NEW CComObject<W3DWebBrowser>; }
inline WebBrowser* LinuxGameEngine::createWebBrowser() { printf("Creating NULL WebBrowser!\n"); return nullptr; }
inline AudioManager* LinuxGameEngine::createAudioManager() { return NEW SdlAudioManager; }

#endif  // end __LINUXGAMEENGINE_H_
