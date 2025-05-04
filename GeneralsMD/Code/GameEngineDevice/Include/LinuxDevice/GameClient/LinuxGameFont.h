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

///// LinuxGameFont.h ////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXGAMEFONT_H_
#define __LINUXGAMEFONT_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "GameClient/GameFont.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////

// TYPE DEFINES ///////////////////////////////////////////////////////////////

// LinuxFontLibrary -----------------------------------------------------------
/** Our font library that uses Linux font implementations */
//-----------------------------------------------------------------------------
class LinuxFontLibrary : public FontLibrary
{

public:

   LinuxFontLibrary();
   virtual ~LinuxFontLibrary();

protected:

   /// load the font data pointer based on everything else we already have set
   Bool loadFontData( GameFont *font );
   /// release the font data pointer
   void releaseFontData( GameFont *font );

};  // end LinuxFontLibrary

// INLINING ///////////////////////////////////////////////////////////////////

// EXTERNALS //////////////////////////////////////////////////////////////////

#endif // __LINUXGAMEFONT_H_

