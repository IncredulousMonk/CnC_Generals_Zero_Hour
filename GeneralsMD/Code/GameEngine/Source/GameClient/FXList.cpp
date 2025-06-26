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

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: FXList.cpp ///////////////////////////////////////////////////////////////////////////////
// Author: Steven Johnson, December 2001
// Desc:   FXList descriptions
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#include "GameClient/FXList.h"

// #include "Common/DrawModule.h"
#include "Common/GameAudio.h"
#include "Common/INI.h"
// #include "Common/Player.h"
// #include "Common/PlayerList.h"
#include "Common/RandomValue.h"
#include "Common/ThingTemplate.h"
#include "Common/ThingFactory.h"

#include "GameLogic/Object.h"
#include "GameLogic/GameLogic.h"
// #include "GameLogic/TerrainLogic.h"
#include "GameClient/Display.h"
#include "GameClient/GameClient.h"
#include "GameClient/Drawable.h"
#include "GameClient/ParticleSys.h"
// #include "GameLogic/PartitionManager.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC DATA ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

FXListStore *TheFXListStore = NULL;					///< the FXList store definition

//-------------------------------------------------------------------------------------------------
// static void adjustVector(Coord3D *vec, const Matrix3D* mtx)
// {
// 	if (mtx)
// 	{
// 		Vector3 vectmp;
// 		vectmp.X = vec->x;
// 		vectmp.Y = vec->y;
// 		vectmp.Z = vec->z;
// 		vectmp = mtx->Rotate_Vector(vectmp);
// 		vec->x = vectmp.X;
// 		vec->y = vectmp.Y;
// 		vec->z = vectmp.Z;
// 	}
// }

///////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE CLASSES ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
void FXNugget::doFXObj(const Object* primary, const Object* secondary) const
{
	const Coord3D* p = primary ? primary->getPosition() : NULL;
	const Matrix3D* mtx = primary ? primary->getTransformMatrix() : NULL;
	const Real speed = 0.0f;	// yes, that's right -- NOT the object's speed.
	const Coord3D* s = secondary ? secondary->getPosition() : NULL;
	doFXPos(p, mtx, speed, s);
}

//-------------------------------------------------------------------------------------------------
class SoundFXNugget : public FXNugget
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(SoundFXNugget, "SoundFXNugget")
	
public:

	virtual void doFXPos(const Coord3D *primary, const Matrix3D* /*primaryMtx*/, const Real /*primarySpeed*/, const Coord3D * /*secondary*/, const Real /*overrideRadius*/ ) const
	{
		AudioEventRTS sound(m_soundName);
		
		if (primary) 
		{
			sound.setPosition(primary);
		}

		TheAudio->addAudioEvent(&sound);
	}

	virtual void doFXObj(const Object* primary, const Object* secondary = NULL) const
	{
DEBUG_CRASH(("SoundFXNugget::doFXObj not yet implemented!"));
(void) primary;
(void) secondary;
#if 0
		AudioEventRTS sound(m_soundName);
		if (primary)
		{
			sound.setPlayerIndex(primary->getControllingPlayer()->getPlayerIndex());
			sound.setPosition(primary->getPosition());
		}

		TheAudio->addAudioEvent(&sound);
#endif // if 0
	}

	static void parseSoundName(INI* ini, void *instance, void *store, const void *userData)
	{
		SoundFXNugget* self = (SoundFXNugget*) instance;
		INI::parseAsciiString(ini, nullptr, &self->m_soundName, nullptr);
	}

	static void parse(INI *ini, void *instance, void* /*store*/, const void* /*userData*/)
	{
		static const FieldParse myFieldParse[] = 
		{
			{ "Name", SoundFXNugget::parseSoundName, NULL, 0 },
			{ 0, 0, 0, 0 }
		};

		SoundFXNugget* nugget = newInstance(SoundFXNugget);	
		ini->initFromINI(nugget, myFieldParse);
		((FXList*)instance)->addFXNugget(nugget);
	}

private:
	AsciiString m_soundName {};
};  
EMPTY_DTOR(SoundFXNugget)

//-------------------------------------------------------------------------------------------------
// static Real calcDist(const Coord3D& src, const Coord3D& dst)
// {
// 	Real dx = dst.x - src.x;
// 	Real dy = dst.y - src.y;
// 	Real dz = dst.z - src.z;
// 	return sqrt(dx*dx + dy*dy + dz*dz);
// }

