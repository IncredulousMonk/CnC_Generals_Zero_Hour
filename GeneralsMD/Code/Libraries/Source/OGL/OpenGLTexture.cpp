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

///// OpenGLTexture.cpp //////////////////////////
// Matthew Gill, December 2025
//////////////////////////////////////////////////

#include "OpenGLTexture.h"
#include <SDL3/SDL.h>

//============================================================================
// OpenGLTexture::OpenGLTexture
//============================================================================
OpenGLTexture::OpenGLTexture(unsigned width, unsigned height)
{
   DEBUG_LOG(("Creating OpenGL texture: %d x %d\n", width, height));
   m_width = static_cast<GLsizei>(width);
   m_height = static_cast<GLsizei>(height);
   m_data.resize(width * height * 4);
}

void OpenGLTexture::createMipmappedTexture() {
   glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   GLsizei levels {1 + static_cast<GLsizei>(floor(log2(fmax(m_width, m_height))))};
   glTextureStorage2D(m_id, levels, GL_RGBA8, m_width, m_height);
   glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_data.data());
   glGenerateTextureMipmap(m_id);
}

void OpenGLTexture::bind(GLuint unit) {
   glBindTextureUnit(unit, m_id);
}

void OpenGLTexture::loadFromPNG(const char* filename) {
   SDL_Surface* surface {SDL_LoadPNG(filename)};
   if (!surface) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "OpenGLTexture::loadFromPNG: Could not load surface \"%s\": %s", filename, SDL_GetError());
      return;
   }
   DEBUG_ASSERTCRASH(surface->format == SDL_PIXELFORMAT_RGB24, ("PNG is not RGB!"));
   m_width = surface->w;
   m_height = surface->h;
   m_data.resize(static_cast<size_t>(m_width * m_height * 4));
   for (int y {0}; y < surface->h; ++y) {
      int offset {y * surface->pitch};
      for (int x {0}; x < surface->w; ++x) {
         GLubyte* p {static_cast<GLubyte*>(surface->pixels)};
         p += offset + x * 3;
         m_data[static_cast<size_t>(y * m_width * 4 + x * 4 + 0)] = *p++;
         m_data[static_cast<size_t>(y * m_width * 4 + x * 4 + 1)] = *p++;
         m_data[static_cast<size_t>(y * m_width * 4 + x * 4 + 2)] = *p++;
         m_data[static_cast<size_t>(y * m_width * 4 + x * 4 + 3)] = 255;
      }
   }
   SDL_DestroySurface(surface);
}

void OpenGLTexture::saveToPNG(const char* filename) {
   SDL_Surface* surface {SDL_CreateSurfaceFrom(m_width, m_height, SDL_PIXELFORMAT_RGBA32, m_data.data(), m_width * 4)};
   if (!surface) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "OpenGLTexture::saveToPNG: Could not create surface: %s", SDL_GetError());
      return;
   }
   if (!SDL_SavePNG(surface, filename)) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "OpenGLTexture::saveToPNG: Could not save surface: %s", SDL_GetError());
   }
   SDL_DestroySurface(surface);
}

void OpenGLTexture::Delete_This() {
   if (m_id) {
      glDeleteTextures(1, &m_id);
   }
   deleteInstance();
}
