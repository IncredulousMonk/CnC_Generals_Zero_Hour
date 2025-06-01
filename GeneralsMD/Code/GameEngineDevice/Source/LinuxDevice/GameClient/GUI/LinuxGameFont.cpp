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

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "Common/Debug.h"
#include "LinuxDevice/GameClient/LinuxGameFont.h"
#include "GameClient/GlobalLanguage.h"

// EXTERNALS //////////////////////////////////////////////////////////////////
extern SDL_Renderer* renderer;

// DEFINES ////////////////////////////////////////////////////////////////////

// PRIVATE TYPES //////////////////////////////////////////////////////////////

// PRIVATE DATA ///////////////////////////////////////////////////////////////

// PUBLIC DATA ////////////////////////////////////////////////////////////////

// PRIVATE PROTOTYPES /////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

LinuxFontLibrary::LinuxFontLibrary()
{
   if (!TTF_Init()) {
      DEBUG_LOG(("Couldn't initialise SDL_ttf: %s\n", SDL_GetError()));
   }
}

LinuxFontLibrary::~LinuxFontLibrary()
{
   TTF_Quit();
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// LinuxFontLibrary::loadFontData =============================================
/** Load a font */
//=============================================================================
Bool LinuxFontLibrary::loadFontData(GameFont *font)
{
   DEBUG_LOG(("Request to load font %s %d point %s\n", font->nameString.str(), font->pointSize, font->bold ? " Bold" : ""));

   // sanity
   if (font == NULL) {
      return FALSE;
   }

   TTF_Font* fontData;
   if ((UnsignedInt)font->pointSize > 100) { //sanity check the size - anything over 100 is probably wrong. -MW
      fontData = NULL;
   } else if (font->nameString.compare("Generals") == 0) {
      fontData = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/Arial_Bold.ttf", font->pointSize == 15 ? 18 : font->pointSize);
   } else {
      if (font->bold) {
         fontData = TTF_OpenFont("/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf", font->pointSize);
      } else {
         fontData = TTF_OpenFont("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf", font->pointSize);
      }
   }
   // FIXME: Credits fonts: Requests Arial 16 point Bold and Arial 22 point, but screen sizes are about double that.

   if( fontData == NULL )
   {
      DEBUG_LOG(("Linux load font: unable to find font '%s'\n", font->nameString.str()));
      DEBUG_ASSERTCRASH(font, ("Missing or Corrupted Font.  Please see log for details"));
      return FALSE;
   }  // end if

   // assign font data
   TTF_SetFontHinting(fontData, TTF_HINTING_LIGHT_SUBPIXEL);
   font->fontData = fontData;
   font->height = TTF_GetFontHeight(fontData);

   // FIXME: Do we need a unicode font as well?
   // FontCharsClass *unicodeFontChar = NULL;

   // // load unicode of same point size
   // if(TheGlobalLanguageData)
   // 	unicodeFontChar = WLinuxAssetManager::
   // 								Get_Instance()->Get_FontChars( TheGlobalLanguageData->m_unicodeFontName.str(), font->pointSize,
   // 																							 font->bold ? true : false );
   // else
   // 	unicodeFontChar = WLinuxAssetManager::
   // 								Get_Instance()->Get_FontChars( "Arial Unicode MS", font->pointSize,
   // 																							 font->bold ? true : false );

   // if ( unicodeFontChar )
   // {
   // 	font->AlternateUnicodeFont = unicodeFontChar;
   // }

   // all done and loaded
   return TRUE;

}  // end loadFont

// LinuxFontLibrary::releaseFontData ==========================================
/** Release font data */
//=============================================================================
void LinuxFontLibrary::releaseFontData(GameFont *font)
{
   DEBUG_LOG(("Request to release font %s %d point %s\n", font->nameString.str(), font->pointSize, font->bold ? " Bold" : ""));

   if (font && font->fontData) {
      TTF_Font* fontData {static_cast<TTF_Font*>(font->fontData)};
      TTF_CloseFont(fontData);
   }
   font->fontData = NULL;

}  // end releaseFont
