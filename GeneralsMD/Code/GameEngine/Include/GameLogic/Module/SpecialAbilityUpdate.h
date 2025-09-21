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

// FILE: SpecialAbilityUpdate.h /////////////////////////////////////////////////////////////////////////
// Author: Kris Morness, July 2002
// Desc:   Handles processing of unit special abilities.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __SPECIAL_ABILITY_UPDATE_H
#define __SPECIAL_ABILITY_UPDATE_H

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/AudioEventRTS.h"
#include "Common/INI.h"
#include "GameLogic/Module/SpecialPowerUpdateModule.h"
#include "GameClient/ParticleSys.h"

class DamageInfo;
class SpecialPowerTemplate;
class SpecialPowerModule;
class FXList;
enum SpecialPowerType;

#define SPECIAL_ABILITY_HUGE_DISTANCE 10000000.0f

//-------------------------------------------------------------------------------------------------
class SpecialAbilityUpdateModuleData : public UpdateModuleData
{
public:
	// MG: Cannot apply offsetof to SpecialAbilityUpdateModuleData, so had to move data into an embedded struct.
	struct IniData
	{
		AsciiString						m_specialObjectName;
		AsciiString						m_specialObjectAttachToBoneName;
		const SpecialPowerTemplate*		m_specialPowerTemplate;		///< pointer to the special power template
		Real							m_startAbilityRange;
		Real							m_abilityAbortRange;
		Real							m_packUnpackVariationFactor;
		Real							m_fleeRangeAfterCompletion;
		Int								m_effectValue;
		Int								m_awardXPForTriggering;
		Int								m_skillPointsForTriggering;
		UnsignedInt						m_preparationFrames;
		UnsignedInt						m_persistentPrepFrames;
		UnsignedInt						m_effectDuration;
		UnsignedInt						m_maxSpecialObjects;
		UnsignedInt						m_packTime;
		UnsignedInt						m_unpackTime;
		UnsignedInt						m_preTriggerUnstealthFrames;
		Bool							m_skipPackingWithNoTarget;
		Bool							m_specialObjectsPersistent;
		Bool							m_uniqueSpecialObjectTargets;
		Bool							m_specialObjectsPersistWhenOwnerDies;
		Bool							m_flipObjectAfterPacking;
		Bool							m_flipObjectAfterUnpacking;
		Bool							m_alwaysValidateSpecialObjects;
		Bool							m_doCaptureFX;					///< the house color flashing while a building is getting captured
		Bool							m_loseStealthOnTrigger;
		Bool							m_approachRequiresLOS;
		Bool							m_needToFaceTarget;
		Bool							m_persistenceRequiresRecharge;

		const ParticleSystemTemplate*	m_disableFXParticleSystem;

		SpecialAbilityUpdateModuleData*	m_obj {};						///< pointer to the parent object
	};

	IniData m_ini {};

	AudioEventRTS						m_packSound {};
	AudioEventRTS						m_unpackSound {};
	AudioEventRTS						m_prepSoundLoop {};
	AudioEventRTS						m_triggerSound {};

	SpecialAbilityUpdateModuleData()
	{
		m_ini.m_specialPowerTemplate = NULL;
		m_ini.m_startAbilityRange = SPECIAL_ABILITY_HUGE_DISTANCE;
		m_ini.m_abilityAbortRange = SPECIAL_ABILITY_HUGE_DISTANCE;
		m_ini.m_preparationFrames = 0;
		m_ini.m_persistentPrepFrames = 0;
		m_ini.m_packTime = 0;
		m_ini.m_unpackTime = 0;
		m_ini.m_packUnpackVariationFactor = 0.0f;
		m_ini.m_effectDuration = 0;
		m_ini.m_maxSpecialObjects = 1;
		m_ini.m_effectValue = 1;
		m_ini.m_specialObjectsPersistent = FALSE;
		m_ini.m_uniqueSpecialObjectTargets = FALSE;
		m_ini.m_specialObjectsPersistWhenOwnerDies = FALSE;
		m_ini.m_skipPackingWithNoTarget = FALSE;
		m_ini.m_flipObjectAfterPacking = FALSE;
		m_ini.m_flipObjectAfterUnpacking = FALSE;
		m_ini.m_disableFXParticleSystem = NULL;
		m_ini.m_fleeRangeAfterCompletion = 0.0f;
		m_ini.m_doCaptureFX = FALSE;
		m_ini.m_alwaysValidateSpecialObjects = FALSE;
		m_ini.m_loseStealthOnTrigger = FALSE;
		m_ini.m_awardXPForTriggering = 0;
		m_ini.m_skillPointsForTriggering = -1;
		m_ini.m_approachRequiresLOS = TRUE;
		m_ini.m_preTriggerUnstealthFrames = 0;
		m_ini.m_needToFaceTarget = TRUE;
		m_ini.m_persistenceRequiresRecharge = FALSE;
		m_ini.m_obj = this;
	}

	// No copies allowed!
	SpecialAbilityUpdateModuleData(const SpecialAbilityUpdateModuleData&) = delete;
	SpecialAbilityUpdateModuleData& operator=(const SpecialAbilityUpdateModuleData&) = delete;

