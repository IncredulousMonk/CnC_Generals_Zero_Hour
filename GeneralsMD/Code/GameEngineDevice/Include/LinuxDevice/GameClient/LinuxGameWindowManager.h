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
#include "LinuxDevice/GameClient/LinuxGadget.h"

//-------------------------------------------------------------------------------------------------
/** Linux implementation of the game window manager which controls all windows
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
inline GameWinDrawFunc LinuxGameWindowManager::getPushButtonImageDrawFunc() { return LinuxGadgetPushButtonImageDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getPushButtonDrawFunc() { return LinuxGadgetPushButtonDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getCheckBoxImageDrawFunc() { return LinuxGadgetCheckBoxImageDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getCheckBoxDrawFunc() { return LinuxGadgetCheckBoxDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getRadioButtonImageDrawFunc() { return LinuxGadgetRadioButtonImageDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getRadioButtonDrawFunc() { return LinuxGadgetRadioButtonDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getTabControlImageDrawFunc() { return W3DGadgetTabControlImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getTabControlDrawFunc() { return W3DGadgetTabControlDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getListBoxImageDrawFunc() { return LinuxGadgetListBoxImageDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getListBoxDrawFunc() { return LinuxGadgetListBoxDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getComboBoxImageDrawFunc() { return W3DGadgetComboBoxImageDraw; }
// inline GameWinDrawFunc LinuxGameWindowManager::getComboBoxDrawFunc() { return W3DGadgetComboBoxDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getHorizontalSliderImageDrawFunc() { return LinuxGadgetHorizontalSliderImageDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getHorizontalSliderDrawFunc() { return LinuxGadgetHorizontalSliderDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getVerticalSliderImageDrawFunc() { return LinuxGadgetVerticalSliderImageDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getVerticalSliderDrawFunc() { return LinuxGadgetVerticalSliderDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getProgressBarImageDrawFunc() { return LinuxGadgetProgressBarImageDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getProgressBarDrawFunc() { return LinuxGadgetProgressBarDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getStaticTextImageDrawFunc() { return LinuxGadgetStaticTextImageDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getStaticTextDrawFunc() { return LinuxGadgetStaticTextDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getTextEntryImageDrawFunc() { return LinuxGadgetTextEntryImageDraw; }
inline GameWinDrawFunc LinuxGameWindowManager::getTextEntryDrawFunc() { return LinuxGadgetTextEntryDraw; }

inline GameWinDrawFunc LinuxGameWindowManager::getTabControlImageDrawFunc() { printf("Creating NULL W3DGadgetTabControlImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getTabControlDrawFunc() { printf("Creating NULL W3DGadgetTabControlDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getComboBoxImageDrawFunc() { printf("Creating NULL W3DGadgetComboBoxImageDraw!\n"); return nullptr; }
inline GameWinDrawFunc LinuxGameWindowManager::getComboBoxDrawFunc() { printf("Creating NULL W3DGadgetComboBoxDraw!\n"); return nullptr; }

#endif // __LINUXGAMEWINDOWMANAGER_H_

