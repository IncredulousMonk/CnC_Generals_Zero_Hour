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

///// LinuxDisplayStringManager.cpph //////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#include "Common/Debug.h"
#include "GameClient/GameClient.h"
#include "GameClient/GameText.h"
#include "GameClient/DisplayString.h"
#include "GameClient/DrawGroupInfo.h"
#include "GameClient/GlobalLanguage.h"
#include "LinuxDevice/GameClient/LinuxDisplayStringManager.h"
#include <SDL3/SDL.h>
#include <cassert>
#include <codecvt>
#include <locale>

// EXTERNALS //////////////////////////////////////////////////////////////////
extern SDL_Renderer* renderer;

///////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS 
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
LinuxDisplayStringManager::LinuxDisplayStringManager()
{
   m_engine = TTF_CreateRendererTextEngine(renderer);
   for (Int i = 0; i < MAX_GROUPS; ++i) 
   {
      m_groupNumeralStrings[i] = NULL;
   }

   m_formationLetterDisplayString = NULL;
}

//-------------------------------------------------------------------------------------------------
LinuxDisplayStringManager::~LinuxDisplayStringManager()
{
   for (Int i = 0; i < MAX_GROUPS; ++i) 
   {
      if (m_groupNumeralStrings[i])
         freeDisplayString(m_groupNumeralStrings[i]);
      m_groupNumeralStrings[i] = NULL;
   }

   if (m_formationLetterDisplayString)
      freeDisplayString( m_formationLetterDisplayString );
   m_formationLetterDisplayString = NULL;
   TTF_DestroyRendererTextEngine(m_engine);
}

//-------------------------------------------------------------------------------------------------
void LinuxDisplayStringManager::postProcessLoad()
{
   // Get the font.
   GameFont *font = TheFontLibrary->getFont(
      TheDrawGroupInfo->m_fontName,
      TheDrawGroupInfo->m_fontSize,
      TheDrawGroupInfo->m_fontIsBold );
   
   for (Int i = 0; i < MAX_GROUPS; ++i) 
   {
      m_groupNumeralStrings[i] = newDisplayString();
      m_groupNumeralStrings[i]->setFont(font);

#ifdef KRIS_BRUTAL_HACK_FOR_AIRCRAFT_CARRIER_DEBUGGING
      UnicodeString displayNumber;
      displayNumber.format( L"%d", i);
      m_groupNumeralStrings[i]->setText( displayNumber );
#else
      AsciiString displayNumber;
      displayNumber.format("NUMBER:%d", i);
      m_groupNumeralStrings[i]->setText(TheGameText->fetch(displayNumber));
#endif
   }

   m_formationLetterDisplayString = newDisplayString();
   m_formationLetterDisplayString->setFont(font);
   AsciiString displayLetter;
   displayLetter.format("LABEL:FORMATION");
   m_formationLetterDisplayString->setText(TheGameText->fetch(displayLetter));


}

//-------------------------------------------------------------------------------------------------
/** Allocate a new display string and tie it to the master list so we
   * can keep track of it */
//-------------------------------------------------------------------------------------------------
DisplayString *LinuxDisplayStringManager::newDisplayString()
{
   DisplayString *newString = newInstance(LinuxDisplayString);

   // sanity
   if( newString == NULL )
   {

      DEBUG_LOG(( "newDisplayString: Could not allocate new Linux display string\n" ));
      assert( 0 );
      return NULL;

   }  // end if

   dynamic_cast<LinuxDisplayString*>(newString)->m_renderer = this;

   // assign a default font
   if (TheGlobalLanguageData && TheGlobalLanguageData->m_ini.m_defaultDisplayStringFont.name.isNotEmpty()) {
      newString->setFont(TheFontLibrary->getFont(
         TheGlobalLanguageData->m_ini.m_defaultDisplayStringFont.name,
         TheGlobalLanguageData->m_ini.m_defaultDisplayStringFont.size,
         TheGlobalLanguageData->m_ini.m_defaultDisplayStringFont.bold) );
   } else {
      newString->setFont( TheFontLibrary->getFont( AsciiString("Times New Roman"), 12, FALSE ) );
   }

   // link string to list
   link( newString );

   // return our new string
   return newString;

}  // end newDisplayString

