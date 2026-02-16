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

// FILE: ModuleFactory.cpp ////////////////////////////////////////////////////////////////////////
// Author: Colin Day, September 2001
// Desc:	 TheModuleFactory is where we actually instance modules for objects
//				 and drawbles.  Those modules are things such as an UpdateModule
//			   or DamageModule or DrawModule etc.
//	
//				 TheModuleFactory will contain a list of ModuleTemplates, when we
//				 request a new module, we will look for that template in our
//				 list and create it
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#include "Common/Module.h"
#include "Common/ModuleFactory.h"
#include "Common/NameKeyGenerator.h"

// behavior includes
#include "GameLogic/Module/AutoHealBehavior.h"
#if 0
#include "GameLogic/Module/GrantStealthBehavior.h"
#include "GameLogic/Module/NeutronBlastBehavior.h"
#include "GameLogic/Module/BehaviorModule.h"
#endif // if 0
#include "GameLogic/Module/BridgeBehavior.h"
#include "GameLogic/Module/BridgeScaffoldBehavior.h"
#include "GameLogic/Module/BridgeTowerBehavior.h"
#include "GameLogic/Module/CountermeasuresBehavior.h"
#if 0
#include "GameLogic/Module/DumbProjectileBehavior.h"
#include "GameLogic/Module/InstantDeathBehavior.h"
#endif // if 0
#include "GameLogic/Module/SlowDeathBehavior.h"
#if 0
#include "GameLogic/Module/HelicopterSlowDeathUpdate.h"
#include "GameLogic/Module/NeutronMissileSlowDeathUpdate.h"
#endif // if 0
#include "GameLogic/Module/CaveContain.h"
#include "GameLogic/Module/OpenContain.h"
#if 0
#include "GameLogic/Module/OverchargeBehavior.h"
#include "GameLogic/Module/HealContain.h"
#include "GameLogic/Module/GarrisonContain.h"
#include "GameLogic/Module/InternetHackContain.h"
#include "GameLogic/Module/RailedTransportContain.h"
#include "GameLogic/Module/RiderChangeContain.h"
#endif // if 0
#include "GameLogic/Module/TransportContain.h"
#if 0
#include "GameLogic/Module/MobNexusContain.h"
#include "GameLogic/Module/TunnelContain.h"
#include "GameLogic/Module/OverlordContain.h"
#endif // if 0
#include "GameLogic/Module/HelixContain.h"
#if 0
#include "GameLogic/Module/ParachuteContain.h"
#ifdef ALLOW_SURRENDER
#include "GameLogic/Module/POWTruckBehavior.h"
#include "GameLogic/Module/PrisonBehavior.h"
#include "GameLogic/Module/PropagandaCenterBehavior.h"
#endif 
#include "GameLogic/Module/PropagandaTowerBehavior.h"
#include "GameLogic/Module/BunkerBusterBehavior.h"
#include "GameLogic/Module/FireWeaponWhenDamagedBehavior.h"
#include "GameLogic/Module/FireWeaponWhenDeadBehavior.h"
#endif // if 0
#include "GameLogic/Module/GenerateMinefieldBehavior.h"
#if 0
#include "GameLogic/Module/ParkingPlaceBehavior.h"
#include "GameLogic/Module/FlightDeckBehavior.h"
#endif // if 0
#include "GameLogic/Module/PoisonedBehavior.h"
#if 0
#include "GameLogic/Module/RebuildHoleBehavior.h"
#include "GameLogic/Module/SupplyWarehouseCripplingBehavior.h"
#include "GameLogic/Module/TechBuildingBehavior.h"
#include "GameLogic/Module/MinefieldBehavior.h"
#include "GameLogic/Module/BattleBusSlowDeathBehavior.h"
#endif // if 0
#include "GameLogic/Module/JetSlowDeathBehavior.h"

#if 0
// die includes
#include "GameLogic/Module/CreateCrateDie.h"
#endif // if 0
#include "GameLogic/Module/CreateObjectDie.h"
#if 0
#include "GameLogic/Module/CrushDie.h"
#include "GameLogic/Module/DamDie.h"
#endif // if 0
#include "GameLogic/Module/DestroyDie.h"
#if 0
#include "GameLogic/Module/EjectPilotDie.h"
#endif // if 0
#include "GameLogic/Module/FXListDie.h"
#if 0
#include "GameLogic/Module/RebuildHoleExposeDie.h"
#endif // if 0
#include "GameLogic/Module/SpecialPowerCompletionDie.h"
#if 0
#include "GameLogic/Module/UpgradeDie.h"
#include "GameLogic/Module/KeepObjectDie.h"

