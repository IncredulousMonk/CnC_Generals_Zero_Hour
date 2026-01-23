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

///// OpenGLSampler.h ////////////////////////////
// Matthew Gill, January 2026
//////////////////////////////////////////////////

#pragma once

#ifndef __OPENGLSAMPLER_H
#define __OPENGLSAMPLER_H

#include "PreRTS.h"
#include "refcount.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>

class OpenGLSampler: public MemoryPoolObject, public RefCountClass {
   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(OpenGLSampler, "OpenGLSampler")
private:
   GLuint m_id {};
public:
   OpenGLSampler();

   void createAnisotropicSampler();
   void bind(GLuint unit);

   void Delete_This() override;
};
EMPTY_DTOR(OpenGLSampler)

#endif // __OPENGLSAMPLER_H
