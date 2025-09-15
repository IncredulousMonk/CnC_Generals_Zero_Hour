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

// FILE: ThingTemplate.cpp ////////////////////////////////////////////////////////////////////////
// Created: Colin Day, April 2001
// Desc:    Thing templates
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#define DEFINE_POWER_NAMES								// for PowerNames[]
#define DEFINE_SHADOW_NAMES								// for TheShadowNames[]
// #define DEFINE_GEOMETRY_NAMES							// for GeometryNames[]
#define DEFINE_BUILD_COMPLETION_NAMES			// for BuildCompletionNames[]
#define DEFINE_EDITOR_SORTING_NAMES				// for EditorSortingNames[]
#define DEFINE_RADAR_PRIORITY_NAMES				// for RadarPriorityNames[]
#define DEFINE_BUILDABLE_STATUS_NAMES			// for BuildableStatusNames[]

#include "Common/DamageFX.h"
#include "Common/GameAudio.h"
#include "Common/GameCommon.h"
#include "Common/GlobalData.h"
#include "Common/INI.h"
#include "Common/MessageStream.h"
#include "Common/Module.h"
#include "Common/ModuleFactory.h"
#include "Common/Player.h"
#include "Common/PlayerList.h"
#include "Common/ProductionPrerequisite.h"
#include "Common/Radar.h"
#include "Common/RandomValue.h"
#include "Common/Science.h"
#include "Common/ThingTemplate.h"
#include "Common/ThingFactory.h"
#include "Common/ThingSort.h"
#include "Common/BitFlagsIO.h"

#include "GameClient/Drawable.h"
#include "GameClient/FXList.h"
#include "GameClient/GameText.h"
#include "GameClient/Image.h"
#include "GameClient/Shadow.h"

#include "GameLogic/GameLogic.h"
#include "GameLogic/Armor.h"
// #include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/Module/SpecialPowerModule.h"
#include "GameLogic/Object.h"
#include "GameLogic/Powers.h"
#include "GameLogic/Weapon.h"

#include "Common/UnitTimings.h" //Contains the DO_UNIT_TIMINGS define jba.	

#ifdef _INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
const Int USE_EXP_VALUE_FOR_SKILL_VALUE = -999;


///////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE DATA ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

AudioEventRTS ThingTemplate::s_audioEventNoSound;

/* 
	NOTE NOTE NOTE -- s_objectFieldParseTable and s_objectReskinFieldParseTable must be updated in tandem!

	"s_objectReskinFieldParseTable" is intended to be used for parsing the "ObjectReskin" keyword, and 
	should only allow you to specify things that affect the appearance of a template. 
	
	The idea is that some units are functionally the same, but different visually; rather than replicate
	all the settings, you can specify it as "this one is like that one, but looks different".

	Thus, currently, the only things in the reskin table are:
		
		-- DrawModules
		-- DrawModuleData
		-- Geometry
	
	So: if you add/remove/modify any settings that deal with visual appearance, you *may* want to 
	add 'em to the reskin table... but do so VERY CAUTIOUSLY and with careful deliberation (and
	after checking around for other opinions).

*/

