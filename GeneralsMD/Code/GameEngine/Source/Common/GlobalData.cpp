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

// FILE: GlobalData.cpp ///////////////////////////////////////////////////////////////////////////
// The GameLogicData object
// Author: trolfs, Michael Booth, Colin Day, April 2001
///////////////////////////////////////////////////////////////////////////////////////////////////

//#pragma once

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "PreRTS.h"	// This must go first in EVERY cpp file in the GameEngine

#define DEFINE_TERRAIN_LOD_NAMES
#define DEFINE_TIME_OF_DAY_NAMES
#define DEFINE_WEATHER_NAMES
#define DEFINE_BODYDAMAGETYPE_NAMES
#define DEFINE_PANNING_NAMES

#include "Common/CRC.h"
#include "Common/File.h"
#include "Common/FileSystem.h"
// #include "Common/GameAudio.h"
#include "Common/INI.h"
// #include "Common/Registry.h"
#include "Common/UserPreferences.h"
// #include "Common/Version.h"

// #include "GameLogic/AI.h"
#include "GameLogic/Weapon.h"
// #include "GameLogic/Module/BodyModule.h"

#include "GameClient/Color.h"
// #include "GameClient/TerrainVisual.h"

#include "GameNetwork/FirewallHelper.h"

#include <SDL3/SDL.h>

// PUBLIC DATA ////////////////////////////////////////////////////////////////////////////////////
GlobalData* TheWritableGlobalData = NULL;				///< The global data singleton

