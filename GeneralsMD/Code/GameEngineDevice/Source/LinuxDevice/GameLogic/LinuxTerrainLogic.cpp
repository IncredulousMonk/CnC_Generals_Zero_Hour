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

// FILE: LinuxTerrainLogic.cpp ////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, September 2025
//
// Linux implementation details for logical terrain
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "Common/GameMemory.h"
#include "LinuxDevice/GameClient/HeightMap.h"
#include "LinuxDevice/GameLogic/LinuxTerrainLogic.h"
#include "LinuxDevice/GameClient/WorldHeightMap.h"
#include "Common/PerfTimer.h"
#include "Common/MapReaderWriterInfo.h"
#include "Common/GlobalData.h"
#include "Common/Xfer.h"
#include "GameClient/GameClient.h"

#include "GameClient/MapUtil.h"
#include "GameLogic/AI.h"
#include "GameLogic/AIPathfind.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxTerrainLogic::LinuxTerrainLogic(): m_mapMinZ(0), m_mapMaxZ(1)
{
   m_mapData = NULL; 
}  // end LinuxTerrainLogic

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxTerrainLogic::~LinuxTerrainLogic()
{
   reset();
}  // end LinuxTerrainLogic

//-------------------------------------------------------------------------------------------------
/** Device DEPENDENT implementation init details for logical terrain */
//-------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::init( void )
{

   // enhancing functionality
   TerrainLogic::init();
   m_mapDX = 0;
   m_mapDY = 0;
   m_mapMinZ = 0;
   m_mapMaxZ = 1;

}  // end init

//-------------------------------------------------------------------------------------------------
/** Reset */
//-------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::reset( void )
{
   TerrainLogic::reset();
   m_mapDX = 0;
   m_mapDY = 0;
   m_mapMinZ = 0;
   m_mapMaxZ = 1;
   WorldHeightMap::freeListOfMapObjects();
}  // end reset

//-------------------------------------------------------------------------------------------------
/** newMap */
//-------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::newMap(Bool saveGame)
{
#if 0
   TheTerrainRenderObject->loadRoadsAndBridges(this, saveGame);
#endif // if 0
   TerrainLogic::newMap(saveGame);
}  // end update

//-------------------------------------------------------------------------------------------------
/** Update */
//-------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::update( void )
{
   TerrainLogic::update();
}  // end update

//-------------------------------------------------------------------------------------------------
/** Device DEPENDENT implementation for load details of logical terrain.
Note - if query is true, we are  */
//-------------------------------------------------------------------------------------------------
Bool LinuxTerrainLogic::loadMap( AsciiString filename , Bool query )
{
   if(!TheMapCache)
      return FALSE;

   WorldHeightMap *terrainHeightMap;   ///< holds raw heightmap data samples

   char tempBuf[PATH_MAX];
   char filenameBuf[PATH_MAX];

   strcpy(tempBuf, filename.str());

   size_t length {strlen(tempBuf)};
   if (length >= 4) {
      memset( filenameBuf, '\0', PATH_MAX);
      strncpy( filenameBuf, tempBuf, length - 4);
   }

//	const char *fname = filename.reverseFind('\\');
//	if (fname)
//		filename = fname+1;

   CachedFileInputStream fileStrm;
   if ( !fileStrm.open(filename) )
   {
      return FALSE;
   }

   ChunkInputStream *pStrm = &fileStrm;

   // allocate new height map data to read from file
   // this will be a dummy object only containing logical map data
   terrainHeightMap = NEW WorldHeightMap(pStrm, true);

   if (terrainHeightMap)
   {
      //copy loaded data
      // Get the whole map, because we don't know which boundary is active yet
      m_mapDX=terrainHeightMap->getXExtent();
      m_mapDY=terrainHeightMap->getYExtent();

      // now, get all the boundaries, and set the current active boundary to boundary 0.
      m_boundaries = terrainHeightMap->getAllBoundaries();
      m_activeBoundary = 0;

      Int i, j, minHt, maxHt;

      minHt = terrainHeightMap->getMaxHeightValue();
      maxHt = 0;

      for (j=0; j<m_mapDY; j++) {
         for (i=0; i<m_mapDX; i++) {
            Short cur = terrainHeightMap->getHeight(i,j);
            if (cur<minHt) minHt = cur;
            if (maxHt<cur) maxHt = cur;
         }
      }
      m_mapMinZ = minHt * MAP_HEIGHT_SCALE;
      m_mapMaxZ = maxHt * MAP_HEIGHT_SCALE;
      //release temporary object used for loading height values
      REF_PTR_RELEASE(terrainHeightMap);
   }
   else
      return FALSE;	//could not create heightmap object.  File not found?

   // MG: Moving this to before the VBOs get created; otherwise, the diffuse colour won't be set properly.
   if( TheWritableGlobalData->setTimeOfDay( TheGlobalData->m_data.m_timeOfDay ) )
      TheGameClient->setTimeOfDay( TheGlobalData->m_data.m_timeOfDay );

   // Note - It is very important that this get called AFTER the map is read in.  jba.
   // enhancing functionality
   if( TerrainLogic::loadMap( filename, query ) == false )
      return FALSE;

   // Map file now contains lighting & time of day info.
   // if( TheWritableGlobalData->setTimeOfDay( TheGlobalData->m_data.m_timeOfDay ) )
   //    TheGameClient->setTimeOfDay( TheGlobalData->m_data.m_timeOfDay );

   return TRUE;  // success

}  // end load

