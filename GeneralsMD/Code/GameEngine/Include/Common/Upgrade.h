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

// FILE: Upgrade.h ////////////////////////////////////////////////////////////////////////////////
// Author: Colin Day, March 2002
// Desc:   Upgrade system for players
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __UPGRADE_H_
#define __UPGRADE_H_

// USER INCLUDES //////////////////////////////////////////////////////////////////////////////////
#include "Common/AudioEventRTS.h"
#include "Common/INI.h"
#include "Common/Snapshot.h"
#include "Common/BitFlags.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class Player;
class UpgradeTemplate;
enum NameKeyType: int;
class Image;
enum AcademyClassificationType: int;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum UpgradeStatusType
{
	UPGRADE_STATUS_INVALID = 0,
	UPGRADE_STATUS_IN_PRODUCTION,
	UPGRADE_STATUS_COMPLETE
};

//The maximum number of upgrades. 
#define UPGRADE_MAX_COUNT 128

typedef BitFlags<UPGRADE_MAX_COUNT>	UpgradeMaskType;

#define MAKE_UPGRADE_MASK(k) UpgradeMaskType(UpgradeMaskType::kInit, (k))
#define MAKE_UPGRADE_MASK2(k,a) UpgradeMaskType(UpgradeMaskType::kInit, (k), (a))
#define MAKE_UPGRADE_MASK3(k,a,b) UpgradeMaskType(UpgradeMaskType::kInit, (k), (a), (b))
#define MAKE_UPGRADE_MASK4(k,a,b,c) UpgradeMaskType(UpgradeMaskType::kInit, (k), (a), (b), (c))
#define MAKE_UPGRADE_MASK5(k,a,b,c,d) UpgradeMaskType(UpgradeMaskType::kInit, (k), (a), (b), (c), (d))

inline Bool TEST_UPGRADE_MASK( const UpgradeMaskType& m, UnsignedInt index ) 
{ 
	return m.test( index ); 
}

inline Bool TEST_UPGRADE_MASK_ANY( const UpgradeMaskType& m, const UpgradeMaskType& mask ) 
{ 
	return m.anyIntersectionWith( mask );
}

inline Bool TEST_UPGRADE_MASK_MULTI( const UpgradeMaskType& m, const UpgradeMaskType& mustBeSet, const UpgradeMaskType& mustBeClear )
{
	return m.testSetAndClear( mustBeSet, mustBeClear );
}

inline Bool UPGRADE_MASK_ANY_SET( const UpgradeMaskType& m) 
{ 
	return m.any(); 
}

inline void CLEAR_UPGRADE_MASK( UpgradeMaskType& m ) 
{ 
	m.clear(); 
}

inline void SET_ALL_UPGRADE_MASK_BITS( UpgradeMaskType& m )
{
	m.clear( );
	m.flip( );
}

inline void FLIP_UPGRADE_MASK( UpgradeMaskType& m )
{
	m.flip();
}


//-------------------------------------------------------------------------------------------------
/** A single upgrade *INSTANCE* */
//-------------------------------------------------------------------------------------------------
class Upgrade : public MemoryPoolObject,
								public Snapshot
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( Upgrade, "Upgrade" )

public:

	Upgrade( const UpgradeTemplate *upgradeTemplate );
	// virtual destructor prototypes provided by memory pool object

	// No copies allowed!
	Upgrade(const Upgrade&) = delete;
	Upgrade& operator=(const Upgrade&) = delete;

	/// get the upgrade template for this instance
	const UpgradeTemplate *getTemplate( void ) const { return m_template; }

	// status access
	UpgradeStatusType getStatus( void ) const { return m_status; }						///< get status
	void setStatus( UpgradeStatusType status ) { m_status = status; }		///< set the status

	// friend access methods
	void friend_setNext( Upgrade *next ) { m_next = next; }
	void friend_setPrev( Upgrade *prev ) { m_prev = prev; }
	Upgrade *friend_getNext( void ) { return m_next; }
	Upgrade *friend_getPrev( void ) { return m_prev; }

protected:

	// snapshot methods
	virtual void crc( Xfer *xfer );
	virtual void xfer( Xfer *xfer );
	virtual void loadPostProcess( void );

	const UpgradeTemplate *m_template {};	///< template this upgrade instance is based on
	UpgradeStatusType m_status {};							///< status of upgrade
	Upgrade *m_next {};				///< next
	Upgrade *m_prev {};				///< prev

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
enum UpgradeType: int
{
	UPGRADE_TYPE_PLAYER = 0,						// upgrade applies to a player as a whole
	UPGRADE_TYPE_OBJECT,								// upgrade applies to an object instance only

	NUM_UPGRADE_TYPES,		// keep this last
};
extern const char *TheUpgradeTypeNames[]; //Change above, change this!

//-------------------------------------------------------------------------------------------------
/** A single upgrade template definition */
//-------------------------------------------------------------------------------------------------
class UpgradeTemplate : public MemoryPoolObject
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( UpgradeTemplate, "UpgradeTemplate" )