// logic update includes
#include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/Module/AnimationSteeringUpdate.h"
#include "GameLogic/Module/AssistedTargetingUpdate.h"
#include "GameLogic/Module/BaseRegenerateUpdate.h"
#include "GameLogic/Module/BoneFXUpdate.h"
#include "GameLogic/Module/ChinookAIUpdate.h"
#include "GameLogic/Module/DefaultProductionExitUpdate.h"
#endif // if 0
#include "GameLogic/Module/DeletionUpdate.h"
#include "GameLogic/Module/SmartBombTargetHomingUpdate.h"
#include "GameLogic/Module/DeliverPayloadAIUpdate.h"
#if 0
#include "GameLogic/Module/DozerAIUpdate.h"
#include "GameLogic/Module/DynamicGeometryInfoUpdate.h"
#include "GameLogic/Module/DynamicShroudClearingRangeUpdate.h"
#include "GameLogic/Module/EnemyNearUpdate.h"
#endif // if 0
#include "GameLogic/Module/FireSpreadUpdate.h"
#if 0
#include "GameLogic/Module/FirestormDynamicGeometryInfoUpdate.h"
#include "GameLogic/Module/FireWeaponUpdate.h"
#endif // if 0
#include "GameLogic/Module/FlammableUpdate.h"
#if 0
#include "GameLogic/Module/FloatUpdate.h"
#include "GameLogic/Module/TensileFormationUpdate.h"
#endif // if 0
#include "GameLogic/Module/HackInternetAIUpdate.h"
#if 0
#include "GameLogic/Module/DeployStyleAIUpdate.h"
#include "GameLogic/Module/AssaultTransportAIUpdate.h"
#endif // if 0
#include "GameLogic/Module/HeightDieUpdate.h"
#if 0
#include "GameLogic/Module/HordeUpdate.h"
#include "GameLogic/Module/JetAIUpdate.h"
#endif // if 0
#include "GameLogic/Module/LaserUpdate.h"
#if 0
#include "GameLogic/Module/PointDefenseLaserUpdate.h"
#include "GameLogic/Module/CleanupHazardUpdate.h"
#endif // if 0
#include "GameLogic/Module/AutoFindHealingUpdate.h"
#include "GameLogic/Module/CommandButtonHuntUpdate.h"
#include "GameLogic/Module/PilotFindVehicleUpdate.h"
#if 0
#include "GameLogic/Module/DemoTrapUpdate.h"
#include "GameLogic/Module/ParticleUplinkCannonUpdate.h"
#endif // if 0
#include "GameLogic/Module/SpectreGunshipUpdate.h"
#if 0
#include "GameLogic/Module/SpectreGunshipDeploymentUpdate.h"
#include "GameLogic/Module/BaikonurLaunchPower.h"
#include "GameLogic/Module/BattlePlanUpdate.h"
#endif // if 0
#include "GameLogic/Module/LifetimeUpdate.h"
#include "GameLogic/Module/RadiusDecalUpdate.h"
#if 0
#include "GameLogic/Module/AutoDepositUpdate.h"
#endif // if 0
#include "GameLogic/Module/MissileAIUpdate.h"
#if 0
#include "GameLogic/Module/NeutronMissileUpdate.h"
#include "GameLogic/Module/OCLUpdate.h"
#endif // if 0
#include "GameLogic/Module/PhysicsUpdate.h"
#if 0
#ifdef ALLOW_SURRENDER
#include "GameLogic/Module/POWTruckAIUpdate.h"
#endif
#endif // if 0
#include "GameLogic/Module/ProductionUpdate.h"
#if 0
#include "GameLogic/Module/ProjectileStreamUpdate.h"
#endif // if 0
#include "GameLogic/Module/ProneUpdate.h"
#if 0
#include "GameLogic/Module/QueueProductionExitUpdate.h"
#include "GameLogic/Module/RadarUpdate.h"
#include "GameLogic/Module/RepairDockUpdate.h"
#ifdef ALLOW_SURRENDER
#include "GameLogic/Module/PrisonDockUpdate.h"
#endif
#include "GameLogic/Module/RailedTransportDockUpdate.h"
#include "GameLogic/Module/RailedTransportAIUpdate.h"
#endif // if 0
#include "GameLogic/Module/RailroadGuideAIUpdate.h"
#include "GameLogic/Module/SlavedUpdate.h"
#if 0
#include "GameLogic/Module/MobMemberSlavedUpdate.h"
#endif // if 0
#include "GameLogic/Module/SpecialAbilityUpdate.h"
#if 0
#include "GameLogic/Module/MissileLauncherBuildingUpdate.h"
#endif // if 0
#include "GameLogic/Module/StealthDetectorUpdate.h"
#include "GameLogic/Module/StealthUpdate.h"
#if 0
#include "GameLogic/Module/SpawnPointProductionExitUpdate.h"
#include "GameLogic/Module/SpawnBehavior.h"
#include "GameLogic/Module/SpyVisionUpdate.h"
#endif // if 0
#include "GameLogic/Module/StickyBombUpdate.h"
#if 0
#include "GameLogic/Module/FireOCLAfterWeaponCooldownUpdate.h"
#include "GameLogic/Module/HijackerUpdate.h"
#include "GameLogic/Module/StructureCollapseUpdate.h"
#include "GameLogic/Module/StructureToppleUpdate.h"
#include "GameLogic/Module/SupplyCenterDockUpdate.h"
#include "GameLogic/Module/SupplyCenterProductionExitUpdate.h"
#endif // if 0
#include "GameLogic/Module/SupplyTruckAIUpdate.h"
#include "GameLogic/Module/SupplyWarehouseDockUpdate.h"
#if 0
#include "GameLogic/Module/ToppleUpdate.h"
#include "GameLogic/Module/TransportAIUpdate.h"
#include "GameLogic/Module/WanderAIUpdate.h"
#include "GameLogic/Module/WaveGuideUpdate.h"
#include "GameLogic/Module/WeaponBonusUpdate.h"
#include "GameLogic/Module/WorkerAIUpdate.h"
#include "GameLogic/Module/PowerPlantUpdate.h"
#include "GameLogic/Module/CheckpointUpdate.h"
#include "GameLogic/Module/EMPUpdate.h"

