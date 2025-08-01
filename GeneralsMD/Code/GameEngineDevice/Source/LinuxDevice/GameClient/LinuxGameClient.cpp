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

//////// LinuxGameClient.cpp //////////////////////////
// Matthew Gill, April 2025
///////////////////////////////////////////////////////

#include "GameClient/Drawable.h"
#include "LinuxDevice/GameClient/LinuxGameClient.h"
#include "LinuxDevice/GameClient/SdlKeyboard.h"
#include "LinuxDevice/GameClient/SdlMouse.h"

// FIXME: Dummy global function to keep the linker happy.
void addIcon(const Coord3D *pos, Real width, Int numFramesDuration, RGBColor color) {
}

LinuxGameClient::LinuxGameClient() {
}

LinuxGameClient::~LinuxGameClient() {
}

//-------------------------------------------------------------------------------------------------
/** Initialize resources for the game client */
//-------------------------------------------------------------------------------------------------
void LinuxGameClient::init(void) {

   // extending initialization routine
   GameClient::init();

}  // end init

//-------------------------------------------------------------------------------------------------
/** Per frame udpate, note we are extending functionality */
//-------------------------------------------------------------------------------------------------
void LinuxGameClient::update(void) {

   // call base
   GameClient::update();

}  // end update

//-------------------------------------------------------------------------------------------------
/** Reset this device client system.  Note we are extending reset functionality from
   * the device independent client */
//-------------------------------------------------------------------------------------------------
void LinuxGameClient::reset(void) {

   // call base class
   GameClient::reset();

}  // end reset

//-------------------------------------------------------------------------------------------------
/** allocate a new drawable using the thing template for initialization.
   * if we want to have the thing manager actually contain the pools of
   * object and drawable storage it seems OK to have it be friends with the
   * GameLogic/Client for those purposes, or we could put the allocation pools
   * in the GameLogic and GameClient themselves */
//-------------------------------------------------------------------------------------------------
Drawable* LinuxGameClient::friend_createDrawable(const ThingTemplate* tmplate, DrawableStatus statusBits) {
   Drawable *draw = NULL;

   // sanity
   if (tmplate == NULL) {
      return NULL;
   }
   
   draw = newInstance(Drawable)(tmplate, statusBits);

   return draw;

}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxGameClient::addScorch(const Coord3D *pos, Real radius, Scorches type)
{
   (void) pos;
   (void) radius;
   (void) type;
   // if (TheTerrainRenderObject) 
   // {
   //    Vector3 loc(pos->x, pos->y, pos->z);
   //    TheTerrainRenderObject->addScorch(loc, radius, type);
   // }
}

//-------------------------------------------------------------------------------------------------
/** create an effect that requires a start and end location */
//-------------------------------------------------------------------------------------------------
void LinuxGameClient::createRayEffectByTemplate( const Coord3D *start, 
                                                       const Coord3D *end, 
                                                       const ThingTemplate* tmpl )
{
   (void) start;
   (void) end;
   (void) tmpl;
   // Drawable *draw = TheThingFactory->newDrawable(tmpl);

   // if( draw )
   // {
   //    Coord3D pos;

   //    // add to world, the location of the drawable is at the midpoint of laser
   //    pos.x = (end->x - start->x) * 0.5f + start->x;
   //    pos.y = (end->y - start->y) * 0.5f + start->y;
   //    pos.z = (end->z - start->z) * 0.5f + start->z;
   //    draw->setPosition( &pos );

   //    // add this ray effect to the list of ray effects
   //    TheRayEffects->addRayEffect( draw, start, end );
         
   // }  // end if

}  // end createRayEffectByTemplate

//-------------------------------------------------------------------------------------------------
/**  Tell all the drawables what time of day it is now */
//-------------------------------------------------------------------------------------------------
void LinuxGameClient::setTimeOfDay( TimeOfDay tod )
{
   (void) tod;

   // GameClient::setTimeOfDay(tod);

   // //tell cloud/water plane to update its lighting/texture
   // if (TheWaterRenderObj)
   //    TheWaterRenderObj->setTimeOfDay(tod);
   // if (TheW3DShadowManager)
   //    TheW3DShadowManager->setTimeOfDay(tod);

   // //tell the display to update its lighting
   // TheDisplay->setTimeOfDay( tod );

}  // end setTimeOfDay


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxGameClient::setTeamColor(Int red, Int green, Int blue)
{
   (void) red;
   (void) green;
   (void) blue;

   // W3DStatusCircle::setColor(red, green, blue);

}  // end setTeamColor

//-------------------------------------------------------------------------------------------------
/** temporary entry point for adjusting LOD for development testing. */
//-------------------------------------------------------------------------------------------------
void LinuxGameClient::adjustLOD( Int adj ) 
{
   (void) adj;
   // if (TheGlobalData == NULL) 
   //    return;

   // TheWritableGlobalData->m_textureReductionFactor += adj;

   // if (TheWritableGlobalData->m_textureReductionFactor > 4)
   //    TheWritableGlobalData->m_textureReductionFactor = 4;	//16x less resolution is probably enough.
   // if (TheWritableGlobalData->m_textureReductionFactor < 0)
   //    TheWritableGlobalData->m_textureReductionFactor = 0;

   // if (WW3D::Get_Texture_Reduction() != TheWritableGlobalData->m_textureReductionFactor)
   // {	WW3D::Set_Texture_Reduction(TheWritableGlobalData->m_textureReductionFactor,32);
   //    TheGameLODManager->setCurrentTextureReduction(TheWritableGlobalData->m_textureReductionFactor);
   //    if( TheTerrainRenderObject ) 
   //         TheTerrainRenderObject->setTextureLOD( TheWritableGlobalData->m_textureReductionFactor );
   // }

}  // end adjustLOD

//-------------------------------------------------------------------------------------------------
/**  Tell the terrain that an object moved, so it can knock down trees or crush grass
      or whatever is appropriate. jba. */
//-------------------------------------------------------------------------------------------------
void LinuxGameClient::notifyTerrainObjectMoved(Object *obj)
{
   (void) obj;
   // if (TheTerrainRenderObject) {
   //    TheTerrainRenderObject->unitMoved(obj);
   // }

}  // end setTimeOfDay

Keyboard* LinuxGameClient::createKeyboard(void) {
   SdlKeyboard* keyboard {NEW SdlKeyboard};
   TheSdlKeyboard = keyboard;
   return keyboard;
}
Mouse* LinuxGameClient::createMouse(void) {
   SdlMouse* mouse {NEW SdlMouse};
   TheSdlMouse = mouse;
   return mouse;
}
