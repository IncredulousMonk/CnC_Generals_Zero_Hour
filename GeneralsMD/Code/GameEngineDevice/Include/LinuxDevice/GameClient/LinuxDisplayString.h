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

///// LinuxDisplayString.h ///////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXDISPLAYSTRING_H_
#define __LINUXDISPLAYSTRING_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////
#include <SDL3_ttf/SDL_ttf.h>

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "Common/GameMemory.h"
#include "GameClient/DisplayString.h"
#include "LinuxDevice/GameClient/ILinuxTextRenderer.h"
// #include "WW3D2/Render2DSentence.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////
class LinuxDisplayStringManager;

// TYPE DEFINES ///////////////////////////////////////////////////////////////

// LinuxDisplayString -----------------------------------------------------------
/** */
//-----------------------------------------------------------------------------
class LinuxDisplayString: public DisplayString
{

   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( LinuxDisplayString, "LinuxDisplayString" )

public:

   friend LinuxDisplayStringManager;

   LinuxDisplayString( void );
   // ~LinuxDisplayString( void );  // destructor defined by memory pool

   // No copies allowed!
   LinuxDisplayString(const LinuxDisplayString&) = delete;
   LinuxDisplayString& operator=(const LinuxDisplayString&) = delete;

   void notifyTextChanged();							///< called when text contents change
   void draw(Int x, Int y, Color color, Color dropColor);  ///< render text
   void draw(Int x, Int y, Color color, Color dropColor, Int xDrop, Int yDrop);  ///< render text with the drop shadow being at the offsets passed in
   void getSize(Int *width, Int *height);		///< get render size
   Int  getWidth(Int charPos = -1);
   void setWordWrap(Int wordWrap);						///< set the word wrap width
   void setWordWrapCentered(Bool isCentered); ///< If this is set to true, the text on a new line is centered
   void setFont(GameFont *font);							///< set a font for display
   void setUseHotkey(Bool useHotkey, Color hotKeyColor = 0xffffffff);
   void setClipRegion(IRegion2D *region);		///< clip text in this region

protected:

   void checkForChangedTextData();  /**< called when we need to update our render sentence and update extents */
   void usingResources(UnsignedInt frame);  /**< call this whenever display resources are in use */
   void computeExtents();  ///< compute text width and height

   ILinuxTextRenderer* m_renderer {};
   TTF_Text* m_text {};
   Bool m_textChanged {};  ///< when contents of string change this is TRUE
   Bool m_fontChanged {};  ///< when font has chagned this is TRUE
   UnicodeString m_hotkey {};		///< holds the current hotkey marker.
   Bool m_useHotKey {};
   ICoord2D m_hotKeyPos {};
   Color m_hotKeyColor {};
   ICoord2D m_textPos {};  ///< current text pos set in text renderer
   Color m_currTextColor {};  ///< current color used in text renderer
   Color m_currDropColor {};  ///< current color used for shadow in text
   ICoord2D m_size {};				///< (width,height) size of rendered text
   IRegion2D m_clipRegion {}; ///< the clipping region for text
   UnsignedInt m_lastResourceFrame {};  ///< last frame resources were used on

};

///////////////////////////////////////////////////////////////////////////////
// INLINING ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
inline void LinuxDisplayString::usingResources( UnsignedInt frame ) { m_lastResourceFrame = frame; }

// EXTERNALS //////////////////////////////////////////////////////////////////

#endif // __LINUXDISPLAYSTRING_H_
