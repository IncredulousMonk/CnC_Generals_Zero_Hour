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

///// OpenGLTexture.h ////////////////////////////
// Matthew Gill, December 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __OPENGLTEXTURE_H
#define __OPENGLTEXTURE_H

#include "PreRTS.h"
#include "refcount.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>

class OpenGLTexture: public MemoryPoolObject, public RefCountClass {
   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(OpenGLTexture, "OpenGLTexture")
private:
   GLsizei m_width {};
   GLsizei m_height {};
   std::vector<GLubyte> m_data {};
   GLuint m_id {};
public:
   OpenGLTexture(unsigned width, unsigned height);

   void createMipmappedTexture();
   void bind(GLuint unit);
   void loadFromPNG(const char* filename);
   void saveToPNG(const char* filename);

   GLsizei getWidth() const { return m_width; }
   GLsizei getHeight() const { return m_height; }
   GLubyte* getDataPointer() { return m_data.data(); }

   void Delete_This() override;
};
EMPTY_DTOR(OpenGLTexture)

#endif // __OPENGLTEXTURE_H
