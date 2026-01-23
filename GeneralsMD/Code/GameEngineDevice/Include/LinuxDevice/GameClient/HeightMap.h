/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#ifndef __HEIGHTMAP_H_
#define __HEIGHTMAP_H_

// #include "always.h"
// #include "rendobj.h"
// #include "w3d_file.h"
// #include "dx8vertexbuffer.h"
// #include "dx8indexbuffer.h"
// #include "dx8wrapper.h"
// #include "shader.h"
// #include "vertmaterial.h"
#include "Lib/BaseType.h"
#include "Common/GameType.h"
#include "WorldHeightMap.h"
#include "BaseHeightMap.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>

#define VERTEX_BUFFER_TILE_LENGTH	32		//tiles of side length 32 (grid of 33x33 vertices).

// Adjust the triangles to make cliff sides most attractive.  jba.
#define FLIP_TRIANGLES 1

struct Tile {
	Coord3D min {};	///<minimum corner of AABB.
	Coord3D max {};	///<maximum corner of AABB.
	Int offset {};	///<offset of this tile within the vertex buffer.
};

/// Custom render object that draws the heightmap and handles intersection tests.
/**
Custom W3D render object that's used to process the terrain.  It handles
virtually everything to do with the terrain, including: drawing, lighting,
scorchmarks and intersection tests.
*/



class HeightMapRenderObjClass : public BaseHeightMapRenderObjClass
{

public:

	HeightMapRenderObjClass(void);
	virtual ~HeightMapRenderObjClass(void);

	// No copies allowed!
	HeightMapRenderObjClass(const HeightMapRenderObjClass&) = delete;
	HeightMapRenderObjClass& operator=(const HeightMapRenderObjClass&) = delete;

#if 0
	// DX8_CleanupHook methods
	virtual void ReleaseResources(void);	///< Release all dx8 resources so the device can be reset.
	virtual void ReAcquireResources(void);  ///< Reacquire all resources after device reset.
#endif // if 0


	/////////////////////////////////////////////////////////////////////////////
	// Render Object Interface (W3D methods)
	/////////////////////////////////////////////////////////////////////////////
	virtual void					Render(RenderInfoClass & rinfo);
#if 0
	virtual void					On_Frame_Update(void); 
#endif // if 0

	///allocate resources needed to render heightmap
	virtual int initHeightData(Int width, Int height, WorldHeightMap *pMap, RefRenderObjListIterator *pLightsIterator, Bool updateExtraPassTiles=TRUE);
	virtual Int freeMapResources(void);	///< free resources used to render heightmap
	virtual void updateCenter(CameraClass *camera, RefRenderObjListIterator *pLightsIterator);

#if 0
	void renderExtraBlendTiles(void);			///< render 3-way blend tiles that have blend of 3 textures.


	virtual void staticLightingChanged(void);
	virtual	void adjustTerrainLOD(Int adj);
	virtual void reset(void);
	virtual void doPartialUpdate(const IRegion2D &partialRange, WorldHeightMap *htMap, RefRenderObjListIterator *pLightsIterator);

	virtual void oversizeTerrain(Int tilesToOversize);
#endif // if 0

	virtual int updateBlock(Int x0, Int y0, Int x1, Int y1, WorldHeightMap *pMap, RefRenderObjListIterator *pLightsIterator);

protected:
	GLuint m_progTerrain {};
	// Coord3D m_cameraPos {-700.0f, -1000.0f, 100.0f};
	Coord3D m_cameraPos {1000.0f, -100.0f, 500.0f};
	Int *m_extraBlendTilePositions {};		///<array holding x,y tile positions of all extra blend tiles. (used for 3 textures per tile).
	Int m_numExtraBlendTiles {};			///<number of blend tiles in m_extraBlendTilePositions.
	Int m_numVisibleExtraBlendTiles {};		///<number rendered last frame.
	Int m_extraBlendTilePositionsSize {};	///<total size of array including unused memory.
	GLuint* m_vertexBufferTiles {};			///<collection of smaller vertex buffers that make up 1 heightmap
	GLuint* m_vertexArrays {};				///<Vertex Array Objects
	char** m_vertexBufferBackup {};			///< In memory copy of the vertex buffer data for quick update of dynamic lighting.
	Int m_originX {};						///< Origin point in the grid.  Slides around.
	Int m_originY {};						///< Origin point in the grid.  Slides around.
	GLuint m_indexBuffer {};				///<indices defining triangles in a VB tile.
	GLuint m_vertexBuffer {};				///<buffer that contains all of the vertices.
	GLuint m_vertexArray {};				///<Vertex Array Object for terrain
	Int m_tileCount {};						///<number of tiles in the map.
	Tile* m_tiles {};						///<array describing the tiles.
	Int m_numVBTilesX {};					///<dimensions of array containing all the vertex buffers 
	Int m_numVBTilesY {};					///<dimensions of array containing all the vertex buffers
	Int m_numVertexBufferTiles {};			///<number of vertex buffers needed to store this heightmap
	Int m_numBlockColumnsInLastVB {};		///<a VB tile may be partially filled, this indicates how many 2x2 vertex blocks are filled.
	Int m_numBlockRowsInLastVB {};			///<a VB tile may be partially filled, this indicates how many 2x2 vertex blocks are filled.


#if 0
	UnsignedInt doTheDynamicLight(VERTEX_FORMAT *vb, VERTEX_FORMAT *vbMirror, Vector3*light, Vector3*normal, W3DDynamicLight *pLights[], Int numLights);
#endif // if 0
	Int getXWithOrigin(Int x);
	Int getYWithOrigin(Int x);
#if 0
	///update vertex diffuse color for dynamic lights inside given rectangle
	Int updateVBForLight(DX8VertexBufferClass *pVB, char *data, Int x0, Int y0, Int x1, Int y1, Int originX, Int originY, W3DDynamicLight *pLights[], Int numLights);
	Int updateVBForLightOptimized(DX8VertexBufferClass	*pVB, char *data, Int x0, Int y0, Int x1, Int y1, Int originX, Int originY, W3DDynamicLight *pLights[], Int numLights);
#endif // if 0
	///update vertex buffer vertices inside given rectangle
	Int updateVB(GLuint vertexbuffer, char *data, Int x0, Int y0, Int x1, Int y1, Int originX, Int originY, WorldHeightMap *pMap, RefRenderObjListIterator *pLightsIterator);
#if 0
	///upate vertex buffers associated with the given rectangle
	void initDestAlphaLUT(void);	///<initialize water depth LUT stored in m_destAlphaTexture
#endif // if 0
	void renderTerrainPass(CameraClass *pCamera);	///< renders additional terrain pass.
#if 0
	Int	getNumExtraBlendTiles(Bool visible) { return visible?m_numVisibleExtraBlendTiles:m_numExtraBlendTiles;}
#endif // if 0
	void freeIndexVertexBuffers(void);
};

#endif  // end __HEIGHTMAP_H_