//-------------------------------------------------------------------------------------------------
class TracerFXNugget : public FXNugget
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(TracerFXNugget, "TracerFXNugget")		
public:

	TracerFXNugget()
	{
		m_ini.m_tracerName.set("GenericTracer");
		m_ini.m_boneName.clear();
		m_ini.m_speed = 0.0f; // means "use passed-in speed"
		m_ini.m_decayAt = 1.0f;
		m_ini.m_length = 10.0f;
		m_ini.m_width = 1.0f;
		m_ini.m_color.red = m_ini.m_color.green = m_ini.m_color.blue = 1.0f;
		m_ini.m_probability = 1.0f;
	}

	virtual void doFXPos(const Coord3D *primary, const Matrix3D* primaryMtx, const Real primarySpeed, const Coord3D *secondary, const Real /*overrideRadius*/ ) const
	{
DEBUG_CRASH(("TracerFXNugget::doFXObj not yet implemented!"));
(void) primary;
(void) primaryMtx;
(void) primarySpeed;
(void) secondary;
#if 0
		if (m_probability <= GameClientRandomValueReal(0, 1))
			return;

		if (primary && secondary)
		{
			Drawable *tracer = TheThingFactory->newDrawable(TheThingFactory->findTemplate(m_tracerName));
			if(!tracer)
				return;
			
			//Kris -- Redid this section Sept 18, 2002
			//Calculate tracer orientations to face from primary to secondary position. This
			//should be the direction that the projectile is being fired towards. It doesn't make
			//sense that the old stuff made use of the muzzle fx bone orientation (because it's a 
			//subobject). It had other problems because of elevation variations the tracers would
			//stay on the ground.
			//tracer->setTransformMatrix(primaryMtx);
			Matrix3D tracerMtx;
			Vector3 pos( primary->x, primary->y, primary->z );
			Vector3 dir( secondary->x - primary->x, secondary->y - primary->y, secondary->z - primary->z );
			dir.Normalize(); //This is fantastically crucial for calling buildTransformMatrix!!!!!
			tracerMtx.buildTransformMatrix( pos, dir );
			tracer->setTransformMatrix( &tracerMtx );
			tracer->setPosition(primary);

			Real speed = m_speed;
			if (speed == 0.0f)
			{
				speed = primarySpeed;
			}

			TracerDrawInterface* tdi = NULL;
			for (DrawModule** d = tracer->getDrawModules(); *d; ++d)
			{
				if ((tdi = (*d)->getTracerDrawInterface()) != NULL)
				{
					tdi->setTracerParms(speed, m_length, m_width, m_color, 1.0f);
				}
			}

			// estimate how long it will take us to get to the destination
			Real dist = calcDist(*primary, *secondary) - m_length;
			Real frames = (dist >= 0.0f && speed >= 0.0f) ? (dist / speed) : 1;
			Int framesAdjusted = REAL_TO_INT_CEIL(frames * m_decayAt);
			tracer->setExpirationDate(TheGameLogic->getFrame() + framesAdjusted);
		}
		else
		{
			DEBUG_CRASH(("You must have a primary and secondary source for this effect"));
		}
#endif // if 0
	}

	static void parse(INI *ini, void *instance, void* /*store*/, const void* /*userData*/)
	{
		static const FieldParse myFieldParse[] = 
		{
			{ "TracerName",		INI::parseAsciiString,		NULL, offsetof( TracerFXNugget::IniData, m_tracerName ) },
			{ "BoneName",		INI::parseAsciiString,		NULL, offsetof( TracerFXNugget::IniData, m_boneName ) },
			{ "Speed",			INI::parseVelocityReal,		NULL, offsetof( TracerFXNugget::IniData, m_speed ) },
			{ "DecayAt",		INI::parseReal,				NULL, offsetof( TracerFXNugget::IniData, m_decayAt ) },
			{ "Length",			INI::parseReal,				NULL, offsetof( TracerFXNugget::IniData, m_length ) },
			{ "Width",			INI::parseReal,				NULL, offsetof( TracerFXNugget::IniData, m_width ) },
			{ "Color",			INI::parseRGBColor,			NULL, offsetof( TracerFXNugget::IniData, m_color ) },
			{ "Probability",	INI::parseReal,				NULL, offsetof( TracerFXNugget::IniData, m_probability ) },
			{ 0, 0, 0, 0 }
		};

		TracerFXNugget* nugget = newInstance( TracerFXNugget );	
		ini->initFromINI(&nugget->m_ini, myFieldParse);
		((FXList*)instance)->addFXNugget(nugget);
	}

private:
	// MG: Cannot apply offsetof to TracerFXNugget, so had to move data into an embedded struct.
	struct IniData
	{
		AsciiString		m_tracerName;
		AsciiString     m_boneName;
		Real            m_speed;
		Real            m_decayAt;
		Real			m_length;
		Real			m_width;
		RGBColor		m_color;
		Real			m_probability;
	};

	IniData m_ini {};
};
EMPTY_DTOR(TracerFXNugget)

