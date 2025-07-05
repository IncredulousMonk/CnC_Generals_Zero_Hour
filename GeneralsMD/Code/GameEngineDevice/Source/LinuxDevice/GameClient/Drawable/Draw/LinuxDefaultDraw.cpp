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
//   Default Linux draw module
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////

#include "Common/FileSystem.h"	// this is only here to pull in LOAD_TEST_ASSETS
#include "Common/GlobalData.h"
#include "Common/ThingTemplate.h"
#include "Common/Xfer.h"
#include "GameClient/Drawable.h"
#include "GameLogic/Object.h"
#include "GameClient/Shadow.h"
#include "GameClient/FXList.h"
#include "GameLogic/TerrainLogic.h"

// #include "WW3D2/HAnim.h"
// #include "WW3D2/HLod.h"
// #include "WW3D2/RendObj.h"
#include "LinuxDevice/GameClient/Module/LinuxDefaultDraw.h"
// #include "W3DDevice/GameClient/W3DAssetManager.h"
// #include "W3DDevice/GameClient/W3DDisplay.h"
// #include "W3DDevice/GameClient/W3DScene.h"
// #include "W3DDevice/GameClient/W3DShadow.h"


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxDefaultDraw::LinuxDefaultDraw(Thing* thing, const ModuleData* moduleData): DrawModule(thing, moduleData)
{
DEBUG_LOG(("LinuxDefaultDraw::LinuxDefaultDraw not yet implemented!"));
#if 0
#ifdef LOAD_TEST_ASSETS
   m_renderObject = NULL;
   m_shadow = NULL;
   if (!getDrawable()->getTemplate()->getLTAName().isEmpty())
   {
      m_renderObject = LinuxDisplay::m_assetManager->Create_Render_Obj(getDrawable()->getTemplate()->getLTAName().str(), getDrawable()->getScale(), 0);

      Shadow::ShadowTypeInfo shadowInfo;
      shadowInfo.m_type=(ShadowType)SHADOW_VOLUME;
      shadowInfo.m_sizeX=0;	//use defaults
      shadowInfo.m_sizeY=0;
      shadowInfo.m_offsetX=0;
      shadowInfo.m_offsetY=0;
      m_shadow = TheLinuxShadowManager->addShadow(m_renderObject, &shadowInfo);


      DEBUG_ASSERTCRASH(m_renderObject, ("Test asset %s not found", getDrawable()->getTemplate()->getLTAName().str()));
      if (m_renderObject)
      {

         LinuxDisplay::m_3DScene->Add_Render_Object(m_renderObject);

         m_renderObject->Set_User_Data(getDrawable()->getDrawableInfo());

         Matrix3D transform;
         ///@todo: Change back to identity once we figure out why objects show up at 0,0,0
         /// OBJECT_PILE
//       transform.Set(Vector3(0,0,9999));
         transform.Set(Vector3(0,0,0));
         m_renderObject->Set_Transform(transform);
      }
   }
#endif
#endif // if 0
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void LinuxDefaultDraw::reactToTransformChange(const Matrix3D* oldMtx, const Coord3D* oldPos, Real oldAngle )
{
(void) oldMtx;
(void) oldPos;
(void) oldAngle;
DEBUG_CRASH(("LinuxDefaultDraw::reactToTransformChange not yet implemented!"));
#if 0
   if( m_renderObject )
      m_renderObject->Set_Transform( *getDrawable()->getTransformMatrix() );
#endif // if 0
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxDefaultDraw::~LinuxDefaultDraw()
{
DEBUG_LOG(("LinuxDefaultDraw::~LinuxDefaultDraw not yet implemented!"));
#if 0
#ifdef LOAD_TEST_ASSETS
   if (TheLinuxShadowManager && m_shadow)
   {	
      TheLinuxShadowManager->removeShadow(m_shadow);
      m_shadow = NULL;
   }
   if (m_renderObject)
   {
      LinuxDisplay::m_3DScene->Remove_Render_Object(m_renderObject);
     REF_PTR_RELEASE(m_renderObject);
      m_renderObject = NULL;
   }
#endif
#endif // if 0
}

//-------------------------------------------------------------------------------------------------
void LinuxDefaultDraw::setShadowsEnabled(Bool enable)
{
(void) enable;
DEBUG_CRASH(("LinuxDefaultDraw::setShadowsEnabled not yet implemented!"));
#if 0
#ifdef LOAD_TEST_ASSETS
   if (m_shadow)
      m_shadow->enableShadowRender(enable);
#endif
#endif // if 0
}

//-------------------------------------------------------------------------------------------------
void LinuxDefaultDraw::setFullyObscuredByShroud(Bool fullyObscured)
{
(void) fullyObscured;
DEBUG_CRASH(("LinuxDefaultDraw::setFullyObscuredByShroud not yet implemented!"));
#if 0
#ifdef LOAD_TEST_ASSETS
   if (m_shadow)
      m_shadow->enableShadowInvisible(fullyObscured);
#endif
#endif // if 0
}

//-------------------------------------------------------------------------------------------------
void LinuxDefaultDraw::doDrawModule(const Matrix3D* transformMtx)
{
(void) transformMtx;
DEBUG_CRASH(("LinuxDefaultDraw::doDrawModule not yet implemented!"));
#if 0
#ifdef LOAD_TEST_ASSETS
   if(m_renderObject)
   {
      Matrix3D scaledTransform;
      if (getDrawable()->getInstanceScale() != 1.0f)
      {	//do custom scaling of the Linux model.
         scaledTransform=*transformMtx;
         scaledTransform.Scale(getDrawable()->getInstanceScale());
         transformMtx = &scaledTransform;
         m_renderObject->Set_ObjectScale(getDrawable()->getInstanceScale());
      }
      else
      {
         m_renderObject->Set_Transform(*transformMtx);
      }
   }
#endif

   return;
#endif // if 0
}

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void LinuxDefaultDraw::crc(Xfer* xfer)
{

   // extend base class
   DrawModule::crc( xfer );

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
   * Version Info:
   * 1: Initial version */
// ------------------------------------------------------------------------------------------------
void LinuxDefaultDraw::xfer(Xfer* xfer)
{

   // version
   XferVersion currentVersion = 1;
   XferVersion version = currentVersion;
   xfer->xferVersion( &version, currentVersion );

   // extend base class
   DrawModule::xfer( xfer );

}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void LinuxDefaultDraw::loadPostProcess()
{

   // extend base class
   DrawModule::loadPostProcess();

}  // end loadPostProcess
