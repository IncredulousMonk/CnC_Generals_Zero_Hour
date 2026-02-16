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

// FILE: LinuxOverlordAircraftDraw.cpp ////////////////////////////////////////////////////////////
// Author: Matthew Gill, February 2026
// Description: 
//  Units that receive portable structure upgrades (like the Overlord Tank) have a super specific special need.
//  He needs his rider to draw explicitly after him,
//  and he needs direct access to get that rider when everyone else can't see it because of the OverlordContain.
//  In the case of aircraft (Helix, SpectreGunship, etc.) we need this draw module which mimics the OverlordTankDraw
//  but does not draw treads, trackmarks, turrets, etc. Whee!
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/Xfer.h"
#include "GameClient/Drawable.h"
#include "GameLogic/Object.h"
#include "GameLogic/Module/ContainModule.h"
#include "LinuxDevice/GameClient/Module/LinuxOverlordAircraftDraw.h"


#ifdef _INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif


//-------------------------------------------------------------------------------------------------
LinuxOverlordAircraftDrawModuleData::LinuxOverlordAircraftDrawModuleData()
{
}

//-------------------------------------------------------------------------------------------------
LinuxOverlordAircraftDrawModuleData::~LinuxOverlordAircraftDrawModuleData()
{
}

//-------------------------------------------------------------------------------------------------
void LinuxOverlordAircraftDrawModuleData::buildFieldParse(void* what, MultiIniFieldParse& p) 
{
	LinuxModelDrawModuleData::buildFieldParse(what, p);

	static const FieldParse dataFieldParse[] = 
	{
		{ 0, 0, 0, 0 }
	};
	p.add(dataFieldParse);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxOverlordAircraftDraw::LinuxOverlordAircraftDraw( Thing *thing, const ModuleData* moduleData )
: LinuxModelDraw( thing, moduleData )
{
} 

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
LinuxOverlordAircraftDraw::~LinuxOverlordAircraftDraw()
{
}

//-------------------------------------------------------------------------------------------------
void LinuxOverlordAircraftDraw::doDrawModule(const Matrix3D* transformMtx)
{
	LinuxModelDraw::doDrawModule(transformMtx);

	// Our big thing is that we get our specific passenger (the turret thing) and then wake it up and make it draw
	// It depends on us because our renderObject is only made correct in the act of drawing.
	Object *me = getDrawable()->getObject();
	if( me 
		&& me->getContain()
		&& me->getContain()->friend_getRider()
		&& me->getContain()->friend_getRider()->getDrawable()
		)
	{
		Drawable *riderDraw = me->getContain()->friend_getRider()->getDrawable();
		if ( riderDraw )
		{
		TintEnvelope *env = getDrawable()->getColorTintEnvelope();
		if ( env )
			riderDraw->setColorTintEnvelope( *env );

		riderDraw->notifyDrawableDependencyCleared();
			riderDraw->draw( NULL );// What the hell?  This param isn't used for anything
		}
		DEBUG_ASSERTCRASH( riderDraw, ("OverlordAircraftDraw finds no rider's drawable") );
		
	}
}

//-------------------------------------------------------------------------------------------------
void LinuxOverlordAircraftDraw::setHidden(Bool h)
{
	LinuxModelDraw::setHidden(h);

	// We need to hide our rider, since he won't realize he's being contained in a contained container
	Object *me = getDrawable()->getObject();
	if( me 
		&& me->getContain()
		&& me->getContain()->friend_getRider()
		&& me->getContain()->friend_getRider()->getDrawable()
		)
	{
		me->getContain()->friend_getRider()->getDrawable()->setDrawableHidden(h);
	}
}
 
//-------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void LinuxOverlordAircraftDraw::crc( Xfer *xfer )
{

	// extend base class
	LinuxModelDraw::crc( xfer );

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
	* Version Info:
	* 1: Initial version */
// ------------------------------------------------------------------------------------------------
void LinuxOverlordAircraftDraw::xfer( Xfer *xfer )
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
void LinuxOverlordAircraftDraw::loadPostProcess( void )
{

	// extend base class
	LinuxModelDraw::loadPostProcess();

}  // end loadPostProcess
