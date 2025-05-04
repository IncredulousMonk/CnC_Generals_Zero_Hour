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

///// ILinuxTextRenderer.h ///////////////////////
// Matthew Gill, May 2025
//////////////////////////////////////////////////

#pragma once

#ifndef _ILINUXTEXTRENDERER_H_
#define _ILINUXTEXTRENDERER_H_

#include "Common/UnicodeString.h"
#include "GameClient/GameFont.h"
#include <SDL3_ttf/SDL_ttf.h>

class ILinuxTextRenderer {
public:
   virtual ~ILinuxTextRenderer() {};

   virtual TTF_Text* createText(UnicodeString& string, GameFont& font) = 0;
};

#endif // _ILINUXTEXTRENDERER_H_
