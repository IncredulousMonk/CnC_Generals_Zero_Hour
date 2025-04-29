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

///// LinuxGameWindow.h //////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXGAMEWINDOW_H_
#define __LINUXGAMEWINDOW_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "GameClient/GameWindow.h"
// #include "WW3D2/Render2DSentence.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////

// TYPE DEFINES ///////////////////////////////////////////////////////////////

// LinuxGameWindow ------------------------------------------------------------
/** Linux implementation for a game window */
// ----------------------------------------------------------------------------
class LinuxGameWindow: public GameWindow
{
   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(LinuxGameWindow, "LinuxGameWindow")

public:

   LinuxGameWindow();
   // already defined by MPO.
   //~LinuxGameWindow();

   /// draw borders for this window only, NO child windows or anything else
   void winDrawBorder();

   Int winSetPosition(Int x, Int y);  ///< set window position
   Int winSetText(UnicodeString newText);  ///< set text string	
   void winSetFont(GameFont *font);  ///< set font for window

   void getTextSize(Int *width, Int *height);  ///< get size of text
   void setTextLoc(Int x, Int y);  ///< set text screen coord loc
   void drawText(Color color);  ///< draw text in the text renderer

protected:

   /// helper function to draw borders
   void blitBorderRect(Int x, Int y, Int width, Int height);

   // Render2DSentenceClass m_textRenderer;  ///< for drawing text
   ICoord2D m_textPos {};  ///< current text pos set in text renderer
   Color m_currTextColor {};  ///< current color used in text renderer
   Bool m_needPolyDraw {};  ///< TRUE need to redo the text polys
   Bool m_newTextPos {};  ///< TRUE when our window has moved and we need a new text pos

};  // end class LinuxGameWindow

// INLINING ///////////////////////////////////////////////////////////////////

// EXTERNALS //////////////////////////////////////////////////////////////////
extern void LinuxGameWinDefaultDraw( GameWindow *window, WinInstanceData *instData );

#endif // __LINUXGAMEWINDOW_H_