//-------------------------------------------------------------------------------------------------
GlobalData* GlobalData::m_theOriginal = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE DATA ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*static*/ const FieldParse GlobalData::s_GlobalDataFieldParseTable[] = 
{
	{ "Windowed",									INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_windowed ) },
	{ "XResolution",							INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_xResolution ) },
	{ "YResolution",							INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_yResolution ) },
	{ "MapName",									INI::parseAsciiString,NULL,			offsetof( GlobalData::Data, m_mapName ) },
	{ "MoveHintName",							INI::parseAsciiString,NULL,			offsetof( GlobalData::Data, m_moveHintName ) },
	{ "UseTrees",									INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useTrees ) },
	{ "UseFPSLimit",							INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useFpsLimit ) },
	{ "DumpAssetUsage",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_dumpAssetUsage ) },
	{ "FramesPerSecondLimit",			INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_framesPerSecondLimit ) },
	{ "ChipsetType",							INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_chipSetType ) },
	{ "MaxShellScreens",					INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxShellScreens ) },
	{ "UseCloudMap",							INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useCloudMap ) },
	{ "UseLightMap",							INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useLightMap ) },
	{ "BilinearTerrainTex",				INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_bilinearTerrainTex ) },
	{ "TrilinearTerrainTex",			INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_trilinearTerrainTex ) },
	{ "MultiPassTerrain",					INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_multiPassTerrain ) },
	{ "AdjustCliffTextures",			INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_adjustCliffTextures ) },
	{ "Use3WayTerrainBlends",			INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_use3WayTerrainBlends ) },
	{ "StretchTerrain",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_stretchTerrain ) },
	{ "UseHalfHeightMap",					INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useHalfHeightMap ) },
	
	
	{ "DrawEntireTerrain",					INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_drawEntireTerrain ) },
	// FIXME: Fix me later!
	// { "TerrainLOD",									INI::parseIndexList,	TerrainLODNames,	offsetof( GlobalData::Data, m_terrainLOD ) },
	{ "TerrainLODTargetTimeMS",			INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_terrainLODTargetTimeMS ) },
	{ "RightMouseAlwaysScrolls",		INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_rightMouseAlwaysScrolls ) },
	{ "UseWaterPlane",							INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useWaterPlane ) },
	{ "UseCloudPlane",							INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useCloudPlane ) },
	{ "DownwindAngle",							INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_downwindAngle ) },
	{ "UseShadowVolumes",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useShadowVolumes ) },
	{ "UseShadowDecals",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useShadowDecals ) },
	{ "TextureReductionFactor",			INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_textureReductionFactor ) },
	{ "UseBehindBuildingMarker",		INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_enableBehindBuildingMarkers ) },
	{ "WaterPositionX",							INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_waterPositionX ) },
	{ "WaterPositionY",							INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_waterPositionY ) },
	{ "WaterPositionZ",							INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_waterPositionZ ) },
	{ "WaterExtentX",								INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_waterExtentX ) },
	{ "WaterExtentY",								INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_waterExtentY ) },
	{ "WaterType",									INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_waterType ) },
	{ "FeatherWater",						  	INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_featherWater ) },
	{ "ShowSoftWaterEdge",					INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_showSoftWaterEdge ) },

	// nasty ick, we need to save this data with a map and not hard code INI values
	{ "VertexWaterAvailableMaps1",		INI::parseAsciiString,	NULL,		offsetof( GlobalData::Data, m_vertexWaterAvailableMaps[ 0 ] ) },
	{ "VertexWaterHeightClampLow1",		INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterHeightClampLow[ 0 ] ) },
	{ "VertexWaterHeightClampHi1",		INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterHeightClampHi[ 0 ] ) },
	{ "VertexWaterAngle1",						INI::parseAngleReal,		NULL,		offsetof( GlobalData::Data, m_vertexWaterAngle[ 0 ] ) },
	{ "VertexWaterXPosition1",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterXPosition[ 0 ] ) },
	{ "VertexWaterYPosition1",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterYPosition[ 0 ] ) },
	{ "VertexWaterZPosition1",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterZPosition[ 0 ] ) },
	{ "VertexWaterXGridCells1",				INI::parseInt,					NULL,		offsetof( GlobalData::Data, m_vertexWaterXGridCells[ 0 ] ) },
	{ "VertexWaterYGridCells1",				INI::parseInt,					NULL,		offsetof( GlobalData::Data, m_vertexWaterYGridCells[ 0 ] ) },
	{ "VertexWaterGridSize1",					INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterGridSize[ 0 ] ) },
	{ "VertexWaterAttenuationA1",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationA[ 0 ] ) },
	{ "VertexWaterAttenuationB1",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationB[ 0 ] ) },
	{ "VertexWaterAttenuationC1",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationC[ 0 ] ) },
	{ "VertexWaterAttenuationRange1",	INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationRange[ 0 ] ) },

	// nasty ick, we need to save this data with a map and not hard code INI values
	{ "VertexWaterAvailableMaps2",		INI::parseAsciiString,	NULL,		offsetof( GlobalData::Data, m_vertexWaterAvailableMaps[ 1 ] ) },
	{ "VertexWaterHeightClampLow2",		INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterHeightClampLow[ 1 ] ) },
	{ "VertexWaterHeightClampHi2",		INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterHeightClampHi[ 1 ] ) },
	{ "VertexWaterAngle2",						INI::parseAngleReal,		NULL,		offsetof( GlobalData::Data, m_vertexWaterAngle[ 1 ] ) },
	{ "VertexWaterXPosition2",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterXPosition[ 1 ] ) },
	{ "VertexWaterYPosition2",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterYPosition[ 1 ] ) },
	{ "VertexWaterZPosition2",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterZPosition[ 1 ] ) },
	{ "VertexWaterXGridCells2",				INI::parseInt,					NULL,		offsetof( GlobalData::Data, m_vertexWaterXGridCells[ 1 ] ) },
	{ "VertexWaterYGridCells2",				INI::parseInt,					NULL,		offsetof( GlobalData::Data, m_vertexWaterYGridCells[ 1 ] ) },
	{ "VertexWaterGridSize2",					INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterGridSize[ 1 ] ) },
	{ "VertexWaterAttenuationA2",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationA[ 1 ] ) },
	{ "VertexWaterAttenuationB2",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationB[ 1 ] ) },
	{ "VertexWaterAttenuationC2",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationC[ 1 ] ) },
	{ "VertexWaterAttenuationRange2",	INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationRange[ 1 ] ) },

	// nasty ick, we need to save this data with a map and not hard code INI values
	{ "VertexWaterAvailableMaps3",		INI::parseAsciiString,	NULL,		offsetof( GlobalData::Data, m_vertexWaterAvailableMaps[ 2 ] ) },
	{ "VertexWaterHeightClampLow3",		INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterHeightClampLow[ 2 ] ) },
	{ "VertexWaterHeightClampHi3",		INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterHeightClampHi[ 2 ] ) },
	{ "VertexWaterAngle3",						INI::parseAngleReal,		NULL,		offsetof( GlobalData::Data, m_vertexWaterAngle[ 2 ] ) },
	{ "VertexWaterXPosition3",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterXPosition[ 2 ] ) },
	{ "VertexWaterYPosition3",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterYPosition[ 2 ] ) },
	{ "VertexWaterZPosition3",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterZPosition[ 2 ] ) },
	{ "VertexWaterXGridCells3",				INI::parseInt,					NULL,		offsetof( GlobalData::Data, m_vertexWaterXGridCells[ 2 ] ) },
	{ "VertexWaterYGridCells3",				INI::parseInt,					NULL,		offsetof( GlobalData::Data, m_vertexWaterYGridCells[ 2 ] ) },
	{ "VertexWaterGridSize3",					INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterGridSize[ 2 ] ) },
	{ "VertexWaterAttenuationA3",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationA[ 2 ] ) },
	{ "VertexWaterAttenuationB3",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationB[ 2 ] ) },
	{ "VertexWaterAttenuationC3",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationC[ 2 ] ) },
	{ "VertexWaterAttenuationRange3",	INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationRange[ 2 ] ) },

	// nasty ick, we need to save this data with a map and not hard code INI values
	{ "VertexWaterAvailableMaps4",		INI::parseAsciiString,	NULL,		offsetof( GlobalData::Data, m_vertexWaterAvailableMaps[ 3 ] ) },
	{ "VertexWaterHeightClampLow4",		INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterHeightClampLow[ 3 ] ) },
	{ "VertexWaterHeightClampHi4",		INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterHeightClampHi[ 3 ] ) },
	{ "VertexWaterAngle4",						INI::parseAngleReal,		NULL,		offsetof( GlobalData::Data, m_vertexWaterAngle[ 3 ] ) },
	{ "VertexWaterXPosition4",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterXPosition[ 3 ] ) },
	{ "VertexWaterYPosition4",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterYPosition[ 3 ] ) },
	{ "VertexWaterZPosition4",				INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterZPosition[ 3 ] ) },
	{ "VertexWaterXGridCells4",				INI::parseInt,					NULL,		offsetof( GlobalData::Data, m_vertexWaterXGridCells[ 3 ] ) },
	{ "VertexWaterYGridCells4",				INI::parseInt,					NULL,		offsetof( GlobalData::Data, m_vertexWaterYGridCells[ 3 ] ) },
	{ "VertexWaterGridSize4",					INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterGridSize[ 3 ] ) },
	{ "VertexWaterAttenuationA4",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationA[ 3 ] ) },
	{ "VertexWaterAttenuationB4",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationB[ 3 ] ) },
	{ "VertexWaterAttenuationC4",			INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationC[ 3 ] ) },
	{ "VertexWaterAttenuationRange4",	INI::parseReal,					NULL,		offsetof( GlobalData::Data, m_vertexWaterAttenuationRange[ 3 ] ) },

	{ "SkyBoxPositionZ",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_skyBoxPositionZ ) },
	{ "SkyBoxScale",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_skyBoxScale ) },
	{ "DrawSkyBox",				INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_drawSkyBox ) },
	{ "CameraPitch",								INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_cameraPitch ) },
	{ "CameraYaw",									INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_cameraYaw ) },
	{ "CameraHeight",								INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_cameraHeight ) },
	{ "MaxCameraHeight",						INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_maxCameraHeight ) },
	{ "MinCameraHeight",						INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_minCameraHeight ) },
	{ "TerrainHeightAtEdgeOfMap",					INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_terrainHeightAtEdgeOfMap ) },
	{ "UnitDamagedThreshold",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_unitDamagedThresh ) },
	{ "UnitReallyDamagedThreshold",	INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_unitReallyDamagedThresh ) },
	{ "GroundStiffness",					INI::parseReal,				NULL,				offsetof( GlobalData::Data, m_groundStiffness ) },
	{ "StructureStiffness",					INI::parseReal,				NULL,				offsetof( GlobalData::Data, m_structureStiffness ) },
	{ "Gravity",									INI::parseAccelerationReal,				NULL,				offsetof( GlobalData::Data, m_gravity ) },
	{ "StealthFriendlyOpacity",		INI::parsePercentToReal,				NULL,				offsetof( GlobalData::Data, m_stealthFriendlyOpacity ) },
	{ "DefaultOcclusionDelay",				INI::parseDurationUnsignedInt,				NULL,			offsetof( GlobalData::Data, m_defaultOcclusionDelay ) },
	
	{ "PartitionCellSize",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_partitionCellSize ) },

	{ "AmmoPipScaleFactor",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_ammoPipScaleFactor ) },
	{ "ContainerPipScaleFactor",	INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_containerPipScaleFactor ) },
	{ "AmmoPipWorldOffset",						INI::parseCoord3D,				NULL,			offsetof( GlobalData::Data, m_ammoPipWorldOffset ) },
	{ "ContainerPipWorldOffset",				INI::parseCoord3D,				NULL,			offsetof( GlobalData::Data, m_containerPipWorldOffset ) },
	{ "AmmoPipScreenOffset",						INI::parseCoord2D,				NULL,			offsetof( GlobalData::Data, m_ammoPipScreenOffset ) },
	{ "ContainerPipScreenOffset",				INI::parseCoord2D,				NULL,			offsetof( GlobalData::Data, m_containerPipScreenOffset ) },

	{ "HistoricDamageLimit",				INI::parseDurationUnsignedInt,				NULL,			offsetof( GlobalData::Data, m_historicDamageLimit ) },

	{ "MaxTerrainTracks",					INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxTerrainTracks ) },
	{ "TimeOfDay",								INI::parseIndexList,	TimeOfDayNames,			offsetof( GlobalData::Data, m_timeOfDay ) },
	{ "Weather",									INI::parseIndexList,	WeatherNames,			offsetof( GlobalData::Data, m_weather ) },
	{ "MakeTrackMarks",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_makeTrackMarks ) },
	{ "HideGarrisonFlags",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_hideGarrisonFlags ) },
	{ "ForceModelsToFollowTimeOfDay",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_forceModelsToFollowTimeOfDay ) },
	{ "ForceModelsToFollowWeather",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_forceModelsToFollowWeather ) },

	{ "LevelGainAnimationName",		INI::parseAsciiString,	NULL,	offsetof( GlobalData::Data, m_levelGainAnimationName ) },
	{ "LevelGainAnimationTime",		INI::parseReal,					NULL, offsetof( GlobalData::Data, m_levelGainAnimationDisplayTimeInSeconds ) },
	{ "LevelGainAnimationZRise",	INI::parseReal,					NULL, offsetof( GlobalData::Data, m_levelGainAnimationZRisePerSecond ) },

	{ "GetHealedAnimationName",		INI::parseAsciiString,	NULL,	offsetof( GlobalData::Data, m_getHealedAnimationName ) },
	{ "GetHealedAnimationTime",		INI::parseReal,					NULL, offsetof( GlobalData::Data, m_getHealedAnimationDisplayTimeInSeconds ) },
	{ "GetHealedAnimationZRise",	INI::parseReal,					NULL, offsetof( GlobalData::Data, m_getHealedAnimationZRisePerSecond ) },

	{ "TerrainLightingMorningAmbient",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_MORNING ][0].ambient ) },
	{ "TerrainLightingMorningDiffuse",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_MORNING ][0].diffuse ) },
	{ "TerrainLightingMorningLightPos",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_MORNING ][0].lightPos ) },
	{ "TerrainLightingAfternoonAmbient",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_AFTERNOON ][0].ambient ) },
	{ "TerrainLightingAfternoonDiffuse",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_AFTERNOON ][0].diffuse ) },
	{ "TerrainLightingAfternoonLightPos",	INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_AFTERNOON ][0].lightPos ) },
	{ "TerrainLightingEveningAmbient",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_EVENING ][0].ambient ) },
	{ "TerrainLightingEveningDiffuse",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_EVENING ][0].diffuse ) },
	{ "TerrainLightingEveningLightPos",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_EVENING ][0].lightPos ) },
	{ "TerrainLightingNightAmbient",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_NIGHT ][0].ambient ) },
	{ "TerrainLightingNightDiffuse",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_NIGHT ][0].diffuse ) },
	{ "TerrainLightingNightLightPos",			INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_NIGHT ][0].lightPos ) },

	{ "TerrainObjectsLightingMorningAmbient",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_MORNING ][0].ambient ) },
	{ "TerrainObjectsLightingMorningDiffuse",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_MORNING ][0].diffuse ) },
	{ "TerrainObjectsLightingMorningLightPos",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_MORNING ][0].lightPos ) },
	{ "TerrainObjectsLightingAfternoonAmbient",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_AFTERNOON ][0].ambient ) },
	{ "TerrainObjectsLightingAfternoonDiffuse",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_AFTERNOON ][0].diffuse ) },
	{ "TerrainObjectsLightingAfternoonLightPos",	INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_AFTERNOON ][0].lightPos ) },
	{ "TerrainObjectsLightingEveningAmbient",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_EVENING ][0].ambient ) },
	{ "TerrainObjectsLightingEveningDiffuse",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_EVENING ][0].diffuse ) },
	{ "TerrainObjectsLightingEveningLightPos",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_EVENING ][0].lightPos ) },
	{ "TerrainObjectsLightingNightAmbient",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_NIGHT ][0].ambient ) },
	{ "TerrainObjectsLightingNightDiffuse",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_NIGHT ][0].diffuse ) },
	{ "TerrainObjectsLightingNightLightPos",			INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_NIGHT ][0].lightPos ) },

	//Secondary global light	
	{ "TerrainLightingMorningAmbient2",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_MORNING ][1].ambient ) },
	{ "TerrainLightingMorningDiffuse2",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_MORNING ][1].diffuse ) },
	{ "TerrainLightingMorningLightPos2",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_MORNING ][1].lightPos ) },
	{ "TerrainLightingAfternoonAmbient2",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_AFTERNOON ][1].ambient ) },
	{ "TerrainLightingAfternoonDiffuse2",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_AFTERNOON ][1].diffuse ) },
	{ "TerrainLightingAfternoonLightPos2",	INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_AFTERNOON ][1].lightPos ) },
	{ "TerrainLightingEveningAmbient2",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_EVENING ][1].ambient ) },
	{ "TerrainLightingEveningDiffuse2",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_EVENING ][1].diffuse ) },
	{ "TerrainLightingEveningLightPos2",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_EVENING ][1].lightPos ) },
	{ "TerrainLightingNightAmbient2",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_NIGHT ][1].ambient ) },
	{ "TerrainLightingNightDiffuse2",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_NIGHT ][1].diffuse ) },
	{ "TerrainLightingNightLightPos2",			INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_NIGHT ][1].lightPos ) },

	{ "TerrainObjectsLightingMorningAmbient2",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_MORNING ][1].ambient ) },
	{ "TerrainObjectsLightingMorningDiffuse2",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_MORNING ][1].diffuse ) },
	{ "TerrainObjectsLightingMorningLightPos2",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_MORNING ][1].lightPos ) },
	{ "TerrainObjectsLightingAfternoonAmbient2",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_AFTERNOON ][1].ambient ) },
	{ "TerrainObjectsLightingAfternoonDiffuse2",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_AFTERNOON ][1].diffuse ) },
	{ "TerrainObjectsLightingAfternoonLightPos2",	INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_AFTERNOON ][1].lightPos ) },
	{ "TerrainObjectsLightingEveningAmbient2",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_EVENING ][1].ambient ) },
	{ "TerrainObjectsLightingEveningDiffuse2",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_EVENING ][1].diffuse ) },
	{ "TerrainObjectsLightingEveningLightPos2",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_EVENING ][1].lightPos ) },
	{ "TerrainObjectsLightingNightAmbient2",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_NIGHT ][1].ambient ) },
	{ "TerrainObjectsLightingNightDiffuse2",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_NIGHT ][1].diffuse ) },
	{ "TerrainObjectsLightingNightLightPos2",			INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_NIGHT ][1].lightPos ) },

	//Third global light
	{ "TerrainLightingMorningAmbient3",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_MORNING ][2].ambient ) },
	{ "TerrainLightingMorningDiffuse3",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_MORNING ][2].diffuse ) },
	{ "TerrainLightingMorningLightPos3",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_MORNING ][2].lightPos ) },
	{ "TerrainLightingAfternoonAmbient3",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_AFTERNOON ][2].ambient ) },
	{ "TerrainLightingAfternoonDiffuse3",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_AFTERNOON ][2].diffuse ) },
	{ "TerrainLightingAfternoonLightPos3",	INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_AFTERNOON ][2].lightPos ) },
	{ "TerrainLightingEveningAmbient3",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_EVENING ][2].ambient ) },
	{ "TerrainLightingEveningDiffuse3",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_EVENING ][2].diffuse ) },
	{ "TerrainLightingEveningLightPos3",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_EVENING ][2].lightPos ) },
	{ "TerrainLightingNightAmbient3",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_NIGHT ][2].ambient ) },
	{ "TerrainLightingNightDiffuse3",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_NIGHT ][2].diffuse ) },
	{ "TerrainLightingNightLightPos3",			INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainLighting[ TIME_OF_DAY_NIGHT ][2].lightPos ) },

	{ "TerrainObjectsLightingMorningAmbient3",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_MORNING ][2].ambient ) },
	{ "TerrainObjectsLightingMorningDiffuse3",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_MORNING ][2].diffuse ) },
	{ "TerrainObjectsLightingMorningLightPos3",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_MORNING ][2].lightPos ) },
	{ "TerrainObjectsLightingAfternoonAmbient3",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_AFTERNOON ][2].ambient ) },
	{ "TerrainObjectsLightingAfternoonDiffuse3",		INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_AFTERNOON ][2].diffuse ) },
	{ "TerrainObjectsLightingAfternoonLightPos3",	INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_AFTERNOON ][2].lightPos ) },
	{ "TerrainObjectsLightingEveningAmbient3",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_EVENING ][2].ambient ) },
	{ "TerrainObjectsLightingEveningDiffuse3",			INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_EVENING ][2].diffuse ) },
	{ "TerrainObjectsLightingEveningLightPos3",		INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_EVENING ][2].lightPos ) },
	{ "TerrainObjectsLightingNightAmbient3",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_NIGHT ][2].ambient ) },
	{ "TerrainObjectsLightingNightDiffuse3",				INI::parseRGBColor,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_NIGHT ][2].diffuse ) },
	{ "TerrainObjectsLightingNightLightPos3",			INI::parseCoord3D,			NULL,			offsetof( GlobalData::Data, m_terrainObjectsLighting[ TIME_OF_DAY_NIGHT ][2].lightPos ) },

	
	{ "NumberGlobalLights",								INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_numGlobalLights)},
	{ "InfantryLightMorningScale",				INI::parseReal,			NULL,			offsetof( GlobalData::Data, m_infantryLightScale[TIME_OF_DAY_MORNING] ) },
	{ "InfantryLightAfternoonScale",				INI::parseReal,			NULL,			offsetof( GlobalData::Data, m_infantryLightScale[TIME_OF_DAY_AFTERNOON] ) },
	{ "InfantryLightEveningScale",				INI::parseReal,			NULL,			offsetof( GlobalData::Data, m_infantryLightScale[TIME_OF_DAY_EVENING] ) },
	{ "InfantryLightNightScale",				INI::parseReal,			NULL,			offsetof( GlobalData::Data, m_infantryLightScale[TIME_OF_DAY_NIGHT] ) },

	{ "MaxTranslucentObjects",						INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxVisibleTranslucentObjects) },
	{ "OccludedColorLuminanceScale",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_occludedLuminanceScale) },

