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

///// LinuxGameWindowManager.h ///////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXGAMEWINDOWMANAGER_H_
#define __LINUXGAMEWINDOWMANAGER_H_

#include "GameClient/GameWindowManager.h"
#include "LinuxDevice/GameClient/LinuxGameWindow.h"
// #include "W3DDevice/GameClient/W3DGadget.h"

//-------------------------------------------------------------------------------------------------
/** W3D implementation of the game window manager which controls all windows
   * and user interface controls */
//-------------------------------------------------------------------------------------------------
class LinuxGameWindowManager: public GameWindowManager
{

public:

   LinuxGameWindowManager();
   virtual ~LinuxGameWindowManager();

   virtual void init();  ///< initialize the singlegon

   virtual GameWindow *allocateNewWindow();  ///< allocate a new game window
   virtual GameWinDrawFunc getDefaultDraw();  ///< return default draw func to use

   virtual GameWinDrawFunc getPushButtonImageDrawFunc();
   virtual GameWinDrawFunc getPushButtonDrawFunc();
   virtual GameWinDrawFunc getCheckBoxImageDrawFunc();
   virtual GameWinDrawFunc getCheckBoxDrawFunc();
   virtual GameWinDrawFunc getRadioButtonImageDrawFunc();
   virtual GameWinDrawFunc getRadioButtonDrawFunc();
   virtual GameWinDrawFunc getTabControlImageDrawFunc();
   virtual GameWinDrawFunc getTabControlDrawFunc();
   virtual GameWinDrawFunc getListBoxImageDrawFunc();
   virtual GameWinDrawFunc getListBoxDrawFunc();
   virtual GameWinDrawFunc getComboBoxImageDrawFunc();
   virtual GameWinDrawFunc getComboBoxDrawFunc();
   virtual GameWinDrawFunc getHorizontalSliderImageDrawFunc();
   virtual GameWinDrawFunc getHorizontalSliderDrawFunc();
   virtual GameWinDrawFunc getVerticalSliderImageDrawFunc();
   virtual GameWinDrawFunc getVerticalSliderDrawFunc();
   virtual GameWinDrawFunc getProgressBarImageDrawFunc();
   virtual GameWinDrawFunc getProgressBarDrawFunc();
   virtual GameWinDrawFunc getStaticTextImageDrawFunc();
   virtual GameWinDrawFunc getStaticTextDrawFunc();
   virtual GameWinDrawFunc getTextEntryImageDrawFunc();
   virtual GameWinDrawFunc getTextEntryDrawFunc();

protected:

};  // end class LinuxGameWindowManager

// INLINE //////////////////////////////////////////////////////////////////////////////////////////
inline GameWindow *LinuxGameWindowManager::allocateNewWindow() { return newInstance(LinuxGameWindow); }
inline GameWinDrawFunc LinuxGameWindowManager::getDefaultDraw() { return LinuxGameWinDefaultDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getPushButtonImageDrawFunc() { return W3DGadgetPushButtonImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getPushButtonDrawFunc() { return W3DGadgetPushButtonDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getCheckBoxImageDrawFunc() { return W3DGadgetCheckBoxImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getCheckBoxDrawFunc() { return W3DGadgetCheckBoxDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getRadioButtonImageDrawFunc() { return W3DGadgetRadioButtonImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getRadioButtonDrawFunc() { return W3DGadgetRadioButtonDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getTabControlImageDrawFunc() { return W3DGadgetTabControlImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getTabControlDrawFunc() { return W3DGadgetTabControlDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getListBoxImageDrawFunc() { return W3DGadgetListBoxImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getListBoxDrawFunc() { return W3DGadgetListBoxDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getComboBoxImageDrawFunc() { return W3DGadgetComboBoxImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getComboBoxDrawFunc() { return W3DGadgetComboBoxDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getHorizontalSliderImageDrawFunc() { return W3DGadgetHorizontalSliderImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getHorizontalSliderDrawFunc() { return W3DGadgetHorizontalSliderDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getVerticalSliderImageDrawFunc() { return W3DGadgetVerticalSliderImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getVerticalSliderDrawFunc() { return W3DGadgetVerticalSliderDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getProgressBarImageDrawFunc() { return W3DGadgetProgressBarImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getProgressBarDrawFunc() { return W3DGadgetProgressBarDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getStaticTextImageDrawFunc() { return W3DGadgetStaticTextImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getStaticTextDrawFunc() { return W3DGadgetStaticTextDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getTextEntryImageDrawFunc() { return W3DGadgetTextEntryImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getTextEntryDrawFunc() { return W3DGadgetTextEntryDraw; }

inline GameWinDrawFunc LinuxGameWindowManager::getPushButtonImageDrawFunc() { printf("Creating NULL W3DGadgetPushButtonImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getPushButtonDrawFunc() { printf("Creating NULL W3DGadgetPushButtonDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getCheckBoxImageDrawFunc() { printf("Creating NULL W3DGadgetCheckBoxImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getCheckBoxDrawFunc() { printf("Creating NULL W3DGadgetCheckBoxDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getRadioButtonImageDrawFunc() { printf("Creating NULL W3DGadgetRadioButtonImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getRadioButtonDrawFunc() { printf("Creating NULL W3DGadgetRadioButtonDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getTabControlImageDrawFunc() { printf("Creating NULL W3DGadgetTabControlImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getTabControlDrawFunc() { printf("Creating NULL W3DGadgetTabControlDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getListBoxImageDrawFunc() { printf("Creating NULL W3DGadgetListBoxImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getListBoxDrawFunc() { printf("Creating NULL W3DGadgetListBoxDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getComboBoxImageDrawFunc() { printf("Creating NULL W3DGadgetComboBoxImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getComboBoxDrawFunc() { printf("Creating NULL W3DGadgetComboBoxDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getHorizontalSliderImageDrawFunc() { printf("Creating NULL W3DGadgetHorizontalSliderImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getHorizontalSliderDrawFunc() { printf("Creating NULL W3DGadgetHorizontalSliderDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getVerticalSliderImageDrawFunc() { printf("Creating NULL W3DGadgetVerticalSliderImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getVerticalSliderDrawFunc() { printf("Creating NULL W3DGadgetVerticalSliderDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getProgressBarImageDrawFunc() { printf("Creating NULL W3DGadgetProgressBarImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getProgressBarDrawFunc() { printf("Creating NULL W3DGadgetProgressBarDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getStaticTextImageDrawFunc() { printf("Creating NULL W3DGadgetStaticTextImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getStaticTextDrawFunc() { printf("Creating NULL W3DGadgetStaticTextDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getTextEntryImageDrawFunc() { printf("Creating NULL W3DGadgetTextEntryImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getTextEntryDrawFunc() { printf("Creating NULL W3DGadgetTextEntryDraw!\n"); return nullptr; }

#endif // __LINUXGAMEWINDOWMANAGER_H_