// upgrade includes
#include "GameLogic/Module/ActiveShroudUpgrade.h"
#endif // if 0
#include "GameLogic/Module/ArmorUpgrade.h"
#if 0
#include "GameLogic/Module/CommandSetUpgrade.h"
#include "GameLogic/Module/GrantScienceUpgrade.h"
#include "GameLogic/Module/PassengersFireUpgrade.h"
#endif // if 0
#include "GameLogic/Module/LocomotorSetUpgrade.h"
#if 0
#include "GameLogic/Module/ObjectCreationUpgrade.h"
#include "GameLogic/Module/RadarUpgrade.h"
#include "GameLogic/Module/PowerPlantUpgrade.h"
#include "GameLogic/Module/ReplaceObjectUpgrade.h"
#include "GameLogic/Module/ModelConditionUpgrade.h"
#include "GameLogic/Module/StatusBitsUpgrade.h"
#include "GameLogic/Module/SubObjectsUpgrade.h"
#include "GameLogic/Module/StealthUpgrade.h"
#endif // if 0
#include "GameLogic/Module/UnpauseSpecialPowerUpgrade.h"
#if 0
#include "GameLogic/Module/WeaponBonusUpgrade.h"
#endif // if 0
#include "GameLogic/Module/WeaponSetUpgrade.h"
#if 0
#include "GameLogic/Module/WeaponBonusUpgrade.h"
#include "GameLogic/Module/CostModifierUpgrade.h"
#endif // if 0
#include "GameLogic/Module/ExperienceScalarUpgrade.h"
#if 0
#include "GameLogic/Module/MaxHealthUpgrade.h"
#endif // if 0

