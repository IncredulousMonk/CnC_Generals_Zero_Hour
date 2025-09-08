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

///// OpenGLRenderer.h ///////////////////////////
// Matthew Gill, August 2025
//////////////////////////////////////////////////

#pragma once

#ifndef OPEN_GL_RENDERER_H
#define OPEN_GL_RENDERER_H

#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>
#include <string>

class OpenGLRenderer {
private:
   GLuint m_vaoGui {};
   GLuint m_vboGui {};
   GLuint m_progGui {};
   GLuint m_texGui {};
public:
   void init();
   void drawSplashImage();
   void beginRender();
   void endRender();

   static void addShader(GLuint program, const char* source, GLenum shaderType);
   static void buildShader(GLuint program);
};

// EXTERNALS //////////////////////////////////////////////////////////////////////////////////////
extern OpenGLRenderer* TheOpenGLRenderer;   ///< singleton definition

#endif