public:

	UpgradeTemplate( void );
	// virtual destructor defined by memory pool object

	UpgradeTemplate(const UpgradeTemplate&) = delete;
	UpgradeTemplate& operator=(const UpgradeTemplate&) = default;

	Int calcTimeToBuild( Player *player ) const;			///< time in logic frames it will take this player to "build" this UpgradeTemplate
	Int calcCostToBuild( Player *player ) const;			///< calc the cost to build this upgrade

	// field access
	void setUpgradeName( const AsciiString& name ) { m_name = name; }
	const AsciiString& getUpgradeName( void ) const { return m_name; }
	void setUpgradeNameKey( NameKeyType key ) { m_nameKey = key; }
	NameKeyType getUpgradeNameKey( void ) const { return m_nameKey; }
	const AsciiString& getDisplayNameLabel( void ) const { return m_ini.m_displayNameLabel; }
	UpgradeMaskType getUpgradeMask() const { return m_upgradeMask; }
	UpgradeType getUpgradeType( void ) const { return m_ini.m_type; }
	const AudioEventRTS* getResearchCompleteSound() const { return &m_researchSound; }
	const AudioEventRTS* getUnitSpecificSound() const { return &m_unitSpecificSound; }
	AcademyClassificationType getAcademyClassificationType() const { return m_ini.m_academyClassificationType; }

	/// inventory pictures
	void cacheButtonImage();
	const Image* getButtonImage() const { return m_buttonImage; }

	/// INI parsing
	const FieldParse *getFieldParse() const { return m_upgradeFieldParseTable; }

	// friend access methods for the UpgradeCenter ONLY
	void friend_setNext( UpgradeTemplate *next ) { m_next = next; }
	void friend_setPrev( UpgradeTemplate *prev ) { m_prev = prev; }
	UpgradeTemplate *friend_getNext( void ) { return m_next; }
	UpgradeTemplate *friend_getPrev( void ) { return m_prev; }
	const UpgradeTemplate *friend_getNext( void ) const { return m_next; }
	const UpgradeTemplate *friend_getPrev( void ) const { return m_prev; }
	void friend_setUpgradeMask( UpgradeMaskType mask ) { m_upgradeMask = mask; }
	void friend_makeVeterancyUpgrade(VeterancyLevel v);

protected:

	// MG: Cannot apply offsetof to UpgradeTemplate, so had to move data into an embedded struct.
	struct IniData
	{
		UpgradeType m_type;									///< upgrade type (PLAYER or OBJECT)
		AsciiString m_displayNameLabel;			///< String manager label for UI display name
		Real m_buildTime;										///< database # for how long it takes to "build" this
		Int m_cost;													///< cost for production 
		AsciiString	m_researchSoundName;
		AsciiString	m_unitSpecificSoundName;
		AcademyClassificationType m_academyClassificationType; ///< A value used by the academy to evaluate advice based on what players do.
		AsciiString m_buttonImageName;			///< "Queue" images to show in the build queue
	};

	IniData m_ini {};

	AsciiString m_name {};									///< upgrade name
	NameKeyType m_nameKey {};							///< name key
	UpgradeMaskType m_upgradeMask {};			///< Unique bitmask for this upgrade template
	AudioEventRTS	m_researchSound {};			///< Sound played when upgrade researched.
	AudioEventRTS	m_unitSpecificSound {};	///< Secondary sound played when upgrade researched.

	UpgradeTemplate *m_next {};						///< next
	UpgradeTemplate *m_prev {};						///< prev

	const Image *m_buttonImage {};

	/// INI field table
	static const FieldParse m_upgradeFieldParseTable[];		///< the parse table

	friend class UpgradeCenter;
};

//-------------------------------------------------------------------------------------------------
/** The upgrade center keeps some basic information about the possible upgrades */
//-------------------------------------------------------------------------------------------------
class UpgradeCenter : public SubsystemInterface
{

public:

	UpgradeCenter( void );
	virtual ~UpgradeCenter( void );

	// No copies allowed!
	UpgradeCenter(const UpgradeCenter&) = delete;
	UpgradeCenter& operator=(const UpgradeCenter&) = delete;

	void init( void );												///< subsystem interface
	void reset( void );												///< subsystem interface
	void update( void ) { }										///< subsystem interface

	UpgradeTemplate *firstUpgradeTemplate( void );	///< return the first upgrade template
	const UpgradeTemplate *findUpgradeByKey( NameKeyType key ) const;		///< find upgrade by name key
	const UpgradeTemplate *findUpgrade( const AsciiString& name ) const;				///< find and return upgrade by name
	const UpgradeTemplate *findVeterancyUpgrade(VeterancyLevel level) const;				///< find and return upgrade by name

	UpgradeTemplate *newUpgrade( const AsciiString& name );				///< allocate, link, and return new upgrade

	/// does this player have all the necessary things to make this upgrade
	Bool canAffordUpgrade( Player *player, const UpgradeTemplate *upgradeTemplate, Bool displayReason = FALSE ) const;
	std::vector<AsciiString> getUpgradeNames( void ) const;	// For WorldBuilder only!!!

	static void parseUpgradeDefinition( INI *ini );

protected:

	UpgradeTemplate *findNonConstUpgradeByKey( NameKeyType key );		///< find upgrade by name key

	void linkUpgrade( UpgradeTemplate *upgrade );			///< link upgrade to list
	void unlinkUpgrade( UpgradeTemplate *upgrade );		///< remove upgrade from list

	UpgradeTemplate *m_upgradeList {};										///< list of all upgrades we can have
	UnsignedInt m_nextTemplateMaskBit {};												///< Each instantiated UpgradeTemplate will be given a Int64 bit as an identifier
	Bool buttonImagesCached {};

};

// EXTERNALS //////////////////////////////////////////////////////////////////////////////////////
extern UpgradeCenter *TheUpgradeCenter;

#endif  // end __UPGRADE_H_
