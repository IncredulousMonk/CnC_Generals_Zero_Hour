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


#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#include "Common/MiscAudio.h"
#include "Common/INI.h"
#include <array>

// MG: offsetof won't work with AudioEventRTS.
// const FieldParse MiscAudio::m_fieldParseTable[] =
// {
// 	{ "RadarNotifyUnitUnderAttackSound",		INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_radarUnitUnderAttackSound ) },
// 	{ "RadarNotifyHarvesterUnderAttackSound",	INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_radarHarvesterUnderAttackSound ) },
// 	{ "RadarNotifyStructureUnderAttackSound",	INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_radarStructureUnderAttackSound ) },
// 	{ "RadarNotifyUnderAttackSound",			INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_radarUnderAttackSound ) },
// 	{ "RadarNotifyInfiltrationSound",			INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_radarInfiltrationSound ) },
// 	{ "RadarNotifyOnlineSound",					INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_radarOnlineSound ) },
// 	{ "RadarNotifyOfflineSound",				INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_radarOfflineSound ) },
// 	{ "DefectorTimerTickSound",					INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_defectorTimerTickSound ) },
// 	{ "DefectorTimerDingSound",					INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_defectorTimerDingSound ) },
// 	{ "LockonTickSound",						INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_lockonTickSound ) },
// 	{ "AllCheerSound",							INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_allCheerSound )	},
// 	{ "BattleCrySound",							INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_battleCrySound )	},
// 	{ "GUIClickSound",							INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_guiClickSound )	},
// 	{ "NoCanDoSound",							INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_noCanDoSound )	},
// 	{ "StealthDiscoveredSound",					INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_stealthDiscoveredSound ) },
// 	{ "StealthNeutralizedSound",				INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_stealthNeutralizedSound ) },
// 	{ "MoneyDepositSound",						INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_moneyDepositSound ) },
// 	{ "MoneyWithdrawSound",						INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_moneyWithdrawSound ) },
// 	{ "BuildingDisabled",						INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_buildingDisabled ) },
// 	{ "BuildingReenabled",						INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_buildingReenabled ) },
// 	{ "VehicleDisabled",						INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_vehicleDisabled ) },
// 	{ "VehicleReenabled",						INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_vehicleReenabled ) },
// 	{ "SplatterVehiclePilotsBrain",				INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_splatterVehiclePilotsBrain ) },
// 	{ "TerroristInCarMoveVoice",				INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_terroristInCarMoveVoice ) },
// 	{ "TerroristInCarAttackVoice",				INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_terroristInCarAttackVoice ) },
// 	{ "TerroristInCarSelectVoice",				INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_terroristInCarSelectVoice ) },
// 	{ "CrateHeal",								INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_crateHeal ) },
// 	{ "CrateShroud",							INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_crateShroud ) },
// 	{ "CrateSalvage",							INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_crateSalvage ) },
// 	{ "CrateFreeUnit",							INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_crateFreeUnit ) },
// 	{ "CrateMoney",								INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_crateMoney ) },
// 	{ "UnitPromoted",							INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_unitPromoted ) },
// 	{ "RepairSparks",							INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_repairSparks ) },
// 	{ "SabotageShutDownBuilding",				INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_sabotageShutDownBuilding ) },
// 	{ "SabotageResetTimeBuilding",				INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_sabotageResetTimerBuilding ) },
// 	{ "AircraftWheelScreech",					INI::parseAudioEventRTS, NULL, offsetof( MiscAudio, m_aircraftWheelScreech ) },
	
// 	{ 0, 0, 0, 0 }
// };

