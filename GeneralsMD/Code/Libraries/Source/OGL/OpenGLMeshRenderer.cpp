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

///// OpenGLMeshRenderer.cpp /////////////////////
// Matthew Gill, March 2026
//////////////////////////////////////////////////

#include "OpenGLMeshRenderer.h"
#include "OpenGLRenderer.h"
#include <rinfo.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>

//============================================================================
// OpenGLMeshRenderer::OpenGLMeshRenderer
//============================================================================
OpenGLMeshRenderer::OpenGLMeshRenderer()
{
   glCreateBuffers(1, &m_indexBuffer);
   glCreateBuffers(1, &m_vertexBuffer);
   glCreateVertexArrays(1, &m_vertexArray);
}

//============================================================================
// OpenGLMeshRenderer::createIndexBuffer
//============================================================================
void OpenGLMeshRenderer::createIndexBuffer(const void* bufferData, size_t bufferSize) {
   glNamedBufferStorage(m_indexBuffer, static_cast<GLsizeiptr>(bufferSize), bufferData, 0);
}

//============================================================================
// OpenGLMeshRenderer::createVertexBuffer
//============================================================================
void OpenGLMeshRenderer::createVertexBuffer(const void* bufferData, size_t bufferSize) {
   glNamedBufferStorage(m_vertexBuffer, static_cast<GLsizeiptr>(bufferSize), bufferData, 0);
}

//============================================================================
// OpenGLMeshRenderer::createVertexArray
//============================================================================
void OpenGLMeshRenderer::createVertexArray(VertexFormat format) {
   configureVertexArray(format);
   glVertexArrayVertexBuffer(m_vertexArray, 0, m_vertexBuffer, 0, vertexFormatSize(format));
   glVertexArrayElementBuffer(m_vertexArray, m_indexBuffer);
}

//============================================================================
// OpenGLMeshRenderer::Clone
//============================================================================
RenderObjClass* OpenGLMeshRenderer::Clone(void) const {
   //FIXME: Figure out how to clone an OpenGLMeshRenderer.
   DEBUG_CRASH(("Not sure how to clone an OpenGLMeshRenderer!\n"));
   return nullptr;
}

//============================================================================
// OpenGLMeshRenderer::Render
//============================================================================
void OpenGLMeshRenderer::Render(RenderInfoClass& rinfo) {
   TheOpenGLRenderer->useShader(SHADER_TEXTURED_OPAQUE);
   Matrix4x4 projectionMatrix {rinfo.Camera.Get_Projection_Matrix()};
   projectionMatrix = projectionMatrix.Transpose();
   glUniformMatrix4fv(TheOpenGLRenderer->getUniformLocation(SHADER_TEXTURED_OPAQUE, "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
   Matrix3D view {rinfo.Camera.Get_View_Matrix()};
   Mat4 viewMatrix {view[0][0], view[1][0], view[2][0], 0.0f, view[0][1], view[1][1], view[2][1], 0.0f, view[0][2], view[1][2], view[2][2], 0.0f, view[0][3], view[1][3], view[2][3], 1.0f};
   glUniformMatrix4fv(TheOpenGLRenderer->getUniformLocation(SHADER_TEXTURED_OPAQUE, "ViewMatrix"), 1, GL_FALSE, viewMatrix.data());
   Mat4 modelMatrix {Transform[0][0], Transform[1][0], Transform[2][0], 0.0f, Transform[0][1], Transform[1][1], Transform[2][1], 0.0f, Transform[0][2], Transform[1][2], Transform[2][2], 0.0f, Transform[0][3], Transform[1][3], Transform[2][3], 1.0f};
   glUniformMatrix4fv(TheOpenGLRenderer->getUniformLocation(SHADER_TEXTURED_OPAQUE, "ModelMatrix"), 1, GL_FALSE, modelMatrix.data());
   m_texture->Peek_GL_Texture()->bind(0);
   glBindVertexArray(m_vertexArray);
   glDrawElements(GL_TRIANGLES, m_triangleCount * 3,  GL_UNSIGNED_SHORT, (GLvoid*)0);
}

//============================================================================
// OpenGLMeshRenderer::Delete_This
//============================================================================
void OpenGLMeshRenderer::Delete_This() {
   glDeleteBuffers(1, &m_indexBuffer);
   glDeleteBuffers(1, &m_vertexBuffer);
   glDeleteVertexArrays(1, &m_vertexArray);
   REF_PTR_RELEASE(m_texture);
   deleteInstance();
}

//-------------------------------------------------------------------------------------------------
void OpenGLMeshRenderer::configureVertexArray(VertexFormat format) {
   DEBUG_ASSERTCRASH(format == DX8_FVF_XYZNUV1, ("Currently only supports DX8_FVF_XYZNUV1\n"));
   glEnableVertexArrayAttrib(m_vertexArray, 0);
   glEnableVertexArrayAttrib(m_vertexArray, 1);
   glEnableVertexArrayAttrib(m_vertexArray, 2);
   glVertexArrayAttribFormat(m_vertexArray, 0, 3, GL_FLOAT, GL_FALSE, 0);
   glVertexArrayAttribFormat(m_vertexArray, 1, 3, GL_FLOAT, GL_FALSE, 3 * 4);
   glVertexArrayAttribFormat(m_vertexArray, 2, 2, GL_FLOAT, GL_FALSE, 6 * 4);
   glVertexArrayAttribBinding(m_vertexArray, 0, 0);
   glVertexArrayAttribBinding(m_vertexArray, 1, 0);
   glVertexArrayAttribBinding(m_vertexArray, 2, 0);
}

//-------------------------------------------------------------------------------------------------
GLsizei OpenGLMeshRenderer::vertexFormatSize(VertexFormat format) {
   DEBUG_ASSERTCRASH(format == DX8_FVF_XYZNUV1, ("Currently only supports DX8_FVF_XYZNUV1\n"));
   return 8 * 4;
}