//-------------------------------------------------------------------------------------------------
class RayEffectFXNugget : public FXNugget
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(RayEffectFXNugget, "RayEffectFXNugget")		
public:

	RayEffectFXNugget()
	{
		m_ini.m_templateName.clear();
		m_ini.m_primaryOffset.x = m_ini.m_primaryOffset.y = m_ini.m_primaryOffset.z = 0;
		m_ini.m_secondaryOffset.x = m_ini.m_secondaryOffset.y = m_ini.m_secondaryOffset.z = 0;
	}

	virtual void doFXPos(const Coord3D *primary, const Matrix3D* /*primaryMtx*/, const Real /*primarySpeed*/, const Coord3D * secondary, const Real /*overrideRadius*/ ) const
	{
		const ThingTemplate* tmpl = TheThingFactory->findTemplate(m_ini.m_templateName);
		DEBUG_ASSERTCRASH(tmpl, ("RayEffect %s not found\n",m_ini.m_templateName.str()));
		if (primary && secondary && tmpl)
		{
			Coord3D sourcePos = *primary;
			sourcePos.x += m_ini.m_primaryOffset.x;
			sourcePos.y += m_ini.m_primaryOffset.y;
			sourcePos.z += m_ini.m_primaryOffset.z;

			Coord3D targetPos = *secondary;
			targetPos.x += m_ini.m_secondaryOffset.x;
			targetPos.y += m_ini.m_secondaryOffset.y;
			targetPos.z += m_ini.m_secondaryOffset.z;

			TheGameClient->createRayEffectByTemplate(&sourcePos, &targetPos, tmpl);
		}
		else
		{
			DEBUG_CRASH(("You must have a primary AND secondary source for this effect"));
		}
	}

	static void parse(INI *ini, void *instance, void* /*store*/, const void* /*userData*/)
	{
		static const FieldParse myFieldParse[] = 
		{
			{ "Name",				INI::parseAsciiString,	NULL, offsetof( RayEffectFXNugget::IniData, m_templateName ) },
			{ "PrimaryOffset",		INI::parseCoord3D,		NULL, offsetof( RayEffectFXNugget::IniData, m_primaryOffset ) },
			{ "SecondaryOffset",	INI::parseCoord3D,		NULL, offsetof( RayEffectFXNugget::IniData, m_secondaryOffset ) },
			{ 0, 0, 0, 0 }
		};

		RayEffectFXNugget* nugget = newInstance( RayEffectFXNugget ); 
		ini->initFromINI(&nugget->m_ini, myFieldParse);
		((FXList*)instance)->addFXNugget(nugget);
	}

private:
	// MG: Cannot apply offsetof to RayEffectFXNugget, so had to move data into an embedded struct.
	struct IniData
	{
		AsciiString	m_templateName;
		Coord3D		m_primaryOffset;
		Coord3D		m_secondaryOffset;
	};

	IniData m_ini {};
};  
EMPTY_DTOR(RayEffectFXNugget)

//-------------------------------------------------------------------------------------------------
class LightPulseFXNugget : public FXNugget
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(LightPulseFXNugget, "LightPulseFXNugget")		
public:

	LightPulseFXNugget()
	{
		m_ini.m_color.red = m_ini.m_color.green = m_ini.m_color.blue = 0;
		m_ini.m_radius = 0;
		m_ini.m_boundingCirclePct = 0;
		m_ini.m_increaseFrames = 0;
		m_ini.m_decreaseFrames = 0;
	}

	virtual void doFXObj(const Object* primary, const Object* /*secondary*/) const
	{
DEBUG_CRASH(("LightPulseFXNugget::doFXObj not yet implemented!"));
(void) primary;
#if 0
		if (primary)
		{
			Real radius = m_radius;

			if (m_boundingCirclePct > 0)
				radius = (primary->getGeometryInfo().getBoundingCircleRadius() * m_boundingCirclePct);

			TheDisplay->createLightPulse(primary->getPosition(), &m_color, 1, radius, m_increaseFrames, m_decreaseFrames);
		}
		else
		{
			DEBUG_CRASH(("You must have a primary source for this effect"));
		}
#endif // if 0
	}

	virtual void doFXPos(const Coord3D *primary, const Matrix3D* /*primaryMtx*/, const Real /*primarySpeed*/, const Coord3D * /*secondary*/, const Real /*overrideRadius*/ ) const
	{
		if (primary)
		{
			TheDisplay->createLightPulse(primary, &m_ini.m_color, 1, m_ini.m_radius, m_ini.m_increaseFrames, m_ini.m_decreaseFrames);
		}
		else
		{
			DEBUG_CRASH(("You must have a primary source for this effect"));
		}
	}

	static void parse(INI *ini, void *instance, void* /*store*/, const void* /*userData*/)
	{
		static const FieldParse myFieldParse[] = 
		{
			{ "Color",							INI::parseRGBColor,				NULL, offsetof( LightPulseFXNugget::IniData, m_color ) },
			{ "Radius",							INI::parseReal,					NULL, offsetof( LightPulseFXNugget::IniData, m_radius ) },
			{ "RadiusAsPercentOfObjectSize",	INI::parsePercentToReal,		NULL, offsetof( LightPulseFXNugget::IniData, m_boundingCirclePct ) },
			{ "IncreaseTime",					INI::parseDurationUnsignedInt,	NULL, offsetof( LightPulseFXNugget::IniData, m_increaseFrames ) },
			{ "DecreaseTime",					INI::parseDurationUnsignedInt,	NULL, offsetof( LightPulseFXNugget::IniData, m_decreaseFrames ) },
			{ 0, 0, 0, 0 }
		};

		LightPulseFXNugget* nugget = newInstance( LightPulseFXNugget );	
		ini->initFromINI(&nugget->m_ini, myFieldParse);
		((FXList*)instance)->addFXNugget(nugget);
	}

