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

///// LinuxView.cpp //////////////////////////////
// Matthew Gill, August 2025
//////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Linux implementation of the game view window.  View windows are literally
// a window into the game world that have width, height, and camera 
// controls for what to look at
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// SYSTEM INCLUDES ////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

// USER INCLUDES //////////////////////////////////////////////////////////////////////////////////
#include "Common/GlobalData.h"

#include "GameClient/CommandXlat.h"
#include "GameClient/Drawable.h"

#include "GameLogic/Object.h"
#include "GameLogic/ScriptEngine.h"

#include "LinuxDevice/Common/LinuxConvert.h"

#include "LinuxDevice/GameClient/CameraShakeSystem.h"
#include "LinuxDevice/GameClient/HeightMap.h"
#include "LinuxDevice/GameClient/LinuxDisplay.h"
#include "LinuxDevice/GameClient/LinuxView.h"
#include "LinuxDevice/GameClient/Module/LinuxModelDraw.h"

#include "GameLogic/AI.h"           ///< For AI debug (yes, I'm cheating for now)
#include "GameLogic/AIPathfind.h"   ///< For AI debug (yes, I'm cheating for now)
#include "LinuxDevice/GameLogic/LinuxTerrainLogic.h"

#include "rinfo.h"
#include "OpenGLRenderer.h"

// 30 fps
Int TheLinuxFrameLengthInMsec = 1000 / LOGICFRAMES_PER_SECOND; // default is 33msec/frame == 30fps. but we may change it depending on sys config.
static const Int MAX_REQUEST_CACHE_SIZE = 40;   // Any size larger than 10, or examine code below for changes. jkmcd.