	static void buildFieldParse(void* what, MultiIniFieldParse& p) 
	{
		UpdateModuleData::buildFieldParse(what, p);

		static const FieldParse dataFieldParse[] = 
		{
			//Primary data values
			{ "SpecialPowerTemplate",				INI::parseSpecialPowerTemplate,							NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_specialPowerTemplate ) },
			{ "StartAbilityRange",					INI::parseReal,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_startAbilityRange ) },
			{ "AbilityAbortRange",					INI::parseReal,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_abilityAbortRange ) },
			{ "PreparationTime",					INI::parseDurationUnsignedInt,							NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_preparationFrames ) },
			{ "PersistentPrepTime",					INI::parseDurationUnsignedInt,							NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_persistentPrepFrames ) },
			{ "PackTime",							INI::parseDurationUnsignedInt,							NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_packTime ) },
			{ "UnpackTime",							INI::parseDurationUnsignedInt,							NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_unpackTime ) },
			{ "PreTriggerUnstealthTime",			INI::parseDurationUnsignedInt,							NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_preTriggerUnstealthFrames ) },
			{ "SkipPackingWithNoTarget",			INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_skipPackingWithNoTarget ) },
			{ "PackUnpackVariationFactor",			INI::parseReal,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_packUnpackVariationFactor ) },
 
			//Secondary data values
			{ "SpecialObject",						INI::parseAsciiString,									NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_specialObjectName ) },
			{ "SpecialObjectAttachToBone",			INI::parseAsciiString,									NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_specialObjectAttachToBoneName ) },
			{ "MaxSpecialObjects",					INI::parseUnsignedInt,									NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_maxSpecialObjects ) },
			{ "SpecialObjectsPersistent",			INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_specialObjectsPersistent ) },
			{ "EffectDuration",						INI::parseDurationUnsignedInt,							NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_effectDuration ) },
			{ "EffectValue",						INI::parseInt,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_effectValue ) },
			{ "UniqueSpecialObjectTargets",			INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_uniqueSpecialObjectTargets ) },
			{ "SpecialObjectsPersistWhenOwnerDies",	INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_specialObjectsPersistWhenOwnerDies ) },
			{ "AlwaysValidateSpecialObjects",		INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_alwaysValidateSpecialObjects ) },
			{ "FlipOwnerAfterPacking",				INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_flipObjectAfterPacking ) },
			{ "FlipOwnerAfterUnpacking",			INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_flipObjectAfterUnpacking ) },
			{ "FleeRangeAfterCompletion",			INI::parseReal,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_fleeRangeAfterCompletion ) },
			{ "DisableFXParticleSystem",			INI::parseParticleSystemTemplate,						NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_disableFXParticleSystem ) },
			{ "DoCaptureFX",						INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_doCaptureFX ) },
			{ "PackSound",							SpecialAbilityUpdateModuleData::parsePackSound,			NULL, 0 },
			{ "UnpackSound",						SpecialAbilityUpdateModuleData::parseUnpackSound,		NULL, 0 },
			{ "PrepSoundLoop",						SpecialAbilityUpdateModuleData::parsePrepSoundLoop,		NULL, 0 },
			{ "TriggerSound",						SpecialAbilityUpdateModuleData::parseTriggerSound,		NULL, 0 },
			{ "LoseStealthOnTrigger",				INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_loseStealthOnTrigger ) },
			{ "AwardXPForTriggering",				INI::parseInt,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_awardXPForTriggering ) },
			{ "SkillPointsForTriggering",			INI::parseInt,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_skillPointsForTriggering ) },
			{ "ApproachRequiresLOS",				INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_approachRequiresLOS ) },
			{ "ApproachRequiresLOS",				INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_approachRequiresLOS ) },
			{ "NeedToFaceTarget",					INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_needToFaceTarget ) },
			{ "PersistenceRequiresRecharge",		INI::parseBool,											NULL, offsetof( SpecialAbilityUpdateModuleData::IniData, m_persistenceRequiresRecharge ) },
			{ 0, 0, 0, 0 }
		};
		SpecialAbilityUpdateModuleData* self {static_cast<SpecialAbilityUpdateModuleData*>(what)};
		size_t offset {static_cast<size_t>(MEMORY_OFFSET(self, &self->m_ini))};
		p.add(dataFieldParse, offset);
	}

private:
	static void parsePackSound(INI* ini, void *instance, void* store, const void* userData);
	static void parseUnpackSound(INI* ini, void *instance, void* store, const void* userData);
	static void parsePrepSoundLoop(INI* ini, void *instance, void* store, const void* userData);
	static void parseTriggerSound(INI* ini, void *instance, void* store, const void* userData);
};