private:
	// MG: Cannot apply offsetof to LightPulseFXNugget, so had to move data into an embedded struct.
	struct IniData
	{
		RGBColor		m_color;
		Real			m_radius;
		Real			m_boundingCirclePct;
		UnsignedInt		m_increaseFrames;
		UnsignedInt		m_decreaseFrames;
	};

	IniData m_ini {};
};
EMPTY_DTOR(LightPulseFXNugget)

//-------------------------------------------------------------------------------------------------
class ViewShakeFXNugget : public FXNugget
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(ViewShakeFXNugget, "ViewShakeFXNugget")		
public:

	ViewShakeFXNugget()
	{
		m_ini.m_shake = View::SHAKE_NORMAL;
	}

	virtual void doFXPos(const Coord3D *primary, const Matrix3D* /*primaryMtx*/, const Real /*primarySpeed*/, const Coord3D * /*secondary*/, const Real /*overrideRadius*/ ) const
	{
DEBUG_CRASH(("ViewShakeFXNugget::doFXObj not yet implemented!"));
(void) primary;
#if 0
		if (primary)
		{
			if (TheTacticalView)
				TheTacticalView->shake(primary, m_ini.m_shake);
		}
		else
		{
			DEBUG_CRASH(("You must have a primary source for this effect"));
		}
#endif // if 0
	}

	static void parse(INI *ini, void *instance, void* /*store*/, const void* /*userData*/)
	{
		static const FieldParse myFieldParse[] = 
		{
			{ "Type",				parseShakeType,								NULL, offsetof( ViewShakeFXNugget::IniData, m_shake ) },
			{ 0, 0, 0, 0 }
		};

		ViewShakeFXNugget* nugget = newInstance( ViewShakeFXNugget );	
		ini->initFromINI(&nugget->m_ini, myFieldParse);
		((FXList*)instance)->addFXNugget(nugget);
	}

protected:
	static void parseShakeType( INI* ini, void *instance, void *store, const void* /*userData*/ )
	{
		static const LookupListRec shakeTypeNames[] = 
		{
			{ "SUBTLE", View::SHAKE_SUBTLE },
			{ "NORMAL", View::SHAKE_NORMAL },
			{ "STRONG", View::SHAKE_STRONG },
			{ "SEVERE", View::SHAKE_SEVERE },
			{ "CINE_EXTREME", View::SHAKE_CINE_EXTREME },
			{ "CINE_INSANE",  View::SHAKE_CINE_INSANE },
			{ 0, 0 }
		};
		*(Int *)store = INI::scanLookupList(ini->getNextToken(), shakeTypeNames);
	}

private:
	// MG: Cannot apply offsetof to ViewShakeFXNugget, so had to move data into an embedded struct.
	struct IniData
	{
		View::CameraShakeType m_shake;
	};

	IniData m_ini {};

};  
EMPTY_DTOR(ViewShakeFXNugget)