//-------------------------------------------------------------------------------------------------
/** Get the 3D extent of the terrain in world coordinates */
//-------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::getExtent( Region3D *extent ) const 
{
   extent->lo.x = 0.0f;

   extent->lo.y = 0.0f;

   // Note - m_boundaries are stored in height map grids wide, so we have to
   // multiply by the grid width.
   if (m_boundaries.size() > 0) {
      extent->hi.x = m_boundaries.data()[m_activeBoundary].x*MAP_XY_FACTOR;
      extent->hi.y = m_boundaries.data()[m_activeBoundary].y*MAP_XY_FACTOR;
   } else {
      extent->hi.x = 0.0f;
      extent->hi.y = 0.0f;
   }

   // Note - m_mapMin and Max Z are already in real coord space.
   extent->lo.z = m_mapMinZ;
   extent->hi.z = m_mapMaxZ;
}

//-------------------------------------------------------------------------------------------------
/** Get the 3D largest bounds defined in the map. */
//-------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::getMaximumPathfindExtent( Region3D *extent ) const
{
   extent->lo.x = 0.0f;

   extent->lo.y = 0.0f;
   extent->hi.x = 0.0f;
   extent->hi.y = 0.0f;

   // Note - m_boundaries are stored in height map grids wide, so we have to
   // multiply by the grid width.
   for (size_t i=0; i<m_boundaries.size(); i++) {
      if (extent->hi.x < m_boundaries[i].x*MAP_XY_FACTOR) {
         extent->hi.x = m_boundaries[i].x*MAP_XY_FACTOR;
      }
      if (extent->hi.y < m_boundaries[i].y*MAP_XY_FACTOR) {
         extent->hi.y = m_boundaries[i].y*MAP_XY_FACTOR;
      }
   }

   // Note - m_mapMin and Max Z are already in real coord space.
   extent->lo.z = m_mapMinZ;
   extent->hi.z = m_mapMaxZ;
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::getExtentIncludingBorder( Region3D *extent ) const 
{
(void) extent;
DEBUG_CRASH(("LinuxTerrainLogic::getExtentIncludingBorder not yet implemented!"));
#if 0
   extent->lo.x = 0.0f;
   extent->lo.y = 0.0f;

   Real border = TheTerrainRenderObject->getMap()->getBorderSizeInline() * MAP_XY_FACTOR;
   extent->lo.x -= border;
   extent->lo.y -= border;
   extent->hi.x = (m_mapDX * MAP_XY_FACTOR)-border;
   extent->hi.y = (m_mapDY * MAP_XY_FACTOR)-border;
#endif // if 0
}

//-------------------------------------------------------------------------------------------------
Bool LinuxTerrainLogic::isClearLineOfSight(const Coord3D& pos, const Coord3D& posOther) const
{
(void) pos;
(void) posOther;
DEBUG_CRASH(("LinuxTerrainLogic::isClearLineOfSight not yet implemented!"));
return false;
#if 0
   if (TheTerrainRenderObject) 
   {
      return TheTerrainRenderObject->isClearLineOfSight(pos, posOther);
   }	
   else
   {
      return false;
   }
#endif // if 0
}

//-------------------------------------------------------------------------------------------------
/** Linux specific get height function for logical terrain */
//-------------------------------------------------------------------------------------------------
Real LinuxTerrainLogic::getGroundHeight(Real x, Real y, Coord3D* normal) const
{
#define USE_THE_TERRAIN_OBJECT
#ifdef USE_THE_TERRAIN_OBJECT
   if (TheTerrainRenderObject) {
      return TheTerrainRenderObject->getHeightMapHeight(x, y, normal);
   } else {
      if (normal) {
         //return a default normal pointing up
         normal->x = 0.0f;
         normal->y = 0.0f;
         normal->z = 1.0f;
      }
      return 0;
   }
#endif
}  // end getHight

//-------------------------------------------------------------------------------------------------
/** Get the height considering the layer. */
//-------------------------------------------------------------------------------------------------
Real LinuxTerrainLogic::getLayerHeight( Real x, Real y, PathfindLayerEnum layer, Coord3D* normal, Bool clip ) const
{
(void) x;
(void) y;
(void) layer;
(void) normal;
(void) clip;
DEBUG_CRASH(("LinuxTerrainLogic::getLayerHeight not yet implemented!"));
return 0.0;
#if 0
#ifdef USE_THE_TERRAIN_OBJECT

   if (!TheTerrainRenderObject)
   {
      if (normal)
      {	
         //return a default normal pointing up
         normal->x=0.0f;
         normal->y=0.0f;
         normal->z=1.0f;
      }
      return 0;
   }

   Real height = TheTerrainRenderObject->getHeightMapHeight(x,y,normal);

   if (layer != LAYER_GROUND) 
   {
      Coord3D loc;
      loc.x = x;
      loc.y = y;
      loc.z = height;
      if (layer == LAYER_WALL) 
      {
         if (!clip || TheAI->pathfinder()->isPointOnWall(&loc)) 
         {
            return TheAI->pathfinder()->getWallHeight();
         }
         else
         {
            return height;
         }
      }
      Bridge* pBridge;
      if ((pBridge = findBridgeLayerAt(&loc, layer, clip)) != 0) 
      {
         Real bridgeHeight = pBridge->getBridgeHeight(&loc, normal);
         if (bridgeHeight > height) 
         {
            return bridgeHeight;	// Don't return bridge height if it's in the ground.
         }
      }
   }

   return height;

#endif
#endif // if 0
}  // end getLayerHeight

//-------------------------------------------------------------------------------------------------
/** Linux isCliffCell for terrain logic */
//-------------------------------------------------------------------------------------------------
Bool LinuxTerrainLogic::isCliffCell( Real x, Real y) const
{
(void) x;
(void) y;
DEBUG_CRASH(("LinuxTerrainLogic::isCliffCell not yet implemented!"));
return false;
#if 0

   return TheTerrainRenderObject->isCliffCell(x,y);

#endif // if 0
}  // end isCliffCell

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::crc( Xfer *xfer )
{

   // extend base class
   TerrainLogic::crc( xfer );

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer
   * Version Info:
   * 1: Initial version */
// ------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::xfer( Xfer *xfer )
{

   // version
   XferVersion currentVersion = 1;
   XferVersion version = currentVersion;
   xfer->xferVersion( &version, currentVersion );
   
   // extend base class
   TerrainLogic::xfer( xfer );
      
}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void LinuxTerrainLogic::loadPostProcess( void )
{

   // extend base class
   TerrainLogic::loadPostProcess();

}  // end loadPostProcess
