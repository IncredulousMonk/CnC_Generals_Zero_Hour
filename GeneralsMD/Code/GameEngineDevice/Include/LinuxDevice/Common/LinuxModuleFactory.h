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

// FILE: LinuxModuleFactory.h //////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, June 2025
// Description: 
//   Linux game logic class, there shouldn't be a lot of new functionality
//   in this class, but there are certain things that need to have close
//   knowledge of each other like the logical and visual terrain.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LINUXMODULEFACTORY_H_
#define __LINUXMODULEFACTORY_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/ModuleFactory.h"

//-------------------------------------------------------------------------------------------------
/** Linux-specific functionality for the module factory */
//-------------------------------------------------------------------------------------------------
class LinuxModuleFactory: public ModuleFactory {

public:

   virtual void init( void );  

};

#endif // __LINUXMODULEFACTORY_H_
