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

///// LinuxConvert.h /////////////////////////////
// Matthew Gill, October 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXCONVERT_H_
#define __LINUXCONVERT_H_

//=============================================================================
//           Includes                                                      
//=============================================================================
#include "Lib/BaseType.h"

//=============================================================================
//           Forward References
//=============================================================================
extern void LinuxLogicalScreenToPixelScreen(Real logX, Real logY, Int *screenX, Int *screenY, Int screenWidth, Int screenHeight);
extern void PixelScreenToLinuxLogicalScreen(Int screenX, Int screenY, Real *logX, Real *logY, Int screenWidth, Int screenHeight);

#endif // __LINUXCONVERT_H_