// NOTE NOTE NOTE -- s_objectFieldParseTable and s_objectReskinFieldParseTable must be updated in tandem -- see comment above
const FieldParse ThingTemplate::s_objectFieldParseTable[] = 
{
	{ "DisplayName",				INI::parseAndTranslateLabel,					NULL,				offsetof( ThingTemplate::IniData, m_displayName ) },
	{ "RadarPriority",				INI::parseByteSizedIndexList,					RadarPriorityNames,	offsetof( ThingTemplate::IniData, m_radarPriority ) },
	{ "TransportSlotCount",			INI::parseUnsignedByte,							NULL,				offsetof( ThingTemplate::IniData, m_transportSlotCount ) },
	{ "FenceWidth",					INI::parseReal,									NULL,				offsetof( ThingTemplate::IniData, m_fenceWidth ) },
	{ "FenceXOffset",				INI::parseReal,									NULL,				offsetof( ThingTemplate::IniData, m_fenceXOffset ) },
	{ "IsBridge",					INI::parseBool,									NULL,				offsetof( ThingTemplate::IniData, m_isBridge ) },
	{ "ArmorSet",					ThingTemplate::parseArmorTemplateSet,			NULL, 				0},
	{ "WeaponSet",					ThingTemplate::parseWeaponTemplateSet,			NULL, 				0},
	{ "VisionRange",				INI::parseReal,									NULL,				offsetof( ThingTemplate::IniData, m_visionRange ) },
	{ "ShroudClearingRange",		INI::parseReal,									NULL,				offsetof( ThingTemplate::IniData, m_shroudClearingRange ) },
	{ "ShroudRevealToAllRange",		INI::parseReal,									NULL,				offsetof( ThingTemplate::IniData, m_shroudRevealToAllRange ) },

	{ "PlacementViewAngle",			INI::parseAngleReal,							NULL,				offsetof( ThingTemplate::IniData, m_placementViewAngle ) },

	{ "FactoryExitWidth",			INI::parseReal,									NULL,				offsetof( ThingTemplate::IniData, m_factoryExitWidth ) },
	{ "FactoryExtraBibWidth",		INI::parseReal,									NULL,				offsetof( ThingTemplate::IniData, m_factoryExtraBibWidth ) },

	{ "SkillPointValue",			ThingTemplate::parseIntList,					(void*)LEVEL_COUNT,	offsetof( ThingTemplate::IniData, m_skillPointValues ) },
	{ "ExperienceValue",			ThingTemplate::parseIntList,					(void*)LEVEL_COUNT,	offsetof( ThingTemplate::IniData, m_experienceValues ) },
	{ "ExperienceRequired",			ThingTemplate::parseIntList,					(void*)LEVEL_COUNT,	offsetof( ThingTemplate::IniData, m_experienceRequired ) },
	{ "IsTrainable",				INI::parseBool,									NULL,				offsetof( ThingTemplate::IniData, m_isTrainable ) },
	{ "EnterGuard",					INI::parseBool,									NULL,				offsetof( ThingTemplate::IniData, m_enterGuard ) },
	{ "HijackGuard",				INI::parseBool,									NULL,				offsetof( ThingTemplate::IniData, m_hijackGuard ) },

	{ "Side",						INI::parseAsciiString,							NULL,				offsetof( ThingTemplate::IniData, m_defaultOwningSide ) },

// NOTE NOTE NOTE -- s_objectFieldParseTable and s_objectReskinFieldParseTable must be updated in tandem -- see comment above
	{ "Prerequisites",				ThingTemplate::parsePrerequisites,	0, 0 },
	{ "Buildable",					INI::parseByteSizedIndexList,					BuildableStatusNames,	offsetof( ThingTemplate::IniData, m_buildable) },
	{ "BuildCost",					INI::parseUnsignedShort,						NULL,					offsetof( ThingTemplate::IniData, m_buildCost ) },
	{ "BuildTime",					INI::parseReal,									NULL,					offsetof( ThingTemplate::IniData, m_buildTime ) },
	{ "RefundValue",				INI::parseUnsignedShort,						NULL,					offsetof( ThingTemplate::IniData, m_refundValue ) },
	{ "BuildCompletion",			INI::parseByteSizedIndexList,					BuildCompletionNames,	offsetof( ThingTemplate::IniData, m_buildCompletion ) },
	{ "EnergyProduction",			INI::parseInt,									NULL,					offsetof( ThingTemplate::IniData, m_energyProduction ) },
	{ "EnergyBonus",				INI::parseInt,									NULL,					offsetof( ThingTemplate::IniData, m_energyBonus ) },
	{ "IsForbidden",				INI::parseBool,									NULL,					offsetof( ThingTemplate::IniData, m_isForbidden ) },
	{ "IsPrerequisite",				INI::parseBool,									NULL,					offsetof( ThingTemplate::IniData, m_isPrerequisite ) },
	{ "DisplayColor",				INI::parseColorInt,								NULL,					offsetof( ThingTemplate::IniData, m_displayColor ) },
	{ "EditorSorting",				INI::parseByteSizedIndexList,					EditorSortingNames,		offsetof( ThingTemplate::IniData, m_editorSorting ) },
	{ "KindOf",						KindOfMaskType::parseFromINI,					NULL,					offsetof( ThingTemplate::IniData, m_kindof ) },
	{ "CommandSet",					INI::parseAsciiString,							NULL,					offsetof( ThingTemplate::IniData, m_commandSetString ) },	
	{ "BuildVariations",			INI::parseAsciiStringVector,					NULL,					offsetof( ThingTemplate::IniData, m_buildVariations ) },

// NOTE NOTE NOTE -- s_objectFieldParseTable and s_objectReskinFieldParseTable must be updated in tandem -- see comment above
	{ "Behavior",					ThingTemplate::parseModuleName,	(const void*)MODULETYPE_BEHAVIOR,		offsetof(ThingTemplate::IniData, m_behaviorModuleInfo) },
	{ "Body",						ThingTemplate::parseModuleName,	(const void*)999, 						offsetof(ThingTemplate::IniData, m_behaviorModuleInfo) },
	{ "Draw",						ThingTemplate::parseModuleName,	(const void*)MODULETYPE_DRAW, 			offsetof(ThingTemplate::IniData, m_drawModuleInfo) },
	{ "ClientUpdate",				ThingTemplate::parseModuleName,	(const void*)MODULETYPE_CLIENT_UPDATE,	offsetof(ThingTemplate::IniData, m_clientUpdateModuleInfo) },
// NOTE NOTE NOTE -- s_objectFieldParseTable and s_objectReskinFieldParseTable must be updated in tandem -- see comment above

	{ "SelectPortrait",				INI::parseAsciiString,	NULL,		offsetof( ThingTemplate::IniData, m_selectedPortraitImageName ) },
	{ "ButtonImage",				INI::parseAsciiString,	NULL,		offsetof( ThingTemplate::IniData, m_buttonImageName ) },
	
	//Code renderer handles these states now.
	//{ "InventoryImageEnabled",	INI::parseAsciiString,	NULL,		offsetof( ThingTemplate, m_inventoryImage[ INV_IMAGE_ENABLED ] ) },
	//{ "InventoryImageDisabled",	INI::parseAsciiString,	NULL,		offsetof( ThingTemplate, m_inventoryImage[ INV_IMAGE_DISABLED ] ) },
	//{ "InventoryImageHilite",		INI::parseAsciiString,	NULL,		offsetof( ThingTemplate, m_inventoryImage[ INV_IMAGE_HILITE ] ) },
	//{ "InventoryImagePushed",		INI::parseAsciiString,	NULL,		offsetof( ThingTemplate, m_inventoryImage[ INV_IMAGE_PUSHED ] ) },
	
	{ "UpgradeCameo1",				INI::parseAsciiString,	NULL,		offsetof( ThingTemplate::IniData, m_upgradeCameoUpgradeNames[ 0 ] ) },
	{ "UpgradeCameo2",				INI::parseAsciiString,	NULL,		offsetof( ThingTemplate::IniData, m_upgradeCameoUpgradeNames[ 1 ] ) },
	{ "UpgradeCameo3",				INI::parseAsciiString,	NULL,		offsetof( ThingTemplate::IniData, m_upgradeCameoUpgradeNames[ 2 ] ) },
	{ "UpgradeCameo4",				INI::parseAsciiString,	NULL,		offsetof( ThingTemplate::IniData, m_upgradeCameoUpgradeNames[ 3 ] ) },
	{ "UpgradeCameo5",				INI::parseAsciiString,	NULL,		offsetof( ThingTemplate::IniData, m_upgradeCameoUpgradeNames[ 4 ] ) },
	
// NOTE NOTE NOTE -- s_objectFieldParseTable and s_objectReskinFieldParseTable must be updated in tandem -- see comment above

	{ "VoiceSelect",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceSelect,			0 },
	{ "VoiceGroupSelect",			ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceGroupSelect,		0 },
	{ "VoiceMove",					ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceMove,			0 },
	{ "VoiceAttack",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceAttack,			0 },
	{ "VoiceEnter",					ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceEnter,			0 },
	{ "VoiceFear",					ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceFear,			0 },
	{ "VoiceSelectElite",			ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceSelectElite,		0 },
	{ "VoiceCreated",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceCreated,			0 },
	{ "VoiceTaskUnable",			ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceTaskUnable,		0 },
	{ "VoiceTaskComplete",			ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceTaskComplete,	0 },
	{ "VoiceMeetEnemy",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceMeetEnemy,		0 },
	{ "VoiceGarrison",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceGarrison,		0 },
#ifdef ALLOW_SURRENDER
	{ "VoiceSurrender",				INI::parseDynamicAudioEventRTS,	NULL,		offsetof( ThingTemplate, m_audioarray.m_audio[TTAUDIO_voiceSurrender]) },
#endif
	{ "VoiceDefect",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceDefect,					0 },
	{ "VoiceAttackSpecial",			ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceAttackSpecial,			0 },
	{ "VoiceAttackAir",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceAttackAir,				0 },
	{ "VoiceGuard",					ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_voiceGuard,					0 },
	{ "SoundMoveStart",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundMoveStart,				0 },
	{ "SoundMoveStartDamaged",		ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundMoveStartDamaged,		0 },
	{ "SoundMoveLoop",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundMoveLoop,				0 },
	{ "SoundMoveLoopDamaged",		ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundMoveLoopDamaged,			0 },
	{ "SoundAmbient",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundAmbient,					0 },
	{ "SoundAmbientDamaged",		ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundAmbientDamaged,			0 },
	{ "SoundAmbientReallyDamaged",	ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundAmbientReallyDamaged,	0 },
	{ "SoundAmbientRubble",			ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundAmbientRubble, 			0 },
	{ "SoundStealthOn",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundStealthOn,				0 },
	{ "SoundStealthOff",			ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundStealthOff,				0 },
	{ "SoundCreated",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundCreated,					0 },
	{ "SoundOnDamaged",				ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundOnDamaged,				0 },
	{ "SoundOnReallyDamaged",		ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundOnReallyDamaged,			0 },
	{ "SoundEnter",					ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundEnter,					0 },
	{ "SoundExit",					ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundExit,					0 },
	{ "SoundPromotedVeteran",		ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundPromotedVeteran,			0 },
	{ "SoundPromotedElite",			ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundPromotedElite,			0 },
	{ "SoundPromotedHero",			ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundPromotedHero,			0 },
	{ "SoundFallingFromPlane",		ThingTemplate::parseDynamicAudioEventRTS,	(void*)(intptr_t) TTAUDIO_soundFalling,					0 },

	{ "UnitSpecificSounds",			ThingTemplate::parsePerUnitSounds,			NULL,			0 },
	{ "UnitSpecificFX",				ThingTemplate::parsePerUnitFX, 				NULL,			0 },
	{ "Scale",						INI::parseReal,								NULL,			offsetof( ThingTemplate::IniData, m_assetScale ) },
	{ "Geometry",					ThingTemplate::parseGeometryType,			NULL,			0 },
	{ "GeometryMajorRadius",		ThingTemplate::parseGeometryMajorRadius,	NULL,			0 },
	{ "GeometryMinorRadius",		ThingTemplate::parseGeometryMinorRadius,	NULL,			0 },
	{ "GeometryHeight",				ThingTemplate::parseGeometryHeight,			NULL,			0 },
	{ "GeometryIsSmall",			ThingTemplate::parseGeometryIsSmall,		NULL,			0 },
	{ "Shadow",						INI::parseBitString8,						TheShadowNames,	offsetof( ThingTemplate::IniData, m_shadowType ) },
	{ "ShadowSizeX",				INI::parseReal,								NULL,			offsetof( ThingTemplate::IniData, m_shadowSizeX ) },
	{ "ShadowSizeY",				INI::parseReal,								NULL,			offsetof( ThingTemplate::IniData, m_shadowSizeY ) },
	{ "ShadowOffsetX",				INI::parseReal,								NULL,			offsetof( ThingTemplate::IniData, m_shadowOffsetX ) },
	{ "ShadowOffsetY",				INI::parseReal,								NULL,			offsetof( ThingTemplate::IniData, m_shadowOffsetY ) },
	{ "ShadowTexture",				INI::parseAsciiString,						NULL,			offsetof( ThingTemplate::IniData, m_shadowTextureName ) },
	{ "OcclusionDelay",				INI::parseDurationUnsignedInt,				NULL,			offsetof( ThingTemplate::IniData, m_occlusionDelay ) },
	{ "AddModule",					ThingTemplate::parseAddModule,				NULL,			0 },
	{ "RemoveModule",				ThingTemplate::parseRemoveModule,			NULL,			0 },
	{ "ReplaceModule",				ThingTemplate::parseReplaceModule,			NULL,			0 },
	{ "InheritableModule",			ThingTemplate::parseInheritableModule,		NULL,			0 },

	{ "OverrideableByLikeKind",		ThingTemplate::OverrideableByLikeKind,		NULL,			0 },

	// FIXME: AIUpdateModuleData
	// { "Locomotor",					AIUpdateModuleData::parseLocomotorSet,		NULL, 0 },
	{ "Locomotor",					ThingTemplate::parseLocomotorSet,			NULL, 0 },
	{ "InstanceScaleFuzziness",		INI::parseReal,								NULL, offsetof( ThingTemplate::IniData, m_instanceScaleFuzziness ) },
	{ "StructureRubbleHeight",		INI::parseUnsignedByte,						NULL, offsetof( ThingTemplate::IniData, m_structureRubbleHeight ) },
	{ "ThreatValue",				INI::parseUnsignedShort,					NULL, offsetof( ThingTemplate::IniData, m_threatValue ) }, 
	{ "MaxSimultaneousOfType",		ThingTemplate::parseMaxSimultaneous,		NULL, offsetof( ThingTemplate::IniData, m_maxSimultaneousOfType ) }, 
	{ "MaxSimultaneousLinkKey",		NameKeyGenerator::parseStringAsNameKeyType,	NULL, offsetof( ThingTemplate::IniData, m_maxSimultaneousLinkKey ) }, 
	{ "CrusherLevel",				INI::parseUnsignedByte,						NULL, offsetof( ThingTemplate::IniData, m_crusherLevel ) },
	{ "CrushableLevel",				INI::parseUnsignedByte,						NULL, offsetof( ThingTemplate::IniData, m_crushableLevel ) },

	{ 0, 0, 0, 0 }  // keep this last

};
// NOTE NOTE NOTE -- s_objectFieldParseTable and s_objectReskinFieldParseTable must be updated in tandem -- see comment above

