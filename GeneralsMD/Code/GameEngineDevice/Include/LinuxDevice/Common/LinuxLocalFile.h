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

///// LinuxLocalFile.h ///////////////////////////
// Matthew Gill, March 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUXLOCALFILE_H
#define __LINUXLOCALFILE_H

#include "Common/LocalFile.h"
#include <fstream>

class LinuxLocalFile: public File {
   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(LinuxLocalFile, "LinuxLocalFile")
private:
   std::fstream m_fs {};
public:
   LinuxLocalFile();
   //virtual ~LinuxLocalFile();

   virtual Bool open(const Char *filename, Int access = 0);   ///< Open a file for access
   virtual void close(void);                                  ///< Close the file
   virtual Int  read(void *buffer, Int bytes);                ///< Read the specified number of bytes in to buffer: See File::read
   virtual Int  write(const void *buffer, Int bytes);         ///< Write the specified number of bytes from the buffer: See File::write
   virtual Int  seek(Int new_pos, seekMode mode = CURRENT);   ///< Set file position: See File::seek
   virtual void nextLine(Char *buf = NULL, Int bufSize = 0);  ///< moves file position to after the next new-line
   virtual Bool scanInt(Int &newInt);                         ///< return what gets read in as an integer at the current file position.
   virtual Bool scanReal(Real &newReal);                      ///< return what gets read in as a float at the current file position.
   virtual Bool scanString(AsciiString &newString);           ///< return what gets read in as a string at the current file position.
   /**
      Allocate a buffer large enough to hold entire file, read 
      the entire file into the buffer, then close the file.
      the buffer is owned by the caller, who is responsible
      for freeing is (via delete[]). This is a Good Thing to
      use because it minimizes memory copies for BIG files.
   */
   virtual char* readEntireAndClose();
   virtual File* convertToRAMFile();
};

#endif // __LINUXLOCALFILE_H