// create includes
#include "GameLogic/Module/LockWeaponCreate.h"
#if 0
#include "GameLogic/Module/SupplyCenterCreate.h"
#include "GameLogic/Module/SupplyWarehouseCreate.h"
#include "GameLogic/Module/GrantUpgradeCreate.h"
#include "GameLogic/Module/PreorderCreate.h"
#include "GameLogic/Module/SpecialPowerCreate.h"
#endif // if 0
#include "GameLogic/Module/VeterancyGainCreate.h"

#if 0
// damage includes
#include "GameLogic/Module/BoneFXDamage.h"
#include "GameLogic/Module/TransitionDamageFX.h"

// collide includes
#include "GameLogic/Module/FireWeaponCollide.h"
#endif // if 0
#include "GameLogic/Module/SquishCollide.h"

#if 0
#include "GameLogic/Module/ConvertToCarBombCrateCollide.h"
#include "GameLogic/Module/ConvertToHijackedVehicleCrateCollide.h"
#include "GameLogic/Module/HealCrateCollide.h"
#endif // if 0
#include "GameLogic/Module/MoneyCrateCollide.h"
#if 0
#include "GameLogic/Module/SabotageCommandCenterCrateCollide.h"
#include "GameLogic/Module/SabotageFakeBuildingCrateCollide.h"
#include "GameLogic/Module/SabotageInternetCenterCrateCollide.h"
#include "GameLogic/Module/SabotageMilitaryFactoryCrateCollide.h"
#include "GameLogic/Module/SabotagePowerPlantCrateCollide.h"
#include "GameLogic/Module/SabotageSuperweaponCrateCollide.h"
#include "GameLogic/Module/SabotageSupplyCenterCrateCollide.h"
#include "GameLogic/Module/SabotageSupplyDropzoneCrateCollide.h"
#endif // if 0
#include "GameLogic/Module/SalvageCrateCollide.h"
#if 0
#include "GameLogic/Module/ShroudCrateCollide.h"
#endif // if 0
#include "GameLogic/Module/UnitCrateCollide.h"
#include "GameLogic/Module/VeterancyCrateCollide.h"

// body includes
#include "GameLogic/Module/InactiveBody.h"
#include "GameLogic/Module/ActiveBody.h"
#if 0
#include "GameLogic/Module/HighlanderBody.h"
#include "GameLogic/Module/ImmortalBody.h"
#endif // if 0
#include "GameLogic/Module/StructureBody.h"
#if 0
#include "GameLogic/Module/HiveStructureBody.h"
#include "GameLogic/Module/UndeadBody.h"

// contain includes
// (none)

// special power modules
#include "GameLogic/Module/CashHackSpecialPower.h"
#include "GameLogic/Module/DefectorSpecialPower.h"
#ifdef ALLOW_DEMORALIZE
#include "GameLogic/Module/DemoralizeSpecialPower.h"
#endif
#include "GameLogic/Module/OCLSpecialPower.h"
#endif // if 0
#include "GameLogic/Module/SpecialAbility.h"
#if 0
#include "GameLogic/Module/SpyVisionSpecialPower.h"
#include "GameLogic/Module/CashBountyPower.h"
#include "GameLogic/Module/CleanupAreaPower.h"
#include "GameLogic/Module/FireWeaponPower.h"

// destroy includes
// (none)

// client update includes
#include "GameClient/Module/AnimatedParticleSysBoneClientUpdate.h"
#include "GameClient/Module/SwayClientUpdate.h"
#include "GameClient/Module/BeaconClientUpdate.h"
#endif // if 0

// PUBLIC DATA ////////////////////////////////////////////////////////////////////////////////////
ModuleFactory *TheModuleFactory = NULL;  ///< the module factory singleton

// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////////////////////////

