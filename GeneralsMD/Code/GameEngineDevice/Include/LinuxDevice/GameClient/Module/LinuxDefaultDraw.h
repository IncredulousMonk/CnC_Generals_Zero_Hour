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

// FILE: LinuxModuleFactory.h //////////////////////////////////////////////////////////////////////
// Author: Matthew Gill, June 2025
// Description: 
//   Default client update module
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LINUXDEFAULTDRAW_H_
#define __LINUXDEFAULTDRAW_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/GameType.h"
#include "Common/DrawModule.h"
#include "Common/FileSystem.h"	// this is only here to pull in LOAD_TEST_ASSETS

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class Thing;
class RenderObjClass;
class FXList;
class Shadow;

//-------------------------------------------------------------------------------------------------
/** The default client update module */
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
class LinuxDefaultDraw: public DrawModule
{

   MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(LinuxDefaultDraw, "LinuxDefaultDraw")
   MAKE_STANDARD_MODULE_MACRO(LinuxDefaultDraw)

public:

   LinuxDefaultDraw(Thing* thing, const ModuleData* moduleData);
   // virtual destructor prototype provided by memory pool declaration

	// No copies allowed!
	LinuxDefaultDraw(const LinuxDefaultDraw&) = delete;
	LinuxDefaultDraw& operator=(const LinuxDefaultDraw&) = delete;

   /// the draw method
   virtual void doDrawModule(const Matrix3D* transformMtx);

   virtual void setShadowsEnabled(Bool enable);
   virtual void releaseShadows() {};  ///< we don't care about preserving temporary shadows.	
   virtual void allocateShadows() {}; ///< we don't care about preserving temporary shadows.
   virtual void setFullyObscuredByShroud(Bool fullyObscured);
   virtual void reactToTransformChange(const Matrix3D* oldMtx, const Coord3D* oldPos, Real oldAngle);
   virtual void reactToGeometryChange() {}

private:

#ifdef LOAD_TEST_ASSETS
   RenderObjClass*   m_renderObject {};   ///< Linux Render object for this drawable
   Shadow*           m_shadow {};         ///< Updates/Renders shadows of this object
#endif
};

#endif // __LINUXDEFAULTDRAW_H_
