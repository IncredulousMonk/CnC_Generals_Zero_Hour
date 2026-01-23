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
#include <cmath>

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

layout(binding = 0) uniform sampler2D Texture;

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

   constexpr GLfloat triStripData[] {
      -1.0,  1.0, 0.0,   0.0, 0.0,
       1.0,  1.0, 0.0,   800.0 / 1024.0, 0.0,
      -1.0, -1.0, 0.0,   0.0, 600.0 / 1024.0,
       1.0, -1.0, 0.0,   800.0 / 1024.0, 600.0 / 1024.0
   };

   glCreateBuffers(1, &m_vboGui);
   glNamedBufferStorage(m_vboGui, sizeof(triStripData), triStripData, 0);
   glCreateVertexArrays(1, &m_vaoGui);
   glEnableVertexArrayAttrib(m_vaoGui, 0);
   glEnableVertexArrayAttrib(m_vaoGui, 1);
   glVertexArrayAttribFormat(m_vaoGui, 0, 3, GL_FLOAT, GL_FALSE, 0);
   glVertexArrayAttribFormat(m_vaoGui, 1, 2, GL_FLOAT, GL_FALSE, 3 * 4);
   glVertexArrayAttribBinding(m_vaoGui, 0, 0);
   glVertexArrayAttribBinding(m_vaoGui, 1, 0);
   glVertexArrayVertexBuffer(m_vaoGui, 0, m_vboGui, 0, 5 * 4);

   glCreateTextures(GL_TEXTURE_2D, 1, &m_texGui);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTextureStorage2D(m_texGui, 1, GL_RGBA8, 1024, 1024);
   glTextureSubImage2D(m_texGui, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
   glCreateSamplers(1, &m_samGui);
   glSamplerParameteri(m_samGui, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glSamplerParameteri(m_samGui, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glBindVertexArray(m_vaoGui);
   m_progGui = glCreateProgram();
   addShader(m_progGui, guiVertSource, GL_VERTEX_SHADER);
   addShader(m_progGui, guiFragSource, GL_FRAGMENT_SHADER);
   buildShader(m_progGui);
   glUseProgram(m_progGui);

   glClearColor(0.0, 0.0, 0.25, 1.0);
   glEnable(GL_DEPTH_TEST);
   // glViewport(0, 0, 800, 600); // Need to call glViewport if window is resized.
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
   glBindTextureUnit(0, m_texGui);
   glBindSampler(0, m_samGui);
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
   glBindTextureUnit(0, m_texGui);
   glBindSampler(0, m_samGui);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   SDL_GL_SwapWindow(window);
}



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

//-------------------------------------------------------------------------------------------------
/// Create a matrix for a perspective-view frustum.
Mat4 OpenGLRenderer::frustum(double left, double right, double bottom, double top, double near, double far) {
   Mat4 result {};
   result[0] = 2.0 * near / (right - left);
   result[5] = 2.0 * near / (top - bottom);
   result[8] = (right + left) / (right - left);
   result[9] = (top + bottom) / (top - bottom);
   result[10] = -(far + near) / (far - near);
   result[11] = -1.0;
   result[14] = -2.0 * far * near / (far - near);
   return result;
}

//-------------------------------------------------------------------------------------------------
/// Create a matrix for a symmetric perspective-view frustum.
Mat4 OpenGLRenderer::perspective(double fieldOfViewY, double aspect, double near, double far) {
   double top {near * std::tan(fieldOfViewY * M_PI / 360.0)};
   double bottom {-top};
   double right {top * aspect};
   double left {-right};
   return frustum(left, right, bottom, top, near, far);
}


using Vector3 = std::array<double, 3>;

Vector3 cross(const Vector3& v1, const Vector3& v2) {
   double x {v1[1] * v2[2] - v1[2] * v2[1]};
   double y {v1[2] * v2[0] - v1[0] * v2[2]};
   double z {v1[0] * v2[1] - v1[1] * v2[0]};
   return {x, y, z};
}

double length(Vector3 v) {
   return std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

Vector3 normalise(Vector3 v) {
   Vector3 result {};
   double len {length(v)};
   if (len < 1e-6) {
      result[0] = 0.0;
      result[1] = 0.0;
      result[2] = 0.0;
   } else {
      double inverse {1.0 / len};
      result[0] = v[0] * inverse;
      result[1] = v[1] * inverse;
      result[2] = v[2] * inverse;
   }
   return result;
}

//-------------------------------------------------------------------------------------------------
Mat4 OpenGLRenderer::lookat(double eyeX, double eyeY, double eyeZ, double centreX, double centreY, double centreZ, double upX, double upY, double upZ) {
   Vector3 eye {eyeX, eyeY, eyeZ};
   Vector3 centre {centreX, centreY, centreZ};
   Vector3 worldUp {upX, upY, upZ};
   Vector3 front {centre[0] - eye[0], centre[1] - eye[1], centre[2] - eye[2]};
   front = normalise(front);
   Vector3 right {cross(front, worldUp)};
   Vector3 up {cross(right, front)};
   Mat4 result {};
   result[0] = right[0];
   result[1] = up[0];
   result[2] = -front[0];
   result[3] = 0.0f;
   result[4] = right[1];
   result[5] = up[1];
   result[6] = -front[1];
   result[7] = 0.0f;
   result[8] = right[2];
   result[9] = up[2];
   result[10] = -front[2];
   result[11] = 0.0f;
   result[12] = -right[0] * eyeX - right[1] * eyeY - right[2] * eyeZ;
   result[13] = -up[0] * eyeX - up[1] * eyeY - up[2] * eyeZ;
   result[14] = front[0] * eyeX + front[1] * eyeY + front[2] * eyeZ;
   result[15] = 1.0f;
   return result;
}