#ifdef _INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
ModuleFactory::ModuleFactory( void )
{
	m_moduleTemplateMap.clear();
	m_moduleDataList.clear();
	
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
ModuleFactory::~ModuleFactory( void )
{
	m_moduleTemplateMap.clear();

	for (ModuleDataList::iterator i = m_moduleDataList.begin(); i != m_moduleDataList.end(); ++i)
	{
		const ModuleData* data = *i;
		delete data;
	}
	m_moduleDataList.clear();

}

//-------------------------------------------------------------------------------------------------
/** Initialize the module factory.  Any class that needs to be attached
	* to objects or drawables as modules needs to add a template
	* for that class here */
//-------------------------------------------------------------------------------------------------
void ModuleFactory::init( void )
{

	// behavior modules
	addModule( AutoHealBehavior );
#if 0
	addModule( GrantStealthBehavior );
	addModule( NeutronBlastBehavior );
#endif // if 0
	addModule( BridgeBehavior );
	addModule( BridgeScaffoldBehavior );
	addModule( BridgeTowerBehavior );
	addModule( CountermeasuresBehavior );
#if 0
	addModule( DumbProjectileBehavior );
#endif // if 0
	addModule( PhysicsBehavior );
#if 0
	addModule( InstantDeathBehavior );
#endif // if 0
	addModule( SlowDeathBehavior );
#if 0
	addModule( HelicopterSlowDeathBehavior );
	addModule( NeutronMissileSlowDeathBehavior );
#endif // if 0
	addModule( CaveContain );
	addModule( OpenContain );
#if 0
	addModule( OverchargeBehavior );
	addModule( HealContain );
	addModule( GarrisonContain );
	addModule( InternetHackContain );
#endif // if 0
	addModule( TransportContain );
#if 0
	addModule( RiderChangeContain );
	addModule( RailedTransportContain );
	addModule( MobNexusContain );
	addModule( TunnelContain );
	addModule( OverlordContain );
#endif // if 0
	addModule( HelixContain );
#if 0
	addModule( ParachuteContain );
#ifdef ALLOW_SURRENDER
	addModule( POWTruckBehavior );
	addModule( PrisonBehavior );
	addModule( PropagandaCenterBehavior );
#endif
	addModule( PropagandaTowerBehavior );
	addModule( BunkerBusterBehavior );
	addModule( FireWeaponWhenDamagedBehavior );
	addModule( FireWeaponWhenDeadBehavior );
#endif // if 0
	addModule( GenerateMinefieldBehavior );
#if 0
	addModule( ParkingPlaceBehavior );
	addModule( FlightDeckBehavior );
#endif // if 0
	addModule( PoisonedBehavior );
#if 0
	addModule( RebuildHoleBehavior );
	addModule( SupplyWarehouseCripplingBehavior );
	addModule( TechBuildingBehavior );
	addModule( MinefieldBehavior );
	addModule( BattleBusSlowDeathBehavior );
#endif // if 0
	addModule( JetSlowDeathBehavior );
	addModule( RailroadBehavior );
#if 0
	addModule( SpawnBehavior );
#endif // if 0

	// die modules
	addModule( DestroyDie );
	addModule( FXListDie );
#if 0
	addModule( CrushDie );
	addModule( DamDie );
	addModule( CreateCrateDie );
#endif // if 0
	addModule( CreateObjectDie );
#if 0
	addModule( EjectPilotDie );
#endif // if 0
	addModule( SpecialPowerCompletionDie );
#if 0
	addModule( RebuildHoleExposeDie );
	addModule( UpgradeDie );
	addModule( KeepObjectDie );

	// update modules
	addModule( AssistedTargetingUpdate );
#endif // if 0
	addModule( AutoFindHealingUpdate );
#if 0
	addModule( BaseRegenerateUpdate );
#endif // if 0
	addModule( StealthDetectorUpdate );
	addModule( StealthUpdate );
	addModule( DeletionUpdate );
	addModule( SmartBombTargetHomingUpdate );
#if 0
	addModule( DynamicShroudClearingRangeUpdate );
	addModule( DeployStyleAIUpdate );
	addModule( AssaultTransportAIUpdate );
	addModule( HordeUpdate );
	addModule( ToppleUpdate );
	addModule( EnemyNearUpdate );
#endif // if 0
	addModule( LifetimeUpdate );
	addModule( RadiusDecalUpdate );
#if 0
	addModule( EMPUpdate );
	addModule( LeafletDropBehavior );
	addModule( AutoDepositUpdate );
	addModule( WeaponBonusUpdate );
#endif // if 0
	addModule( MissileAIUpdate );
#if 0
	addModule( NeutronMissileUpdate );
#endif // if 0
	addModule( FireSpreadUpdate );
#if 0
	addModule( FireWeaponUpdate );
#endif // if 0
	addModule( FlammableUpdate );
#if 0
	addModule( FloatUpdate );
	addModule( TensileFormationUpdate );
#endif // if 0
	addModule( HeightDieUpdate );
#if 0
	addModule( ChinookAIUpdate );
	addModule( JetAIUpdate );
#endif // if 0
	addModule( AIUpdateInterface );
	addModule( SupplyTruckAIUpdate );
	addModule( DeliverPayloadAIUpdate );
	addModule( HackInternetAIUpdate );
#if 0
	addModule( DynamicGeometryInfoUpdate );
	addModule( FirestormDynamicGeometryInfoUpdate );
#endif // if 0
	addModule( LaserUpdate );
#if 0
	addModule( PointDefenseLaserUpdate );
	addModule( CleanupHazardUpdate );
#endif // if 0
	addModule( CommandButtonHuntUpdate );
	addModule( PilotFindVehicleUpdate );
#if 0
	addModule( DemoTrapUpdate );
	addModule( ParticleUplinkCannonUpdate );
#endif // if 0
	addModule( SpectreGunshipUpdate );
#if 0
	addModule( SpectreGunshipDeploymentUpdate );
	addModule( BaikonurLaunchPower );
	addModule( BattlePlanUpdate );
	addModule( ProjectileStreamUpdate );
	addModule( QueueProductionExitUpdate );
	addModule( RepairDockUpdate );
#ifdef ALLOW_SURRENDER
	addModule( PrisonDockUpdate );
#endif
	addModule( RailedTransportDockUpdate );
	addModule( DefaultProductionExitUpdate );
	addModule( SpawnPointProductionExitUpdate );
	addModule( SpyVisionUpdate );
#endif // if 0
	addModule( SlavedUpdate );
#if 0
	addModule( MobMemberSlavedUpdate );
	addModule( OCLUpdate );
#endif // if 0
	addModule( SpecialAbilityUpdate );
#if 0
	addModule( MissileLauncherBuildingUpdate );
	addModule( SupplyCenterProductionExitUpdate );
	addModule( SupplyCenterDockUpdate );
#endif // if 0
	addModule( SupplyWarehouseDockUpdate );
#if 0
	addModule( DozerAIUpdate );
#ifdef ALLOW_SURRENDER
	addModule( POWTruckAIUpdate );
#endif
	addModule( RailedTransportAIUpdate );
#endif // if 0
	addModule( ProductionUpdate );
	addModule( ProneUpdate );
	addModule( StickyBombUpdate );
#if 0
	addModule( FireOCLAfterWeaponCooldownUpdate );
	addModule( HijackerUpdate );
	addModule( StructureToppleUpdate );
	addModule( StructureCollapseUpdate );
	addModule( BoneFXUpdate );
	addModule( RadarUpdate );
	addModule( AnimationSteeringUpdate );
	addModule( TransportAIUpdate );
	addModule( WanderAIUpdate );
	addModule( WaveGuideUpdate );
	addModule( WorkerAIUpdate );
	addModule( PowerPlantUpdate );
	addModule( CheckpointUpdate );

	// upgrade modules
	addModule( CostModifierUpgrade );
	addModule( ActiveShroudUpgrade );
#endif // if 0
	addModule( ArmorUpgrade );
#if 0
	addModule( CommandSetUpgrade );
	addModule( GrantScienceUpgrade );
	addModule( PassengersFireUpgrade );
	addModule( StatusBitsUpgrade );
	addModule( SubObjectsUpgrade );
	addModule( StealthUpgrade );
	addModule( RadarUpgrade );
	addModule( PowerPlantUpgrade );
#endif // if 0
	addModule( LocomotorSetUpgrade );
#if 0
	addModule( ObjectCreationUpgrade );
	addModule( ReplaceObjectUpgrade );
	addModule( ModelConditionUpgrade );
#endif // if 0
	addModule( UnpauseSpecialPowerUpgrade );
#if 0
	addModule( WeaponBonusUpgrade );
#endif // if 0
	addModule( WeaponSetUpgrade );
#if 0
	addModule( WeaponBonusUpgrade );
#endif // if 0
	addModule( ExperienceScalarUpgrade );
#if 0
	addModule( MaxHealthUpgrade );
#endif // if 0

	// create modules
	addModule( LockWeaponCreate );
#if 0
	addModule( PreorderCreate );
	addModule( SupplyCenterCreate );
	addModule( SupplyWarehouseCreate );
	addModule( SpecialPowerCreate );
	addModule( GrantUpgradeCreate );
#endif // if 0
	addModule( VeterancyGainCreate );

	// damage modules
#if 0
	addModule( BoneFXDamage );
	addModule( TransitionDamageFX );

	// collide modules
	addModule( FireWeaponCollide );
#endif // if 0
	addModule( SquishCollide );

#if 0
	addModule( HealCrateCollide );
#endif // if 0
	addModule( MoneyCrateCollide );
#if 0
	addModule( ShroudCrateCollide );
#endif // if 0
	addModule( UnitCrateCollide );
	addModule( VeterancyCrateCollide );
#if 0
	addModule( ConvertToCarBombCrateCollide );
	addModule( ConvertToHijackedVehicleCrateCollide );
	addModule( SabotageCommandCenterCrateCollide );
	addModule( SabotageFakeBuildingCrateCollide );
	addModule( SabotageInternetCenterCrateCollide );
	addModule( SabotageMilitaryFactoryCrateCollide );
	addModule( SabotagePowerPlantCrateCollide );
	addModule( SabotageSuperweaponCrateCollide );
	addModule( SabotageSupplyCenterCrateCollide );
	addModule( SabotageSupplyDropzoneCrateCollide );
#endif // if 0
	addModule( SalvageCrateCollide );

	// body modules
	addModule( InactiveBody );
	addModule( ActiveBody );
#if 0
	addModule( HighlanderBody );
	addModule( ImmortalBody );
#endif // if 0
	addModule( StructureBody );
#if 0
	addModule( HiveStructureBody );
	addModule( UndeadBody );

	// contain modules
	// (none)

	// special power modules
	addModule( CashHackSpecialPower );
	addModule( DefectorSpecialPower );
#ifdef ALLOW_DEMORALIZE
	addModule( DemoralizeSpecialPower );
#endif
	addModule( OCLSpecialPower );
	addModule( FireWeaponPower );
#endif // if 0
	addModule( SpecialAbility );
#if 0
	addModule( SpyVisionSpecialPower );
	addModule( CashBountyPower );
	addModule( CleanupAreaPower );

	// destroy modules
	// (none)

	// client update modules
	addModule( AnimatedParticleSysBoneClientUpdate );
	addModule( SwayClientUpdate );
	addModule( BeaconClientUpdate );
#endif // if 0

}  // end init

//-------------------------------------------------------------------------------------------------
Int ModuleFactory::findModuleInterfaceMask(const AsciiString& name, ModuleType type)
{
	if (name.isEmpty())
		return 0;

	const ModuleTemplate* moduleTemplate = findModuleTemplate(name, type);
	if (moduleTemplate)
	{
		return moduleTemplate->m_whichInterfaces;
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
ModuleData* ModuleFactory::newModuleDataFromINI(INI* ini, const AsciiString& name, ModuleType type, const AsciiString& moduleTag)
{
	if (name.isEmpty())
		return NULL;

	const ModuleTemplate* moduleTemplate = findModuleTemplate(name, type);
	if (moduleTemplate)
	{
		ModuleData* md = (*moduleTemplate->m_createDataProc)(ini);
		md->setModuleTagNameKey( NAMEKEY( moduleTag ) );
		m_moduleDataList.push_back(md);
		return md;
	}

	return NULL;
}

// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
/*static*/ NameKeyType ModuleFactory::makeDecoratedNameKey(const AsciiString& name, ModuleType type)
{
	char tmp[256];
	tmp[0] = '0' + (int)type;
	strcpy(&tmp[1], name.str());
	return TheNameKeyGenerator->nameToKey(tmp);
}

//-------------------------------------------------------------------------------------------------
const ModuleFactory::ModuleTemplate* ModuleFactory::findModuleTemplate(const AsciiString& name, ModuleType type)
{
	NameKeyType namekey = makeDecoratedNameKey(name, type);

  ModuleTemplateMap::const_iterator it = m_moduleTemplateMap.find(namekey);
  if (it == m_moduleTemplateMap.end()) 
	{
		DEBUG_CRASH(( "ModuleFactory::findModuleTemplate: Module name '%s' not found\n", name.str() ));
		return NULL;
	}
	else
	{
		return &(*it).second;
	}
}

//-------------------------------------------------------------------------------------------------
/** Allocate a new acton class istance given the name */
//-------------------------------------------------------------------------------------------------
Module *ModuleFactory::newModule( Thing *thing, const AsciiString& name, const ModuleData* moduleData, ModuleType type )
{
	// sanity
	if( name.isEmpty() )
	{
		DEBUG_CRASH(("attempting to create module with empty name\n"));
		return NULL;
	}
	const ModuleTemplate* mt = findModuleTemplate(name, type);
	if (mt)
	{
		Module* mod = (*mt->m_createProc)( thing, moduleData );

#ifdef _DEBUG
		if (type == MODULETYPE_BEHAVIOR)
		{
			BehaviorModule* bm = (BehaviorModule*)mod;

			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_BODY)) != 0) == (bm->getBody() != NULL), 
				("getInterfaceMask bad for MODULE_BODY (%s)\n",name.str()));
			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_COLLIDE)) != 0) == (bm->getCollide() != NULL), 
				("getInterfaceMask bad for MODULE_COLLIDE (%s)\n",name.str()));
			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_CONTAIN)) != 0) == (bm->getContain() != NULL), 
				("getInterfaceMask bad for MODULE_CONTAIN (%s)\n",name.str()));
			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_CREATE)) != 0) == (bm->getCreate() != NULL), 
				("getInterfaceMask bad for MODULE_CREATE (%s)\n",name.str()));
			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_DAMAGE)) != 0) == (bm->getDamage() != NULL), 
				("getInterfaceMask bad for MODULE_DAMAGE (%s)\n",name.str()));
			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_DESTROY)) != 0) == (bm->getDestroy() != NULL), 
				("getInterfaceMask bad for MODULE_DESTROY (%s)\n",name.str()));
			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_DIE)) != 0) == (bm->getDie() != NULL), 
				("getInterfaceMask bad for MODULE_DIE (%s)\n",name.str()));
			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_SPECIAL_POWER)) != 0) == (bm->getSpecialPower() != NULL), 
				("getInterfaceMask bad for MODULE_SPECIAL_POWER (%s)\n",name.str()));
			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_UPDATE)) != 0) == (bm->getUpdate() != NULL), 
				("getInterfaceMask bad for MODULE_UPDATE (%s)\n",name.str()));
			DEBUG_ASSERTCRASH(
				((mt->m_whichInterfaces & (MODULEINTERFACE_UPGRADE)) != 0) == (bm->getUpgrade() != NULL), 
				("getInterfaceMask bad for MODULE_UPGRADE (%s)\n",name.str()));
		}