//=================================================================================================
inline Real minf(Real a, Real b) { if (a < b) return a; else return b; }
inline Real maxf(Real a, Real b) { if (a > b) return a; else return b; }

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#define TERRAIN_SAMPLE_SIZE 40.0f
static Real getHeightAroundPos(Real x, Real y)
{
   // terrain height + desired height offset == cameraOffset * actual zoom
   Real terrainHeight = TheTerrainLogic->getGroundHeight(x, y);

   // find best approximation of max terrain height we can see
   Real terrainHeightMax = terrainHeight;
   terrainHeightMax = max(terrainHeightMax, TheTerrainLogic->getGroundHeight(x+TERRAIN_SAMPLE_SIZE, y-TERRAIN_SAMPLE_SIZE));
   terrainHeightMax = max(terrainHeightMax, TheTerrainLogic->getGroundHeight(x-TERRAIN_SAMPLE_SIZE, y-TERRAIN_SAMPLE_SIZE));
   terrainHeightMax = max(terrainHeightMax, TheTerrainLogic->getGroundHeight(x+TERRAIN_SAMPLE_SIZE, y+TERRAIN_SAMPLE_SIZE));
   terrainHeightMax = max(terrainHeightMax, TheTerrainLogic->getGroundHeight(x-TERRAIN_SAMPLE_SIZE, y+TERRAIN_SAMPLE_SIZE));

   return terrainHeightMax;
}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxView::LinuxView()
{
   m_3dCamera = NULL;
   m_2dCamera = NULL;
   m_groundLevel = 10.0;

   // MG: What is this camera offset thing?
   // m_cameraOffset.z = TheGlobalData->m_data.m_cameraHeight;
   // m_cameraOffset.y = -(m_cameraOffset.z / tan(TheGlobalData->m_data.m_cameraPitch * (PI / 180.0)));
   // m_cameraOffset.x = -(m_cameraOffset.y * tan(TheGlobalData->m_data.m_cameraYaw * (PI / 180.0)));

   m_viewFilterMode = FM_VIEW_DEFAULT;
   m_viewFilter = FT_VIEW_DEFAULT;
   m_isWireFrameEnabled = m_nextWireFrameEnabled = FALSE;
   m_shakeOffset.x = 0.0f;
   m_shakeOffset.y = 0.0f;
   m_shakeIntensity = 0.0f;
   m_FXPitch = 1.0f;
   m_freezeTimeForCameraMovement = false;
   m_cameraHasMovedSinceRequest = true;
   m_locationRequests.clear();
   m_locationRequests.reserve(MAX_REQUEST_CACHE_SIZE + 10);	// This prevents the vector from ever re-allocing

   //Enhancements from CNC3 WST 4/15/2003. JSC Integrated 5/20/03.
   m_CameraArrivedAtWaypointOnPathFlag = false;	// Scripts for polling camera reached targets
   m_isCameraSlaved = false;						// This is for 3DSMax camera playback
   m_useRealZoomCam = false;						// true;	//WST 10/18/2002
   m_shakerAngles.X = 0.0f;							// Proper camera shake generator & sources
   m_shakerAngles.Y = 0.0f;
   m_shakerAngles.Z = 0.0f;
}  // end LinuxView

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxView::~LinuxView()
{
   REF_PTR_RELEASE(m_3dCamera);

   for (auto objp: m_3dObjects) {
      objp->Release_Ref();
   }
}  // end ~LinuxView

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::init()
{
   // extend View functionality
   View::init();
   setName("LinuxView");

   // set default camera "lookat" point
   Coord3D pos;
   pos.x = 87.0f;
   pos.y = 77.0f;
   pos.z = 0;

   pos.x *= MAP_XY_FACTOR;
   pos.y *= MAP_XY_FACTOR;

   setPosition(&pos);

   // create our 3D camera
   m_3dCamera = NEW_REF(CameraClass, ());
   setCameraTransform();

   m_cameraConstraintValid = false;
   m_scrollAmountCutoff = TheGlobalData->m_data.m_scrollAmountCutoff;
}  // end init

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::reset()
{
   View::reset();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::updateView()
{
   UPDATE();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::update()
{
}

//-------------------------------------------------------------------------------------------------
/** Draw member for the Linux window, this will literally draw the window 
  * for this view */
//-------------------------------------------------------------------------------------------------
void LinuxView::drawView()
{
   DRAW();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::draw()
{
   RenderInfoClass rinfo {*m_3dCamera};
   for (auto objp: m_3dObjects) {
      objp->Render(rinfo);
   }
}

//-------------------------------------------------------------------------------------------------
/** Set the view angle & pitch back to default */
//-------------------------------------------------------------------------------------------------
void LinuxView::setAngleAndPitchToDefault()
{ 
   // call our base class, we are adding functionality
   View::setAngleAndPitchToDefault();

   this->m_FXPitch = 1.0;

   // set the camera
   setCameraTransform();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::lookAt(const Coord3D* o)
{
   Coord3D pos = *o;


   // Don't call the super-lookAt, because that's for 2D views.
   // View::lookAt(&pos);

   DEBUG_ASSERTLOG(pos.z == 0, ("LinuxView::lookAt: o.z was not zero, it was %.3f.\n", pos.z));
#if 0
   if (o->z > PATHFIND_CELL_SIZE_F + TheTerrainLogic->getGroundHeight(pos.x, pos.y)) {
      // Pos.z is not used, so if we want to look at something off the ground, 
      // we have to look at the spot on the ground such that the object intersects
      // with the look at vector in the center of the screen.  jba.
      Vector3 rayStart, rayEnd;
      LineSegClass lineseg;
      CastResultStruct result;
      Vector3 intersection(0, 0, 0);

      rayStart = m_3dCamera->Get_Position(); //get camera location
      m_3dCamera->Un_Project(rayEnd, Vector2(0.0f, 0.0f));  //get world space point
      rayEnd -= rayStart;  //vector camera to world space point
      rayEnd.Normalize();  //make unit vector
      rayEnd *= m_3dCamera->Get_Depth();  //adjust length to reach far clip plane
      rayStart.Set(pos.x, pos.y, pos.z);
      rayEnd += rayStart;  //get point on far clip plane along ray from camera.
      lineseg.Set(rayStart,rayEnd);

      RayCollisionTestClass raytest(lineseg,&result);

      if( TheTerrainRenderObject->Cast_Ray(raytest) )
      {
         // get the point of intersection according to W3D
         pos.x = result.ContactPoint.X;
         pos.y = result.ContactPoint.Y;

      }  // end if
   }
#endif // if 0

   pos.z = 0;
   setPosition(&pos); 
   m_doingRotateCamera = false;
   m_doingMoveCameraOnWaypointPath = false;
   m_CameraArrivedAtWaypointOnPathFlag = false;
   m_doingScriptedCameraLock = false;

   setCameraTransform();

}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::initHeightForMap()
{
   m_groundLevel = TheTerrainLogic->getGroundHeight(m_pos.x, m_pos.y);
   const Real MAX_GROUND_LEVEL = 120.0; // jba - starting ground level can't exceed this height.
   if (m_groundLevel>MAX_GROUND_LEVEL) {
      m_groundLevel = MAX_GROUND_LEVEL;
   }

   m_cameraOffset.z = m_groundLevel + TheGlobalData->m_data.m_cameraHeight;
   m_cameraOffset.y = -(m_cameraOffset.z / tan(TheGlobalData->m_data.m_cameraPitch * (PI / 180.0)));
   m_cameraOffset.x = -(m_cameraOffset.y * tan(TheGlobalData->m_data.m_cameraYaw * (PI / 180.0)));
   m_cameraConstraintValid = false;	// possible ground level change invalidates cam constraints
   setCameraTransform();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::setZoomToDefault()
{
   // default zoom has to be max, otherwise players will just zoom to max always

   // terrain height + desired height offset == cameraOffset * actual zoom
   // find best approximation of max terrain height we can see
   Real terrainHeightMax = getHeightAroundPos(m_pos.x, m_pos.y);

   Real desiredHeight = (terrainHeightMax + m_maxHeightAboveGround);
   Real desiredZoom = desiredHeight / m_cameraOffset.z;

   //DEBUG_LOG(("LinuxView::setZoomToDefault() Current zoom: %g  Desired zoom: %g\n", m_zoom, desiredZoom));

   m_zoom = desiredZoom;
   m_heightAboveGround = m_maxHeightAboveGround;

   m_doingMoveCameraOnWaypointPath = false;
   m_CameraArrivedAtWaypointOnPathFlag = false;
   m_doingRotateCamera = false;
   m_doingPitchCamera = false;
   m_doingZoomCamera = false;
   m_doingScriptedCameraLock = false;
   m_cameraConstraintValid = false; // recalc it.
   setCameraTransform();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::add3dObject(RenderObjClass* obj)
{
   m_3dObjects.push_front(obj);
   obj->Add_Ref();
}



//-------------------------------------------------------------------------------------------------
//         Private Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/** set the transform matrix of m_3DCamera, based on m_pos & m_angle */
//-------------------------------------------------------------------------------------------------
void LinuxView::setCameraTransform( void )
{
   m_cameraHasMovedSinceRequest = true;
   Matrix3D cameraTransform(1);

   Real nearZ, farZ;
   // m_3DCamera->Get_Clip_Planes(nearZ, farZ);
   // Set the near to MAP_XY_FACTOR.  Improves zbuffer resolution.
   nearZ = MAP_XY_FACTOR;
   farZ = 1200.0f;

   if (m_useRealZoomCam) { //WST 10.19.2002
      if (m_FXPitch < 0.95f) {
         farZ = farZ / m_FXPitch; //Extend far Z when we pitch up for RealZoomCam
      }
   } else {
      if ((TheGlobalData && TheGlobalData->m_data.m_drawEntireTerrain) || (m_FXPitch < 0.95f || m_zoom > 1.05)) {
         //need to extend far clip plane so entire terrain can be visible
         farZ *= MAP_XY_FACTOR;
      }
   }

   m_3dCamera->Set_Clip_Planes(nearZ, farZ);
#if defined(_DEBUG) || defined(_INTERNAL)
   if (TheGlobalData->m_data.m_useCameraConstraints)
#endif
   {
      if (!m_cameraConstraintValid) {
         buildCameraTransform(&cameraTransform);
         m_3dCamera->Set_Transform(cameraTransform);
         calcCameraConstraints();
      }
      DEBUG_ASSERTLOG(m_cameraConstraintValid,("*** cam constraints are not valid!!!\n"));

      if (m_cameraConstraintValid) {
         Coord3D pos = *getPosition();
         pos.x = maxf(m_cameraConstraint.lo.x, pos.x);
         pos.x = minf(m_cameraConstraint.hi.x, pos.x);
         pos.y = maxf(m_cameraConstraint.lo.y, pos.y);
         pos.y = minf(m_cameraConstraint.hi.y, pos.y);
         setPosition(&pos);
      }
   }

#if defined(_DEBUG) || defined(_INTERNAL)
   m_3dCamera->Set_View_Plane(m_FOV, -1);
#endif

   // rebuild it (even if we just did it due to camera constraints)
   buildCameraTransform( &cameraTransform );
   m_3dCamera->Set_Transform( cameraTransform );

   if (TheTerrainRenderObject) {
// FIXME: createLightsIterator
      // UpdateCenter is messing with the VBOs.  Why?!
      // TheTerrainRenderObject->updateCenter(m_3dCamera, nullptr);
#if 0
      RefRenderObjListIterator *it = W3DDisplay::m_3DScene->createLightsIterator();
      TheTerrainRenderObject->updateCenter(m_3dCamera, it);
      if (it) {
         W3DDisplay::m_3DScene->destroyLightsIterator(it);
         it = NULL;
      }
#endif // if 0
   }
}

//-------------------------------------------------------------------------------------------------
/** @todo This is inefficient. We should construct the matrix directly using vectors. */
//-------------------------------------------------------------------------------------------------
#define MIN_CAPPED_ZOOM (0.5f) //WST 10.19.2002. JSC integrated 5/20/03.
void LinuxView::buildCameraTransform(Matrix3D *transform)
{
   Vector3 sourcePos, targetPos;

   Real groundLevel = m_groundLevel; // 93.0f; 

   Real zoom = getZoom();
   Real angle = getAngle();
   Real pitch = getPitch();
   Coord3D pos = *getPosition();

   // add in the camera shake, if any
   pos.x += m_shakeOffset.x;
   pos.y += m_shakeOffset.y;

   if (m_cameraConstraintValid) {
      pos.x = maxf(m_cameraConstraint.lo.x, pos.x);
      pos.x = minf(m_cameraConstraint.hi.x, pos.x);
      pos.y = maxf(m_cameraConstraint.lo.y, pos.y);
      pos.y = minf(m_cameraConstraint.hi.y, pos.y);
   }

   // set position of camera itself
   if (m_useRealZoomCam) { //WST 10/10/2002 Real Zoom using FOV
      sourcePos.X = m_cameraOffset.x;
      sourcePos.Y = m_cameraOffset.y;
      sourcePos.Z = m_cameraOffset.z;
      Real capped_zoom = zoom;
      if (capped_zoom > 1.0f) {
         capped_zoom= 1.0f;
      }
      if (capped_zoom < MIN_CAPPED_ZOOM) {
         capped_zoom = MIN_CAPPED_ZOOM;
      }
      m_FOV = 50.0f * PI/180.0f * capped_zoom * capped_zoom;
   } else {
      sourcePos.X = m_cameraOffset.x*zoom;
      sourcePos.Y = m_cameraOffset.y*zoom;
      sourcePos.Z = m_cameraOffset.z*zoom;
   }

#ifdef NOT_IN_USE
   if (TheGlobalData->m_isOffsetCameraZ && TheTerrainLogic) {
      sourcePos.Z += TheTerrainLogic->getGroundHeight(pos.x, pos.y);
      if (m_prevSourcePosZ != SOURCEPOS_INVALID) {
         const Real MAX_SPZ_VARIATION = 0.05f;
         Real spzMin = m_prevSourcePosZ*(1.0-MAX_SPZ_VARIATION);
         Real spzMax			Coord3D center;
 = m_prevSourcePosZ*(1.0+MAX_SPZ_VARIATION);
         if (sourcePos.Z < spzMin) sourcePos.Z = spzMin;
         if (sourcePos.Z > spzMax) sourcePos.Z = spzMax;
      }
      m_prevSourcePosZ = sourcePos.Z;
   }
#endif

   // camera looking at origin
   targetPos.X = 0;
   targetPos.Y = 0;
   targetPos.Z = 0;

   Real factor = 1.0 - (groundLevel/sourcePos.Z );

   // construct a matrix to rotate around the up vector by the given angle
   Matrix3D angleTransform( Vector3( 0.0f, 0.0f, 1.0f ), angle );

   // construct a matrix to rotate around the horizontal vector by the given angle
   Matrix3D pitchTransform( Vector3( 1.0f, 0.0f, 0.0f ), pitch );

   // rotate camera position (pitch, then angle)
#ifdef ALLOW_TEMPORARIES
   sourcePos = pitchTransform * sourcePos;
   sourcePos = angleTransform * sourcePos;
#else
   pitchTransform.mulVector3(sourcePos);
   angleTransform.mulVector3(sourcePos);
#endif
   sourcePos *= factor;

   // translate to current XY position
   sourcePos.X += pos.x;
   sourcePos.Y += pos.y;
   sourcePos.Z += groundLevel;
   
   targetPos.X += pos.x;
   targetPos.Y += pos.y;
   targetPos.Z += groundLevel;

   // do m_FXPitch adjustment.
   //WST Real height = sourcePos.Z - targetPos.Z;
   //WST height *= m_FXPitch;
   //WST targetPos.Z = sourcePos.Z - height;

   // The following code moves camera down and pitch up when player zooms in.
   // Use scripts to switch to useRealZoomCam
   if (m_useRealZoomCam) {
      Real pitch_adjust = 1.0f;

      if (!TheDisplay->isLetterBoxed()) {
         Real capped_zoom = zoom;
         if (capped_zoom > 1.0f) {
             capped_zoom= 1.0f;
         }
         if (capped_zoom < MIN_CAPPED_ZOOM) {
            capped_zoom = MIN_CAPPED_ZOOM;
         }
         sourcePos.Z = sourcePos.Z * ( 0.5f + capped_zoom * 0.5f); // move camera down physically
         pitch_adjust = capped_zoom;	// adjust camera to pitch up
      }
      m_FXPitch = 1.0f * (0.25f + pitch_adjust*0.75f);
   }

   // do fxPitch adjustment
   if (m_useRealZoomCam) {
      sourcePos.X = targetPos.X + ((sourcePos.X - targetPos.X) / m_FXPitch);
      sourcePos.Y = targetPos.Y + ((sourcePos.Y - targetPos.Y) / m_FXPitch);
   } else {
      if (m_FXPitch <= 1.0f) {
         Real height = sourcePos.Z - targetPos.Z;
         height *= m_FXPitch;
         targetPos.Z = sourcePos.Z - height;
      } else {
         sourcePos.X = targetPos.X + ((sourcePos.X - targetPos.X) / m_FXPitch);
         sourcePos.Y = targetPos.Y + ((sourcePos.Y - targetPos.Y) / m_FXPitch);
      }
   }

   //m_3DCamera->Set_View_Plane(DEG_TO_RADF(50.0f));
   //DEBUG_LOG(("zoom %f, SourceZ %f, posZ %f, groundLevel %f CamOffZ %f\n",
   //   zoom, sourcePos.Z, pos.z, groundLevel,m_cameraOffset.z));

   // build new camera transform
   transform->Make_Identity();
   transform->Look_At( sourcePos, targetPos, 0 );

   //WST 11/12/2002 New camera shaker system 
   CameraShakerSystem.Timestep(1.0f/30.0f); 
   CameraShakerSystem.Update_Camera_Shaker(sourcePos, &m_shakerAngles);
   transform->Rotate_X(m_shakerAngles.X);
   transform->Rotate_Y(m_shakerAngles.Y);
   transform->Rotate_Z(m_shakerAngles.Z);

   //if (m_shakerAngles.X >= 0.0f)
   //{
   //	DEBUG_LOG(("m_shakerAngles %f, %f, %f\n", m_shakerAngles.X, m_shakerAngles.Y, m_shakerAngles.Z));
   //}

   // (gth) check if the camera is being controlled by an animation
   if (m_isCameraSlaved) {
      // find object named m_cameraSlaveObjectName
      Object * obj = TheScriptEngine->getUnitNamed(m_cameraSlaveObjectName);
      
      if (obj != NULL) {
         // dig out the drawable
         Drawable * draw = obj->getDrawable();
         if (draw != NULL) {

            // dig out the first draw module with an ObjectDrawInterface
            for (DrawModule ** dm = draw->getDrawModules(); *dm; ++dm) {
               const ObjectDrawInterface* di = (*dm)->getObjectDrawInterface();
               if (di) {
                  Matrix3D tm;
                  di->clientOnly_getRenderObjBoneTransform(m_cameraSlaveObjectBoneName, &tm);

                  // Ok, slam it into the camera!
                  *transform = tm;

                  //--------------------------------------------------------------------
                  // WST 10.22.2002. Update the Listener positions used by audio system
                  //--------------------------------------------------------------------
                  Vector3 position = transform->Get_Translation();
                  m_pos.x = position.X; 
                  m_pos.y = position.Y; 
                  m_pos.z = position.Z; 

                  //DEBUG_LOG(("mpos x%f, y%f, z%f\n", m_pos.x, m_pos.y, m_pos.z ));

                  break;
               }
            }
         } else {
            m_isCameraSlaved = false;
         }
      } else {
         m_isCameraSlaved = false;
      }
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxView::calcCameraConstraints()
{
//	const Matrix3D& cameraTransform = m_3DCamera->Get_Transform();

//	DEBUG_LOG(("*** rebuilding cam constraints\n"));

   // ok, now check to ensure that we can't see outside the map region,
   // and twiddle the camera if needed
   if (TheTerrainLogic) {
      Region3D mapRegion;
      TheTerrainLogic->getExtent( &mapRegion );
      
   /*
      Note the following restrictions on camera constraints!

      -- they assume that all maps are height 'm_groundLevel' at the edges.
            (since you need to add some "buffer" around the edges of your map
            anyway, this shouldn't be an issue.)

      -- for angles/pitches other than zero, it may show boundaries.
            since we currently plan the game to be restricted to this,
            it shouldn't be an issue.

   */
      Real maxEdgeZ = m_groundLevel;
//    const Real BORDER_FUDGE = MAP_XY_FACTOR * 1.414f;
      Coord3D center, bottom;
      ICoord2D screen;

      //Pick at the center
      screen.x=0.5f*getWidth()+m_originX;
      screen.y=0.5f*getHeight()+m_originY;

      Vector3 rayStart,rayEnd;

      getPickRay(&screen,&rayStart,&rayEnd);

      center.x = Vector3::Find_X_At_Z(maxEdgeZ, rayStart, rayEnd);
      center.y = Vector3::Find_Y_At_Z(maxEdgeZ, rayStart, rayEnd);
      center.z = maxEdgeZ;

      screen.y = m_originY+ 0.95f*getHeight();
      getPickRay(&screen,&rayStart,&rayEnd);
      bottom.x = Vector3::Find_X_At_Z(maxEdgeZ, rayStart, rayEnd);
      bottom.y = Vector3::Find_Y_At_Z(maxEdgeZ, rayStart, rayEnd);
      bottom.z = maxEdgeZ;
      center.x -= bottom.x;
      center.y -= bottom.y;

      Real offset = center.length();

      if (TheGlobalData->m_data.m_debugAI) {
         offset = -1000; // push out the constraints so we can look at staging areas.
      }

      m_cameraConstraint.lo.x = mapRegion.lo.x + offset;
      m_cameraConstraint.hi.x = mapRegion.hi.x - offset;
      // this looks inverted, but is correct
      m_cameraConstraint.lo.y = mapRegion.lo.y + offset;
      m_cameraConstraint.hi.y = mapRegion.hi.y - offset;
      m_cameraConstraintValid = true;
   }
}

//-------------------------------------------------------------------------------------------------
/** Returns a world-space ray originating at a given screen pixel position
   and ending at the far clip plane for current camera.  Screen coordinates
   assumed in absolute values relative to full display resolution.*/
//-------------------------------------------------------------------------------------------------
void LinuxView::getPickRay(const ICoord2D *screen, Vector3 *rayStart, Vector3 *rayEnd)
{
   Real logX,logY;

   //W3D Screen coordinates are -1 to 1, so we need to do some conversion:
   PixelScreenToLinuxLogicalScreen(screen->x - m_originX,screen->y - m_originY, &logX, &logY,getWidth(),getHeight());

   *rayStart = m_3dCamera->Get_Position(); //get camera location
   m_3dCamera->Un_Project(*rayEnd,Vector2(logX,logY)); //get world space point
   *rayEnd -= *rayStart; //vector camera to world space point
   rayEnd->Normalize(); //make unit vector
   *rayEnd *= m_3dCamera->Get_Depth(); //adjust length to reach far clip plane
   *rayEnd += *rayStart; //get point on far clip plane along ray from camera.
}