/* These are internal use only, they do not need file definitons 
	{ "TerrainAmbientRGB",				INI::parseRGBColor,		NULL,			offsetof( GlobalData::Data, m_terrainAmbient ) },
	{ "TerrainDiffuseRGB",				INI::parseRGBColor,		NULL,			offsetof( GlobalData::Data, m_terrainDiffuse ) },
	{ "TerrainLightPos",					INI::parseCoord3D,		NULL,			offsetof( GlobalData::Data, m_terrainLightPos ) },
*/
	{ "MaxRoadSegments",						INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxRoadSegments ) },
	{ "MaxRoadVertex",							INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxRoadVertex ) },
	{ "MaxRoadIndex",								INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxRoadIndex ) },
	{ "MaxRoadTypes",								INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxRoadTypes ) },

	{ "ValuePerSupplyBox",					INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_baseValuePerSupplyBox ) },
	
	{ "AudioOn",										INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_audioOn ) },
	{ "MusicOn",										INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_musicOn ) },
	{ "SoundsOn",										INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_soundsOn ) },
	{ "Sounds3DOn",									INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_sounds3DOn ) },
	{ "SpeechOn",										INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_speechOn ) },
	{ "VideoOn",										INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_videoOn ) },
	{ "DisableCameraMovements",			INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_disableCameraMovement ) },

/* These are internal use only, they do not need file definitons 		
	/// @todo remove this hack
	{ "InGame",											INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_inGame ) },
*/

	{ "DebugAI",										INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_debugAI ) },
	{ "DebugAIObstacles",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_debugAIObstacles ) },
	{ "ShowClientPhysics",				INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_showClientPhysics ) },
	{ "ShowTerrainNormals",				INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_showTerrainNormals ) },
	{ "ShowObjectHealth",						INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_showObjectHealth ) },

	{ "ParticleScale",										INI::parseReal,					NULL,	 offsetof( GlobalData::Data, m_particleScale ) },
	{ "AutoFireParticleSmallPrefix",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoFireParticleSmallPrefix ) },
	{ "AutoFireParticleSmallSystem",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoFireParticleSmallSystem ) },
	{ "AutoFireParticleSmallMax",					INI::parseInt,					NULL,	 offsetof( GlobalData::Data, m_autoFireParticleSmallMax ) },
	{ "AutoFireParticleMediumPrefix",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoFireParticleMediumPrefix ) },
	{ "AutoFireParticleMediumSystem",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoFireParticleMediumSystem ) },
	{ "AutoFireParticleMediumMax",				INI::parseInt,					NULL,	 offsetof( GlobalData::Data, m_autoFireParticleMediumMax ) },
	{ "AutoFireParticleLargePrefix",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoFireParticleLargePrefix ) },
	{ "AutoFireParticleLargeSystem",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoFireParticleLargeSystem ) },
	{ "AutoFireParticleLargeMax",					INI::parseInt,					NULL,	 offsetof( GlobalData::Data, m_autoFireParticleLargeMax ) },
	{ "AutoSmokeParticleSmallPrefix",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoSmokeParticleSmallPrefix ) },
	{ "AutoSmokeParticleSmallSystem",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoSmokeParticleSmallSystem ) },
	{ "AutoSmokeParticleSmallMax",				INI::parseInt,					NULL,	 offsetof( GlobalData::Data, m_autoSmokeParticleSmallMax ) },
	{ "AutoSmokeParticleMediumPrefix",		INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoSmokeParticleMediumPrefix ) },
	{ "AutoSmokeParticleMediumSystem",		INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoSmokeParticleMediumSystem ) },
	{ "AutoSmokeParticleMediumMax",				INI::parseInt,					NULL,	 offsetof( GlobalData::Data, m_autoSmokeParticleMediumMax ) },
	{ "AutoSmokeParticleLargePrefix",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoSmokeParticleLargePrefix ) },
	{ "AutoSmokeParticleLargeSystem",			INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoSmokeParticleLargeSystem ) },
	{ "AutoSmokeParticleLargeMax",				INI::parseInt,					NULL,	 offsetof( GlobalData::Data, m_autoSmokeParticleLargeMax ) },
	{ "AutoAflameParticlePrefix",					INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoAflameParticlePrefix ) },
	{ "AutoAflameParticleSystem",					INI::parseAsciiString,  NULL,  offsetof( GlobalData::Data, m_autoAflameParticleSystem ) },
	{ "AutoAflameParticleMax",						INI::parseInt,					NULL,	 offsetof( GlobalData::Data, m_autoAflameParticleMax ) },