// NOTE NOTE NOTE -- s_objectFieldParseTable and s_objectReskinFieldParseTable must be updated in tandem -- see comment above
const FieldParse ThingTemplate::s_objectReskinFieldParseTable[] = 
{
	{ "Draw",					ThingTemplate::parseModuleName,	(const void*)MODULETYPE_DRAW, offsetof(ThingTemplate::IniData, m_drawModuleInfo) },

	{ "Geometry",				ThingTemplate::parseGeometryType,			NULL,	0 },
	{ "GeometryMajorRadius",	ThingTemplate::parseGeometryMajorRadius,	NULL,	0 },
	{ "GeometryMinorRadius",	ThingTemplate::parseGeometryMinorRadius,	NULL,	0 },
	{ "GeometryHeight",			ThingTemplate::parseGeometryHeight,			NULL,	0 },
	{ "GeometryIsSmall",		ThingTemplate::parseGeometryIsSmall,		NULL,	0 },
	{ "FenceWidth",				INI::parseReal,								NULL,	offsetof( ThingTemplate::IniData, m_fenceWidth ) },
	{ "FenceXOffset",			INI::parseReal,								NULL,	offsetof( ThingTemplate::IniData, m_fenceXOffset ) },

	// Needed to avoid some cheats with the scud storm rebuild hole
	{ "MaxSimultaneousOfType",	ThingTemplate::parseMaxSimultaneous,		NULL,	offsetof( ThingTemplate::IniData, m_maxSimultaneousOfType ) }, 
	{ "MaxSimultaneousLinkKey",	NameKeyGenerator::parseStringAsNameKeyType,	NULL,	offsetof( ThingTemplate::IniData, m_maxSimultaneousLinkKey ) }, 

	{ 0, 0, 0, 0 }  // keep this last

};
// NOTE NOTE NOTE -- s_objectFieldParseTable and s_objectReskinFieldParseTable must be updated in tandem -- see comment above

// ------------------------------------------------------------------------------------------------
/** See if the tag string is present in any of the module info entries here */
// ------------------------------------------------------------------------------------------------
const ModuleInfo::Nugget *ModuleInfo::getNuggetWithTag( const AsciiString& tag ) const
{

	std::vector< Nugget >::const_iterator it;
	for( it = m_info.begin(); it != m_info.end(); ++it )
		if( (*it).m_moduleTag == tag )
			return &(*it);

	// no match
	return NULL;

}  // end isTagPresent

// ------------------------------------------------------------------------------------------------
/** Add this module info to the thing template */
// ------------------------------------------------------------------------------------------------
void ModuleInfo::addModuleInfo(ThingTemplate *thingTemplate, 
															 const AsciiString& name, 
															 const AsciiString& moduleTag, 
															 const ModuleData* data, 
															 Int interfaceMask, 
															 Bool inheritable,
                               Bool overrideableByLikeKind)
{

	//
	// there must be a module tag present, and it must be unique across all module infos
	// for this thing template
	//
#if defined(_DEBUG) || defined(_INTERNAL)
	// get module info
	const Nugget *nugget;
	
	nugget = thingTemplate->getBehaviorModuleInfo().getNuggetWithTag( moduleTag );
	if( nugget != NULL )
	{

		// compare this nugget tag against the tag for the new data we're going to submit
		DEBUG_ASSERTCRASH( nugget->m_moduleTag != moduleTag,
											 ("addModuleInfo - ERROR defining module '%s' on thing template '%s'.  The module '%s' has the tag '%s' which must be unique among all modules for this object, but the tag '%s' is also already on module '%s' within this object.\n\nPlease make unique tag names within an object definition\n",
												name.str(),
												thingTemplate->getName().str(),
												name.str(),
												moduleTag.str(),
												moduleTag.str(),
												nugget->first.str()) );

		// srj sez: prevent people from ignoring this.
		throw INI_INVALID_DATA;
	}  // end if

	nugget = thingTemplate->getDrawModuleInfo().getNuggetWithTag( moduleTag );
	if( nugget != NULL )
	{

		// compare this nugget tag against the tag for the new data we're going to submit
		DEBUG_ASSERTCRASH( nugget->m_moduleTag != moduleTag,
											 ("addModuleInfo - ERROR defining module '%s' on thing template '%s'.  The module '%s' has the tag '%s' which must be unique among all modules for this object, but the tag '%s' is also already on module '%s' within this object.\n\nPlease make unique tag names within an object definition\n",
												name.str(),
												thingTemplate->getName().str(),
												name.str(),
												moduleTag.str(),
												moduleTag.str(),
												nugget->first.str()) );

		// srj sez: prevent people from ignoring this.
		throw INI_INVALID_DATA;
	}  // end if

	nugget = thingTemplate->getClientUpdateModuleInfo().getNuggetWithTag( moduleTag );
	if( nugget != NULL )
	{

		// compare this nugget tag against the tag for the new data we're going to submit
		DEBUG_ASSERTCRASH( nugget->m_moduleTag != moduleTag,
											 ("addModuleInfo - ERROR defining module '%s' on thing template '%s'.  The module '%s' has the tag '%s' which must be unique among all modules for this object, but the tag '%s' is also already on module '%s' within this object.\n\nPlease make unique tag names within an object definition\n",
												name.str(),
												thingTemplate->getName().str(),
												name.str(),
												moduleTag.str(),
												moduleTag.str(),
												nugget->first.str()) );
		// srj sez: prevent people from ignoring this.
		throw INI_INVALID_DATA;
	}  // end if

#endif

	m_info.push_back(Nugget(name, moduleTag, data, interfaceMask, inheritable, overrideableByLikeKind));

}

//-------------------------------------------------------------------------------------------------
Bool ModuleInfo::clearModuleDataWithTag(const AsciiString& tagToClear, AsciiString& clearedModuleNameOut) 
{ 
	Bool cleared = false;

	// do NOT clear... we only want to modify this if we return true.
	// if we return false, we should leave this unmodified.
	//clearedModuleNameOut.clear();

	for (std::vector<Nugget>::iterator it = m_info.begin(); it != m_info.end(); /* empty */ ) 
	{
		if (it->m_moduleTag == tagToClear)
		{
			DEBUG_ASSERTCRASH(!cleared, ("Hmm, multiple clears in ModuleInfo::clearModuleDataWithTag, should this be possible?"));
			clearedModuleNameOut = it->first;
			it = m_info.erase(it);
			cleared = true;
		}
		else
		{
			++it;
		}
	}
	return cleared;
}





//-------------------------------------------------------------------------------------------------
Bool ModuleInfo::clearCopiedFromDefaultEntries(Int interfaceMask, const AsciiString &newName, const ThingTemplate *fullTemplate ) 
{ 
  static KindOfMaskType ImmuneToGPSScramblerMask;
  KindOfMaskType &m = ImmuneToGPSScramblerMask;
  m.set(KINDOF_AIRCRAFT);// NO PLANES or helicopters
  m.set(KINDOF_SHRUBBERY);// NO trees or bushes
  m.set(KINDOF_OPTIMIZED_TREE);
  m.set(KINDOF_STRUCTURE);// NO buildings
  m.set(KINDOF_DRAWABLE_ONLY);
  m.set(KINDOF_MOB_NEXUS);
  m.set(KINDOF_IGNORED_IN_GUI);
  m.set(KINDOF_CLEARED_BY_BUILD);
  m.set(KINDOF_DEFENSIVE_WALL);
  m.set(KINDOF_BALLISTIC_MISSILE);
  m.set(KINDOF_SUPPLY_SOURCE);
  m.set(KINDOF_BOAT);
  m.set(KINDOF_INERT);
  m.set(KINDOF_BRIDGE);
  m.set(KINDOF_LANDMARK_BRIDGE);
  m.set(KINDOF_BRIDGE_TOWER);
  Bool disallowed =  fullTemplate->isAnyKindOf( ImmuneToGPSScramblerMask );

  static KindOfMaskType CandidateForGPSScramblerMask;
  CandidateForGPSScramblerMask.set(KINDOF_SCORE);
  CandidateForGPSScramblerMask.set(KINDOF_VEHICLE);
  CandidateForGPSScramblerMask.set(KINDOF_INFANTRY);
  CandidateForGPSScramblerMask.set(KINDOF_PORTABLE_STRUCTURE);
  Bool candidate =  fullTemplate->isAnyKindOf( CandidateForGPSScramblerMask );

  Bool ret = false;

	std::vector<Nugget>::iterator it = m_info.begin();
	while( it != m_info.end() )
	{
		if( (it->interfaceMask & interfaceMask) != 0 && it->copiedFromDefault )
		{
      if ( it->inheritable )
			{
				if( it->m_moduleTag.compare("ModuleTag_DefaultAutoHealBehavior") == 0  && !fullTemplate->isTrainable() )
				{
					// Don't inherit this module if it is entirely useless to us.
          it = m_info.erase( it );
			    ret = true;
				}
				else
				{
					++it;//skip to the next nugget, 'cause we inherit this one
				}
			}
      else if ( it->overrideableByLikeKind)
      {
        
        AsciiString oldName = it->first;
        if ( oldName == newName  //we will dump this instance, since the INI author requested a specific one of the same class
             || disallowed  // or, we just do not Add these special overrideables to these kinds of templates, so just dump it
             || candidate == FALSE )
        {
          it = m_info.erase( it );
			    ret = true;
        }
        else
			    ++it;//no match, preserve the default instnace of this Module for now
      }
      else // just dump this instance of this Module, since one of the same interface mask has been added by caller
      {
        it = m_info.erase( it );
			  ret = true;
      }
    }
    else
			++it;
	}
	return ret;
}








