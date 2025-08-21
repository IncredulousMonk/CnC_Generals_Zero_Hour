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

///// LinuxDisplayStringManager.h ////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef _LINUXDISPLAYSTRINGMANAGER_H_
#define _LINUXDISPLAYSTRINGMANAGER_H_

#include "GameClient/DisplayStringManager.h"
#include "LinuxDevice/GameClient/LinuxDisplayString.h"
#include "LinuxDevice/GameClient/ILinuxTextRenderer.h"
#include <SDL3_ttf/SDL_ttf.h>

//-------------------------------------------------------------------------------------------------
/** Access for creating game managed display strings */
//-------------------------------------------------------------------------------------------------
//#define KRIS_BRUTAL_HACK_FOR_AIRCRAFT_CARRIER_DEBUGGING

#ifdef KRIS_BRUTAL_HACK_FOR_AIRCRAFT_CARRIER_DEBUGGING
   #define MAX_GROUPS 20
#else
   #define MAX_GROUPS 10
#endif

class LinuxDisplayStringManager : public DisplayStringManager, ILinuxTextRenderer
{

public:

   LinuxDisplayStringManager();
   virtual ~LinuxDisplayStringManager();

   // No copies allowed!
   LinuxDisplayStringManager(const LinuxDisplayStringManager&) = delete;
   LinuxDisplayStringManager& operator=(const LinuxDisplayStringManager&) = delete;

   // Initialize our numeral strings in postProcessLoad
   virtual void postProcessLoad();

   /// update method for all our display strings
   virtual void update();

   /// allocate a new display string
   virtual DisplayString *newDisplayString();

   /// free a display string
   virtual void freeDisplayString(DisplayString *string);
   
   // This is used to save us a few FPS and storage space. There's no reason to 
   // duplicate the DisplayString on every drawable when 1 copy will suffice.
   virtual DisplayString *getGroupNumeralString(Int numeral);
   virtual DisplayString *getFormationLetterString() { return m_formationLetterDisplayString; };

   // Rendering
   virtual TTF_Text* createText(UnicodeString& string, GameFont& font);

protected:
   DisplayString *m_groupNumeralStrings[ MAX_GROUPS ] {};
   DisplayString *m_formationLetterDisplayString {};
   TTF_TextEngine *m_engine {};
};

#endif // _LINUXDISPLAYSTRINGMANAGER_H_
