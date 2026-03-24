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

// FILE: LinuxSupplyDraw.cpp //////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, July 2025
// Description: 
//   Draw module reacts to SupplyStatus setting by hiding an equal number of the specified bone array.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "Common/Xfer.h"
#include "GameClient/Drawable.h"
#include "LinuxDevice/GameClient/Module/LinuxSupplyDraw.h"

//-------------------------------------------------------------------------------------------------
LinuxSupplyDrawModuleData::LinuxSupplyDrawModuleData() 
{
}

//-------------------------------------------------------------------------------------------------
LinuxSupplyDrawModuleData::~LinuxSupplyDrawModuleData()
{
}

//-------------------------------------------------------------------------------------------------
void LinuxSupplyDrawModuleData::buildFieldParse(void* what, MultiIniFieldParse& p) 
{
   LinuxModelDrawModuleData::buildFieldParse(what, p);

   static const FieldParse dataFieldParse[] = 
   {
      { "SupplyBonePrefix", INI::parseAsciiString, NULL, offsetof(LinuxSupplyDrawModuleData::IniData, m_supplyBonePrefix) },

      { 0, 0, 0, 0 }
   };
   LinuxSupplyDrawModuleData* self {static_cast<LinuxSupplyDrawModuleData*>(what)};
   size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
   p.add(dataFieldParse, offset);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxSupplyDraw::LinuxSupplyDraw(Thing* thing, const ModuleData* moduleData): LinuxModelDraw(thing, moduleData)
{
   m_totalBones = -1;
   m_lastNumberShown = 0;
}

//-------------------------------------------------------------------------------------------------
LinuxSupplyDraw::~LinuxSupplyDraw()
{
}

void LinuxSupplyDraw::updateDrawModuleSupplyStatus( Int maxSupply, Int currentSupply )
{
   LinuxModelDraw::updateDrawModuleSupplyStatus( maxSupply, currentSupply );

   AsciiString boneName = getLinuxSupplyDrawModuleData()->m_ini.m_supplyBonePrefix;
   if( m_totalBones == -1 )
   {
      m_totalBones = getDrawable()->getPristineBonePositions( boneName.str(), 1, NULL, NULL, INT_MAX );// The last arg is to guard the size of the arrays.  I am not passing any in, I am just counting bones.
      m_lastNumberShown = m_totalBones;
   }

   // Figure the % of our bones we should show, and if it is a different % than last time
   // start showing and hiding them.
   Int bonesToShow = ceil(m_totalBones * ( currentSupply / (float)maxSupply ));
   bonesToShow = min( bonesToShow, m_totalBones );

   if( bonesToShow != m_lastNumberShown )
   {
      // Show/hide the bones that are now different, the indices between last and now (low, high].
      Int lowIndex = min( m_lastNumberShown, bonesToShow );
      Int highIndex = max( m_lastNumberShown, bonesToShow );
      Bool hide = bonesToShow < m_lastNumberShown;
      Int currentIndex = lowIndex + 1;

      std::vector<ModelConditionInfo::HideShowSubObjInfo> boneVector;
      while( currentIndex <= highIndex )
      {
         char buffer[16];
         sprintf( buffer, "%s%02d", boneName.str(), currentIndex );
         ModelConditionInfo::HideShowSubObjInfo info {};
         info.hide = hide;
         info.subObjName = buffer;
         boneVector.push_back(info);

         ++currentIndex;
      }
      doHideShowSubObjs(&boneVector);

      m_lastNumberShown = bonesToShow;
   }
}

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void LinuxSupplyDraw::crc( Xfer *xfer )
{

   // extend base class
   LinuxModelDraw::crc( xfer );

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
   * Version Info:
   * 1: Initial version */
// ------------------------------------------------------------------------------------------------
void LinuxSupplyDraw::xfer( Xfer *xfer )
{

   // version
   XferVersion currentVersion = 1;
   XferVersion version = currentVersion;
   xfer->xferVersion( &version, currentVersion );

   // extend base class
   LinuxModelDraw::xfer( xfer );

   // Graham says there's no data to save here

}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void LinuxSupplyDraw::loadPostProcess( void )
{

   // extend base class
   LinuxModelDraw::loadPostProcess();

}  // end loadPostProcess
