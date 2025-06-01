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

///// SdlFileStream.h ////////////////////////////
// Matthew Gill, May 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __SDL_FILE_STREAM_H
#define __SDL_FILE_STREAM_H

#include "Common/File.h"
#include <SDL3/SDL.h>

/// @brief Adapts the File class to match the SDL_IOStreamInterface interface.
///
/// If the constructor is called without an argument then setFile must be called!
class SdlFileStream {
public:
   SdlFileStream(File* file = nullptr);
   ~SdlFileStream() {};

   // No copies allowed!
   SdlFileStream(const SdlFileStream&) = delete;
   SdlFileStream& operator=(const SdlFileStream&) = delete;

   Sint64 size();
   Sint64 seek(Sint64 offset, SDL_IOWhence whence);
   size_t read(void *ptr, size_t size, SDL_IOStatus *status);
   bool close();

   SDL_IOStreamInterface* interface() { return &m_interface; }
   void setFile(File* file) { m_file = file; }
private:
   SDL_IOStreamInterface m_interface {};
   File* m_file {};
};

#endif // __SDL_FILE_STREAM_H
