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

// FILE: LinuxTreeDraw.cpp ////////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, March 2026
// Desc:   Tracer drawing
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include "Common/Thing.h"
#include "Common/ThingTemplate.h"
#include "Common/Xfer.h"
#include "GameLogic/Object.h"
#include "GameClient/Drawable.h"
#include "LinuxDevice/GameClient/Module/LinuxTreeDraw.h"
#include "LinuxDevice/GameClient/BaseHeightMap.h"

#ifdef _INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

//-------------------------------------------------------------------------------------------------
LinuxTreeDrawModuleData::LinuxTreeDrawModuleData()
{
   m_ini.m_framesToMoveInward = 1;
   m_ini.m_framesToMoveOutward = 1;
   m_ini.m_darkening = 0.0f;
   m_ini.m_maxOutwardMovement = 1.0f;

   // Topple parameters. [7/7/2003]
   const Real START_VELOCITY_PERCENT = 0.2f;
   const Real START_ACCEL_PERCENT = 0.01f;
   const Real VELOCITY_BOUNCE_PERCENT = 0.3f;   // multiply the velocity by this when you bounce
   const Real MINIMUM_TOPPLE_SPEED = 0.5f;      // Won't let trees fall slower than this
   m_ini.m_toppleFX = NULL;
   m_ini.m_bounceFX = NULL;
   m_ini.m_stumpName.clear();
   m_ini.m_killWhenToppled = true;
   m_ini.m_doTopple = false;
   m_ini.m_doShadow = false;
   m_ini.m_initialVelocityPercent = START_VELOCITY_PERCENT;
   m_ini.m_initialAccelPercent = START_ACCEL_PERCENT;
   m_ini.m_bounceVelocityPercent = VELOCITY_BOUNCE_PERCENT;
   m_ini.m_minimumToppleSpeed = MINIMUM_TOPPLE_SPEED;
   m_ini.m_sinkFrames = 10*LOGICFRAMES_PER_SECOND;
   m_ini.m_sinkDistance = 20.0f;

}

//-------------------------------------------------------------------------------------------------
LinuxTreeDrawModuleData::~LinuxTreeDrawModuleData()
{
}

//-------------------------------------------------------------------------------------------------
void LinuxTreeDrawModuleData::buildFieldParse(void* what, MultiIniFieldParse& p) 
{
   ModuleData::buildFieldParse(what, p);
   static const FieldParse dataFieldParse[] = 
   {
      { "ModelName",                   INI::parseAsciiString,           NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_modelName) },
      { "TextureName",                 INI::parseAsciiString,           NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_textureName) },
      { "MoveOutwardTime",             INI::parseDurationUnsignedInt,   NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_framesToMoveOutward) },
      { "MoveInwardTime",              INI::parseDurationUnsignedInt,   NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_framesToMoveInward) },
      { "MoveOutwardDistanceFactor",   INI::parseReal,                  NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_maxOutwardMovement) },
      { "DarkeningFactor",             INI::parseReal,                  NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_darkening) },

// Topple parameters [7/7/2003]
      { "ToppleFX",                    INI::parseFXList,                NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_toppleFX ) },
      { "BounceFX",                    INI::parseFXList,                NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_bounceFX ) },
      { "StumpName",                   INI::parseAsciiString,           NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_stumpName ) },
      { "KillWhenFinishedToppling",    INI::parseBool,                  NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_killWhenToppled ) },
      { "DoTopple",                    INI::parseBool,                  NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_doTopple ) },
      { "InitialVelocityPercent",      INI::parsePercentToReal,         NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_initialVelocityPercent ) },
      { "InitialAccelPercent",         INI::parsePercentToReal,         NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_initialAccelPercent ) },
      { "BounceVelocityPercent",       INI::parsePercentToReal,         NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_bounceVelocityPercent ) },
      { "MinimumToppleSpeed",          INI::parsePositiveNonZeroReal,   NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_minimumToppleSpeed ) },
      { "SinkDistance",                INI::parsePositiveNonZeroReal,   NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_sinkDistance ) },
      { "SinkTime",                    INI::parseDurationUnsignedInt,   NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_sinkFrames) },
      { "DoShadow",                    INI::parseBool,                  NULL, offsetof(LinuxTreeDrawModuleData::IniData, m_doShadow ) },
      { 0, 0, 0, 0 }
   };
   LinuxTreeDrawModuleData* self {static_cast<LinuxTreeDrawModuleData*>(what)};
   size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
   p.add(dataFieldParse, offset);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxTreeDraw::LinuxTreeDraw( Thing *thing, const ModuleData* moduleData ) : DrawModule( thing, moduleData ),
m_treeAdded(false)
{

}  // end LinuxTreeDraw


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxTreeDraw::~LinuxTreeDraw( void )
{
}

//-------------------------------------------------------------------------------------------------
void LinuxTreeDraw::reactToTransformChange(const Matrix3D* /* oldMtx */, const Coord3D* /* oldPos */, Real /* oldAngle */)
{
   Drawable *draw = getDrawable();
   if (m_treeAdded) {
      return;
   }
   if (draw->getPosition()->x==0.0f && draw->getPosition()->y == 0.0f) {
      return;
   }
   m_treeAdded = true;
   const LinuxTreeDrawModuleData *moduleData = getLinuxTreeDrawModuleData();
   if (!moduleData) {
      return;
   }
   Real scale = draw->getScale();
   Real scaleRandomness = draw->getTemplate()->getInstanceScaleFuzziness();
   scaleRandomness = 0.0f; // We use the scale fuzziness inside WB to generate random scales, so they don't change at load time. jba. [4/22/2003]
   TheTerrainRenderObject->addTree(draw->getID(), *draw->getPosition(),
      scale, draw->getOrientation(), scaleRandomness, moduleData);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxTreeDraw::doDrawModule(const Matrix3D* /* transformMtx */)
{
   return;
}

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void LinuxTreeDraw::crc( Xfer *xfer )
{

   // extend base class
   DrawModule::crc( xfer );

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
   * Version Info:
   * 1: Initial version */
// ------------------------------------------------------------------------------------------------
void LinuxTreeDraw::xfer( Xfer *xfer )
{

   // version
   XferVersion currentVersion = 1;
   XferVersion version = currentVersion;
   xfer->xferVersion( &version, currentVersion );

   // extend base class
   DrawModule::xfer( xfer );

   // no data to save here, nobody will ever notice

}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void LinuxTreeDraw::loadPostProcess( void )
{

   // extend base class
   DrawModule::loadPostProcess();

}  // end loadPostProcess

