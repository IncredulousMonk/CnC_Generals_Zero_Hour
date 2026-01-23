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

///// LinuxGameClient.h //////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXGAMECLIENT_H
#define __LINUXGAMECLIENT_H

#include "GameClient/GameClient.h"
#include "LinuxDevice/GameClient/LinuxDisplay.h"
#include "LinuxDevice/GameClient/LinuxInGameUI.h"
#include "LinuxDevice/GameClient/LinuxDisplayStringManager.h"
#include "LinuxDevice/GameClient/LinuxGameFont.h"
#include "LinuxDevice/GameClient/LinuxGameWindowManager.h"
#include "LinuxDevice/GameClient/LinuxTerrainVisual.h"
#include "LinuxDevice/FFmpeg/FFmpegVideoPlayer.h"

class LinuxGameClient: public GameClient {
public:
   LinuxGameClient();
   virtual ~LinuxGameClient();

   /// given a type, create a drawable
   virtual Drawable* friend_createDrawable(const ThingTemplate* thing, DrawableStatus statusBits = DRAWABLE_STATUS_NONE);

   virtual void init();      ///< initialize resources
   virtual void update();    ///< per frame update
   virtual void reset();     ///< reset system

   virtual void addScorch(const Coord3D *pos, Real radius, Scorches type);
   virtual void createRayEffectByTemplate(const Coord3D *start, const Coord3D *end, const ThingTemplate* tmpl);  ///< create effect needing start and end location

   virtual void setTimeOfDay(TimeOfDay tod);    ///< Tell all the drawables what time of day it is now

   virtual void setTeamColor(Int red, Int green, Int blue);  ///< @todo superhack for demo, remove!!!
   virtual void adjustLOD(Int adj); ///< @todo hack for evaluation, remove.
   virtual void notifyTerrainObjectMoved(Object *obj);
protected:
   virtual Keyboard *createKeyboard();      ///< factory for the keyboard
   virtual Mouse *createMouse();            ///< factory for the mouse

   /// factory for creating TheDisplay
   virtual Display *createGameDisplay() { return NEW LinuxDisplay; }

   /// factory for creating TheInGameUI
   virtual InGameUI *createInGameUI() { return NEW LinuxInGameUI; }

   /// factory for creating the window manager
   virtual GameWindowManager *createWindowManager() { return NEW LinuxGameWindowManager; }

   /// factory for creating the font library
   virtual FontLibrary *createFontLibrary() { return NEW LinuxFontLibrary; }

   /// Manager for display strings
   virtual DisplayStringManager *createDisplayStringManager() { return NEW LinuxDisplayStringManager; }

   virtual VideoPlayerInterface *createVideoPlayer() { return NEW FFmpegVideoPlayer; }

   /// factory for creating the TerrainVisual
   virtual TerrainVisual *createTerrainVisual() { return NEW LinuxTerrainVisual; }

   /// factory for creating the snow manager
   virtual SnowManager *createSnowManager() { printf("!!! Creating NULL SnowManager !!!\n"); return nullptr; }

   virtual void setFrameRate(Real msecsPerFrame) { (void) msecsPerFrame; }
};

#endif // __LINUXGAMECLIENT_H