/* These are internal use only, they do not need file definitons 	
	{ "LatencyAverage",							INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_latencyAverage ) },
	{ "LatencyAmplitude",						INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_latencyAmplitude ) },
	{ "LatencyPeriod",							INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_latencyPeriod ) },
	{ "LatencyNoise",								INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_latencyNoise ) },
	{ "PacketLoss",									INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_packetLoss ) },
*/

	{ "BuildSpeed",									INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_BuildSpeed ) },
	{ "MinDistFromEdgeOfMapForBuild",	 INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_MinDistFromEdgeOfMapForBuild ) },
	{ "SupplyBuildBorder",	 INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_SupplyBuildBorder ) },
	{ "AllowedHeightVariationForBuilding", INI::parseReal,NULL,			offsetof( GlobalData::Data, m_allowedHeightVariationForBuilding ) },
	{ "MinLowEnergyProductionSpeed",INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_MinLowEnergyProductionSpeed ) },
	{ "MaxLowEnergyProductionSpeed",INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_MaxLowEnergyProductionSpeed ) },
	{ "LowEnergyPenaltyModifier",		INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_LowEnergyPenaltyModifier ) },
	{ "MultipleFactory",						INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_MultipleFactory ) },
	{ "RefundPercent",							INI::parsePercentToReal,			NULL,			offsetof( GlobalData::Data, m_RefundPercent ) },

	{ "CommandCenterHealRange",			INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_commandCenterHealRange ) },
	{ "CommandCenterHealAmount",		INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_commandCenterHealAmount ) },

	{ "StandardMinefieldDensity",		INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_standardMinefieldDensity ) },
	{ "StandardMinefieldDistance",		INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_standardMinefieldDistance ) },

	{ "MaxLineBuildObjects",				INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxLineBuildObjects ) },
	{ "MaxTunnelCapacity",					INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxTunnelCapacity ) },

	{ "MaxParticleCount",						INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxParticleCount ) },
	{ "MaxFieldParticleCount",						INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxFieldParticleCount ) },
	{ "HorizontalScrollSpeedFactor",INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_horizontalScrollSpeedFactor ) },
	{ "VerticalScrollSpeedFactor",	INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_verticalScrollSpeedFactor ) },
	{ "ScrollAmountCutoff",					INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_scrollAmountCutoff ) },
	{ "CameraAdjustSpeed",					INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_cameraAdjustSpeed ) },
	{ "EnforceMaxCameraHeight",			INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_enforceMaxCameraHeight ) },
	{ "KeyboardScrollSpeedFactor",	INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_keyboardScrollFactor ) },
	{ "KeyboardDefaultScrollSpeedFactor",	INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_keyboardDefaultScrollFactor ) },
	// FIXME: Fix me later!
	// { "MovementPenaltyDamageState",	INI::parseIndexList,	TheBodyDamageTypeNames,	 offsetof( GlobalData::Data, m_movementPenaltyDamageState ) },

// you cannot set this; it always has a value of 100%.
//{ "HealthBonus_Regular",				INI::parsePercentToReal, NULL,	offsetof( GlobalData, m_healthBonus[LEVEL_REGULAR]) },
	{ "HealthBonus_Veteran",				INI::parsePercentToReal, NULL,	offsetof( GlobalData::Data, m_healthBonus[LEVEL_VETERAN]) },
	{ "HealthBonus_Elite",					INI::parsePercentToReal, NULL,	offsetof( GlobalData::Data, m_healthBonus[LEVEL_ELITE]) },
	{ "HealthBonus_Heroic",					INI::parsePercentToReal, NULL,	offsetof( GlobalData::Data, m_healthBonus[LEVEL_HEROIC]) },

	{ "HumanSoloPlayerHealthBonus_Easy",					INI::parsePercentToReal,			NULL,			offsetof( GlobalData::Data, m_soloPlayerHealthBonusForDifficulty[PLAYER_HUMAN][DIFFICULTY_EASY] ) },
	{ "HumanSoloPlayerHealthBonus_Normal",				INI::parsePercentToReal,			NULL,			offsetof( GlobalData::Data, m_soloPlayerHealthBonusForDifficulty[PLAYER_HUMAN][DIFFICULTY_NORMAL] ) },
	{ "HumanSoloPlayerHealthBonus_Hard",				INI::parsePercentToReal,			NULL,			offsetof( GlobalData::Data, m_soloPlayerHealthBonusForDifficulty[PLAYER_HUMAN][DIFFICULTY_HARD] ) },

	{ "AISoloPlayerHealthBonus_Easy",					INI::parsePercentToReal,			NULL,			offsetof( GlobalData::Data, m_soloPlayerHealthBonusForDifficulty[PLAYER_COMPUTER][DIFFICULTY_EASY] ) },
	{ "AISoloPlayerHealthBonus_Normal",				INI::parsePercentToReal,			NULL,			offsetof( GlobalData::Data, m_soloPlayerHealthBonusForDifficulty[PLAYER_COMPUTER][DIFFICULTY_NORMAL] ) },
	{ "AISoloPlayerHealthBonus_Hard",				INI::parsePercentToReal,			NULL,			offsetof( GlobalData::Data, m_soloPlayerHealthBonusForDifficulty[PLAYER_COMPUTER][DIFFICULTY_HARD] ) },

	{ "WeaponBonus",								WeaponBonusSet::parseWeaponBonusSetPtr,	NULL,	offsetof( GlobalData::Data, m_weaponBonusSet ) },

	{ "DefaultStructureRubbleHeight",	INI::parseReal,			NULL,			offsetof( GlobalData::Data, m_defaultStructureRubbleHeight ) },

	{ "FixedSeed",									INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_fixedSeed ) },

	{ "ShellMapName",								INI::parseAsciiString,NULL,			offsetof( GlobalData::Data, m_shellMapName ) },
	{ "ShellMapOn",									INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_shellMapOn ) },
	{	"PlayIntro",									INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_playIntro ) },

	{ "FirewallBehavior",						INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_firewallBehavior ) },
	{ "FirewallPortOverride",				INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_firewallPortOverride ) },
	{	"FirewallPortAllocationDelta",INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_firewallPortAllocationDelta) },

	{	"GroupSelectMinSelectSize",		INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_groupSelectMinSelectSize ) },
	{	"GroupSelectVolumeBase",			INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_groupSelectVolumeBase ) },
	{	"GroupSelectVolumeIncrement",	INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_groupSelectVolumeIncrement ) },
	{	"MaxUnitSelectSounds",				INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_maxUnitSelectSounds ) },

	{	"SelectionFlashSaturationFactor",	INI::parseReal,		NULL,			offsetof( GlobalData::Data, m_selectionFlashSaturationFactor ) },
	{	"SelectionFlashHouseColor",	      INI::parseBool,		NULL,			offsetof( GlobalData::Data, m_selectionFlashHouseColor ) },

	{	"CameraAudibleRadius",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_cameraAudibleRadius ) },
	{ "GroupMoveClickToGatherAreaFactor", INI::parseReal,	NULL,			offsetof( GlobalData::Data, m_groupMoveClickToGatherFactor ) },
	{ "ShakeSubtleIntensity",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_shakeSubtleIntensity ) },
	{ "ShakeNormalIntensity",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_shakeNormalIntensity ) },
	{ "ShakeStrongIntensity",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_shakeStrongIntensity ) },
	{ "ShakeSevereIntensity",				INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_shakeSevereIntensity ) },
	{ "ShakeCineExtremeIntensity",	INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_shakeCineExtremeIntensity ) },
	{ "ShakeCineInsaneIntensity",		INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_shakeCineInsaneIntensity ) },
	{ "MaxShakeIntensity",					INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_maxShakeIntensity ) },
	{ "MaxShakeRange",							INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_maxShakeRange) },
	{ "SellPercentage",							INI::parsePercentToReal,	NULL,			offsetof( GlobalData::Data, m_sellPercentage ) },
	{ "BaseRegenHealthPercentPerSecond", INI::parsePercentToReal, NULL,	offsetof( GlobalData::Data, m_baseRegenHealthPercentPerSecond ) },
	{ "BaseRegenDelay",							INI::parseDurationUnsignedInt, NULL,offsetof( GlobalData::Data, m_baseRegenDelay ) },

#ifdef ALLOW_SURRENDER
	{ "PrisonBountyMultiplier",			INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_prisonBountyMultiplier ) },
	{ "PrisonBountyTextColor",			INI::parseColorInt,		NULL,			offsetof( GlobalData::Data, m_prisonBountyTextColor ) },
#endif

	{ "SpecialPowerViewObject",			INI::parseAsciiString,	NULL,			offsetof( GlobalData::Data, m_specialPowerViewObjectName ) },

	{ "StandardPublicBone", INI::parseAsciiStringVectorAppend, NULL, offsetof(GlobalData::Data, m_standardPublicBones) },
	{ "ShowMetrics",								INI::parseBool,				   NULL,		offsetof( GlobalData::Data, m_showMetrics ) },
	{ "DefaultStartingCash",				GlobalData::parseDefaultStartingCash, NULL,		0 },

