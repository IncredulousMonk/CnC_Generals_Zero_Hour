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

///// LinuxFunctionLexicon.cpp ///////////////////
// Matthew Gill, May 2025
//////////////////////////////////////////////////

#include "GameClient/GameWindow.h"
#include "LinuxDevice/Common/LinuxFunctionLexicon.h"
#include "LinuxDevice/GameClient/LinuxGUICallbacks.h"
#include "LinuxDevice/GameClient/LinuxGameWindow.h"
#include "LinuxDevice/GameClient/LinuxGadget.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE DATA 
///////////////////////////////////////////////////////////////////////////////////////////////////

// Game Window draw methods -----------------------------------------------------------------------
static FunctionLexicon::TableEntry gameWinDrawTable [] = 
{

   { NAMEKEY_INVALID, "GameWinDefaultDraw",                    reinterpret_cast<void*>(GameWinDefaultDraw) },
   { NAMEKEY_INVALID, "LinuxGameWinDefaultDraw",               reinterpret_cast<void*>(LinuxGameWinDefaultDraw) },

   { NAMEKEY_INVALID, "LinuxGadgetPushButtonDraw",             reinterpret_cast<void*>(LinuxGadgetPushButtonDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetPushButtonImageDraw",        reinterpret_cast<void*>(LinuxGadgetPushButtonImageDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetCheckBoxDraw",               reinterpret_cast<void*>(LinuxGadgetCheckBoxDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetCheckBoxImageDraw",          reinterpret_cast<void*>(LinuxGadgetCheckBoxImageDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetRadioButtonDraw",            reinterpret_cast<void*>(LinuxGadgetRadioButtonDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetRadioButtonImageDraw",       reinterpret_cast<void*>(LinuxGadgetRadioButtonImageDraw) },
   // { NAMEKEY_INVALID, "LinuxGadgetTabControlDraw",             reinterpret_cast<void*>(LinuxGadgetTabControlDraw) },
   // { NAMEKEY_INVALID, "LinuxGadgetTabControlImageDraw",        reinterpret_cast<void*>(LinuxGadgetTabControlImageDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetListBoxDraw",                reinterpret_cast<void*>(LinuxGadgetListBoxDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetListBoxImageDraw",           reinterpret_cast<void*>(LinuxGadgetListBoxImageDraw) },
   // { NAMEKEY_INVALID, "LinuxGadgetComboBoxDraw",               reinterpret_cast<void*>(LinuxGadgetComboBoxDraw) },
   // { NAMEKEY_INVALID, "LinuxGadgetComboBoxImageDraw",          reinterpret_cast<void*>(LinuxGadgetComboBoxImageDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetHorizontalSliderDraw",       reinterpret_cast<void*>(LinuxGadgetHorizontalSliderDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetHorizontalSliderImageDraw",  reinterpret_cast<void*>(LinuxGadgetHorizontalSliderImageDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetVerticalSliderDraw",         reinterpret_cast<void*>(LinuxGadgetVerticalSliderDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetVerticalSliderImageDraw",    reinterpret_cast<void*>(LinuxGadgetVerticalSliderImageDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetProgressBarDraw",            reinterpret_cast<void*>(LinuxGadgetProgressBarDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetProgressBarImageDraw",       reinterpret_cast<void*>(LinuxGadgetProgressBarImageDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetStaticTextDraw",             reinterpret_cast<void*>(LinuxGadgetStaticTextDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetStaticTextImageDraw",        reinterpret_cast<void*>(LinuxGadgetStaticTextImageDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetTextEntryDraw",              reinterpret_cast<void*>(LinuxGadgetTextEntryDraw) },
   { NAMEKEY_INVALID, "LinuxGadgetTextEntryImageDraw",         reinterpret_cast<void*>(LinuxGadgetTextEntryImageDraw) },

   // { NAMEKEY_INVALID, "W3DLeftHUDDraw",                   reinterpret_cast<void*>(LinuxLeftHUDDraw) },
   // { NAMEKEY_INVALID, "W3DCameoMovieDraw",                reinterpret_cast<void*>(LinuxCameoMovieDraw) },
   // { NAMEKEY_INVALID, "W3DRightHUDDraw",                  reinterpret_cast<void*>(LinuxRightHUDDraw) },
   // { NAMEKEY_INVALID, "W3DPowerDraw",                     reinterpret_cast<void*>(LinuxPowerDraw) },
   // { NAMEKEY_INVALID, "W3DMainMenuDraw",                  reinterpret_cast<void*>(LinuxMainMenuDraw) },
   // { NAMEKEY_INVALID, "W3DMainMenuFourDraw",              reinterpret_cast<void*>(LinuxMainMenuFourDraw) },
   // { NAMEKEY_INVALID, "W3DMetalBarMenuDraw",              reinterpret_cast<void*>(LinuxMetalBarMenuDraw) },
   { NAMEKEY_INVALID, "W3DCreditsMenuDraw",               reinterpret_cast<void*>(LinuxCreditsMenuDraw) },
   { NAMEKEY_INVALID, "W3DClockDraw",                     reinterpret_cast<void*>(LinuxClockDraw) },
   // { NAMEKEY_INVALID, "W3DMainMenuMapBorder",             reinterpret_cast<void*>(LinuxMainMenuMapBorder) },
   // { NAMEKEY_INVALID, "W3DMainMenuButtonDropShadowDraw",  reinterpret_cast<void*>(LinuxMainMenuButtonDropShadowDraw) },
   // { NAMEKEY_INVALID, "W3DMainMenuRandomTextDraw",        reinterpret_cast<void*>(LinuxMainMenuRandomTextDraw) },
   // { NAMEKEY_INVALID, "W3DThinBorderDraw",                reinterpret_cast<void*>(LinuxThinBorderDraw) },
   { NAMEKEY_INVALID, "W3DShellMenuSchemeDraw",           reinterpret_cast<void*>(LinuxShellMenuSchemeDraw) },
   // { NAMEKEY_INVALID, "W3DCommandBarBackgroundDraw",      reinterpret_cast<void*>(LinuxCommandBarBackgroundDraw) },
   // { NAMEKEY_INVALID, "W3DCommandBarTopDraw",             reinterpret_cast<void*>(LinuxCommandBarTopDraw) },
   // { NAMEKEY_INVALID, "W3DCommandBarGenExpDraw",          reinterpret_cast<void*>(LinuxCommandBarGenExpDraw) },
   // { NAMEKEY_INVALID, "W3DCommandBarHelpPopupDraw",       reinterpret_cast<void*>(LinuxCommandBarHelpPopupDraw) },

   // { NAMEKEY_INVALID, "W3DCommandBarGridDraw",            reinterpret_cast<void*>(LinuxCommandBarGridDraw) },


   // { NAMEKEY_INVALID, "W3DCommandBarForegroundDraw",      reinterpret_cast<void*>(LinuxCommandBarForegroundDraw) },
   { NAMEKEY_INVALID, "W3DNoDraw",                             reinterpret_cast<void*>(LinuxNoDraw) },
   // { NAMEKEY_INVALID, "W3DDrawMapPreview",                reinterpret_cast<void*>(LinuxDrawMapPreview) },

   { NAMEKEY_INVALID, NULL,                                    NULL },

};

// Game Window init methods -----------------------------------------------------------------------
static FunctionLexicon::TableEntry layoutInitTable [] = 
{

   { NAMEKEY_INVALID, "W3DMainMenuInit",                       reinterpret_cast<void*>(LinuxMainMenuInit) },

   { NAMEKEY_INVALID, NULL,                                    NULL },

};

///////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS 
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxFunctionLexicon::LinuxFunctionLexicon( void )
{

}  // end LinuxFunctionLexicon

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxFunctionLexicon::~LinuxFunctionLexicon( void )
{

}  // end ~LinuxFunctionLexicon

//-------------------------------------------------------------------------------------------------
/** Initialize the function table specific for our implementations of
   * the Linux device */
//-------------------------------------------------------------------------------------------------
void LinuxFunctionLexicon::init( void )
{

   // extend functionality
   FunctionLexicon::init();

   // load our own tables
   loadTable( gameWinDrawTable, TABLE_GAME_WIN_DEVICEDRAW );
   loadTable( layoutInitTable, TABLE_WIN_LAYOUT_DEVICEINIT );

}  // end init

//-------------------------------------------------------------------------------------------------
/** Reset */
//-------------------------------------------------------------------------------------------------
void LinuxFunctionLexicon::reset( void )
{

   // Pay attention to the order of what happens in the base class as you reset

   // extend
   FunctionLexicon::reset();

}  // end reset

//-------------------------------------------------------------------------------------------------
/** Update */
//-------------------------------------------------------------------------------------------------
void LinuxFunctionLexicon::update( void )
{

   // extend?
   FunctionLexicon::update();

}  // end update
