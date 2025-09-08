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

///// LinuxView.cpp //////////////////////////////
// Matthew Gill, August 2025
//////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Linux implementation of the game view window.  View windows are literally
// a window into the game world that have width, height, and camera 
// controls for what to look at
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

// USER INCLUDES //////////////////////////////////////////////////////////////////////////////////
#include "LinuxDevice/GameClient/LinuxDisplay.h"
#include "LinuxDevice/GameClient/LinuxView.h"
#include "OpenGLRenderer.h"

// 30 fps
Int TheLinuxFrameLengthInMsec = 1000 / LOGICFRAMES_PER_SECOND; // default is 33msec/frame == 30fps. but we may change it depending on sys config.

// PRIVATE DATA ///////////////////////////////////////////////////////////////////////////////////
static const char* triVertSource {R"(
#version 460 core

layout(location = 0) in vec3 Position;

void main(void)
{
   gl_Position = vec4(Position, 1);
}
)"};

static const char* triFragSource {R"(
#version 460 core

out vec4 FragmentColour;

void main(void)
{
   FragmentColour = vec4(0.5, 0.0, 0.5, 1.0);
}
)"};



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxView::LinuxView()
{
}  // end LinuxView

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxView::~LinuxView()
{
}  // end ~LinuxView

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::init()
{
   // extend View functionality
   View::init();
   setName("LinuxView");

   m_progTri = glCreateProgram();
   OpenGLRenderer::addShader(m_progTri, triVertSource, GL_VERTEX_SHADER);
   OpenGLRenderer::addShader(m_progTri, triFragSource, GL_FRAGMENT_SHADER);
   OpenGLRenderer::buildShader(m_progTri);

   constexpr GLfloat triangleData[] {0.0, 0.5, 0.0,   -0.5, -0.5, 0.0,   0.5, -0.5, 0.0};

   glGenVertexArrays(1, &m_vaoTri);
   glBindVertexArray(m_vaoTri);
   glGenBuffers(1, &m_vboTri);
   glBindBuffer(GL_ARRAY_BUFFER, m_vboTri);
   glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, reinterpret_cast<void*>(0));
   glEnableVertexAttribArray(0);
}  // end init

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::reset()
{
   View::reset();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::updateView()
{
   UPDATE();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::update()
{
}

//-------------------------------------------------------------------------------------------------
/** Draw member for the Linux window, this will literally draw the window 
  * for this view */
//-------------------------------------------------------------------------------------------------
void LinuxView::drawView()
{
   DRAW();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::draw()
{
   glUseProgram(m_progTri);
   glBindVertexArray(m_vaoTri);
   glDrawArrays(GL_TRIANGLES, 0, 3);
}
