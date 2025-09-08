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

// FILE: LinuxFileSystem.h /////////////////////////////////////////////////////
//
// Linux implementation of a file factory.
//
// Matthew Gill, September 2025
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#ifndef __LINUXFILESYSTEM_H_
#define __LINUXFILESYSTEM_H_

#include "ffactory.h"
#include "Common/File.h"

//-------------------------------------------------------------------------------------------------
/** Game file access.  At present this allows us to access test assets, assets from
  * legacy GDI assets, and the current flat directory access for textures, models etc */
//-------------------------------------------------------------------------------------------------
class GameFileClass: public FileClass
{

public:

   GameFileClass(char const *filename);
   GameFileClass(void);
   virtual ~GameFileClass(void);

	// No copies allowed!
	GameFileClass(const GameFileClass&) = delete;
	GameFileClass& operator=(const GameFileClass&) = delete;

   virtual char const * File_Name(void) const;
   virtual char const * Set_Name(char const *filename);

   // (gth) had to re-instate these functions in the base class, for now just give empty implementations...
   virtual int Create(void) { assert(0); return 1; }
   virtual int Delete(void) { assert(0); return 1; }

   virtual bool Is_Available(int forced=false);
   virtual bool Is_Open(void) const;
   virtual int Open(char const *filename, int rights=READ);
   virtual int Open(int rights=READ);
   virtual int Read(void *buffer, int len);
   virtual int Seek(int pos, int dir=SEEK_CUR);
   virtual int Size(void);
   virtual int Write(void const *buffer, int len);
   virtual void Close(void);

   virtual char const * File_Path(void) const { return m_filePath; }

protected:

   File* m_theFile {}; /// < The file
   Bool m_fileExists {};		///< TRUE if the file exists
   char m_filePath[PATH_MAX];  ///< the file name *and* path (relative)
   char m_filename[PATH_MAX];	///< The file name only

};


/*
** LinuxFileSystem is a derived FileFactoryClass which uses GDI assets.
*/
class	LinuxFileSystem: public FileFactoryClass {
public:
   LinuxFileSystem(void);
   ~LinuxFileSystem(void);

   virtual FileClass* Get_File(char const* filename);
   virtual void Return_File(FileClass* file);
};

extern LinuxFileSystem *TheLinuxFileSystem;
#endif
