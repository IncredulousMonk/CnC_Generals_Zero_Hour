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

///////// LinuxLocalFileSystem.cpp /////////////////////////
// Matthew Gill, March 2025
////////////////////////////////////////////////////////////

#include "Common/AsciiString.h"
#include "Common/GameMemory.h"
#include "Common/PerfTimer.h"
#include "LinuxDevice/Common/LinuxLocalFileSystem.h"
#include "LinuxDevice/Common/LinuxLocalFile.h"
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

LinuxLocalFileSystem::LinuxLocalFileSystem(): LocalFileSystem() {
}

LinuxLocalFileSystem::~LinuxLocalFileSystem() {
}

//DECLARE_PERF_TIMER(LinuxLocalFileSystem_openFile)
File * LinuxLocalFileSystem::openFile(const Char *filename, Int access /* = 0 */) {
   //USE_PERF_TIMER(LinuxLocalFileSystem_openFile)
   LinuxLocalFile *file = newInstance(LinuxLocalFile);

   // sanity check
   if (strlen(filename) <= 0) {
      return NULL;
   }

   if (access & File::WRITE) {
      // if opening the file for writing, we need to make sure the directory is there
      // before we try to create the file.
      const fs::path fullPath{filename};
      AsciiString dirName {fullPath.parent_path().c_str()};
      createDirectory(dirName);
   }

   if (file->open(filename, access) == FALSE) {
      file->close();
      file->deleteInstance();
      file = NULL;
   } else {
      file->deleteOnClose();
   }

   return file;
}

void LinuxLocalFileSystem::update() {
}

void LinuxLocalFileSystem::init() {
}

void LinuxLocalFileSystem::reset() {
}

//DECLARE_PERF_TIMER(LinuxLocalFileSystem_doesFileExist)
Bool LinuxLocalFileSystem::doesFileExist(const Char *filename) const {
   //USE_PERF_TIMER(LinuxLocalFileSystem_doesFileExist)
   const fs::path path{filename};
   return fs::exists(path);
}

template<typename DirectoryIter>
void getFileList([[maybe_unused]] const AsciiString& searchName, FilenameList & filenameList, DirectoryIter it) {
   Bool useRegex = (searchName != "*.*");
   std::string search{searchName.str()};
   search = std::regex_replace(search, std::regex("\\."), "\\.");
   search = std::regex_replace(search, std::regex("\\*"), ".*") + "$";
   // printf("Search: [%s]\n", search.c_str());
   std::regex regex{search, std::regex_constants::icase};
   for (auto const& dir_entry: it) {
      if (!dir_entry.is_directory()) {
         if (useRegex) {
            if (std::regex_search(dir_entry.path().string(), regex)) {
               AsciiString filename {dir_entry.path().c_str()};
               if (filenameList.find(filename) == filenameList.end()) {
                  filenameList.insert(filename);
               }
            }
         } else {
            AsciiString filename {dir_entry.path().c_str()};
            if (filenameList.find(filename) == filenameList.end()) {
               filenameList.insert(filename);
            }
         }
      }
   }
}

void LinuxLocalFileSystem::getFileListInDirectory(const AsciiString&, const AsciiString& originalDirectory, const AsciiString& searchName, FilenameList & filenameList, Bool searchSubdirectories) const {
   const fs::path path{originalDirectory.str()};
   if (searchSubdirectories) {
      getFileList(searchName, filenameList, fs::recursive_directory_iterator{path});
   } else {
      getFileList(searchName, filenameList, fs::directory_iterator{path});
   }
}

Bool LinuxLocalFileSystem::getFileInfo(const AsciiString& filename, FileInfo *fileInfo) const {
   const fs::path path{filename.str()};
   std::error_code ec {};

   std::uintmax_t fsize {fs::file_size(path, ec)};
   if (fsize == static_cast<std::uintmax_t>(-1)) {
      return FALSE;
   }

   fileInfo->size = fsize;

   std::filesystem::file_time_type ftime = std::filesystem::last_write_time(path, ec);
   if (ftime == fs::file_time_type::min()) {
      return FALSE;
   }

   fileInfo->timestamp = static_cast<UnsignedInt64>(ftime.time_since_epoch().count());

   return TRUE;
}

Bool LinuxLocalFileSystem::createDirectory(AsciiString directory) {
   if ((directory.getLength() > 0) && (directory.getLength() < PATH_MAX)) {
      const fs::path path{directory.str()};
      return fs::create_directories(path);
   }
   return FALSE;
}
