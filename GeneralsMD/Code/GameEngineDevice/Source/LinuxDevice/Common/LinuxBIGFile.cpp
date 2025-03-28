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

////// LinuxBIGFile.cpp /////////////////////////////
// Matthew Gill, March 2025
/////////////////////////////////////////////////////

#include "Common/LocalFile.h"
#include "Common/LocalFileSystem.h"
#include "Common/RAMFile.h"
#include "Common/StreamingArchiveFile.h"
#include "Common/GameMemory.h"
#include "Common/PerfTimer.h"
#include "LinuxDevice/Common/LinuxBIGFile.h"

//============================================================================
// LinuxBIGFile::LinuxBIGFile
//============================================================================

LinuxBIGFile::LinuxBIGFile() {
}

//============================================================================
// LinuxBIGFile::~LinuxBIGFile
//============================================================================

LinuxBIGFile::~LinuxBIGFile() {
}

//============================================================================
// LinuxBIGFile::openFile
//============================================================================

File* LinuxBIGFile::openFile(const Char* filename, Int access) {
   const ArchivedFileInfo* fileInfo {getArchivedFileInfo(AsciiString(filename))};

   if (fileInfo == NULL) {
      return NULL;
   }

   RAMFile *ramFile = NULL;
   
   if (BitTest(access, File::STREAMING)) 
      ramFile = newInstance(StreamingArchiveFile);
   else 
      ramFile = newInstance(RAMFile);

   ramFile->deleteOnClose();
   if (ramFile->openFromArchive(m_file, fileInfo->m_filename, static_cast<Int>(fileInfo->m_offset), static_cast<Int>(fileInfo->m_size)) == FALSE) {
      ramFile->close();
      ramFile = NULL;
      return NULL;
   }

   if ((access & File::WRITE) == 0) {
      // requesting read only access. Just return the RAM file.
      return ramFile;
   }

   // whoever is opening this file wants write access, so copy the file to the local disk
   // and return that file pointer.

   File* localFile {TheLocalFileSystem->openFile(filename, access)};
   if (localFile != NULL) {
      ramFile->copyDataToFile(localFile);
   }

   ramFile->close();
   ramFile = NULL;

   return localFile;
}

//============================================================================
// LinuxBIGFile::closeAllFiles
//============================================================================

void LinuxBIGFile::closeAllFiles(void) {
}

//============================================================================
// LinuxBIGFile::getName
//============================================================================

AsciiString LinuxBIGFile::getName(void) {
   return m_name;
}

//============================================================================
// LinuxBIGFile::getPath
//============================================================================

AsciiString LinuxBIGFile::getPath(void) {
   return m_path;
}

//============================================================================
// LinuxBIGFile::setSearchPriority
//============================================================================

void LinuxBIGFile::setSearchPriority(Int) {
}

//============================================================================
// LinuxBIGFile::close
//============================================================================

void LinuxBIGFile::close(void) {
}

//============================================================================
// LinuxBIGFile::getFileInfo
//============================================================================

Bool LinuxBIGFile::getFileInfo(const AsciiString& filename, FileInfo *fileInfo) const {
   const ArchivedFileInfo *tempFileInfo = getArchivedFileInfo(filename);

   if (tempFileInfo == NULL) {
      return FALSE;
   }

   TheLocalFileSystem->getFileInfo(AsciiString(m_file->getName()), fileInfo);

   fileInfo->size = tempFileInfo->m_size;

   return TRUE;
}