//-------------------------------------------------------------------------------------------------
/** Remove a display string from the master list and delete the data */
//-------------------------------------------------------------------------------------------------
void LinuxDisplayStringManager::freeDisplayString(DisplayString *string)
{

   // sanity
   if (string == NULL) {
      return;
   }

   // unlink
   unLink(string);

   // if the string happens to fall where our current checkpoint was, set the checkpoint to null
   if (m_currentCheckpoint == string) {
      m_currentCheckpoint = NULL;
   }

   // free data
   string->deleteInstance();

}  // end freeDisplayString

//-------------------------------------------------------------------------------------------------
void LinuxDisplayStringManager::update()
{
   // call base in case we add something later
   DisplayStringManager::update();

   // FIXME: Remove all of the "free resources" code if we don't end up using textures.
   // LinuxDisplayString *string = static_cast<LinuxDisplayString *>(m_stringList);

   // // if the m_currentCheckpoint is valid, use it for the starting point for the search
   // if (m_currentCheckpoint) {
   //    string = static_cast<LinuxDisplayString *>(m_currentCheckpoint);
   // }

   // UnsignedInt currFrame = TheGameClient->getFrame();
   // const UnsignedInt LinuxCleanupTime = 60;  /** any string not rendered after
   //                                                             this many frames will have its
   //                                                             render resources freed */

   // int numStrings = 10;
   // // looping through all the strings eats up a lot of ambient time. Instead, 
   // // loop through 10 (arbitrarily chosen) or till the end is hit.
   // while ( numStrings-- && string)
   // {

   //    //
   //    // has this string "expired" in terms of using resources, a string
   //    // with a resource frame of zero isn't using any resources at all
   //    //
   //    if( string->m_lastResourceFrame != 0 &&
   //          currFrame - string->m_lastResourceFrame > LinuxCleanupTime )
   //    {

   //       // // free the resources
   //       // string->m_textRenderer.Reset();
   //       // string->m_textRendererHotKey.Reset();
   //       //
   //       // mark data in the string as changed so that if it needs to
   //       // be drawn again it will know to reconstruct the render data
   //       //
   //       string->m_textChanged = TRUE;

   //       //
   //       // set the last resource frame to zero, this allows us to ignore it
   //       // in future cleanup passes of this update routine
   //       //
   //       string->m_lastResourceFrame = 0;
         
   //    }  // end if

   //    // move to next string
   //    string = static_cast<LinuxDisplayString *>(string->next());

   // }  // end while
   
   // // reset the starting point for our next search
   // m_currentCheckpoint = string;
}  // end update

//-------------------------------------------------------------------------------------------------
DisplayString *LinuxDisplayStringManager::getGroupNumeralString(Int numeral)
{
   if (numeral < 0 || numeral > MAX_GROUPS - 1 )
   {
      DEBUG_CRASH(("Numeral '%d' out of range.\n", numeral));
      return m_groupNumeralStrings[0];
   }

   return m_groupNumeralStrings[numeral];
}

//-------------------------------------------------------------------------------------------------
TTF_Text* LinuxDisplayStringManager::createText(UnicodeString& string, GameFont& font)
{
   TTF_Font* ttfFont {static_cast<TTF_Font*>(font.fontData)};
   // TTF_CreateText wants UTF-8 text, so we must convert from WideChar string to UTF-8 string.
   const WideChar* wc = string.str();
   std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
   std::string utf8text = utf8_conv.to_bytes(wc);
   return TTF_CreateText(m_engine, ttfFont, utf8text.c_str(), 0);
}
