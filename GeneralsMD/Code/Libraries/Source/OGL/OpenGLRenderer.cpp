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

///// OpenGLRenderer.cpp /////////////////////////
// Matthew Gill, August 2025
//////////////////////////////////////////////////

#include "OpenGLRenderer.h"
#include "Common/Debug.h"
#include <SDL3/SDL.h>
#include <vector>

// GLOBALS ////////////////////////////////////////////////////////////////////////////////////////
OpenGLRenderer* TheOpenGLRenderer {nullptr};

// EXTERNALS //////////////////////////////////////////////////////////////////////////////////////
extern SDL_Window* window;
extern SDL_Surface* surface;
extern SDL_Renderer* renderer;
extern SDL_Texture* splashTexture;

// PRIVATE DATA ///////////////////////////////////////////////////////////////////////////////////
static const char* guiVertSource {R"(
#version 460 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;

out vec2 TexCoordF;

void main(void)
{
   TexCoordF = TexCoord;
   gl_Position = vec4(Position, 1);
}
)"};

static const char* guiFragSource {R"(
#version 460 core

uniform sampler2D Texture;

in vec2 TexCoordF;

out vec4 FragmentColour;

void main(void)
{
   FragmentColour = texture(Texture, TexCoordF);
}
)"};

// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////////////////////////
static std::string debugSourceString(GLenum source) {
   switch (source) {
      case GL_DEBUG_SOURCE_API: return "API";
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW_SYSTEM";
      case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
      case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD_PARTY";
      case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
      case GL_DEBUG_SOURCE_OTHER: return "OTHER";
      default: return "UNKNOWN";
   }
}

static std::string debugTypeString(GLenum type) {
   switch (type) {
      case GL_DEBUG_TYPE_ERROR: return "ERROR";
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOUR";
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOUR";
      case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
      case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
      case GL_DEBUG_TYPE_OTHER: return "OTHER";
      case GL_DEBUG_TYPE_MARKER: return "MARKER";
      case GL_DEBUG_TYPE_PUSH_GROUP: return "PUSH_GROUP";
      case GL_DEBUG_TYPE_POP_GROUP: return "POP_GROUP";
      default: return "UNKNOWN";
   }
}

static std::string debugSeverityString(GLenum severity) {
   switch (severity) {
      case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
      case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
      case GL_DEBUG_SEVERITY_LOW: return "LOW";
      case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
      default: return "UNKNOWN";
   }
}

static void messageCallback(GLenum source, GLenum type, GLuint /* id */, GLenum severity, GLsizei /* length */, const GLchar* message, const void* /* userParam */) {
   DEBUG_LOG(("GL CALLBACK: source = %s, type = %s, severity = %s, message = %s\n",
      debugSourceString(source).c_str(), debugTypeString(type).c_str(), debugSeverityString(severity).c_str(), message));
}


//============================================================================
// OpenGLRenderer::init
//============================================================================
void OpenGLRenderer::init() {
   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(messageCallback, nullptr);

   m_progGui = glCreateProgram();
   addShader(m_progGui, guiVertSource, GL_VERTEX_SHADER);
   addShader(m_progGui, guiFragSource, GL_FRAGMENT_SHADER);
   buildShader(m_progGui);
   glUseProgram(m_progGui);
   glUniform1i(glGetUniformLocation(m_progGui, "Texture"), 0);

   constexpr GLfloat triStripData[] {
      -1.0,  1.0, 0.0,   0.0, 0.0,
       1.0,  1.0, 0.0,   800.0 / 1024.0, 0.0,
      -1.0, -1.0, 0.0,   0.0, 600.0 / 1024.0,
       1.0, -1.0, 0.0,   800.0 / 1024.0, 600.0 / 1024.0
   };

   glGenVertexArrays(1, &m_vaoGui);
   glBindVertexArray(m_vaoGui);
   glGenBuffers(1, &m_vboGui);
   glBindBuffer(GL_ARRAY_BUFFER, m_vboGui);
   glBufferData(GL_ARRAY_BUFFER, sizeof(triStripData), triStripData, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * 4, reinterpret_cast<void*>(0));
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * 4, reinterpret_cast<void*>(3 * 4));
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);

   glActiveTexture(GL_TEXTURE0);
   glGenTextures(1, &m_texGui);
   glBindTexture(GL_TEXTURE_2D, m_texGui);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

   glClearColor(0.0, 0.0, 0.0, 1.0);
   // glEnable(GL_DEPTH_TEST);
   // glViewport(0, 0, 800, 600); // Need to call glViewport if window is resized.
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//============================================================================
// OpenGLRenderer::drawSplashImage
//============================================================================
void OpenGLRenderer::drawSplashImage() {
   SDL_RenderClear(renderer);
   SDL_FRect destRect {0, 0, 800, 600};
   SDL_RenderTexture(renderer, splashTexture, NULL, &destRect);
   SDL_RenderPresent(renderer);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glTextureSubImage2D(m_texGui, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
   glUseProgram(m_progGui);
   glBindVertexArray(m_vaoGui);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   SDL_GL_SwapWindow(window);
}

//============================================================================
// OpenGLRenderer::beginRender
//============================================================================
void OpenGLRenderer::beginRender() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//============================================================================
// OpenGLRenderer::endRender
//============================================================================
void OpenGLRenderer::endRender() {
   glTextureSubImage2D(m_texGui, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
   glUseProgram(m_progGui);
   glBindVertexArray(m_vaoGui);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   SDL_GL_SwapWindow(window);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS 
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
void OpenGLRenderer::addShader(GLuint program, const char* source, GLenum shaderType) {
   GLuint shader {glCreateShader(shaderType)};
   glShaderSource(shader, 1, &source, nullptr);
   glCompileShader(shader);
   std::string shaderInfoLog {};
   GLint infoLogLength {};
   glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
   if (infoLogLength > 0) {
      std::vector<GLchar> log(static_cast<std::size_t>(infoLogLength));
      glGetShaderInfoLog(shader, infoLogLength, nullptr, &log[0]);
      shaderInfoLog = &log[0];
   }
   GLint compiled {};
   glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
   if (compiled == GL_FALSE) {
      glDeleteShader(shader);
      DEBUG_CRASH(("Failed to compile shader.  Shader InfoLog: %s\n", shaderInfoLog.c_str()));
   }
   glAttachShader(program, shader);
   glDeleteShader(shader);   // Won't actually delete until shader program is deleted.
}

//-------------------------------------------------------------------------------------------------
void OpenGLRenderer::buildShader(GLuint program) {
   glLinkProgram(program);
   std::string programInfoLog {};
   GLint infoLogLength {};
   glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
   if (infoLogLength > 0) {
      std::vector<GLchar> log(static_cast<std::size_t>(infoLogLength));
      glGetProgramInfoLog(program, infoLogLength, nullptr, &log[0]);
      programInfoLog = &log[0];
   }
   GLint linked {};
   glGetProgramiv(program, GL_LINK_STATUS, &linked);
   if (linked == GL_FALSE) {
      DEBUG_CRASH(("Failed to build shader.  Program InfoLog: %s\n", programInfoLog.c_str()));
   }
}
