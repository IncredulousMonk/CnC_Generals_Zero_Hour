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

///// OpenGLSampler.cpp //////////////////////////
// Matthew Gill, January 2026
//////////////////////////////////////////////////

#include "OpenGLSampler.h"
#include <SDL3/SDL.h>

//============================================================================
// OpenGLSampler::OpenGLSampler
//============================================================================
OpenGLSampler::OpenGLSampler()
{
   glCreateSamplers(1, &m_id);
}

void OpenGLSampler::createAnisotropicSampler() {
   glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glSamplerParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
}

void OpenGLSampler::bind(GLuint unit) {
   glBindSampler(unit, m_id);
}

void OpenGLSampler::Delete_This() {
   glDeleteSamplers(1, &m_id);
   deleteInstance();
}