//-------------------------------------------------------------------------------------------------
Bool ModuleInfo::clearAiModuleInfo() 
{ 
	Bool ret = false;

	std::vector<Nugget>::iterator it = m_info.begin();
	while( it != m_info.end() )
	{
		if (it->second->isAiModuleData() )
		{
			it = m_info.erase( it );
			ret = true;
		}
		else
		{
			++it;
		}
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseModuleName(INI* ini, void *instance, void* store, const void* userData)
{
	ThingTemplate::IniData* inidata = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = inidata->m_obj;
	ModuleInfo* mi = (ModuleInfo*)store;
	ModuleType type = (ModuleType)(intptr_t)userData;
	const char* token = ini->getNextToken();
	AsciiString tokenStr = token;

	// get the tag string (it is now required)
	AsciiString moduleTagStr;
	try
	{
		moduleTagStr = ini->getNextToken();
	}
	catch( ... )
	{

		DEBUG_CRASH(( "[LINE: %d - FILE: '%s'] Module tag not found for module '%s' on thing template '%s'.  Module tags are required and must be unique for all modules within an object definition\n",
									ini->getLineNum(), ini->getFilename().str(), 
									tokenStr.str(), self->getName().str() ));
		throw;
				
	}

	Int interfaceMask;

	// ugh -- special case for "Body".
	if (type == 999)
	{
		type = MODULETYPE_BEHAVIOR;
	// what interface(s) does this module support?
		interfaceMask = TheModuleFactory->findModuleInterfaceMask(tokenStr, type);
		if ((interfaceMask & (MODULEINTERFACE_BODY)) == 0)
		{
			DEBUG_CRASH(("Only Body allowed here"));
			throw INI_INVALID_DATA;
		}
	}
	else
	{
		interfaceMask = TheModuleFactory->findModuleInterfaceMask(tokenStr, type);
		if ((interfaceMask & (MODULEINTERFACE_BODY)) != 0)
		{
			DEBUG_CRASH(("No Body allowed here"));
			throw INI_INVALID_DATA;
		}
	}
	
	// if we're overriding, we can totally skip over this block
	if (ini->getLoadType() == INI_LOAD_CREATE_OVERRIDES)
	{	
		if (self->m_ini.m_moduleParsingMode == MODULEPARSE_ADD_REMOVE_REPLACE)
		{
			// do nothing, just fall thru
		}
		else
		{
			DEBUG_CRASH(("[LINE: %d - FILE: '%s'] You must use AddModule to add modules in override INI files.\n",
				ini->getLineNum(), ini->getFilename().str(), self->getName().str()));
			throw INI_INVALID_DATA;
		}
	}
	else
	{

//    if (self->getName().compare("GLAVehicleQuadCannon"))
//      DEBUG_ASSERTCRASH( FALSE, ("WE ARE CLEARING DEFAULT MODULES FROM A QUAD CANNON.") );

		self->m_ini.m_behaviorModuleInfo.clearCopiedFromDefaultEntries(interfaceMask, tokenStr, self );
		self->m_ini.m_drawModuleInfo.clearCopiedFromDefaultEntries(interfaceMask, tokenStr, self );
		self->m_ini.m_clientUpdateModuleInfo.clearCopiedFromDefaultEntries(interfaceMask, tokenStr, self );
	}

	if (self->m_ini.m_moduleParsingMode == MODULEPARSE_ADD_REMOVE_REPLACE 
			&& self->m_ini.m_moduleBeingReplacedName.isNotEmpty()
			&& self->m_ini.m_moduleBeingReplacedName != tokenStr)
	{
		DEBUG_CRASH(("[LINE: %d - FILE: '%s'] ReplaceModule must replace modules with another module of the same type, but you are attempting to replace a %s with a %s for Object %s.\n",
			ini->getLineNum(), ini->getFilename().str(), self->m_ini.m_moduleBeingReplacedName.str(), tokenStr.str(), self->getName().str()));
		throw INI_INVALID_DATA;
	}

	if (self->m_ini.m_moduleParsingMode == MODULEPARSE_ADD_REMOVE_REPLACE 
			&& self->m_ini.m_moduleBeingReplacedTag.isNotEmpty()
			&& self->m_ini.m_moduleBeingReplacedTag == moduleTagStr)
	{
		DEBUG_CRASH(("[LINE: %d - FILE: '%s'] ReplaceModule must specify a new, unique tag for the replaced module, but you are not doing so for %s (%s) for Object %s.\n",
			ini->getLineNum(), ini->getFilename().str(), moduleTagStr.str(), self->m_ini.m_moduleBeingReplacedName.str(), self->getName().str()));
		throw INI_INVALID_DATA;
	}

	ModuleData* data = TheModuleFactory->newModuleDataFromINI(ini, tokenStr, type, moduleTagStr);

	if (data->isAiModuleData())
	{
		Bool replaced = mi->clearAiModuleInfo();
		if (replaced)
		{
			//Kris: Commented this out for SPAM reasons. Do we really need this?
			//DEBUG_LOG(("replaced an AI for %s!\n",self->getName().str()));
		}
	}

	Bool inheritable = (self->m_ini.m_moduleParsingMode == MODULEPARSE_INHERITABLE);
	Bool overrideableByLikeKind = (self->m_ini.m_moduleParsingMode == MODULEPARSE_OVERRIDEABLE_BY_LIKE_KIND);
	mi->addModuleInfo(self, tokenStr, moduleTagStr, data, interfaceMask, inheritable, overrideableByLikeKind);
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseIntList(INI* ini, void* /* instance */, void* store, const void* userData)
{
	Int numberEntries = (intptr_t)userData;
	Int *intList = (Int*)store;

	for( Int intIndex = 0; intIndex < numberEntries; intIndex ++ )
	{
		const char *token = ini->getNextToken();
		intList[intIndex] = ini->scanInt(token);
	}
}

//-------------------------------------------------------------------------------------------------
static void parsePrerequisiteUnit( INI* ini, void *instance, void * /*store*/, const void* /*userData*/ )
{
	std::vector<ProductionPrerequisite>* v = (std::vector<ProductionPrerequisite>*)instance;

	ProductionPrerequisite prereq;
	Bool orUnitWithPrevious = FALSE;
	for (const char *token = ini->getNextToken(); token != NULL; token = ini->getNextTokenOrNull())
	{
		prereq.addUnitPrereq( AsciiString( token ), orUnitWithPrevious );
		orUnitWithPrevious = TRUE;
	}

	v->push_back(prereq);
}

//-------------------------------------------------------------------------------------------------
static void parsePrerequisiteScience( INI* ini, void *instance, void * /*store*/, const void* /*userData*/ )
{
	std::vector<ProductionPrerequisite>* v = (std::vector<ProductionPrerequisite>*)instance;

	ProductionPrerequisite prereq;
	prereq.addSciencePrereq(INI::scanScience(ini->getNextToken()));

	v->push_back(prereq);
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parsePrerequisites( INI* ini, void *instance, void* /* store */, const void* /* userData */ )
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;

	static const FieldParse myFieldParse[] = 
	{
		{ "Object", parsePrerequisiteUnit, 0, 0 },
		{ "Science", parsePrerequisiteScience,	0, 0 },
		{ 0, 0, 0, 0 }
	};

	if (ini->getLoadType() == INI_LOAD_CREATE_OVERRIDES)
	{
		data->m_prereqInfo.clear();
	}

	ini->initFromINI(&data->m_prereqInfo, myFieldParse);
}

//-------------------------------------------------------------------------------------------Static
static void parseArbitraryFXIntoMap( INI* ini, void *instance, void* /* store */, const void* userData )
{
	PerUnitFXMap* mapFX = (PerUnitFXMap*)instance;
	const char* name = (const char*)userData;
	const char* token = ini->getNextToken();
	const FXList* fxl = TheFXListStore->findFXList(token);	// could be null!
	DEBUG_ASSERTCRASH(fxl != NULL || strcasecmp(token, "None") == 0, ("FXList %s not found!\n",token));
	mapFX->insert(std::make_pair(AsciiString(name), fxl));	
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parsePerUnitFX( INI* ini, void *instance, void* /* store */, const void* /* userData */ )
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;
	PerUnitFXMap* fxmap {&self->m_perUnitFX};

	fxmap->clear();

	static const FieldParse myFieldParse[] = 
	{
		{ 0, parseArbitraryFXIntoMap, NULL, 0 },
		{ 0, 0, 0, 0 }
	};

	ini->initFromINI(fxmap, myFieldParse);
}

//-------------------------------------------------------------------------------------------Static
static void parseArbitrarySoundsIntoMap( INI* ini, void *instance, void* /* store */, const void* userData )
{
	PerUnitSoundMap *mapSounds = (PerUnitSoundMap*) instance;
	const char* name = (const char*)userData;
	const char* token = ini->getNextToken();
	
	AudioEventRTS a;
	if (token)
		a.setEventName(token);
	mapSounds->insert(std::make_pair(AsciiString(name), a));	
}

//-------------------------------------------------------------------------------------------------
/** Parse Additional per unit sounds such as TankTurretMove and TankTurretMoveLoop. */
//-------------------------------------------------------------------------------------------------
void ThingTemplate::parsePerUnitSounds(INI* ini, void *instance, void* /* store */, const void* /* userData */)
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;
	PerUnitSoundMap* mapSounds {&self->m_perUnitSounds};
	mapSounds->clear();

	static const FieldParse myFieldParse[] = 
	{
		{ 0, parseArbitrarySoundsIntoMap, NULL, 0 },
		{ 0, 0, 0, 0 }
	};

	ini->initFromINI(mapSounds, myFieldParse);
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseLocomotorSet(INI* ini, void *instance, void *store, const void *userData)
{
	(void) ini;
	(void) instance;
	(void) store;
	(void) userData;
	// FIXME: Remove this dummy parse function once AIUpdateModuleData is implemented.
	DEBUG_LOG(("$$$ Dummy parseLocomotorSet!\n"));
	ini->getNextToken();
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseDynamicAudioEventRTS(INI* ini, void *instance, void* /* store */, const void *userData)
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;
	intptr_t index {reinterpret_cast<intptr_t>(userData)};
	INI::parseDynamicAudioEventRTS(ini, nullptr, &self->m_audioarray.m_audio[index], nullptr);
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseGeometryType( INI* ini, void *instance, void* /* store */, const void *userData )
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;
	GeometryInfo::parseGeometryType(ini, nullptr, &self->m_geometryInfo, userData);
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseGeometryMajorRadius( INI* ini, void *instance, void* /* store */, const void *userData )
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;
	GeometryInfo::parseGeometryMajorRadius(ini, nullptr, &self->m_geometryInfo, userData);
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseGeometryMinorRadius( INI* ini, void *instance, void* /* store */, const void *userData )
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;
	GeometryInfo::parseGeometryMinorRadius(ini, nullptr, &self->m_geometryInfo, userData);
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseGeometryHeight( INI* ini, void *instance, void* /* store */, const void *userData )
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;
	GeometryInfo::parseGeometryHeight(ini, nullptr, &self->m_geometryInfo, userData);
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseGeometryIsSmall( INI* ini, void *instance, void* /* store */, const void *userData )
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;
	GeometryInfo::parseGeometryIsSmall(ini, nullptr, &self->m_geometryInfo, userData);
}

//-------------------------------------------------------------------------------------------------
/** Parse modules to add to the existing set of modules. */
//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseAddModule(INI *ini, void *instance, void* /* store */, const void* /* userData */)
{
	// don't care about the result.
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;

	ModuleParseMode oldMode = (ModuleParseMode)data->m_moduleParsingMode;
	if (oldMode != MODULEPARSE_NORMAL)
		throw INI_INVALID_DATA;

	data->m_moduleParsingMode = MODULEPARSE_ADD_REMOVE_REPLACE;

	ini->initFromINI(data, self->getFieldParse());

	data->m_moduleParsingMode = oldMode;
}

//-------------------------------------------------------------------------------------------------
/** Parse modules to remove from the existing set of modules. */
//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseRemoveModule(INI *ini, void *instance, void* /* store */, const void* /* userData */)
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;

	ModuleParseMode oldMode = (ModuleParseMode)data->m_moduleParsingMode;
	if (oldMode != MODULEPARSE_NORMAL)
		throw INI_INVALID_DATA;

	data->m_moduleParsingMode = MODULEPARSE_ADD_REMOVE_REPLACE;

	const char *modToRemove = ini->getNextToken();
	AsciiString removedModuleName;
	Bool removed = self->removeModuleInfo(modToRemove, removedModuleName);
	if (!removed)
	{
		DEBUG_ASSERTCRASH(removed, ("RemoveModule %s was not found for %s. The game will crash now!\n",modToRemove, self->getName().str()));
		throw INI_INVALID_DATA;
	}

	data->m_moduleParsingMode = oldMode;
}

//-------------------------------------------------------------------------------------------------
/** Replace the existing tagged modules with the new modules. */
//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseReplaceModule(INI *ini, void *instance, void* /* store */, const void* /* userData */)
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;

	ModuleParseMode oldMode = (ModuleParseMode)data->m_moduleParsingMode;
	if (oldMode != MODULEPARSE_NORMAL)
		throw INI_INVALID_DATA;

	data->m_moduleParsingMode = MODULEPARSE_ADD_REMOVE_REPLACE;

	const char *modToRemove = ini->getNextToken();
	AsciiString removedModuleName;
	Bool removed = self->removeModuleInfo(modToRemove, removedModuleName);
	if (!removed)
	{
		DEBUG_CRASH(("[LINE: %d - FILE: '%s'] ThingTemplate::parseReplaceModule: ReplaceModule %s was not found for %s; cannot continue.\n",
															ini->getLineNum(), ini->getFilename().str(), modToRemove, self->getName().str()));
		throw INI_INVALID_DATA;
	}

	data->m_moduleBeingReplacedName = removedModuleName;
	data->m_moduleBeingReplacedTag = modToRemove;
	ini->initFromINI(data, self->getFieldParse());
	data->m_moduleBeingReplacedName.clear();
	data->m_moduleBeingReplacedTag.clear();

	data->m_moduleParsingMode = oldMode;
}

//-------------------------------------------------------------------------------------------------
/** mark the module(s) as being "Inheritable". */
//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseInheritableModule(INI *ini, void *instance, void* /* store */, const void* /* userData */)
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;

	ModuleParseMode oldMode = (ModuleParseMode)data->m_moduleParsingMode;
	if (oldMode != MODULEPARSE_NORMAL)
		throw INI_INVALID_DATA;

	data->m_moduleParsingMode = MODULEPARSE_INHERITABLE;

	ini->initFromINI(data, self->getFieldParse());

	data->m_moduleParsingMode = oldMode;
}


