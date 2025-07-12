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

///// SdlFileStream.cpp //////////////////////////
// Matthew Gill, May 2025
//////////////////////////////////////////////////

#include "LinuxDevice/Common/SdlFileStream.h"

Sint64 streamSize(void* userdata) {
   SdlFileStream* stream {static_cast<SdlFileStream*>(userdata)};
   return stream->size();
}

Sint64 streamSeek(void* userdata, Sint64 offset, SDL_IOWhence whence) {
   SdlFileStream* stream {static_cast<SdlFileStream*>(userdata)};
   return stream->seek(offset, whence);
}

size_t streamRead(void* userdata, void* ptr, size_t size, SDL_IOStatus* status) {
   SdlFileStream* stream {static_cast<SdlFileStream*>(userdata)};
   return stream->read(ptr, size, status);
}

bool streamClose(void* userdata) {
   SdlFileStream* stream {static_cast<SdlFileStream*>(userdata)};
   return stream->close();
}

SdlFileStream::SdlFileStream(File* file): m_file {file}
{
   SDL_INIT_INTERFACE(&m_interface);
   m_interface.size = streamSize;
   m_interface.seek = streamSeek;
   m_interface.read = streamRead;
   m_interface.close = streamClose;
};

Sint64 SdlFileStream::size() {
   DEBUG_ASSERTCRASH(m_file, ("SdlFileStream::size: m_file must be set!"));
   return m_file->size();
}

Sint64 SdlFileStream::seek(Sint64 offset, SDL_IOWhence whence) {
   DEBUG_ASSERTCRASH(m_file, ("SdlFileStream::seek: m_file must be set!"));
   File::seekMode mode {};
   switch (whence) {
   case SDL_IO_SEEK_SET:
      mode = File::START;
      break;
   
   case SDL_IO_SEEK_END:
      mode = File::END;
      break;
   
   case SDL_IO_SEEK_CUR:
      mode = File::CURRENT;
      break;

   default:
      break;
   }

   return m_file->seek(offset, mode);
}

size_t SdlFileStream::read(void* ptr, size_t size, SDL_IOStatus* /*status*/) {
   DEBUG_ASSERTCRASH(m_file, ("SdlFileStream::read: m_file must be set!"));
   Int bytesRead {m_file->read(ptr, static_cast<Int>(size))};
   if (bytesRead < 0) {
      return 0;
   } else {
      return static_cast<size_t>(bytesRead);
   }
}

bool SdlFileStream::close() {
   DEBUG_ASSERTCRASH(m_file, ("SdlFileStream::close: m_file must be set!"));
   m_file->close();
   return true;
}