//-------------------------------------------------------------------------------------------------
class TerrainScorchFXNugget : public FXNugget
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(TerrainScorchFXNugget, "TerrainScorchFXNugget")		
public:

	TerrainScorchFXNugget()
	{
		m_ini.m_scorch = -1;
		m_ini.m_radius = 0;
	}

	virtual void doFXPos(const Coord3D *primary, const Matrix3D* /*primaryMtx*/, const Real /*primarySpeed*/, const Coord3D * /*secondary*/, const Real /*overrideRadius*/ ) const
	{
		if (primary)
		{
			Int scorch = m_ini.m_scorch;
			if (scorch < 0)
			{
				scorch = GameClientRandomValue( SCORCH_1, SCORCH_4 );
			}
			TheGameClient->addScorch(primary, m_ini.m_radius, (Scorches)scorch);
		}
		else
		{
			DEBUG_CRASH(("You must have a primary source for this effect"));
		}
	}

	static void parse(INI *ini, void *instance, void* /*store*/, const void* /*userData*/)
	{
		static const FieldParse myFieldParse[] = 
		{
			{ "Type",			parseScorchType,			NULL, offsetof( TerrainScorchFXNugget::IniData, m_scorch ) },
			{ "Radius",			INI::parseReal,				NULL, offsetof( TerrainScorchFXNugget::IniData, m_radius ) },
			{ 0, 0, 0, 0 }
		};

		TerrainScorchFXNugget* nugget = newInstance( TerrainScorchFXNugget );	
		ini->initFromINI(&nugget->m_ini, myFieldParse);
		((FXList*)instance)->addFXNugget(nugget);
	}

protected:

	static void parseScorchType( INI* ini, void *instance, void *store, const void* /*userData*/ )
	{
		static const LookupListRec scorchTypeNames[] = 
		{
			{ "SCORCH_1",				SCORCH_1 },
			{ "SCORCH_2",				SCORCH_2 },
			{ "SCORCH_3",				SCORCH_3 },
			{ "SCORCH_4",				SCORCH_4 },
			{ "SHADOW_SCORCH",	SHADOW_SCORCH },
			{ "RANDOM",					-1 },
			{ 0, 0 }
		};
		*(Int *)store = INI::scanLookupList(ini->getNextToken(), scorchTypeNames);
	}

private:
	// MG: Cannot apply offsetof to DeletionUpdateModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		Int		m_scorch;
		Real	m_radius;
	};

	IniData m_ini {};
};  
EMPTY_DTOR(TerrainScorchFXNugget)

//-------------------------------------------------------------------------------------------------
class ParticleSystemFXNugget : public FXNugget
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(ParticleSystemFXNugget, "ParticleSystemFXNugget")		
public:

	ParticleSystemFXNugget()
	{
		m_ini.m_name.clear();
		m_ini.m_count = 1;
		m_ini.m_radius.setRange(0, 0, GameClientRandomVariable::CONSTANT);
		m_ini.m_height.setRange(0, 0, GameClientRandomVariable::CONSTANT);
		// -1 means "don't mess with it, just accept the particle-system's defaults"
		m_ini.m_delay.setRange(-1, -1, GameClientRandomVariable::CONSTANT);
		m_ini.m_offset.x = m_ini.m_offset.y = m_ini.m_offset.z = 0;
		m_ini.m_orientToObject = false;
		m_ini.m_attachToObject = false;
		m_ini.m_createAtGroundHeight = FALSE;
		m_ini.m_useCallersRadius = FALSE;
		m_ini.m_rotateX = m_ini.m_rotateY = m_ini.m_rotateZ = 0;
	}

	virtual void doFXPos(const Coord3D *primary, const Matrix3D* primaryMtx, const Real /*primarySpeed*/, const Coord3D * /*secondary*/, const Real overrideRadius ) const
	{
		if (primary)
		{
			reallyDoFX(primary, primaryMtx, NULL, overrideRadius);
		}
		else
		{
			DEBUG_CRASH(("You must have a primary source for this effect"));
		}
	}

	virtual void doFXObj(const Object* primary, const Object* secondary) const
	{
		if (primary)
		{

			if (m_ini.m_ricochet && secondary)
			{
				// HERE WE MUST BUILD A MATRIX WHICH WILL ORIENT THE NEW PARTICLE SYSTEM TO FACE AWAY FROM THE SECONDARY OBJECT
				// THE RESULT SHOULD LOOK LIKE THE DIRECTION OF THE "ATTACK" IS CARRIED THROUGH LIKE A RICOCHET
				Real deltaX = primary->getPosition()->x - secondary->getPosition()->x;
				Real deltaY = primary->getPosition()->y - secondary->getPosition()->y;
				Real aimingAngle = atan2(deltaY, deltaX);
				Matrix3D aimingMatrix(1);
				aimingMatrix.Rotate_Z( aimingAngle );

				reallyDoFX(primary->getPosition(), &aimingMatrix, primary, 0.0f);
			}
			else
				// if we have an object, then adjust the offset and direction by the object's transformation
				// matrix, so that (say) an offset of +10 in the z axis "follows" the orientation of the object.
				reallyDoFX(primary->getPosition(), primary->getTransformMatrix(), primary, 0.0f);
		}
		else
		{
			DEBUG_CRASH(("You must have a primary source for this effect"));
		}
	}

	static void parse(INI *ini, void *instance, void* /*store*/, const void* /*userData*/)
	{
		static const FieldParse myFieldParse[] = 
		{
			{ "Name",					INI::parseAsciiString,					NULL, offsetof( ParticleSystemFXNugget::IniData, m_name ) },
			{ "Count",					INI::parseInt,							NULL, offsetof( ParticleSystemFXNugget::IniData, m_count ) },
			{ "Offset",					INI::parseCoord3D,						NULL, offsetof( ParticleSystemFXNugget::IniData, m_offset ) },
			{ "Radius",					INI::parseGameClientRandomVariable,		NULL, offsetof( ParticleSystemFXNugget::IniData, m_radius ) },
			{ "Height",					INI::parseGameClientRandomVariable,		NULL, offsetof( ParticleSystemFXNugget::IniData, m_height ) },
			{ "InitialDelay",			INI::parseGameClientRandomVariable,		NULL, offsetof( ParticleSystemFXNugget::IniData, m_delay ) },
			{ "RotateX",				INI::parseAngleReal,					NULL, offsetof( ParticleSystemFXNugget::IniData, m_rotateX ) },
			{ "RotateY",				INI::parseAngleReal,					NULL, offsetof( ParticleSystemFXNugget::IniData, m_rotateY ) },
			{ "RotateZ",				INI::parseAngleReal,					NULL, offsetof( ParticleSystemFXNugget::IniData, m_rotateZ ) },
			{ "OrientToObject",			INI::parseBool,							NULL, offsetof( ParticleSystemFXNugget::IniData, m_orientToObject ) },
			{ "Ricochet",				INI::parseBool,							NULL, offsetof( ParticleSystemFXNugget::IniData, m_ricochet ) },
			{ "AttachToObject",			INI::parseBool,							NULL, offsetof( ParticleSystemFXNugget::IniData, m_attachToObject ) },
			{ "CreateAtGroundHeight",	INI::parseBool,							NULL, offsetof( ParticleSystemFXNugget::IniData, m_createAtGroundHeight ) },
			{ "UseCallersRadius",		INI::parseBool,							NULL, offsetof( ParticleSystemFXNugget::IniData, m_useCallersRadius ) },
			{ 0, 0, 0, 0 }
		};

		ParticleSystemFXNugget* nugget = newInstance( ParticleSystemFXNugget );	
		ini->initFromINI(&nugget->m_ini, myFieldParse);
		((FXList*)instance)->addFXNugget(nugget);
	}

