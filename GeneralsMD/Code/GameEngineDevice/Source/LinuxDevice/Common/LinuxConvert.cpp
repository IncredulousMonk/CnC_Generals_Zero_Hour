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

///// LinuxConvert.cpp ///////////////////////////
// Matthew Gill, October 2025
//////////////////////////////////////////////////

//=============================================================================
//         Includes                                                      
//=============================================================================
#include "LinuxDevice/Common/LinuxConvert.h"

//=============================================================================
// LinuxLogicalScreenToPixelScreen
//=============================================================================
/** Translate a Linux logical pixel coord with (-1,-1) at the lower left and
  * (1,1) at the upper right to a pixel screen coord with (0,0) at the
  * upper left */
//=============================================================================
void LinuxLogicalScreenToPixelScreen(Real logX, Real logY, Int *screenX, Int *screenY, Int screenWidth, Int screenHeight)
{
   *screenX = REAL_TO_INT((screenWidth  * (logX + 1.0f)) / 2.0f);
   *screenY = REAL_TO_INT((screenHeight * (-logY + 1.0f)) / 2.0f);
}  // end LinuxLogicalScreenToPixelScreen

//=============================================================================
// PixelScreenToLinuxLogicalScreen
//=============================================================================
/** Translate a pixel coord with (0,0) at the upper left to the Linux logical
  * coord system used with (-1,-1) in the lower left corner and (1,1) the
   * upper right corner */
//=============================================================================
void PixelScreenToLinuxLogicalScreen(Int screenX, Int screenY, Real *logX, Real *logY, Int screenWidth, Int screenHeight)
{
   *logX = ((2.0f * screenX) / (Real)screenWidth) - 1.0f;
   *logY = -(((2.0f * screenY) / (Real)screenHeight) - 1.0f);
}  // end PixelScreenToLinuxLogicalScreen