//-------------------------------------------------------------------------------------------------
class SpecialAbilityUpdate : public SpecialPowerUpdateModule
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( SpecialAbilityUpdate, "SpecialAbilityUpdate" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( SpecialAbilityUpdate, SpecialAbilityUpdateModuleData )

public:

	SpecialAbilityUpdate( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	// SpecialPowerUpdateInterface
	virtual Bool initiateIntentToDoSpecialPower(const SpecialPowerTemplate *specialPowerTemplate, const Object *targetObj, const Coord3D *targetPos, const Waypoint *way, UnsignedInt commandOptions );
	virtual Bool isSpecialAbility() const { return true; }
	virtual Bool isSpecialPower() const { return false; }
	virtual Bool isActive() const { return m_active; }
	virtual Bool doesSpecialPowerHaveOverridableDestinationActive() const { return false; } //Is it active now?
	virtual Bool doesSpecialPowerHaveOverridableDestination() const { return false; }	//Does it have it, even if it's not active?
	virtual void setSpecialPowerOverridableDestination( const Coord3D * /* loc */ ) {}
	virtual Bool isPowerCurrentlyInUse( const CommandButton *command = NULL ) const;

//	virtual Bool isBusy() const { return m_isBusy; }

	// UpdateModule
	virtual SpecialPowerUpdateInterface* getSpecialPowerUpdateInterface() { return this; }
	virtual CommandOption getCommandOption() const { return (CommandOption)0; }
	virtual UpdateSleepTime update();	

	// ??? ugh, public stuff that shouldn't be -- hell yeah!
	UnsignedInt getSpecialObjectCount() const;
	UnsignedInt getSpecialObjectMax() const;
	Object* findSpecialObjectWithProducerID( const Object *target );
	SpecialPowerType getSpecialPowerType( void ) const;

protected:
	void onExit( Bool cleanup );

	const SpecialPowerTemplate* getTemplate() const;

	SpecialPowerModuleInterface* getMySPM();

	//Wrapper function that determines if our object is close enough to the target object or position (auto knowledge)... only if necessary.
	//Returns true if it's not necessary, in which case, it can skip any approach.
	Bool isWithinStartAbilityRange() const;
	Bool isWithinAbilityAbortRange() const; //If not, we abort the ability

	Bool initLaser(Object* specialObject, Object* target);

	//Various steps of performing any special ability. Not all special abilities will use all of them.
	Bool approachTarget();			//Approaches the target before starting the special attack (if appropriate)
	void startPreparation();		//Begins the preparation of ability -- like firing off a hacker attack, or laser tracer, etc.
	Bool continuePreparation();	//Updates the preparation of ability -- like recalculating tracer lines.
	void triggerAbilityEffect();	//After the preparation time has elapsed, this part actually triggers the desired effect of the special ability.
	void finishAbility();

	void validateSpecialObjects();

	Object* createSpecialObject();
	void killSpecialObjects();

	Bool handlePackingProcessing();
	void startPacking(Bool success);
	void startUnpacking();
	Bool needToPack() const;
	Bool needToUnpack() const;

	Bool isPreparationComplete() const { return !m_prepFrames; }
	void endPreparation();
	Bool isPersistentAbility() const { return getSpecialAbilityUpdateModuleData()->m_ini.m_persistentPrepFrames > 0; }
	void resetPreparation() { m_prepFrames = getSpecialAbilityUpdateModuleData()->m_ini.m_persistentPrepFrames; }

	Bool isFacing();
	Bool needToFace() const;
	void startFacing();

  // Lorenzen added this additional flag to support the NapalmBombDrop
  // It causes this update to force a recharge of the SPM between drops
  Bool getDoesPersistenceRequireRecharge() const { return getSpecialAbilityUpdateModuleData()->m_ini.m_persistenceRequiresRecharge; }
//	void setBusy ( Bool is ) { m_isBusy = is; }
//	Bool m_isBusy; ///< whether I am between trigger and completion

protected:

	UpdateSleepTime calcSleepTime()
	{
		return (m_active || getSpecialAbilityUpdateModuleData()->m_ini.m_alwaysValidateSpecialObjects) ? UPDATE_SLEEP_NONE : UPDATE_SLEEP_FOREVER;
	}

private:

	enum PackingState
	{
		STATE_NONE,
		STATE_PACKING,
		STATE_UNPACKING,
		STATE_PACKED,
		STATE_UNPACKED,
	};
	
	AudioEventRTS			m_prepSoundLoop {};
	UnsignedInt				m_prepFrames {};
	UnsignedInt				m_animFrames {};				//Used for packing/unpacking unit before or after using ability.
	ObjectID				m_targetID {};
	Coord3D					m_targetPos {};
	Int						m_locationCount {};
	std::list<ObjectID>		m_specialObjectIDList {};		//The list of special objects
	UnsignedInt				m_specialObjectEntries {};		//The size of the list of member Objects
	Real					m_captureFlashPhase {};			///< used to track the accellerating flash of the capture FX
	PackingState			m_packingState {};
	Bool					m_active {};
	Bool					m_noTargetCommand {};
	Bool					m_facingInitiated {};
	Bool					m_facingComplete {};
	Bool					m_withinStartAbilityRange {};
	Bool					m_doDisableFXParticles {};		// smaller targets cause this flag to toggle, making the particle effect more sparse
};

#endif // _SPECIAL_POWER_UPDATE_H_