protected:

	void reallyDoFX(const Coord3D *primary, const Matrix3D* mtx, const Object* thingToAttachTo, Real overrideRadius ) const
	{
DEBUG_CRASH(("ParticleSystemFXNugget::reallyDoFX not yet implemented!"));
(void) primary;
(void) mtx;
(void) thingToAttachTo;
(void) overrideRadius;
#if 0
		Coord3D offset = m_ini.m_offset;
		if (mtx)
		{
			adjustVector(&offset, mtx);
		}

		const ParticleSystemTemplate *tmp = TheParticleSystemManager->findTemplate(m_ini.m_name);
		DEBUG_ASSERTCRASH(tmp, ("ParticleSystem %s not found\n",m_ini.m_name.str()));
		if (tmp)
		{
			for (Int i = 0; i < m_ini.m_count; i++ )
			{
				ParticleSystem *sys = TheParticleSystemManager->createParticleSystem(tmp);
				if (sys)
				{
					Coord3D newPos;
					Real radius = m_ini.m_radius.getValue();
					Real angle = GameClientRandomValueReal( 0.0f, 2.0f * PI );

					newPos.x = primary->x + offset.x + radius * cos(angle);
					newPos.y = primary->y + offset.y + radius * sin(angle);
					if( m_ini.m_createAtGroundHeight && TheTerrainLogic )
					{
						//old way:
						//newPos.z = TheTerrainLogic->getGrsoundHeight( newPos.x, newPos.y ) + 1;// The plus one prevents scissoring with terrain
						
						//new way: now we allow bridges in the GroundHeight.
						PathfindLayerEnum layer = TheTerrainLogic->getLayerForDestination(&newPos);
						newPos.z = TheTerrainLogic->getLayerHeight( newPos.x, newPos.y, layer );
					}
					else
						newPos.z = primary->z + offset.z + m_ini.m_height.getValue();


					if (m_ini.m_orientToObject && mtx)
					{
						sys->setLocalTransform(mtx);
					}
					if (m_ini.m_rotateX != 0.0f)
						sys->rotateLocalTransformX(m_ini.m_rotateX);
					if (m_ini.m_rotateY != 0.0f)
						sys->rotateLocalTransformY(m_ini.m_rotateY);
					if (m_ini.m_rotateZ != 0.0f)
						sys->rotateLocalTransformZ(m_ini.m_rotateZ);

					if (m_ini.m_attachToObject && thingToAttachTo)
						sys->attachToObject(thingToAttachTo);
					else
						sys->setPosition( &newPos );

					Real delayInMsec = m_ini.m_delay.getValue();
					if (delayInMsec >= 0.0f)
					{
						UnsignedInt delayInFrames = REAL_TO_INT_CEIL(ConvertDurationFromMsecsToFrames(delayInMsec));
						sys->setInitialDelay(delayInFrames);
					}

					if( m_ini.m_useCallersRadius  &&  overrideRadius )
					{
						ParticleSystemInfo::EmissionVolumeType type = sys->getEmisionVolumeType();

						if( type == ParticleSystemInfo::EmissionVolumeType::SPHERE )
							sys->setEmissionVolumeSphereRadius( overrideRadius );
						else if( type == ParticleSystemInfo::EmissionVolumeType::CYLINDER )
							sys->setEmissionVolumeCylinderRadius( overrideRadius );
					}
				}
			}
		}
#endif // if 0
	}