// NOTE: m_doubleClickTimeMS is still in use, but we disallow setting it from the GameData.ini file. It is now set in the constructor according to the windows parameter.
//	{ "DoubleClickTimeMS",									INI::parseUnsignedInt,			NULL, offsetof( GlobalData::Data, m_doubleClickTimeMS ) },
	
	{ "ShroudColor",		INI::parseRGBColor,						NULL,	offsetof( GlobalData::Data, m_shroudColor) },
	{ "ClearAlpha",			INI::parseUnsignedByte,				NULL,	offsetof( GlobalData::Data, m_clearAlpha) },
	{ "FogAlpha",				INI::parseUnsignedByte,				NULL,	offsetof( GlobalData::Data, m_fogAlpha) },
	{ "ShroudAlpha",		INI::parseUnsignedByte,				NULL,	offsetof( GlobalData::Data, m_shroudAlpha) },

	{ "HotKeyTextColor",										INI::parseColorInt,					NULL,	offsetof( GlobalData::Data, m_hotKeyTextColor ) },

	{ "PowerBarBase",												INI::parseInt,							NULL,	offsetof( GlobalData::Data, m_powerBarBase) },
	{ "PowerBarIntervals",									INI::parseReal,							NULL,	offsetof( GlobalData::Data, m_powerBarIntervals) },
	{ "PowerBarYellowRange",								INI::parseInt,							NULL,	offsetof( GlobalData::Data, m_powerBarYellowRange) },
	{ "UnlookPersistDuration",							INI::parseDurationUnsignedInt, NULL, offsetof( GlobalData::Data, m_unlookPersistDuration) },

	{ "NetworkFPSHistoryLength", INI::parseInt, NULL, offsetof(GlobalData::Data, m_networkFPSHistoryLength) },
	{ "NetworkLatencyHistoryLength", INI::parseInt, NULL, offsetof(GlobalData::Data, m_networkLatencyHistoryLength) },
	{ "NetworkRunAheadMetricsTime", INI::parseInt, NULL, offsetof(GlobalData::Data, m_networkRunAheadMetricsTime) },
	{ "NetworkCushionHistoryLength", INI::parseInt, NULL, offsetof(GlobalData::Data, m_networkCushionHistoryLength) },
	{ "NetworkRunAheadSlack", INI::parseInt, NULL, offsetof(GlobalData::Data, m_networkRunAheadSlack) },
	{ "NetworkKeepAliveDelay", INI::parseInt, NULL, offsetof(GlobalData::Data, m_networkKeepAliveDelay) },
	{ "NetworkDisconnectTime", INI::parseInt, NULL, offsetof(GlobalData::Data, m_networkDisconnectTime) },
	{ "NetworkPlayerTimeoutTime", INI::parseInt, NULL, offsetof(GlobalData::Data, m_networkPlayerTimeoutTime) },
	{ "NetworkDisconnectScreenNotifyTime", INI::parseInt, NULL, offsetof(GlobalData::Data, m_networkDisconnectScreenNotifyTime) },
	
	{ "KeyboardCameraRotateSpeed", INI::parseReal, NULL, offsetof( GlobalData::Data, m_keyboardCameraRotateSpeed ) },
	{ "PlayStats",									INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_playStats ) },

#if defined(_DEBUG) || defined(_INTERNAL)
	{ "DisableCameraFade",			INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_disableCameraFade ) },
	{ "DisableScriptedInputDisabling",			INI::parseBool,		NULL,			offsetof( GlobalData::Data, m_disableScriptedInputDisabling ) },
	{ "DisableMilitaryCaption",			INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_disableMilitaryCaption ) },
	{ "BenchmarkTimer",			INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_benchmarkTimer ) },
	{ "CheckMemoryLeaks", INI::parseBool, NULL, offsetof(GlobalData::Data, m_checkForLeaks) },
	{ "Wireframe",								INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_wireframe ) },
	{ "StateMachineDebug",				INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_stateMachineDebug ) },
	{ "UseCameraConstraints",				INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useCameraConstraints ) },
	{ "ShroudOn",										INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_shroudOn ) },
	{ "FogOfWarOn",										INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_fogOfWarOn ) },
	{ "ShowCollisionExtents",				INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_showCollisionExtents ) },
  { "ShowAudioLocations",  				INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_showAudioLocations ) },
	{ "DebugProjectileTileWidth",		INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_debugProjectileTileWidth) },
	{ "DebugProjectileTileDuration",INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_debugProjectileTileDuration) },
	{ "DebugProjectileTileColor",		INI::parseRGBColor,		NULL,			offsetof( GlobalData::Data, m_debugProjectileTileColor) },
	{ "DebugVisibilityTileCount",		INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_debugVisibilityTileCount) },
	{ "DebugVisibilityTileWidth",		INI::parseReal,				NULL,			offsetof( GlobalData::Data, m_debugVisibilityTileWidth) },
	{ "DebugVisibilityTileDuration",INI::parseInt,				NULL,			offsetof( GlobalData::Data, m_debugVisibilityTileDuration) },
	{ "DebugVisibilityTileTargettableColor",INI::parseRGBColor, NULL,	offsetof( GlobalData::Data, m_debugVisibilityTargettableColor) },
	{ "DebugVisibilityTileDeshroudColor",		INI::parseRGBColor,	NULL,	offsetof( GlobalData::Data, m_debugVisibilityDeshroudColor) },
	{ "DebugVisibilityTileGapColor",				INI::parseRGBColor,	NULL,	offsetof( GlobalData::Data, m_debugVisibilityGapColor) },
	{ "DebugThreatMapTileDuration",					INI::parseInt,							NULL,	offsetof( GlobalData::Data, m_debugThreatMapTileDuration) },
	{ "MaxDebugThreatMapValue",							INI::parseUnsignedInt,			NULL,	offsetof( GlobalData::Data, m_maxDebugThreat) },
	{ "DebugCashValueMapTileDuration",			INI::parseInt,							NULL,	offsetof( GlobalData::Data, m_debugCashValueMapTileDuration) },
	{ "MaxDebugCashValueMapValue",					INI::parseUnsignedInt,			NULL,	offsetof( GlobalData::Data, m_maxDebugValue) },
	{ "VTune", INI::parseBool,	NULL,			offsetof( GlobalData::Data, m_vTune ) },
	{ "SaveStats",									INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_saveStats ) },
	{ "UseLocalMOTD",								INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_useLocalMOTD ) },
	{ "BaseStatsDir",								INI::parseAsciiString,NULL,			offsetof( GlobalData::Data, m_baseStatsDir ) },
	{ "LocalMOTDPath",							INI::parseAsciiString,NULL,			offsetof( GlobalData::Data, m_MOTDPath ) },
	{ "ExtraLogging",								INI::parseBool,				NULL,			offsetof( GlobalData::Data, m_extraLogging ) },
