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

///// LinuxGUICallbacks.h ////////////////////////
// Matthew Gill, May 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXGUICALLBACKS_H_
#define __LINUXGUICALLBACKS_H_

class GameWindow;
class WindowLayout;
class WinInstanceData;

// EXTERNALS //////////////////////////////////////////////////////////////////////////////////////

// Message of the day message window --------------------------------------------------------------
// extern void LinuxLeftHUDDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxCameoMovieDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxRightHUDDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxPowerDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxMainMenuDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxMainMenuFourDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxMetalBarMenuDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxCreditsMenuDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxClockDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxMainMenuMapBorder( GameWindow *window, WinInstanceData *instData );
// extern void LinuxMainMenuButtonDropShadowDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxMainMenuRandomTextDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxThinBorderDraw( GameWindow *window, WinInstanceData *instData );
extern void LinuxShellMenuSchemeDraw( GameWindow *window, WinInstanceData *instData );

// extern void LinuxCommandBarGridDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxCommandBarGenExpDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxCommandBarHelpPopupDraw( GameWindow *window, WinInstanceData *instData );

// extern void LinuxCommandBarBackgroundDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxCommandBarForegroundDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxCommandBarTopDraw( GameWindow *window, WinInstanceData *instData );

extern void LinuxNoDraw( GameWindow *window, WinInstanceData *instData );
// extern void LinuxDrawMapPreview( GameWindow *window, WinInstanceData *instData );

void LinuxMainMenuInit( WindowLayout *layout, void *userData );

#endif // __LINUXGUICALLBACKS_H_

