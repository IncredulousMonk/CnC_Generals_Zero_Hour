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

// FILE: LinuxScienceModelDraw.cpp ////////////////////////////////////////////////////////////////
// Author: Matthew Gill, July 2025
// Description: 
//   Draw module just like Model, except it only draws if the local player has the specified
//   science.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "LinuxDevice/GameClient/Module/LinuxScienceModelDraw.h"

#include "Common/Player.h"
#include "Common/PlayerList.h"
#include "Common/Science.h"
#include "Common/Xfer.h"

//-------------------------------------------------------------------------------------------------
LinuxScienceModelDrawModuleData::LinuxScienceModelDrawModuleData() 
{
   m_ini.m_requiredScience = SCIENCE_INVALID;
}

//-------------------------------------------------------------------------------------------------
LinuxScienceModelDrawModuleData::~LinuxScienceModelDrawModuleData()
{
}

//-------------------------------------------------------------------------------------------------
void LinuxScienceModelDrawModuleData::buildFieldParse(void* what, MultiIniFieldParse& p) 
{
   LinuxModelDrawModuleData::buildFieldParse(what, p);

   static const FieldParse dataFieldParse[] = 
   {
      { "RequiredScience", INI::parseScience, NULL, offsetof(LinuxScienceModelDrawModuleData::IniData, m_requiredScience) },

      { 0, 0, 0, 0 }
   };
   LinuxScienceModelDrawModuleData* self {static_cast<LinuxScienceModelDrawModuleData*>(what)};
   size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
   p.add(dataFieldParse, offset);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxScienceModelDraw::LinuxScienceModelDraw(Thing* thing, const ModuleData* moduleData): LinuxModelDraw(thing, moduleData)
{
}

//-------------------------------------------------------------------------------------------------
LinuxScienceModelDraw::~LinuxScienceModelDraw()
{
}

//-------------------------------------------------------------------------------------------------
// All this does is stop the call path if we haven't been cleared to draw yet
void LinuxScienceModelDraw::doDrawModule(const Matrix3D* transformMtx)
{
(void) transformMtx;
DEBUG_CRASH(("LinuxScienceModelDraw::doDrawModule not yet implemented!"));
#if 0
   ScienceType science = getLinuxScienceModelDrawModuleData()->m_requiredScience;
   if( science == SCIENCE_INVALID )
   {
      DEBUG_ASSERTCRASH(science != SCIENCE_INVALID, ("ScienceModelDraw has invalid science as condition.") );
      setHidden( TRUE );
      return;
   }

   if( !ThePlayerList->getLocalPlayer()->hasScience(science) 
         && ThePlayerList->getLocalPlayer()->isPlayerActive()
      )
   {
      // We just don't draw for people without our science except for Observers
      setHidden( TRUE );
      return;
   }

   LinuxModelDraw::doDrawModule(transformMtx);
#endif // if 0
}

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void LinuxScienceModelDraw::crc( Xfer *xfer )
{

   // extend base class
   LinuxModelDraw::crc( xfer );

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
   * Version Info:
   * 1: Initial version */
// ------------------------------------------------------------------------------------------------
void LinuxScienceModelDraw::xfer( Xfer *xfer )
{

   // version
   XferVersion currentVersion = 1;
   XferVersion version = currentVersion;
   xfer->xferVersion( &version, currentVersion );

   // extend base class
   LinuxModelDraw::xfer( xfer );

}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void LinuxScienceModelDraw::loadPostProcess( void )
{

   // extend base class
   LinuxModelDraw::loadPostProcess();

}  // end loadPostProcess