#endif

	{ NULL,					NULL,						NULL,						0 }  // keep this last

};


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
GlobalData::GlobalData()
{
	Int i, j;

	//
	// we have now instanced a global data instance, if theOriginal is NULL, this is
	// *the* very first instance and it shall be recorded.  This way, when we load 
	// overrides of the global data, we can revert to the common, original data
	// in m_theOriginal
	//
	if( m_theOriginal == NULL )
		m_theOriginal = this;
	m_next = NULL;

#if defined(_DEBUG) || defined(_INTERNAL) || defined(_ALLOW_DEBUG_CHEATS_IN_RELEASE)
	m_data.m_specialPowerUsesDelay = TRUE;
#endif
	m_data.m_TiVOFastMode = FALSE;

#if defined(_DEBUG) || defined(_INTERNAL)
	m_data.m_wireframe = 0;
	m_data.m_stateMachineDebug = FALSE;
	m_data.m_useCameraConstraints = TRUE;
	m_data.m_shroudOn = TRUE;
	m_data.m_fogOfWarOn = FALSE;
	m_data.m_jabberOn = FALSE;
	m_data.m_munkeeOn = FALSE;
	m_data.m_showCollisionExtents = FALSE;
	m_data.m_showAudioLocations = FALSE;
	m_data.m_debugCamera = FALSE;
	m_data.m_debugVisibility = FALSE;
	m_data.m_debugVisibilityTileCount = 32;	// default to 32.
	m_data.m_debugVisibilityTileDuration = LOGICFRAMES_PER_SECOND;
	m_data.m_debugProjectilePath = FALSE;
	m_data.m_debugProjectileTileWidth = 10;
	m_data.m_debugProjectileTileDuration = LOGICFRAMES_PER_SECOND;  // Changed By Sadullah Nader
	m_data.m_debugThreatMap = FALSE;
	m_data.m_maxDebugThreat = 5000;
	m_data.m_debugThreatMapTileDuration = LOGICFRAMES_PER_SECOND;  // Changed By Sadullah Nader
	m_data.m_debugCashValueMap = FALSE;
	m_data.m_maxDebugValue = 10000;
	m_data.m_debugCashValueMapTileDuration = LOGICFRAMES_PER_SECOND; // Changed By Sadullah Nader
	m_data.m_debugIgnoreAsserts = FALSE;
	m_data.m_debugIgnoreStackTrace = FALSE;
	m_data.m_vTune = false;
	m_data.m_checkForLeaks = TRUE;
	m_data.m_benchmarkTimer = -1;

	m_data.m_allowUnselectableSelection = FALSE;
	m_data.m_disableCameraFade = false;
	m_data.m_disableScriptedInputDisabling = false;
	m_data.m_disableMilitaryCaption = false;
	m_data.m_latencyAverage = 0;
	m_data.m_latencyAmplitude = 0;
	m_data.m_latencyPeriod = 0;
	m_data.m_latencyNoise = 0;
	m_data.m_packetLoss = 0;
	m_data.m_saveStats = FALSE;
	m_data.m_saveAllStats = FALSE;
	m_data.m_useLocalMOTD = FALSE;
	m_data.m_baseStatsDir = ".\\";
	m_data.m_MOTDPath = "MOTD.txt";
	m_data.m_extraLogging = FALSE;
#endif

	m_data.m_playStats = -1;
	m_data.m_incrementalAGPBuf = FALSE;
	m_data.m_mapName.clear();
	m_data.m_moveHintName.clear();
	m_data.m_useTrees = 0;
	m_data.m_useTreeSway = TRUE;
	m_data.m_useDrawModuleLOD = FALSE;
	m_data.m_useHeatEffects = TRUE;
	m_data.m_useFpsLimit = FALSE;
	m_data.m_dumpAssetUsage = FALSE;
	m_data.m_framesPerSecondLimit = 0;
	m_data.m_chipSetType = 0;
	m_data.m_windowed = 0;
	m_data.m_xResolution = 800;
	m_data.m_yResolution = 600;
	m_data.m_maxShellScreens = 0;
	m_data.m_useCloudMap = FALSE;
	m_data.m_use3WayTerrainBlends = 1;
	m_data.m_useLightMap = FALSE;
	m_data.m_bilinearTerrainTex = FALSE;
	m_data.m_trilinearTerrainTex = FALSE;
	m_data.m_multiPassTerrain = FALSE;
	m_data.m_adjustCliffTextures = FALSE;
	m_data.m_stretchTerrain = FALSE;
	m_data.m_useHalfHeightMap = FALSE;
	// FIXME: Fix me later!
	// m_data.m_terrainLOD = TERRAIN_LOD_AUTOMATIC;
	m_data.m_terrainLODTargetTimeMS = 0;
	m_data.m_enableDynamicLOD = TRUE;
	m_data.m_enableStaticLOD = TRUE;
	m_data.m_rightMouseAlwaysScrolls = FALSE;
	m_data.m_useWaterPlane = FALSE;
	m_data.m_useCloudPlane = FALSE;
	m_data.m_downwindAngle = ( -0.785f );//Northeast!
	m_data.m_useShadowVolumes = FALSE;
	m_data.m_useShadowDecals = FALSE;
	m_data.m_textureReductionFactor = -1;
	m_data.m_enableBehindBuildingMarkers = TRUE;
	m_data.m_scriptDebug = FALSE;
	m_data.m_particleEdit = FALSE;
	m_data.m_displayDebug = FALSE;
	m_data.m_winCursors = TRUE;
	m_data.m_constantDebugUpdate = FALSE;
	m_data.m_showTeamDot = FALSE;
	m_data.m_fixedSeed = -1; // disabled
	m_data.m_horizontalScrollSpeedFactor = 1.0;
	m_data.m_verticalScrollSpeedFactor = 1.0;

	m_data.m_waterPositionX = 0.0f;
	m_data.m_waterPositionY = 0.0f;
	m_data.m_waterPositionZ = 0.0f;
	m_data.m_waterExtentX = 0.0f;	
	m_data.m_waterExtentY = 0.0f;
	m_data.m_waterType = 0;
	m_data.m_featherWater = FALSE;
	m_data.m_showSoftWaterEdge = TRUE;	//display soft water edge
	m_data.m_usingWaterTrackEditor = FALSE;
	m_data.m_isWorldBuilder = FALSE;

	m_data.m_showMetrics = false;

	for( i = 0; i < m_data.MAX_WATER_GRID_SETTINGS; i++ )
	{

		m_data.m_vertexWaterHeightClampLow[ i ] = 0.0f;
		m_data.m_vertexWaterHeightClampHi[ i ] = 0.0f;
		m_data.m_vertexWaterAngle[ i ] = 0.0f;
		m_data.m_vertexWaterXPosition[ i ] = 0.0f;
		m_data.m_vertexWaterYPosition[ i ] = 0.0f;
		m_data.m_vertexWaterZPosition[ i ] = 0.0f;
		m_data.m_vertexWaterXGridCells[ i ] = 0;
		m_data.m_vertexWaterYGridCells[ i ] = 0;
		m_data.m_vertexWaterGridSize[ i ] = 0.0f;
		m_data.m_vertexWaterAttenuationA[ i ] = 0.0f;
		m_data.m_vertexWaterAttenuationB[ i ] = 0.0f;
		m_data.m_vertexWaterAttenuationC[ i ] = 0.0f;
		m_data.m_vertexWaterAttenuationRange[ i ] = 0.0f;
		//Added By Sadullah Nader
		//Initializations missing and needed
		m_data.m_vertexWaterAvailableMaps[i].clear();
	}  // end for i

	m_data.m_skyBoxPositionZ = 0.0f;
	m_data.m_drawSkyBox = FALSE;
	m_data.m_skyBoxScale = 4.5f;

	m_data.m_historicDamageLimit = 0;
	m_data.m_maxTerrainTracks = 0;

	m_data.m_levelGainAnimationDisplayTimeInSeconds = 0.0f;
	m_data.m_levelGainAnimationZRisePerSecond = 0.0f;

	m_data.m_getHealedAnimationDisplayTimeInSeconds = 0.0f;
	m_data.m_getHealedAnimationZRisePerSecond = 0.0f;

	m_data.m_maxTankTrackEdges=100;
	m_data.m_maxTankTrackOpaqueEdges=25;
	m_data.m_maxTankTrackFadeDelay=300000;

	m_data.m_timeOfDay = TIME_OF_DAY_AFTERNOON;
	m_data.m_weather = WEATHER_NORMAL;
	m_data.m_makeTrackMarks = FALSE;
	m_data.m_hideGarrisonFlags = FALSE;
	m_data.m_forceModelsToFollowTimeOfDay = true;
	m_data.m_forceModelsToFollowWeather = true;

	m_data.m_partitionCellSize = 0.0f;
	m_data.m_ammoPipScaleFactor = 1.0f;
	m_data.m_containerPipScaleFactor = 1.0f;
	m_data.m_ammoPipWorldOffset.zero();
	m_data.m_containerPipWorldOffset.zero();
	m_data.m_ammoPipScreenOffset.x = m_data.m_ammoPipScreenOffset.y = 0;
	m_data.m_containerPipScreenOffset.x = m_data.m_containerPipScreenOffset.y = 0;

	for (i=0; i<MAX_GLOBAL_LIGHTS; i++)
	{
		m_data.m_terrainAmbient[i].red = 0.0f;
		m_data.m_terrainAmbient[i].green = 0.0f;
		m_data.m_terrainAmbient[i].blue = 0.0f;
		m_data.m_terrainDiffuse[i].red = 0.0f;
		m_data.m_terrainDiffuse[i].green = 0.0f;
		m_data.m_terrainDiffuse[i].blue = 0.0f;
		m_data.m_terrainLightPos[i].x = 0.0f;
		m_data.m_terrainLightPos[i].y = 0.0f;
		m_data.m_terrainLightPos[i].z = -1.0f;

		for (j=0; j<TIME_OF_DAY_COUNT; j++)
		{	m_data.m_terrainLighting[ j ][i].ambient.red=0;
			m_data.m_terrainLighting[ j ][i].ambient.green=0;
			m_data.m_terrainLighting[ j ][i].ambient.blue=0;
			m_data.m_terrainLighting[ j ][i].diffuse.red=0;
			m_data.m_terrainLighting[ j ][i].diffuse.green=0;
			m_data.m_terrainLighting[ j ][i].diffuse.blue=0;
			m_data.m_terrainLighting[ j ][i].lightPos.x=0;
			m_data.m_terrainLighting[ j ][i].lightPos.y=0;
			m_data.m_terrainLighting[ j ][i].lightPos.z=-1.0f;

			m_data.m_terrainObjectsLighting[ j ][i].ambient.red=0;
			m_data.m_terrainObjectsLighting[ j ][i].ambient.green=0;
			m_data.m_terrainObjectsLighting[ j ][i].ambient.blue=0;
			m_data.m_terrainObjectsLighting[ j ][i].diffuse.red=0;
			m_data.m_terrainObjectsLighting[ j ][i].diffuse.green=0;
			m_data.m_terrainObjectsLighting[ j ][i].diffuse.blue=0;
			m_data.m_terrainObjectsLighting[ j ][i].lightPos.x=0;
			m_data.m_terrainObjectsLighting[ j ][i].lightPos.y=0;
			m_data.m_terrainObjectsLighting[ j ][i].lightPos.z=-1.0f;
		}
	}

	for (j=TIME_OF_DAY_FIRST; j<TIME_OF_DAY_COUNT; j++)
		m_data.m_infantryLightScale[j] = 1.5f;
	
	m_data.m_scriptOverrideInfantryLightScale = -1.0f;

	m_data.m_numGlobalLights = 3;
	m_data.m_maxRoadSegments = 0;
	m_data.m_maxRoadVertex = 0;
	m_data.m_maxRoadIndex = 0;
	m_data.m_maxRoadTypes = 0;

	m_data.m_baseValuePerSupplyBox = 100;

	m_data.m_audioOn = TRUE;
	m_data.m_musicOn = TRUE;
	m_data.m_soundsOn = TRUE;
	m_data.m_sounds3DOn = TRUE;
	m_data.m_speechOn = TRUE;
	m_data.m_videoOn = TRUE;
	m_data.m_disableCameraMovement = FALSE;
	m_data.m_maxVisibleTranslucentObjects = 512;
	m_data.m_maxVisibleOccluderObjects = 512;
	m_data.m_maxVisibleOccludeeObjects = 512;
	m_data.m_maxVisibleNonOccluderOrOccludeeObjects = 512;
	m_data.m_occludedLuminanceScale = 0.5f;

	m_data.m_useFX = TRUE;

//	m_data.m_inGame = FALSE;	

	m_data.m_noDraw = 0;
	m_data.m_particleScale = 1.0f;

	m_data.m_autoFireParticleSmallMax = 0;
	m_data.m_autoFireParticleMediumMax = 0;
	m_data.m_autoFireParticleLargeMax = 0;
	m_data.m_autoSmokeParticleSmallMax = 0;
	m_data.m_autoSmokeParticleMediumMax = 0;
	m_data.m_autoSmokeParticleLargeMax = 0;
	m_data.m_autoAflameParticleMax = 0;
  
	// Added By Sadullah Nader
	// Initializations missing and needed
	m_data.m_autoFireParticleSmallPrefix.clear();
	m_data.m_autoFireParticleMediumPrefix.clear();
	m_data.m_autoFireParticleLargePrefix.clear();
	m_data.m_autoSmokeParticleSmallPrefix.clear();
	m_data.m_autoSmokeParticleMediumPrefix.clear();
	m_data.m_autoSmokeParticleLargePrefix.clear();
	m_data.m_autoAflameParticlePrefix.clear();

	m_data.m_autoFireParticleSmallSystem.clear();
	m_data.m_autoFireParticleMediumSystem.clear();
	m_data.m_autoFireParticleLargeSystem.clear();
	m_data.m_autoSmokeParticleSmallSystem.clear();
	m_data.m_autoSmokeParticleMediumSystem.clear();
	m_data.m_autoSmokeParticleLargeSystem.clear();
	m_data.m_autoAflameParticleSystem.clear();
	m_data.m_levelGainAnimationName.clear();
	m_data.m_getHealedAnimationName.clear();
	m_data.m_specialPowerViewObjectName.clear();

	m_data.m_drawEntireTerrain = FALSE;
	m_data.m_maxParticleCount = 0;
	m_data.m_maxFieldParticleCount = 30;
	
	// End Add

	// m_data.m_debugAI = AI_DEBUG_NONE; // FIXME: Once AI header is fixed.
	m_data.m_debugSupplyCenterPlacement = FALSE;
	m_data.m_debugAIObstacles = FALSE;
	m_data.m_showClientPhysics = TRUE;
	m_data.m_showTerrainNormals = FALSE;
	m_data.m_showObjectHealth = FALSE;

	m_data.m_particleEdit = FALSE;

	m_data.m_cameraPitch = 0.0f;
	m_data.m_cameraYaw = 0.0f;
	m_data.m_cameraHeight = 0.0f;
	m_data.m_minCameraHeight = 100.0f;
	m_data.m_maxCameraHeight = 300.0f;
	m_data.m_terrainHeightAtEdgeOfMap = 0.0f;

	m_data.m_unitDamagedThresh = 0.5f;
	m_data.m_unitReallyDamagedThresh = 0.1f;
	m_data.m_groundStiffness = 0.5f;
	m_data.m_structureStiffness = 0.5f;
	m_data.m_gravity = -1.0f;
	m_data.m_stealthFriendlyOpacity = 0.5f;
	m_data.m_defaultOcclusionDelay = LOGICFRAMES_PER_SECOND * 3;	//default to 3 seconds

	m_data.m_preloadAssets = FALSE;
	m_data.m_preloadEverything = FALSE;
	m_data.m_preloadReport = FALSE;

	m_data.m_netMinPlayers = 1; // allowing sandbox mode

	m_data.m_defaultIP = 0;

	m_data.m_BuildSpeed = 0.0f;
	m_data.m_MinDistFromEdgeOfMapForBuild = 0.0f;
	m_data.m_SupplyBuildBorder = 0.0f;
	m_data.m_allowedHeightVariationForBuilding = 0.0f;
	m_data.m_MinLowEnergyProductionSpeed = 0.0f;
	m_data.m_MaxLowEnergyProductionSpeed = 0.0f;
	m_data.m_LowEnergyPenaltyModifier = 0.0f;
	m_data.m_MultipleFactory = 0.0f;
	m_data.m_RefundPercent = 0.0f;

	m_data.m_commandCenterHealRange = 0.0f;
	m_data.m_commandCenterHealAmount = 0.0f;
	m_data.m_maxTunnelCapacity = 0;
	m_data.m_maxLineBuildObjects = 0;

	m_data.m_standardMinefieldDensity = 0.01f;
	m_data.m_standardMinefieldDistance = 40.0f;
	
	m_data.m_groupSelectMinSelectSize = 5;
	m_data.m_groupSelectVolumeBase = 0.5f;
	m_data.m_groupSelectVolumeIncrement = 0.02f;
	m_data.m_maxUnitSelectSounds = 8;

	m_data.m_selectionFlashSaturationFactor = 0.5f; /// how colorful should the selection flash be? 0-4
	m_data.m_selectionFlashHouseColor = FALSE;  /// skip the house color and just use white.

	m_data.m_cameraAudibleRadius = 500.0;
	m_data.m_groupMoveClickToGatherFactor = 1.0f;

	m_data.m_shakeSubtleIntensity = 0.5f;
	m_data.m_shakeNormalIntensity = 1.0f;
	m_data.m_shakeStrongIntensity = 2.5f;
	m_data.m_shakeSevereIntensity = 5.0f;
	m_data.m_shakeCineExtremeIntensity = 8.0f;
	m_data.m_shakeCineInsaneIntensity = 12.0f;
	m_data.m_maxShakeIntensity = 10.0f;
	m_data.m_maxShakeRange = 150.f;

	m_data.m_sellPercentage = 1.0f;
	m_data.m_baseRegenHealthPercentPerSecond = 0.0f;
	m_data.m_baseRegenDelay = 0;

#ifdef ALLOW_SURRENDER
	m_data.m_prisonBountyMultiplier = 1.0f;
	m_data.m_prisonBountyTextColor = GameMakeColor( 255, 255, 255, 255 );
#endif

	m_data.m_hotKeyTextColor = GameMakeColor(255,255,0,255);

	// THis is put ON ice until later
	//  m_data.m_cheaterHasBeenSpiedIfMyLowestBitIsTrue = GameMakeColor(255,128,0,0);// orange, to the hacker's eye

	m_data.m_shroudColor.setFromInt( 0x00FFFFFF ) ;
	m_data.m_clearAlpha = 255;
	m_data.m_fogAlpha = 127;
	m_data.m_shroudAlpha = 0;

	m_data.m_powerBarBase = 7;
	m_data.m_powerBarIntervals = 3;
	m_data.m_powerBarYellowRange = 5;
	m_data.m_displayGamma = 1.0f;	//ramp that does nothing

	m_data.m_standardPublicBones.clear();

	m_data.m_antiAliasBoxValue = 0;

//	m_data.m_languageFilterPref = false;
	m_data.m_languageFilterPref = true;
	m_data.m_firewallBehavior = FirewallHelperClass::FIREWALL_TYPE_UNKNOWN;
	m_data.m_firewallSendDelay = FALSE;
	m_data.m_firewallPortOverride = 0;
	m_data.m_firewallPortAllocationDelta = 0;
	m_data.m_loadScreenDemo = FALSE;
	m_data.m_disableRender = false;
	
	m_data.m_saveCameraInReplay = FALSE;
	m_data.m_useCameraInReplay = FALSE;


	m_data.m_debugShowGraphicalFramerate = FALSE;

	// By default, show all asserts.

	m_data.m_unlookPersistDuration		= 30;

	//-----------------------------------------------------------------------------------------------

	// network timing values.  Having these default to 0 would be bad. - BGC
	m_data.m_networkFPSHistoryLength = 30;
	m_data.m_networkLatencyHistoryLength = 200;
	m_data.m_networkRunAheadMetricsTime = 500;
	m_data.m_networkCushionHistoryLength = 10;
	m_data.m_networkRunAheadSlack = 10;
	m_data.m_networkKeepAliveDelay = 20;
	m_data.m_networkDisconnectTime = 5000;
	m_data.m_networkPlayerTimeoutTime = 60000;
	m_data.m_networkDisconnectScreenNotifyTime = 15000;

	m_data.m_isBreakableMovie = FALSE;
	m_data.m_breakTheMovie = FALSE;

	setTimeOfDay( m_data.m_timeOfDay );

	m_data.m_buildMapCache = FALSE;
	m_data.m_initialFile.clear();
	m_data.m_pendingFile.clear();

	for (i = LEVEL_FIRST; i <= LEVEL_LAST; ++i)
		m_data.m_healthBonus[i] = 1.0f;

	for (i = 0; i < PLAYERTYPE_COUNT; ++i)
	{
		for (j = 0; j < DIFFICULTY_COUNT; ++j)
		{
			m_data.m_soloPlayerHealthBonusForDifficulty[i][j] = 1.0f;
		}
	}

	m_data.m_defaultStructureRubbleHeight = 1.0f;
	// FIXME: Fix this.
	// m_data.m_weaponBonusSet = newInstance(WeaponBonusSet);

	m_data.m_shellMapName.set("Maps\\ShellMap1\\ShellMap1.map");
	m_data.m_shellMapOn =TRUE;
	m_data.m_playIntro = TRUE;
	m_data.m_playSizzle = TRUE;
	m_data.m_afterIntro = FALSE;
	m_data.m_allowExitOutOfMovies = FALSE;
	m_data.m_loadScreenRender = FALSE;
	m_data.m_musicVolumeFactor = 0.5f;
 	m_data.m_SFXVolumeFactor = 0.5f;
	m_data.m_voiceVolumeFactor = 0.5f;
	m_data.m_3DSoundPref = false;

	m_data.m_keyboardDefaultScrollFactor = m_data.m_keyboardScrollFactor = 0.5f;
	m_data.m_scrollAmountCutoff = 10.0f;
	m_data.m_cameraAdjustSpeed = 0.1f;
	m_data.m_enforceMaxCameraHeight = TRUE;
	
	m_data.m_animateWindows = TRUE;
	
	m_data.m_iniCRC = 0;
	m_data.m_exeCRC = 0;
	
#if 0 // FIXME: Once CRC stuff is added.
	// lets CRC the executable!  Whee!
	const Int blockSize = 65536;
	Char buffer[ _MAX_PATH ];
	CRC exeCRC;
	GetModuleFileName( NULL, buffer, sizeof( buffer ) );
	File *fp = TheFileSystem->openFile(buffer, File::READ | File::BINARY);
	if (fp != NULL) {
		unsigned char crcBlock[blockSize];
		Int amtRead = 0;
		while ( (amtRead=fp->read(crcBlock, blockSize)) > 0 )
		{
			exeCRC.computeCRC(crcBlock, amtRead);
		}
		fp->close();
		fp = NULL;
	}
	if (TheVersion)
	{
		UnsignedInt version = TheVersion->getVersionNumber();
		exeCRC.computeCRC( &version, sizeof(UnsignedInt) );
	}
	// Add in MP scripts to the EXE CRC, since the game will go out of sync if they change
	fp = TheFileSystem->openFile("Data\\Scripts\\SkirmishScripts.scb", File::READ | File::BINARY);
	if (fp != NULL) {
		unsigned char crcBlock[blockSize];
		Int amtRead = 0;
		while ( (amtRead=fp->read(crcBlock, blockSize)) > 0 )
		{
			exeCRC.computeCRC(crcBlock, amtRead);
		}
		fp->close();
		fp = NULL;
	}
	fp = TheFileSystem->openFile("Data\\Scripts\\MultiplayerScripts.scb", File::READ | File::BINARY);
	if (fp != NULL) {
		unsigned char crcBlock[blockSize];
		Int amtRead = 0;
		while ( (amtRead=fp->read(crcBlock, blockSize)) > 0 )
		{
			exeCRC.computeCRC(crcBlock, amtRead);
		}
		fp->close();
		fp = NULL;
	}

	m_data.m_exeCRC = exeCRC.get();
	DEBUG_LOG(("EXE CRC: 0x%8.8X\n", m_data.m_exeCRC));
#endif

	// FIXME: Fix me later!
	// m_data.m_movementPenaltyDamageState = BODY_REALLYDAMAGED;

	m_data.m_shouldUpdateTGAToDDS = FALSE;
	
	// Default DoubleClickTime to System double click time.
	// m_data.m_doubleClickTimeMS = GetDoubleClickTime(); // Note: This is actual MS, not frames.
	m_data.m_doubleClickTimeMS = 500;

#ifdef DUMP_PERF_STATS
	m_data.m_dumpPerformanceStatistics = FALSE;
	m_data.m_dumpStatsAtInterval = FALSE;
	m_data.m_statsInterval = 30;      
#endif

	m_data.m_forceBenchmark = FALSE;	///<forces running of CPU detection benchmark, even on known cpu's.

	m_data.m_keyboardCameraRotateSpeed = 0.1f;

	char* prefsPath {SDL_GetPrefPath("IncredulousMonk", "Command and Conquer Generals Zero Hour Data")};
	m_userDataDir = prefsPath;
	SDL_free(prefsPath);
	DEBUG_LOG(("User data directory: %s\n", m_userDataDir.str()));

	//-allAdvice feature
	//m_data.m_allAdvice = FALSE;

	m_data.m_clientRetaliationModeEnabled = TRUE; //On by default.

	m_data.m_obj = this;

}  // end GlobalData


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
GlobalData::~GlobalData( void )
{
	DEBUG_ASSERTCRASH( TheWritableGlobalData->m_next == NULL, ("~GlobalData: theOriginal is not original\n") );

	// FIXME: Fix this.
	// if (m_data.m_weaponBonusSet)
	// 	m_data.m_weaponBonusSet->deleteInstance();

	if( m_theOriginal == this )	{
		m_theOriginal = NULL;
		TheWritableGlobalData = NULL;
	}

}  // end ~GlobalData

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
Bool GlobalData::setTimeOfDay( TimeOfDay tod )
{
	if( tod >= TIME_OF_DAY_COUNT || tod < TIME_OF_DAY_FIRST )
	{
		return FALSE;
	}

	m_data.m_timeOfDay = tod;
	for (Int i=0; i<MAX_GLOBAL_LIGHTS; i++)
	{	m_data.m_terrainAmbient[i] = m_data.m_terrainLighting[ tod ][i].ambient;
		m_data.m_terrainDiffuse[i] = m_data.m_terrainLighting[ tod ][i].diffuse;
		m_data.m_terrainLightPos[i] = m_data.m_terrainLighting[ tod ][i].lightPos;
	}

	return TRUE;

}

