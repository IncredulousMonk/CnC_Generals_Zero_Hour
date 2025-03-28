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

//////// LinuxLocalFile.cpp ///////////////////////////
// Matthew Gill, March 2025
///////////////////////////////////////////////////////

#include "LinuxDevice/Common/LinuxLocalFile.h"
#include "Common/RAMFile.h"
#include <iostream>

//=================================================================
// LinuxLocalFile::LinuxLocalFile
//=================================================================

LinuxLocalFile::LinuxLocalFile(): File() {
}

//=================================================================
// LinuxLocalFile::~LinuxLocalFile	
//=================================================================

LinuxLocalFile::~LinuxLocalFile() {
}

//=================================================================
// LinuxLocalFile::open	
//=================================================================
/**
  *	This function opens a file using the standard C open() call. Access flags
   * are mapped to the appropriate open flags. Returns true if file was opened
   * successfully.
   */
//=================================================================

Bool LinuxLocalFile::open(const Char *filename, Int access) {
   if (!File::open(filename, access)) {
      return FALSE;
   }

   std::ios::openmode mode {};

   if (m_access & TRUNCATE) {
      mode |= std::ios::trunc;
   }
   if (m_access & APPEND) {
      mode |= std::ios::ate;
   }
   if (m_access & BINARY) {
      mode |= std::ios::binary;
   }

   if (m_access & READ) {
      mode |= std::ios::in;
   }
   if (m_access & WRITE) {
      mode |= std::ios::out;
   }

   m_fs.open(filename, mode);

   if (!m_fs.is_open()) {
      if (m_access & CREATE) {
         m_fs.clear();
         m_fs.open(filename, std::ios::out); // create file
         m_fs.close();
         m_fs.open(filename, mode);
         if (!m_fs.is_open()) {
            close();
            return FALSE;
         }
      } else {
         close();
         return FALSE;
      }
   }

   return TRUE;
}

//=================================================================
// LinuxLocalFile::close 	
//=================================================================
/**
   * Closes the current file if it is open.
  * Must call LinuxLocalFile::close() for each successful LinuxLocalFile::open() call.
   */
//=================================================================

void LinuxLocalFile::close(void) {
   File::close();
}

//=================================================================
// LinuxLocalFile::read 
//=================================================================

Int LinuxLocalFile::read(void *buffer, Int bytes) {
   if (!m_open) {
      return -1;
   }

   if (buffer == NULL) {
      m_fs.seekp(bytes, std::ios::cur);
      return bytes;
   }

   m_fs.read(static_cast<char*>(buffer), bytes);

   return m_fs.gcount();
}

//=================================================================
// LinuxLocalFile::write 
//=================================================================

Int LinuxLocalFile::write(const void *buffer, Int bytes) {
   if (!m_open || !buffer) {
      return -1;
   }

   m_fs.write(static_cast<const char*>(buffer), bytes);

   if (m_fs.bad()) {
      return -1;
   }

   return bytes;
}

//=================================================================
// LinuxLocalFile::seek 
//=================================================================

Int LinuxLocalFile::seek(Int pos, seekMode mode) {
   std::ios::seekdir dir;

   switch (mode) {
      case START:
         dir = std::ios::beg;
         break;
      case CURRENT:
         dir = std::ios::cur;
         break;
      case END:
         DEBUG_ASSERTCRASH(pos <= 0, ("LocalFile::seek - pos should be <= 0 for a seek starting at the end of the file"));
         dir = std::ios::end;
         break;
      default:
         // bad seek mode
         return -1;
   }

   m_fs.seekp(pos, dir);
   return m_fs.tellp();
}

//=================================================================
// LinuxLocalFile::scanInt
//=================================================================
// skips preceding whitespace and stops at the first non-number
// or at EOF
Bool LinuxLocalFile::scanInt(Int &newInt) {
   m_fs >> newInt;
   return m_fs.good();
}

//=================================================================
// LinuxLocalFile::scanReal
//=================================================================
// skips preceding whitespace and stops at the first non-number
// or at EOF
Bool LinuxLocalFile::scanReal(Real &newReal) {
   m_fs >> newReal;
   return m_fs.good();
}

//=================================================================
// LinuxLocalFile::scanString
//=================================================================
// skips preceding whitespace and stops at the first whitespace
// or at EOF
Bool LinuxLocalFile::scanString(AsciiString &newString) {
   Char c;

   newString.clear();

   // skip the preceding whitespace
   do {
      m_fs.get(c);
   } while (m_fs.good() && (isspace(c)));

   if (!m_fs.good()) {
      return FALSE;
   }

   do {
      newString.concat(c);
      m_fs.get(c);
   } while (m_fs.good() && (!isspace(c)));

   if (m_fs.good()) {
      m_fs.unget();
   }

   return TRUE;
}

//=================================================================
// LinuxLocalFile::nextLine
//=================================================================
// scans to the first character after a new-line or at EOF
void LinuxLocalFile::nextLine(Char *buf, Int bufSize) 
{
   Char c = 0;
   Int i = 0;

   // seek to the next new-line.
   do {
      if ((buf == NULL) || (i >= (bufSize-1))) {
         m_fs.get(c);
      } else {
         m_fs.get(buf[i]);
         c = buf[i];
      }
      ++i;
   } while (m_fs.good() && (c != '\n'));

   if (buf != NULL) {
      if (i < bufSize) {
         buf[i] = 0;
      } else {
         buf[bufSize] = 0;
      }
   }
}

//=================================================================
// LinuxLocalFile::convertToRAMFile
//=================================================================
File* LinuxLocalFile::convertToRAMFile() {
   RAMFile* ramFile = newInstance(RAMFile);
   if (ramFile->open(this)) {
      if (this->m_deleteOnClose) {
         ramFile->deleteOnClose();
         this->close(); // is deleteonclose, so should delete.
      } else {
         this->close();
         this->deleteInstance();
      }
      return ramFile;
   } else {
      ramFile->close();
      ramFile->deleteInstance();
      return this;
   }
}

//=================================================================
// LinuxLocalFile::readEntireAndClose
//=================================================================
/**
   Allocate a buffer large enough to hold entire file, read 
   the entire file into the buffer, then close the file.
   the buffer is owned by the caller, who is responsible
   for freeing is (via delete[]). This is a Good Thing to
   use because it minimizes memory copies for BIG files.
*/
char* LinuxLocalFile::readEntireAndClose() {
   Int fileSize = size();
   char* buffer = NEW char[fileSize];

   read(buffer, fileSize);

   close();

   return buffer;
}
