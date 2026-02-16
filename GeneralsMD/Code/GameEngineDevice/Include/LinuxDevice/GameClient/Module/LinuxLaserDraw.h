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

// FILE: LinuxLaserDraw.h /////////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, February 2026
// Description: 
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LINUXLASERDRAW_H_
#define __LINUXLASERDRAW_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/DrawModule.h"
//#include "WLinux2/Line3D.h"
#include "GameClient/Color.h"

class SegmentedLineClass;
class TextureClass;

class LinuxLaserDrawModuleData: public ModuleData
{
public:
   // MG: Cannot apply offsetof to LinuxLaserDrawModuleData, so had to move data into an embedded struct.
   struct IniData
   {
      Color m_innerColor;
      Color m_outerColor;
      Real m_innerBeamWidth;
      Real m_outerBeamWidth;
      Real m_scrollRate;
      Bool m_tile;
      UnsignedInt m_numBeams;
      UnsignedInt m_maxIntensityFrames;
      UnsignedInt m_fadeFrames;
      AsciiString m_textureName;
      UnsignedInt m_segments;
      Real m_arcHeight;
      Real m_segmentOverlapRatio;
      Real m_tilingScalar;
   };

   IniData m_ini {};

   LinuxLaserDrawModuleData();
   ~LinuxLaserDrawModuleData();
   static void buildFieldParse(void* what, MultiIniFieldParse& p);
};

//-------------------------------------------------------------------------------------------------
/** Linux laser draw */
//-------------------------------------------------------------------------------------------------
class LinuxLaserDraw: public DrawModule, public LaserDrawInterface
{

   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(LinuxLaserDraw, "LinuxLaserDraw")
   MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA(LinuxLaserDraw, LinuxLaserDrawModuleData)

public:

   LinuxLaserDraw(Thing *thing, const ModuleData* moduleData);
   // virtual destructor prototype provided by memory pool declaration

	// No copies allowed!
	LinuxLaserDraw(const LinuxLaserDraw&) = delete;
	LinuxLaserDraw& operator=(const LinuxLaserDraw&) = delete;

   virtual void doDrawModule(const Matrix3D* transformMtx);
   virtual void releaseShadows(void) {};  ///< we don't care about preserving temporary shadows.	
   virtual void allocateShadows(void) {}; ///< we don't care about preserving temporary shadows.
   virtual void setShadowsEnabled(Bool /* enable */) { }
   virtual void setFullyObscuredByShroud(Bool /* fullyObscured */) { };
   virtual void reactToTransformChange(const Matrix3D* /* oldMtx */, const Coord3D* /* oldPos */, Real /* oldAngle */) { }
   virtual void reactToGeometryChange() { }
   virtual Bool isLaser() const { return true; }
   Real getLaserTemplateWidth() const;
   
   virtual LaserDrawInterface* getLaserDrawInterface() { return this; }
   virtual const LaserDrawInterface* getLaserDrawInterface() const { return this; }

protected:

   SegmentedLineClass** m_line3D;   ///< line 3D for effect
   TextureClass* m_texture;
   Real m_textureAspectRatio;       ///< aspect ratio of texture
   Bool m_selfDirty;                // not saved
};

#endif // __LINUXLASERDRAW_H_
