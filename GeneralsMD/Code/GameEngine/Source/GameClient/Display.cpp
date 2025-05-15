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

// FILE: Display.cpp //////////////////////////////////////////////////////////
// The implementation of the Display class
// Author: Michael S. Booth, March 2001

#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#include "GameClient/Display.h"
#include "GameClient/Mouse.h"
#include "GameClient/VideoPlayer.h"
#include "GameClient/DisplayStringManager.h"
#include "GameClient/GameText.h"
#include "GameClient/GlobalLanguage.h"
//#include "GameLogic/ScriptEngine.h"
//#include "GameLogic/GameLogic.h"

/// The Display singleton instance.
Display *TheDisplay = NULL;


Display::Display()
{
	m_viewList = NULL;
	m_width = 800;
	m_height = 600;
	m_bitDepth = 0;
	m_windowed = FALSE;
	m_videoBuffer = NULL;
	m_videoStream = NULL;
	m_debugDisplayCallback = NULL;
	m_debugDisplayUserData = NULL;
	m_debugDisplay = NULL;
	m_letterBoxFadeLevel = 0;
	m_letterBoxEnabled = FALSE;
	m_cinematicText = AsciiString::TheEmptyString;
	m_cinematicFont = NULL;
	m_cinematicTextFrames = 0;  
	m_movieHoldTime	= -1;
	m_copyrightHoldTime = -1;
	m_movieStartTime = std::chrono::steady_clock::time_point::min();
	m_elapsedCopywriteTime = std::chrono::steady_clock::time_point::min();
	m_copyrightDisplayString = NULL;

	// Added by Sadullah Nader
	// Initializations missing and needed
	m_currentlyPlayingMovie.clear();
	m_letterBoxFadeStartTime = 0;
	// End Add
}

/**
 * Destructor for the Display.  Destroy all views attached to it.
 */
Display::~Display()
{

	stopMovie();
	// delete all our views if present
	deleteViews();

}

/**
	* Delete all views in the Display
	*/
void Display::deleteViews( void )
{
	View *v, *next;

	for( v = m_viewList; v; v = next )
	{
		next = v->getNextView();
		delete v;
	}
	m_viewList = NULL;
}

/**
 * Attach the given view to the world
 * @todo Rethink the "attachView" notion...
 */
void Display::attachView( View *view )
{
	// prepend to head of list
	m_viewList = view->prependViewToList( m_viewList );
}

/**
 * Render all views of the world
 */
void Display::drawViews( void )
{

	for( View *v = m_viewList; v; v = v->getNextView() )
		v->drawView();

}

/**
 * Updates all views of the world.  This forces state variables
   to refresh without actually drawing anything.
 */
void Display::updateViews( void )
{

	for( View *v = m_viewList; v; v = v->getNextView() )
		v->updateView();

}

/// Redraw the entire display
void Display::draw( void )
{
	// redraw all views
	drawViews();
	
	// redraw the in-game user interface
	/// @todo Switch between in-game and shell interfaces

}

/** Sets screen resolution/mode*/
Bool Display::setDisplayMode( UnsignedInt xres, UnsignedInt yres, UnsignedInt /*bitdepth*/, Bool /*windowed*/ )
{
	// FIXME: Uncomment when TheTacticalView is implemented.
	// //Get old values
	// UnsignedInt oldDisplayHeight=getHeight();
	// UnsignedInt oldDisplayWidth=getWidth();
	// Int oldViewWidth=TheTacticalView->getWidth();
	// Int oldViewHeight=TheTacticalView->getHeight();
	// Int oldViewOriginX,oldViewOriginY;
	// TheTacticalView->getOrigin(&oldViewOriginX,&oldViewOriginY);

	setWidth(xres);
	setHeight(yres);

	// //Adjust view to match previous proportions
	// TheTacticalView->setWidth((Real)oldViewWidth/(Real)oldDisplayWidth*(Real)xres);
	// TheTacticalView->setHeight((Real)oldViewHeight/(Real)oldDisplayHeight*(Real)yres);
	// TheTacticalView->setOrigin((Real)oldViewOriginX/(Real)oldDisplayWidth*(Real)xres,
	// (Real)oldViewOriginY/(Real)oldDisplayHeight*(Real)yres);
	return TRUE;
}

// Display::setWidth ==========================================================
/** Set the width of the display */
//=============================================================================
void Display::setWidth( UnsignedInt width )
{

	// set the new width
	m_width = width;

	// FIXME: Uncomment when TheMouse is implemented.
	// // set the new mouse limits
	// if( TheMouse )
	// 	TheMouse->setMouseLimits();

}  // end setWidth

// Display::setHeight =========================================================
/** Set the height of the display */
//=============================================================================
void Display::setHeight( UnsignedInt height )
{

	// se the new height
	m_height = height;

	// FIXME: Uncomment when TheMouse is implemented.
	// // set the new mouse limits
	// if( TheMouse )
	// 	TheMouse->setMouseLimits();

}  // end setHeight

//============================================================================
// Display::playLogoMovie
// minMovieLength is in milliseconds
// minCopyrightLength
//============================================================================

void Display::playLogoMovie( AsciiString movieName, Int minMovieLength, Int minCopyrightLength )
{
	
	stopMovie();

	m_videoStream = TheVideoPlayer->open( movieName );

	if ( m_videoStream == NULL )
	{
		return;
	}
	
	m_currentlyPlayingMovie = movieName;
	m_movieHoldTime = minMovieLength;
	m_copyrightHoldTime = minCopyrightLength;
	m_movieStartTime = std::chrono::steady_clock::now();  // we're using time get time because legal want's actual "Seconds"
	
	// m_videoBuffer = createVideoBuffer();
	// if (m_videoBuffer == NULL || 
	// 		!m_videoBuffer->allocate(static_cast<UnsignedInt>(m_videoStream->width()), 
	// 			static_cast<UnsignedInt>(m_videoStream->height()))
	// 	)
	// {
	// 	stopMovie();
	// 	return;
	// }
	
}