//-------------------------------------------------------------------------------------------------
void INI::parseMiscAudio( INI *ini )
{
	MiscAudio* ma {TheAudio->friend_getMiscAudio()};
	std::array<FieldParse, 37> fields = {{
		{ "RadarNotifyUnitUnderAttackSound",		INI::parseAudioEventRTS, &ma->m_radarUnitUnderAttackSound, 0 },
		{ "RadarNotifyHarvesterUnderAttackSound",	INI::parseAudioEventRTS, &ma->m_radarHarvesterUnderAttackSound, 0 },
		{ "RadarNotifyStructureUnderAttackSound",	INI::parseAudioEventRTS, &ma->m_radarStructureUnderAttackSound, 0 },
		{ "RadarNotifyUnderAttackSound",			INI::parseAudioEventRTS, &ma->m_radarUnderAttackSound, 0 },
		{ "RadarNotifyInfiltrationSound",			INI::parseAudioEventRTS, &ma->m_radarInfiltrationSound, 0 },
		{ "RadarNotifyOnlineSound",					INI::parseAudioEventRTS, &ma->m_radarOnlineSound, 0 },
		{ "RadarNotifyOfflineSound",				INI::parseAudioEventRTS, &ma->m_radarOfflineSound, 0 },
		{ "DefectorTimerTickSound",					INI::parseAudioEventRTS, &ma->m_defectorTimerTickSound, 0 },
		{ "DefectorTimerDingSound",					INI::parseAudioEventRTS, &ma->m_defectorTimerDingSound, 0 },
		{ "LockonTickSound",						INI::parseAudioEventRTS, &ma->m_lockonTickSound, 0 },
		{ "AllCheerSound",							INI::parseAudioEventRTS, &ma->m_allCheerSound, 0	},
		{ "BattleCrySound",							INI::parseAudioEventRTS, &ma->m_battleCrySound, 0	},
		{ "GUIClickSound",							INI::parseAudioEventRTS, &ma->m_guiClickSound, 0	},
		{ "NoCanDoSound",							INI::parseAudioEventRTS, &ma->m_noCanDoSound, 0	},
		{ "StealthDiscoveredSound",					INI::parseAudioEventRTS, &ma->m_stealthDiscoveredSound, 0 },
		{ "StealthNeutralizedSound",				INI::parseAudioEventRTS, &ma->m_stealthNeutralizedSound, 0 },
		{ "MoneyDepositSound",						INI::parseAudioEventRTS, &ma->m_moneyDepositSound, 0 },
		{ "MoneyWithdrawSound",						INI::parseAudioEventRTS, &ma->m_moneyWithdrawSound, 0 },
		{ "BuildingDisabled",						INI::parseAudioEventRTS, &ma->m_buildingDisabled, 0 },
		{ "BuildingReenabled",						INI::parseAudioEventRTS, &ma->m_buildingReenabled, 0 },
		{ "VehicleDisabled",						INI::parseAudioEventRTS, &ma->m_vehicleDisabled, 0 },
		{ "VehicleReenabled",						INI::parseAudioEventRTS, &ma->m_vehicleReenabled, 0 },
		{ "SplatterVehiclePilotsBrain",				INI::parseAudioEventRTS, &ma->m_splatterVehiclePilotsBrain, 0 },
		{ "TerroristInCarMoveVoice",				INI::parseAudioEventRTS, &ma->m_terroristInCarMoveVoice, 0 },
		{ "TerroristInCarAttackVoice",				INI::parseAudioEventRTS, &ma->m_terroristInCarAttackVoice, 0 },
		{ "TerroristInCarSelectVoice",				INI::parseAudioEventRTS, &ma->m_terroristInCarSelectVoice, 0 },
		{ "CrateHeal",								INI::parseAudioEventRTS, &ma->m_crateHeal, 0 },
		{ "CrateShroud",							INI::parseAudioEventRTS, &ma->m_crateShroud, 0 },
		{ "CrateSalvage",							INI::parseAudioEventRTS, &ma->m_crateSalvage, 0 },
		{ "CrateFreeUnit",							INI::parseAudioEventRTS, &ma->m_crateFreeUnit, 0 },
		{ "CrateMoney",								INI::parseAudioEventRTS, &ma->m_crateMoney, 0 },
		{ "UnitPromoted",							INI::parseAudioEventRTS, &ma->m_unitPromoted, 0 },
		{ "RepairSparks",							INI::parseAudioEventRTS, &ma->m_repairSparks, 0 },
		{ "SabotageShutDownBuilding",				INI::parseAudioEventRTS, &ma->m_sabotageShutDownBuilding, 0 },
		{ "SabotageResetTimeBuilding",				INI::parseAudioEventRTS, &ma->m_sabotageResetTimerBuilding, 0 },
		{ "AircraftWheelScreech",					INI::parseAudioEventRTS, &ma->m_aircraftWheelScreech, 0 },
		
		{ 0, 0, 0, 0 }
	}};
	ini->initFromINI(TheAudio->friend_getMiscAudio(), fields.data());
	// ini->initFromINI(TheAudio->friend_getMiscAudio(), MiscAudio::m_fieldParseTable);
}
