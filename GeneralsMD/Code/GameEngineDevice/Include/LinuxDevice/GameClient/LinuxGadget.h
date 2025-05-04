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

///// LinuxGadget.h //////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXGADGET_H_
#define __LINUXGADGET_H_

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////

// USER INCLUDES //////////////////////////////////////////////////////////////
#include "GameClient/Gadget.h"
#include "LinuxDevice/GameClient/LinuxGameWindow.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////

// TYPE DEFINES ///////////////////////////////////////////////////////////////
/// when drawing line art for gadgets, the borders are this size
#define WIN_DRAW_LINE_WIDTH (1.0f)

// INLINING ///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// EXTERNALS //////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

extern void LinuxGadgetPushButtonDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetPushButtonImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetCheckBoxDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetCheckBoxImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetRadioButtonDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetRadioButtonImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetTabControlDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetTabControlImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetListBoxDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetListBoxImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetComboBoxDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetComboBoxImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetHorizontalSliderDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetHorizontalSliderImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetVerticalSliderDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetVerticalSliderImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetProgressBarDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetProgressBarImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetStaticTextDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetStaticTextImageDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetTextEntryDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxGadgetTextEntryImageDraw( GameWindow *window, WinInstanceData *instData );

#endif // __LINUXGADGET_H_