//-------------------------------------------------------------------------------------------------
/** mark the module(s) as being "OverrideableByLikeKind". default module will be replaced by any of the exact same class */
//-------------------------------------------------------------------------------------------------
void ThingTemplate::OverrideableByLikeKind(INI *ini, void *instance, void* /* store */, const void* /* userData */)
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;

	ModuleParseMode oldMode = (ModuleParseMode)data->m_moduleParsingMode;
	if (oldMode != MODULEPARSE_NORMAL)
		throw INI_INVALID_DATA;

	data->m_moduleParsingMode = MODULEPARSE_OVERRIDEABLE_BY_LIKE_KIND;

	ini->initFromINI(data, self->getFieldParse());

	data->m_moduleParsingMode = oldMode;
}



//-------------------------------------------------------------------------------------------------
/** Remove the module whose tag matches moduleToRemove. */
//-------------------------------------------------------------------------------------------------
Bool ThingTemplate::removeModuleInfo(const AsciiString& moduleToRemove, AsciiString& clearedModuleNameOut)
{
	Bool removed = false;

	// do NOT clear... we only want to modify this if we return true.
	// if we return false, we should leave this unmodified.
	//clearedModuleNameOut.clear();

	if (m_ini.m_behaviorModuleInfo.clearModuleDataWithTag(moduleToRemove, clearedModuleNameOut))
	{
		DEBUG_ASSERTCRASH(!removed, ("Hmm, multiple removed in ThingTemplate::removeModuleInfo, should this be possible?"));
		removed = true;
	}
	if (m_ini.m_drawModuleInfo.clearModuleDataWithTag(moduleToRemove, clearedModuleNameOut))
	{
		DEBUG_ASSERTCRASH(!removed, ("Hmm, multiple removed in ThingTemplate::removeModuleInfo, should this be possible?"));
		removed = true;
	}
	if (m_ini.m_clientUpdateModuleInfo.clearModuleDataWithTag(moduleToRemove, clearedModuleNameOut))
	{
		DEBUG_ASSERTCRASH(!removed, ("Hmm, multiple removed in ThingTemplate::removeModuleInfo, should this be possible?"));
		removed = true;
	}

	return removed;
}

