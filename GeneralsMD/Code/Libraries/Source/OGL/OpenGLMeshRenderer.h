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

///// OpenGLMeshRenderer.h ///////////////////////
// Matthew Gill, March 2026
//////////////////////////////////////////////////

#pragma once

#ifndef __OPENGLMESHRENDERER_H
#define __OPENGLMESHRENDERER_H

#include "PreRTS.h"
#include "rendobj.h"
#include "dx8fvf.h"
#include "texture.h"
#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>

class OpenGLMeshRenderer: public MemoryPoolObject, public RenderObjClass {
   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(OpenGLMeshRenderer, "OpenGLMeshRenderer")
private:
   GLuint m_indexBuffer {};
   GLuint m_vertexBuffer {};
   GLuint m_vertexArray {};
   GLsizei m_triangleCount {};
   TextureClass* m_texture {};
public:
   OpenGLMeshRenderer();

	// No copies allowed!
	OpenGLMeshRenderer(const OpenGLMeshRenderer&) = delete;
	OpenGLMeshRenderer& operator=(const OpenGLMeshRenderer&) = delete;

   void createIndexBuffer(const void* bufferData, size_t bufferSize);
   void createVertexBuffer(const void* bufferData, size_t bufferSize);
   void createVertexArray(VertexFormat format);

   GLsizei getTriangleCount() const { return m_triangleCount; };
   void setTriangleCount(GLsizei count) { m_triangleCount = count; };
   void setTexture(TextureClass* texture) { m_texture = texture; };

   /////////////////////////////////////////////////////////////////////////////
   // Render Object Interface 
   /////////////////////////////////////////////////////////////////////////////
   virtual RenderObjClass* Clone(void) const;
   virtual int Class_ID(void) const { return CLASSID_GLMESH; }
   virtual void Render(RenderInfoClass& rinfo);

   /////////////////////////////////////////////////////////////////////////////
   // RefCountClass Interface 
   /////////////////////////////////////////////////////////////////////////////
   void Delete_This() override;
private:
   void configureVertexArray(VertexFormat format);
   GLsizei vertexFormatSize(VertexFormat format);
};
EMPTY_DTOR(OpenGLMeshRenderer)

#endif // __OPENGLMESHRENDERER_H