//============================================================================
// Display::playMovie
//============================================================================

void Display::playMovie( AsciiString movieName)
{
	
	stopMovie();



	m_videoStream = TheVideoPlayer->open( movieName );

	if ( m_videoStream == NULL )
	{
		return;
	}
	
	m_currentlyPlayingMovie = movieName;

	// m_videoBuffer = createVideoBuffer();
	// if (m_videoBuffer == NULL || 
	// 		!m_videoBuffer->allocate(static_cast<UnsignedInt>(m_videoStream->width()), 
	// 			static_cast<UnsignedInt>(m_videoStream->height()))
	// 	)
	// {
	// 	stopMovie();
	// 	return;
	// }
	
}

//============================================================================
// Display::stopMovie
//============================================================================

void Display::stopMovie( void )
{
	// delete m_videoBuffer;
	// m_videoBuffer = NULL;

	if ( m_videoStream )
	{
		m_videoStream->close();
		m_videoStream = NULL;
	}

	if (!m_currentlyPlayingMovie.isEmpty()) {
		//TheScriptEngine->notifyOfCompletedVideo(m_currentlyPlayingMovie); // Removing this sync-error cause MDC
		m_currentlyPlayingMovie = AsciiString::TheEmptyString;
	}
	// if(m_copyrightDisplayString)
	// {
	// 	TheDisplayStringManager->freeDisplayString(m_copyrightDisplayString);
	// 	m_copyrightDisplayString = NULL;
	// }
	m_copyrightHoldTime = -1;
	m_movieHoldTime = -1;
}

//============================================================================
// Display::update
//============================================================================

void Display::update( void )
{
	// if ( m_videoStream && m_videoBuffer )
	// {
	// 	if ( m_videoStream->isFrameReady())
	// 	{
	// 		m_videoStream->frameDecompress();
	// 		m_videoStream->frameRender( m_videoBuffer );
	// 		if (m_videoStream->frameIndex() != m_videoStream->frameCount() - 1) {
	// 			m_videoStream->frameNext();
	// 		} else if (m_copyrightHoldTime >= 0 || m_movieHoldTime >= 0) {
	// 			// MG: I don't think that this copyright display message thing works.
	// 			// if (m_elapsedCopywriteTime == 0 && m_elapsedCopywriteTime >= 0) {
	// 			// 	//display the copyrighttext;
	// 			// 	if (m_copyrightDisplayString) {
	// 			// 		m_copyrightDisplayString->deleteInstance();
	// 			// 	}
	// 			// 	m_copyrightDisplayString = TheDisplayStringManager->newDisplayString();
	// 			// 	m_copyrightDisplayString->setText(TheGameText->fetch("GUI:EACopyright"));
	// 			// 	if (TheGlobalLanguageData && TheGlobalLanguageData->m_copyrightFont.name.isNotEmpty())
	// 			// 	{
	// 			// 		FontDesc *fontdesc=&TheGlobalLanguageData->m_copyrightFont;
	// 			// 		m_copyrightDisplayString->setFont(TheFontLibrary->getFont(fontdesc->name,
	// 			// 			TheGlobalLanguageData->adjustFontSize(fontdesc->size),
	// 			// 			fontdesc->bold));	
	// 			// 	} else {
	// 			// 		m_copyrightDisplayString->setFont(TheFontLibrary->getFont("Courier", 
	// 			// 		TheGlobalLanguageData->adjustFontSize(12), TRUE));	
	// 			// 	}
	// 			// 	m_elapsedCopywriteTime = std::chrono::steady_clock::now();
	// 			// }
	// 			auto now {std::chrono::steady_clock::now()};
	// 			std::chrono::duration<double, std::milli> movieElapsed {now - m_movieStartTime};
	// 			if (m_movieHoldTime + m_copyrightHoldTime < movieElapsed.count()) {
	// 				m_movieHoldTime = -1;
	// 				m_movieStartTime = std::chrono::steady_clock::time_point::min();
	// 				m_elapsedCopywriteTime = std::chrono::steady_clock::time_point::min();
	// 				m_copyrightHoldTime = -1;
	// 			}
	// 		}
	// 		else {
	// 			stopMovie();
	// 		}
	// 	}
	// }
	if (m_videoStream) {
		if (m_videoStream->isVideoFinished()) {
			stopMovie();
		}
	}
}

//============================================================================
// Display::reset
//============================================================================

void Display::reset()
{
	//Remove letterbox border that may have been enabled by a script
	m_letterBoxFadeLevel = 0;
	m_letterBoxEnabled = FALSE;
	stopMovie();

	// Reset all views that need resetting
	for( View *v = m_viewList; v; v = v->getNextView() )
		v->reset();
}

//============================================================================
// Display::isMoviePlaying
//============================================================================

Bool Display::isMoviePlaying(void)
{
	// return m_videoStream != NULL && m_videoBuffer != NULL;
	return m_videoStream && !m_currentlyPlayingMovie.isEmpty();
}

//============================================================================
// Display::setDebugDisplayCallback
//============================================================================

void Display::setDebugDisplayCallback( DebugDisplayCallback *callback, void *userData )
{
	m_debugDisplayCallback = callback;
	m_debugDisplayUserData = userData;
}

//============================================================================
// Display::getDebugDisplayCallback
//============================================================================

Display::DebugDisplayCallback *Display::getDebugDisplayCallback()
{
	return m_debugDisplayCallback;
}