private:
	// MG: Cannot apply offsetof to ParticleSystemFXNugget, so had to move data into an embedded struct.
	struct IniData
	{
		AsciiString					m_name;
		Int							m_count;
		Coord3D						m_offset;
		GameClientRandomVariable	m_radius;
		GameClientRandomVariable	m_height;
		GameClientRandomVariable	m_delay;
		Real						m_rotateX, m_rotateY, m_rotateZ;
		Bool						m_orientToObject;
		Bool						m_attachToObject;
		Bool						m_createAtGroundHeight;
		Bool						m_useCallersRadius;
		Bool						m_ricochet;
	};

	IniData m_ini {};
};
EMPTY_DTOR(ParticleSystemFXNugget)

//-------------------------------------------------------------------------------------------------
class FXListAtBonePosFXNugget : public FXNugget
{
	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(FXListAtBonePosFXNugget, "FXListAtBonePosFXNugget")		
public:

	FXListAtBonePosFXNugget()
	{
		m_ini.m_fx = NULL;
		m_ini.m_boneName.clear();
		m_ini.m_orientToBone = true;
	}

	virtual void doFXPos(const Coord3D *primary, const Matrix3D* primaryMtx, const Real /*primarySpeed*/, const Coord3D * /*secondary*/, const Real /*overrideRadius*/ ) const
	{
		DEBUG_CRASH(("You must use the object form for this effect"));
	}

	virtual void doFXObj(const Object* primary, const Object* /*secondary*/) const
	{
		if (primary)
		{
			// first, try the unadorned name.
			doFxAtBones(primary, 0);

			// then, try the 01,02,03...etc names.
			doFxAtBones(primary, 1);
		}
		else
		{
			DEBUG_CRASH(("You must have a primary source for this effect"));
		}
	}

	static void parse(INI *ini, void *instance, void* /*store*/, const void* /*userData*/)
	{
		static const FieldParse myFieldParse[] = 
		{
			{ "FX",				INI::parseFXList,			NULL, offsetof( FXListAtBonePosFXNugget::IniData, m_fx ) },
			{ "BoneName",		INI::parseAsciiString,		NULL, offsetof( FXListAtBonePosFXNugget::IniData, m_boneName ) },
			{ "OrientToBone",	INI::parseBool,				NULL, offsetof( FXListAtBonePosFXNugget::IniData, m_orientToBone ) },
			{ 0, 0, 0, 0 }
		};

		FXListAtBonePosFXNugget* nugget = newInstance( FXListAtBonePosFXNugget );
		ini->initFromINI(&nugget->m_ini, myFieldParse);
		((FXList*)instance)->addFXNugget(nugget);
	}

protected:

	void doFxAtBones(const Object* obj, Int start) const
	{
DEBUG_CRASH(("FXListAtBonePosFXNugget::doFxAtBones not yet implemented!"));
(void) obj;
(void) start;
#if 0
		Coord3D bonePos[MAX_BONE_POINTS];
		Matrix3D boneMtx[MAX_BONE_POINTS];

		Drawable* draw = obj->getDrawable();
		if (draw)
		{
			// yes, BONEPOS_CURRENT_CLIENT_ONLY -- this is client-only, so should be safe to do.
			Int count = draw->getCurrentClientBonePositions(m_boneName.str(), start, bonePos, boneMtx, MAX_BONE_POINTS);
			for (Int i = 0; i < count; ++i)
			{
				Coord3D p;
				Matrix3D m;
				obj->convertBonePosToWorldPos(&bonePos[i], &boneMtx[i], &p, &m);
				FXList::doFXPos(m_fx, &p, &m, 0.0f, NULL, 0.0f);
			}
		}
#endif // if 0
	}

private:

	enum { MAX_BONE_POINTS = 40 };

	// MG: Cannot apply offsetof to FXListAtBonePosFXNugget, so had to move data into an embedded struct.
	struct IniData
	{
		const FXList*	m_fx;
		AsciiString		m_boneName;
		Bool			m_orientToBone;
	};

	IniData m_ini {};

};
EMPTY_DTOR(FXListAtBonePosFXNugget)

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

static const FieldParse TheFXListFieldParse[] =
{
	{ "Sound",											SoundFXNugget::parse, 0, 0},		
	{ "RayEffect",									RayEffectFXNugget::parse, 0, 0},		
	{ "Tracer",											TracerFXNugget::parse, 0, 0},		
	{ "LightPulse",									LightPulseFXNugget::parse, 0, 0},		
	{ "ViewShake",									ViewShakeFXNugget::parse, 0, 0},		
	{ "TerrainScorch",							TerrainScorchFXNugget::parse, 0, 0},		
	{ "ParticleSystem",							ParticleSystemFXNugget::parse, 0, 0},		
	{ "FXListAtBonePos",						FXListAtBonePosFXNugget::parse, 0, 0},		
	{ NULL, NULL, 0, 0 }  // keep this last
};

//-------------------------------------------------------------------------------------------------
FXList::FXList()
{
}

//-------------------------------------------------------------------------------------------------
FXList::~FXList()
{
	clear();
}

//-------------------------------------------------------------------------------------------------
void FXList::clear()
{
	for (FXNuggetList::iterator it = m_nuggets.begin(); it != m_nuggets.end(); ++it)
	{
		if (*it)
			(*it)->deleteInstance();
	}
	m_nuggets.clear();
}

//-------------------------------------------------------------------------------------------------
void FXList::doFXPos(const Coord3D *primary, const Matrix3D* primaryMtx, const Real primarySpeed, const Coord3D *secondary, const Real overrideRadius ) const
{
DEBUG_CRASH(("FXList::doFXPos not yet implemented!"));
(void) primary;
(void) primaryMtx;
(void) primarySpeed;
(void) secondary;
(void) overrideRadius;
#if 0
	if (ThePartitionManager->getShroudStatusForPlayer(ThePlayerList->getLocalPlayer()->getPlayerIndex(), primary) != CELLSHROUD_CLEAR)
		return;

	for (FXNuggetList::const_iterator it = m_nuggets.begin(); it != m_nuggets.end(); ++it)
	{
		(*it)->doFXPos(primary, primaryMtx, primarySpeed, secondary, overrideRadius);
	}
#endif // if 0
}

//-------------------------------------------------------------------------------------------------
void FXList::doFXObj(const Object* primary, const Object* secondary) const
{
	// FIXME: ThePlayerList
	// if (primary && primary->getShroudedStatus(ThePlayerList->getLocalPlayer()->getPlayerIndex()) > OBJECTSHROUD_PARTIAL_CLEAR)
	// 	return;	//the primary object is fogged or shrouded so don't bother with the effect.

	for (FXNuggetList::const_iterator it = m_nuggets.begin(); it != m_nuggets.end(); ++it)
	{

		// HERE THE PRIMARY IS THE GUY RECEIVING THE FX, AND SECONDARY MIGHT BE THE GUY DEALING IT
		(*it)->doFXObj(primary, secondary);
	}
}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
FXListStore::FXListStore()
{
}

//-------------------------------------------------------------------------------------------------
FXListStore::~FXListStore()
{
	m_fxmap.clear();
}

//-------------------------------------------------------------------------------------------------
const FXList *FXListStore::findFXList(const char* name) const
{
	if (strcasecmp(name, "None") == 0)
		return NULL;

  FXListMap::const_iterator it = m_fxmap.find(NAMEKEY(name));
  if (it != m_fxmap.end()) 
	{
		return &(*it).second;
	}
	return NULL;
}

//-------------------------------------------------------------------------------------------------
/*static */ void FXListStore::parseFXListDefinition(INI *ini)
{
	// read the FXList name
	const char *c = ini->getNextToken();
	NameKeyType key = TheNameKeyGenerator->nameToKey(c);
	FXList& fxl = TheFXListStore->m_fxmap[key];
	fxl.clear();
	ini->initFromINI(&fxl, TheFXListFieldParse);
}

//-------------------------------------------------------------------------------------------------
/*static*/ void INI::parseFXListDefinition(INI *ini)
{
	FXListStore::parseFXListDefinition(ini);
}