//-------------------------------------------------------------------------------------------------
/// @todo srj -- move this to another file
void ArmorTemplateSet::parseArmorTemplateSet( INI* ini )
{
	static const FieldParse myFieldParse[] = 
	{
		{ "Conditions", ArmorSetFlags::parseFromINI, NULL, offsetof( ArmorTemplateSet, m_types ) },
		{ "Armor", INI::parseArmorTemplate,	NULL, offsetof( ArmorTemplateSet, m_template ) },
		{ "DamageFX",	INI::parseDamageFX,	NULL, offsetof( ArmorTemplateSet, m_fx ) },
		{ 0, 0, 0, 0 }
	};

	ini->initFromINI(this, myFieldParse);
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseArmorTemplateSet( INI* ini, void *instance, void * /*store*/, const void* /*userData*/ )
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;

	if (data->m_armorCopiedFromDefault == TRUE)
	{
		data->m_armorCopiedFromDefault = FALSE;
		self->m_armorTemplateSets.clear();
	}

	ArmorTemplateSet ws {};
	ws.parseArmorTemplateSet(ini);
#if defined(_DEBUG) || defined(_INTERNAL)
	if (ini->getLoadType() != INI_LOAD_CREATE_OVERRIDES)
	{
		for (ArmorTemplateSetVector::const_iterator it = self->m_armorTemplateSets.begin(); it != self->m_armorTemplateSets.end(); ++it)
		{
			if (it->getNthConditionsYes(0) == ws.getNthConditionsYes(0))
			{
				DEBUG_CRASH(("dup armorset condition in %s\n",self->getName().str()));
			}
		}
	}
#endif
	self->m_armorTemplateSets.push_back(ws);
	self->m_armorTemplateSetFinder.clear();
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::parseWeaponTemplateSet( INI* ini, void *instance, void * /*store*/, const void* /*userData*/ )
{
	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	ThingTemplate* self = data->m_obj;

	if (data->m_weaponsCopiedFromDefault == TRUE)
	{
		data->m_weaponsCopiedFromDefault = FALSE;
		data->m_weaponTemplateSets.clear();
	}

	WeaponTemplateSet ws;
	ws.parseWeaponTemplateSet(ini, self);
#if defined(_DEBUG) || defined(_INTERNAL)
	if (ini->getLoadType() != INI_LOAD_CREATE_OVERRIDES)
	{
		for (WeaponTemplateSetVector::const_iterator it = data->m_weaponTemplateSets.begin(); it != data->m_weaponTemplateSets.end(); ++it)
		{
			if (it->getNthConditionsYes(0) == ws.getNthConditionsYes(0))
			{
				DEBUG_CRASH(("dup weaponset condition in %s\n",self->getName().str()));
			}
		}
	}
#endif
	data->m_weaponTemplateSets.push_back(ws);
	self->m_weaponTemplateSetFinder.clear();
}

//-------------------------------------------------------------------------------------------------
// Parse the "maxSimultaneousOfType" keyword
void ThingTemplate::parseMaxSimultaneous(INI *ini, void *instance, void *store, const void* /* userData */)
{
	// Most of the time, this is an UnsignedShort, but sometimes this is the keyword
	// "DeterminedBySuperweaponRestriction"
	const char DETERMINED_BY_SUPERWEAPON_KEYWORD[] = "DeterminedBySuperweaponRestriction";

	ThingTemplate::IniData* data = (ThingTemplate::IniData*) instance;
	DEBUG_ASSERTCRASH ( &data->m_maxSimultaneousOfType == store, ("Bad store passed to parseMaxSimultaneous" ) );

	const char * token = ini->getNextToken();
	if ( strcasecmp( token, DETERMINED_BY_SUPERWEAPON_KEYWORD ) == 0 )
	{
		data->m_maxSimultaneousDeterminedBySuperweaponRestriction = true;
		*(UnsignedShort *)store = 0;
	}
	else
	{
		// Copied from parseUnsignedShort
		Int value = INI::scanInt(token);
		if (value < 0 || value > 65535)
		{
			DEBUG_CRASH(("Bad value parseMaxSimultaneous"));
			throw ERROR_BUG;
		}
		*(UnsignedShort *)store = (UnsignedShort)value;
		data->m_maxSimultaneousDeterminedBySuperweaponRestriction = false;
	}
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
ThingTemplate::ThingTemplate() :
	m_geometryInfo(GEOMETRY_SPHERE, FALSE, 1, 1, 1)
{
	m_ini.m_moduleParsingMode = MODULEPARSE_NORMAL;
	m_ini.m_reskinnedFrom = NULL;
	m_ini.m_radarPriority = RADAR_PRIORITY_INVALID;

	m_ini.m_nextThingTemplate = NULL;
	m_ini.m_transportSlotCount = 0;
	m_ini.m_fenceWidth = 0;
	m_ini.m_fenceXOffset = 0;
	m_ini.m_visionRange = 0.0f;
	m_ini.m_shroudClearingRange = -1.0f;
	m_ini.m_shroudRevealToAllRange = -1.0f;

	m_ini.m_buildCost = 0;
	m_ini.m_buildTime = 1;
	m_ini.m_refundValue = 0;
	m_ini.m_energyProduction = 0;
	m_ini.m_energyBonus = 0;
	m_ini.m_buildCompletion = BC_APPEARS_AT_RALLY_POINT;

	for( Int levelIndex = 0; levelIndex < LEVEL_COUNT; levelIndex++ )
	{
		m_ini.m_experienceValues[levelIndex] = 0;
		m_ini.m_experienceRequired[levelIndex] = 0;
		// -1 means "same value as experienceValues for that level"
		m_ini.m_skillPointValues[levelIndex] = USE_EXP_VALUE_FOR_SKILL_VALUE;
	}
	m_ini.m_isTrainable = FALSE;
	m_ini.m_enterGuard = FALSE;
	m_ini.m_hijackGuard = FALSE;

	m_ini.m_templateID = 0;
	m_ini.m_kindof = KINDOFMASK_NONE;
	//m_defaultOwningSide = "";	// unnecessary
	m_ini.m_isBuildFacility = FALSE;
	m_ini.m_isPrerequisite = FALSE;
	m_ini.m_placementViewAngle = 0.0f;
	m_ini.m_factoryExitWidth = 0.0f;
	m_ini.m_factoryExtraBibWidth = 0.0f;

	m_ini.m_selectedPortraitImage = NULL;
	m_ini.m_buttonImage = NULL;

	m_ini.m_shadowType = SHADOW_NONE;
	m_ini.m_shadowSizeX = 0.0f;
	m_ini.m_shadowSizeY = 0.0f;
	m_ini.m_shadowOffsetX = 0.0f;
	m_ini.m_shadowOffsetY = 0.0f;
	m_ini.m_occlusionDelay = TheGlobalData->m_data.m_defaultOcclusionDelay;

	m_ini.m_structureRubbleHeight = 0;
	m_ini.m_instanceScaleFuzziness = 0;
	m_ini.m_threatValue = 0;
	m_ini.m_maxSimultaneousOfType = 0;	// unlimited
	m_ini.m_maxSimultaneousLinkKey = NAMEKEY_INVALID; // Not linked
	m_ini.m_maxSimultaneousDeterminedBySuperweaponRestriction = false;
	m_ini.m_crusherLevel = 0;			//Unspecified, this object is unable to crush anything!
	m_ini.m_crushableLevel = 255; //Unspecified, this object is unable to be crushed by anything!

	m_ini.m_obj = this;
}

//-------------------------------------------------------------------------------------------------
AIUpdateModuleData *ThingTemplate::friend_getAIModuleInfo(void)
{
	Int numModInfos = m_ini.m_behaviorModuleInfo.getCount();
	for (int j = 0; j < numModInfos; ++j) 
	{
		if (m_ini.m_behaviorModuleInfo.getNthData(j) && m_ini.m_behaviorModuleInfo.getNthData(j)->isAiModuleData()) 
		{
			return (AIUpdateModuleData *)m_ini.m_behaviorModuleInfo.friend_getNthData(j);
		}
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void ThingTemplate::validateAudio()
{
#if defined(_DEBUG) || defined(_INTERNAL)

	#define AUDIO_TEST(y) \
		if (!get##y()->getEventName().isEmpty() && get##y()->getEventName().compareNoCase("NoSound") != 0) { \
			DEBUG_ASSERTLOG(TheAudio->isValidAudioEvent(get##y()), ("Invalid Sound '%s' in Object '%s'. (%s?)\n", #y, getName().str(), get##y()->getEventName().str())); \
		}

	AUDIO_TEST(VoiceSelect)
	AUDIO_TEST(VoiceGroupSelect)
	AUDIO_TEST(VoiceMove)
	AUDIO_TEST(VoiceAttack)
	AUDIO_TEST(VoiceEnter)
	AUDIO_TEST(VoiceFear)
	AUDIO_TEST(VoiceSelectElite)
	AUDIO_TEST(VoiceCreated)
	AUDIO_TEST(VoiceNearEnemy)
	AUDIO_TEST(VoiceTaskUnable)
	AUDIO_TEST(VoiceTaskComplete)
	AUDIO_TEST(VoiceMeetEnemy)
	AUDIO_TEST(VoiceGarrison)
#ifdef ALLOW_SURRENDER
	AUDIO_TEST(VoiceSurrender)
#endif
	AUDIO_TEST(VoiceDefect)
	AUDIO_TEST(VoiceAttackSpecial)
	AUDIO_TEST(VoiceAttackAir)
	AUDIO_TEST(VoiceGuard)

	AUDIO_TEST(SoundMoveStart)
	AUDIO_TEST(SoundMoveStartDamaged)
	AUDIO_TEST(SoundMoveLoop)
	AUDIO_TEST(SoundMoveLoopDamaged)
	AUDIO_TEST(SoundAmbient)
	AUDIO_TEST(SoundAmbientDamaged)
	AUDIO_TEST(SoundAmbientReallyDamaged)
	AUDIO_TEST(SoundAmbientRubble)
	AUDIO_TEST(SoundStealthOn)
	AUDIO_TEST(SoundStealthOff)
	AUDIO_TEST(SoundCreated)

	AUDIO_TEST(SoundOnDamaged)
	AUDIO_TEST(SoundOnReallyDamaged)
	AUDIO_TEST(SoundEnter)
	AUDIO_TEST(SoundExit)
	AUDIO_TEST(SoundPromotedVeteran)
	AUDIO_TEST(SoundPromotedElite)
	AUDIO_TEST(SoundPromotedHero)
	
	#undef AUDIO_TEST

	const PerUnitSoundMap *perUnitSounds = getAllPerUnitSounds();
	if (!perUnitSounds) 
	{
		return;
	}

	for (PerUnitSoundMap::const_iterator it = perUnitSounds->begin(); it != perUnitSounds->end(); ++it) 
	{
		if (!it->second.getEventName().isEmpty() && it->second.getEventName().compareNoCase("NoSound") != 0) 
		{
			DEBUG_ASSERTCRASH(TheAudio->isValidAudioEvent(&it->second), 
												("Invalid UnitSpecificSound '%s' in Object '%s'. (%s?)", 
												it->first.str(), 
												getName().str(), 
												it->second.getEventName().str())); 
		}
	}
#endif
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::validate()
{
	if (m_ini.m_shadowTextureName.isEmpty())
	{	
		// no texture given, pick a default
		switch (getTemplateGeometryInfo().getGeomType())
		{
			case GEOMETRY_SPHERE:
			case GEOMETRY_CYLINDER:
				m_ini.m_shadowTextureName = "shadow";
				break;
			case GEOMETRY_BOX:
				m_ini.m_shadowTextureName = "shadows";
				break;
			default:
				break;
		}
	}

	validateAudio();

#if defined(_DEBUG) || defined(_INTERNAL)
	
	if (getName() == "DefaultThingTemplate")
		return;

	// neutron missile is an old special case....
	if (getName() == "NeutronMissile")
		return;

	// another cheesy special case....
	if (getName() == "FlamethrowerProjectileStream")
		return;

	// drawable-only templates are exempt from further checks.
	if (isKindOf(KINDOF_DRAWABLE_ONLY))
		return;

	// build-variation templates are exempt from further checks.
	if (!m_ini.m_buildVariations.empty())
		return;

	Bool isImmobile = isKindOf(KINDOF_IMMOBILE);
	
	if (isKindOf(KINDOF_SHRUBBERY) && !isImmobile)
	{
		DEBUG_CRASH(("SHRUBBERY %s must be marked IMMOBILE!",getName().str()));
	}

	if (isKindOf(KINDOF_STRUCTURE) && !isImmobile)
	{
		DEBUG_CRASH(("Structure %s is not marked immobile, but probably should be -- please fix it. (If we ever add mobile structures, this debug sniffer will need to be revised.)\n",getName().str()));
	}

	if (isKindOf(KINDOF_STICK_TO_TERRAIN_SLOPE) && !isImmobile)
	{
		DEBUG_CRASH(("item %s is marked STICK_TO_TERRAIN_SLOPE but not IMMOBILE -- please fix it.\n",getName().str()));
	}

	// FIXME: TheArmorStore
	// if (isKindOf(KINDOF_STRUCTURE))
	// {
	// 	if (m_armorTemplateSets.empty() || (m_armorTemplateSets.size() == 1 && m_armorTemplateSets[0].getArmorTemplate() == NULL))
	// 	{
	// 		DEBUG_CRASH(("Structure %s has no armor, but probably should (StructureArmor) -- please fix it.)\n",getName().str()));
	// 	}
	// 	for (ArmorTemplateSetVector::const_iterator it = m_armorTemplateSets.begin(); it != m_armorTemplateSets.end(); ++it)
	// 	{
	// 		if (it->getDamageFX() == NULL)
	// 		{
	// 			DEBUG_CRASH(("Structure %s has no ArmorDamageFX, and really should.\n",getName().str()));
	// 		}
	// 	}
	// }

#endif
}

//-------------------------------------------------------------------------------------------------
// copy the guts of that into this, but preserve this' name, id, and list-links.
void ThingTemplate::copyFrom(const ThingTemplate* that)
{
	if (!that)
		return;

	ThingTemplate* next = this->m_ini.m_nextThingTemplate;
	UnsignedShort id = this->m_ini.m_templateID;
	AsciiString name = this->m_ini.m_nameString;

	*this = *that;

	this->m_ini.m_nextThingTemplate = next;
	this->m_ini.m_templateID = id;
	this->m_ini.m_nameString = name;
}

//-------------------------------------------------------------------------------------------------
void ThingTemplate::setCopiedFromDefault()
{
	m_ini.m_armorCopiedFromDefault = true;
	m_ini.m_weaponsCopiedFromDefault = true;
	m_ini.m_behaviorModuleInfo.setCopiedFromDefault(true);
	m_ini.m_drawModuleInfo.setCopiedFromDefault(true);
	m_ini.m_clientUpdateModuleInfo.setCopiedFromDefault(true);
}

//-------------------------------------------------------------------------------------------------
ThingTemplate::~ThingTemplate()
{
	// note, we don't need to take any special action for Armor/WeaponSets...
	// though it is just a list of 'raw' pointers, we don't have ownership of 'em,
	// and so we MUST NOT delete them
} 

//=============================================================================
void ThingTemplate::resolveNames()
{
	//Kris: July 31, 2003
	//NOTE: Make sure that all code in this function supports caching properly. For example,
	//      templates can be partially overridden by map.ini files. When this happens, strings
	//      that have been parsed are looked up, cached, then cleared. The problem is if a string
	//      gets cached, but not overridden, it will be clear the next time we call this function.
	//      so we will want to make sure we don't NULL out cached data if the string is empty. A
	//      concrete example is overriding an object with prerequisites. We just override the portrait.
	//      So the 1st time we call this function, we get the standard template data. During this first
	//      call, the strings are looked up, cached, and cleared. Then we override the portrait in the 
	//      map.ini. The next time we call this function, we look up all the strings again. The prereq
	//      names didn't used to check for empty strings so they would NULL out all the previous prereqs
	//      the object had. So be sure to make sure all string lookups don't blindly lookup things -- check
	//      if the string isNotEmpty first!

	for (size_t i = 0; i < m_ini.m_prereqInfo.size(); i++)
	{
		m_ini.m_prereqInfo[i].resolveNames();
	}

	const Int MAX_BF = 32;
	const ThingTemplate* tmpls[MAX_BF];
	for (size_t i = 0; i < m_ini.m_prereqInfo.size(); i++)
	{
		Int count = m_ini.m_prereqInfo[i].getAllPossibleBuildFacilityTemplates(tmpls, MAX_BF);
		for (int j = 0; j < count; j++)
		{
			// casting const away is a little evil, but justified in this case:
			// PropductionPrerequisite should only be allowed 'const' access,
			// but ThingTemplate can muck with stuff with gleeful abandon. (srj)
			if( tmpls[ j ] )
				const_cast<ThingTemplate*>(tmpls[j])->m_ini.m_isBuildFacility = true;
			// DEBUG_LOG(("BF: %s is a buildfacility for %s\n",tmpls[j]->m_nameString.str(),this->m_nameString.str()));
		}
	}
	
	if (isKindOf(KINDOF_COMMANDCENTER)) {
		// Command centers are considered factories. jba.
		m_ini.m_isBuildFacility = true;
	}

	// keep a pointer to portrait and button image if present for speed later
	if( TheMappedImageCollection )
	{
		if( m_ini.m_selectedPortraitImageName.isNotEmpty() )
		{
			m_ini.m_selectedPortraitImage = TheMappedImageCollection->findImageByName( m_ini.m_selectedPortraitImageName );
			DEBUG_ASSERTCRASH( m_ini.m_selectedPortraitImage, ("%s is looking for Portrait %s but can't find it. Skipping...", getName().str(), m_ini.m_buttonImageName.str() ) );
			m_ini.m_selectedPortraitImageName.clear();	// we're done with this, so nuke it
		}
		if( m_ini.m_buttonImageName.isNotEmpty() )
		{
			m_ini.m_buttonImage = TheMappedImageCollection->findImageByName( m_ini.m_buttonImageName );
			DEBUG_ASSERTCRASH( m_ini.m_buttonImage, ("%s is looking for ButtonImage %s but can't find it. Skipping...", getName().str(), m_ini.m_buttonImageName.str() ) );
			m_ini.m_buttonImageName.clear();	// we're done with this, so nuke it
		}
	}

}

//=============================================================================
#ifdef LOAD_TEST_ASSETS
void ThingTemplate::initForLTA(const AsciiString& name)
{
	m_ini.m_nameString = name;

	char buffer[1024];
	strncpy(buffer, name.str(), sizeof(buffer));
	int i {0};
	for (i=0; buffer[i]; i++) {
		if (buffer[i] == '/') {
			i++;
			break;
		}
	}
	m_ini.m_LTAName = AsciiString(buffer+i);

	m_ini.m_behaviorModuleInfo.clear();
	m_ini.m_drawModuleInfo.clear();
	m_ini.m_clientUpdateModuleInfo.clear();

	AsciiString moduleTag;

	moduleTag.format( "LTA_%sDestroyDie", m_ini.m_LTAName.str() );
	m_ini.m_behaviorModuleInfo.addModuleInfo(this, "DestroyDie", moduleTag, TheModuleFactory->newModuleDataFromINI(NULL, "DestroyDie", MODULETYPE_BEHAVIOR, moduleTag), (MODULEINTERFACE_DIE), false);

	moduleTag.format( "LTA_%sInactiveBody", m_ini.m_LTAName.str() );
	m_ini.m_behaviorModuleInfo.addModuleInfo(this, "InactiveBody", moduleTag, TheModuleFactory->newModuleDataFromINI(NULL, "InactiveBody", MODULETYPE_BEHAVIOR, moduleTag), (MODULEINTERFACE_BODY), false);

	moduleTag.format( "LTA_%sW3DDefaultDraw", m_ini.m_LTAName.str() );
	m_ini.m_drawModuleInfo.addModuleInfo(this, "W3DDefaultDraw", moduleTag, TheModuleFactory->newModuleDataFromINI(NULL, "W3DDefaultDraw", MODULETYPE_DRAW, moduleTag), (MODULEINTERFACE_DRAW), false);

	m_ini.m_armorCopiedFromDefault = false;
	m_ini.m_weaponsCopiedFromDefault = false;

	m_ini.m_kindof = KINDOFMASK_NONE;
	m_ini.m_assetScale = 1.0f;
	m_ini.m_instanceScaleFuzziness = 0.0f;	///< tolerance to randomly vary scale per instance
	m_ini.m_structureRubbleHeight = 0.0f;		// zero means "use global default"
	m_ini.m_displayName.translate( name );
	m_ini.m_shadowType = SHADOW_VOLUME;

	m_geometryInfo.set(GEOMETRY_SPHERE, false, 10.0, 10.0, 10.0);
	
}
#endif


//=============================================================================
const ArmorTemplateSet* ThingTemplate::findArmorTemplateSet(const ArmorSetFlags& t) const
{
  return m_armorTemplateSetFinder.findBestInfo(m_armorTemplateSets, t);
}

//=============================================================================
const WeaponTemplateSet* ThingTemplate::findWeaponTemplateSet(const WeaponSetFlags& t) const
{
  return m_weaponTemplateSetFinder.findBestInfo(m_ini.m_weaponTemplateSets, t);
}

//-----------------------------------------------------------------------------
// returns true iff we have at least one weaponset that contains a weapon.
// returns false if we have no weaponsets, or they are all empty.
Bool ThingTemplate::canPossiblyHaveAnyWeapon() const
{
	for (WeaponTemplateSetVector::const_iterator it = m_ini.m_weaponTemplateSets.begin(); 
					it != m_ini.m_weaponTemplateSets.end();
					++it)
	{
		if (it->hasAnyWeapons())
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
Int ThingTemplate::getSkillPointValue(Int level) const 
{ 
	Int value = m_ini.m_skillPointValues[level]; 
	
	if (value == USE_EXP_VALUE_FOR_SKILL_VALUE)
		value = getExperienceValue(level);
	
	return value;
}

//-----------------------------------------------------------------------------
const ThingTemplate *ThingTemplate::getBuildFacilityTemplate( const Player *player ) const
{
	if (getPrereqCount() > 0)
	{
		return m_ini.m_prereqInfo[0].getExistingBuildFacilityTemplate(player);	// might return null
	}
	else
	{
		return NULL;
	}
}

//-------------------------------------------------------------------------------------------------
BuildableStatus ThingTemplate::getBuildable() const 
{ 
	BuildableStatus bs;
	if (TheGameLogic && TheGameLogic->findBuildableStatusOverride(this, bs))
		return bs;

	return (BuildableStatus)m_ini.m_buildable; 
}

//-------------------------------------------------------------------------------------------------
const FXList *ThingTemplate::getPerUnitFX(const AsciiString& fxName) const
{
	if (fxName.isEmpty()) 
	{
		return NULL;
	}
	
	PerUnitFXMap::const_iterator it = m_perUnitFX.find(fxName);
	if (it == m_perUnitFX.end()) 
	{
		DEBUG_CRASH(("Unknown FX name (%s) asked for in ThingTemplate (%s). ", fxName.str(), m_ini.m_nameString.str()));
		return NULL;
	}

	return (it->second);
}

//-------------------------------------------------------------------------------------------------
const AudioEventRTS *ThingTemplate::getPerUnitSound(const AsciiString& soundName) const
{
	if (soundName.isEmpty()) 
	{
		return &s_audioEventNoSound;
	}
	
	PerUnitSoundMap::const_iterator it = m_perUnitSounds.find(soundName);
	if (it == m_perUnitSounds.end()) 
	{
#ifndef DO_UNIT_TIMINGS
    DEBUG_LOG(("Unknown Audio name (%s) asked for in ThingTemplate (%s).\n", soundName.str(), m_ini.m_nameString.str()));
#endif
    return &s_audioEventNoSound;
	}

	return &(it->second);
}

//-------------------------------------------------------------------------------------------------
UnsignedInt ThingTemplate::getMaxSimultaneousOfType() const
{
  if ( m_ini.m_maxSimultaneousDeterminedBySuperweaponRestriction && TheGameLogic )
  {
    return TheGameLogic->getSuperweaponRestriction();
  }

  return m_ini.m_maxSimultaneousOfType;
}




//-------------------------------------------------------------------------------------------------
Bool ThingTemplate::isEquivalentTo(const ThingTemplate* tt) const
{
	// sanity
	if (!tt) 
		return false;

	// sanity
	if (this == tt) 
		return true;
	
	if (this->getFinalOverride() == tt->getFinalOverride()) 
		return true;

	// This reskinned from that?
	if (this->m_ini.m_reskinnedFrom == tt)
		return true;

	// That reskinned from this?
	if (this == tt->m_ini.m_reskinnedFrom)
		return true;

	// This reskinned from that reskinned from?
	// Kris: added case (chassis 2 compared to chassis 3 -- NULL possible if not reskinned)
	if( this->m_ini.m_reskinnedFrom && this->m_ini.m_reskinnedFrom == tt->m_ini.m_reskinnedFrom )
		return true;

	// Is this thing a build variation of that thing or vice versa
	size_t numVariations = m_ini.m_buildVariations.size();
	for (size_t i = 0; i < numVariations; ++i) 
		if (m_ini.m_buildVariations[i].compareNoCase(tt->getName()) == 0)
			return true;
	
	numVariations = tt->m_ini.m_buildVariations.size();
	for (size_t i = 0; i < numVariations; ++i)
		if (tt->m_ini.m_buildVariations[i].compareNoCase(getName()) == 0)
			return true;

	// Guess we're not equivalent.
	return false;
}

//-------------------------------------------------------------------------------------------------
Bool ThingTemplate::isBuildableItem(void) const
{
	return (getBuildCost() != 0);
}

//-------------------------------------------------------------------------------------------------
/** NOTE that we're not paying attention to m_override here, instead the portions
	* that retrieve template data values use the get() wrappers, which *DO* pay
	* attention to the override values */
//-------------------------------------------------------------------------------------------------
Int ThingTemplate::calcCostToBuild( const Player* player) const
{
	if (!player)
		return 0;

	// changePercent format is "-.2 equals 20% cheaper"
	Real factionModifier = 1 + player->getProductionCostChangePercent( getName() );
	factionModifier *= player->getProductionCostChangeBasedOnKindOf( m_ini.m_kindof );
	return getBuildCost() * factionModifier * player->getHandicap()->getHandicap(Handicap::BUILDCOST, this);
}

#if 0
//-------------------------------------------------------------------------------------------------
/** NOTE that we're not paying attention to m_override here, instead the portions
	* that retrieve template data values use the get() wrappers, which *DO* pay
	* attention to the override values */
//-------------------------------------------------------------------------------------------------
Int ThingTemplate::calcTimeToBuild( const Player* player) const
{
	Real buildTime = getBuildTime() * static_cast<Real>(LOGICFRAMES_PER_SECOND);
	buildTime *= player->getHandicap()->getHandicap(Handicap::BUILDTIME, this);

	Real factionModifier = 1 + player->getProductionTimeChangePercent( getName() );
	buildTime *= factionModifier;

#if defined (_DEBUG) || defined (_INTERNAL) || defined(_ALLOW_DEBUG_CHEATS_IN_RELEASE)
	if( player->buildsInstantly() )
	{
		buildTime = 1;
	}
#endif

	// Adjust build time based on energy supply.

	Real EnergyPercent = player->getEnergy()->getEnergySupplyRatio();	//I'm at 80% Energy
	if (EnergyPercent > 1.0f)
		EnergyPercent = 1.0f;	// getEnergySupplyRatio() returns a true ratio, but we don't care about excess.
	Real EnergyShort = 1.0f - EnergyPercent;					//so I am 20% short
	EnergyShort *= TheGlobalData->m_data.m_LowEnergyPenaltyModifier;	//which is a 40% penalty, or a 10% penalty
	Real penaltyRate = 1.0f - EnergyShort;
	penaltyRate = max(penaltyRate, TheGlobalData->m_data.m_MinLowEnergyProductionSpeed);	//bind so 0% does not dead stop you

	if( EnergyPercent < 1.0f )	//and make 99% look like 80% (eg) since most of the time you are down only a little
		penaltyRate = min(penaltyRate, TheGlobalData->m_data.m_MaxLowEnergyProductionSpeed);

	if (penaltyRate <= 0.0f)
		penaltyRate = 0.01f;	// Design won't make the minimum 0, they promise

	buildTime /= penaltyRate;//and voila.  Makes sense, designers have control, all is happy.

	//	Multiple build facilities can have an added production bonus.

	if (getBuildCompletion() == BC_APPEARS_AT_RALLY_POINT)
	{
		const ThingTemplate *tmpl = getBuildFacilityTemplate(player);	// could be null if none exist
		Int count = 0;
		if (tmpl)
		{
			player->countObjectsByThingTemplate(1, &tmpl, false, &count);
			Real factoryMult = TheGlobalData->m_data.m_MultipleFactory;
			if (factoryMult > 0.0f)
			{
				for(int i=0; i < count - 1; i++)
					buildTime *= factoryMult;
			}
		}
	}

	return(buildTime);
}
#endif // if 0

//---------------------------------------------------------------------------------------ModuleInfo
//-------------------------------------------------------------------------------------------------
ModuleData* ModuleInfo::friend_getNthData(Int i)
{
	if (i >= 0 && i < static_cast<Int>(m_info.size()))
	{
		// This is kinda naughty, but its necessary.
		return const_cast<ModuleData*>(m_info.data()[i].second);
	}
	return NULL;
}