//-------------------------------------------------------------------------------------------------
/** Create a new global data instance to override the existing data set.  The
	* initial values of the newly created instance will be a copy of the current
	* data (or the most recently created override) */
//-------------------------------------------------------------------------------------------------
GlobalData *GlobalData::newOverride( void )
{
	GlobalData *override = NEW GlobalData;

	// copy the data from the latest override (TheWritableGlobalData) to the newly created instance
	DEBUG_ASSERTCRASH( TheWritableGlobalData, ("GlobalData::newOverride() - no existing data\n") );
	override->m_data = TheWritableGlobalData->m_data;

	//
	// link the override to the previously created one, the link order is important here
	// for the reset function, if you change the way things are linked
	// for overrides make sure you update the reset function
	//
	override->m_next = TheWritableGlobalData;

	// set this new instance as the 'most current override' where we will access all data from
	TheWritableGlobalData = override;

	return override;

}  // end newOveride

//-------------------------------------------------------------------------------------------------
void GlobalData::init( void )
{
	// nothing
}

//-------------------------------------------------------------------------------------------------
/** Reset, remove any override data instances and return to just the initial one
	*/
//-------------------------------------------------------------------------------------------------
void GlobalData::reset( void )
{
	DEBUG_ASSERTCRASH(this == TheWritableGlobalData, ("calling reset on wrong GlobalData"));

	//
	// delete	any data instances that were loaded as an override and set the original
	// global data instance as the singleton TheWritableGlobalData once again
	//
	while (TheWritableGlobalData != GlobalData::m_theOriginal)
	{

		// get next instance
		GlobalData* next = TheWritableGlobalData->m_next;

		// delete the head of the global data list (the latest override)
		delete TheWritableGlobalData;

		// set next as top
		TheWritableGlobalData = next;

	}  // end while

	//
	// we now have the one single global data in TheWritableGlobalData singleton, lets sanity check
	// some of all that
	//
	DEBUG_ASSERTCRASH( TheWritableGlobalData->m_next == NULL, ("ResetGlobalData: theOriginal is not original\n") );
	DEBUG_ASSERTCRASH( TheWritableGlobalData == GlobalData::m_theOriginal, ("ResetGlobalData: oops\n") );

}  // end ResetGlobalData

