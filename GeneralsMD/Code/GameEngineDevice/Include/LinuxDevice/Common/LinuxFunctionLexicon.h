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

///// LinuxFunctionLexicon.h /////////////////////
// Matthew Gill, May 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXFUNCTIONLEXICON_H_
#define __LINUXFUNCTIONLEXICON_H_

#include "Common/FunctionLexicon.h"

//-------------------------------------------------------------------------------------------------
/** The function lexicon that can also contain Linux device methods */
//-------------------------------------------------------------------------------------------------
class LinuxFunctionLexicon : public FunctionLexicon
{

public:

   LinuxFunctionLexicon( void );
   virtual ~LinuxFunctionLexicon( void );

   virtual void init( void );
   virtual void reset( void );
   virtual void update( void );
   
protected:

};

#endif // __LINUXFUNCTIONLEXICON_H_