#endif

		return mod;
	}

	return NULL;

}  // end newModule

//-------------------------------------------------------------------------------------------------
/** Add a module template to our list of templates */
//-------------------------------------------------------------------------------------------------
void ModuleFactory::addModuleInternal( NewModuleProc proc, NewModuleDataProc dataproc, ModuleType type, const AsciiString& name, Int whichIntf )
{
	NameKeyType namekey = makeDecoratedNameKey(name, type);
	ModuleTemplate& mtm = m_moduleTemplateMap[namekey];	// this creates it if it does not exist already
	mtm.m_createProc = proc;
	mtm.m_createDataProc = dataproc;
	mtm.m_whichInterfaces = whichIntf;
}

//-------------------------------------------------------------------------------------------------
void ModuleFactory::crc( Xfer *xfer )
{
	for (ModuleDataList::iterator mdIt = m_moduleDataList.begin(); mdIt != m_moduleDataList.end(); ++mdIt)
	{
		((ModuleData *)(*mdIt))->crc(xfer);
	}
}

//-------------------------------------------------------------------------------------------------
void ModuleFactory::xfer( Xfer *xfer )
{

	// version
	XferVersion currentVersion = 1;
	XferVersion version = currentVersion;
	xfer->xferVersion( &version, currentVersion );

	for (ModuleDataList::iterator mdIt = m_moduleDataList.begin(); mdIt != m_moduleDataList.end(); ++mdIt)
	{
		((ModuleData *)(*mdIt))->xfer(xfer);
	}
}

//-------------------------------------------------------------------------------------------------
void ModuleFactory::loadPostProcess( void )
{
}