//-------------------------------------------------------------------------------------------------
/** Parse GameData entry */
//-------------------------------------------------------------------------------------------------
void GlobalData::parseGameDataDefinition( INI* ini )
{
	if( TheWritableGlobalData && ini->getLoadType() != INI_LOAD_MULTIFILE)
	{

		// 
		// if the type of loading we're doing creates override data, we need to
		// be loading into a new override item
		//
		if( ini->getLoadType() == INI_LOAD_CREATE_OVERRIDES )
			TheWritableGlobalData->newOverride();

	}  // end if
	else if (!TheWritableGlobalData)
	{

		// we don't have any global data instance at all yet, create one
		TheWritableGlobalData = NEW GlobalData;

	}  // end else
	// If we're multifile, then continue loading stuff into the Global Data as normal.

	// parse the ini weapon definition
	ini->initFromINI( &TheWritableGlobalData->m_data, s_GlobalDataFieldParseTable );


	// // override INI values with user preferences
	// OptionPreferences optionPref;
	// TheWritableGlobalData->m_data.m_useAlternateMouse = optionPref.getAlternateMouseModeEnabled();
	// TheWritableGlobalData->m_data.m_clientRetaliationModeEnabled = optionPref.getRetaliationModeEnabled();
	// TheWritableGlobalData->m_data.m_doubleClickAttackMove = optionPref.getDoubleClickAttackMoveEnabled();
	// TheWritableGlobalData->m_data.m_keyboardScrollFactor = optionPref.getScrollFactor();
	// TheWritableGlobalData->m_data.m_defaultIP = optionPref.getLANIPAddress();
	// TheWritableGlobalData->m_data.m_firewallSendDelay = optionPref.getSendDelay();
	// TheWritableGlobalData->m_data.m_firewallBehavior = optionPref.getFirewallBehavior();
	// TheWritableGlobalData->m_data.m_firewallPortAllocationDelta = optionPref.getFirewallPortAllocationDelta();
	// TheWritableGlobalData->m_data.m_firewallPortOverride = optionPref.getFirewallPortOverride();
	
	// TheWritableGlobalData->m_data.m_saveCameraInReplay = optionPref.saveCameraInReplays();
	// TheWritableGlobalData->m_data.m_useCameraInReplay = optionPref.useCameraInReplays();
	
	// Int val=optionPref.getGammaValue();
	// //generate a value between 0.6 and 2.0.
	// if (val < 50)
	// {	//darker gamma
	// 	if (val <= 0)
	// 		TheWritableGlobalData->m_data.m_displayGamma = 0.6f;
	// 	else
	// 		TheWritableGlobalData->m_data.m_displayGamma=1.0f-(0.4f) * (Real)(50-val)/50.0f;
	// }
	// else
	// if (val > 50)
	// 	TheWritableGlobalData->m_data.m_displayGamma=1.0f+(1.0f) * (Real)(val-50)/50.0f;

	// Int xres,yres;
	// optionPref.getResolution(&xres, &yres);

	// TheWritableGlobalData->m_data.m_xResolution = xres;
	// TheWritableGlobalData->m_data.m_yResolution = yres;
}

//-------------------------------------------------------------------------------------------------
void GlobalData::parseDefaultStartingCash(INI* ini, void* instance, void* /*store*/, const void* /*userData*/)
{
	GlobalData::Data* data = (GlobalData::Data*) instance;
	GlobalData* self = data->m_obj;
	Money::parseMoneyAmount(ini, nullptr, &self->m_defaultStartingCash, nullptr);
}
