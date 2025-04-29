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

//////// LinuxDisplay.cpp //////////////////////////
// Matthew Gill, April 2025
///////////////////////////////////////////////////////

#include "LinuxDevice/GameClient/LinuxDisplay.h"
#include <SDL3/SDL.h>

extern SDL_Renderer* renderer; // FIXME: How to handle this?

LinuxDisplay::LinuxDisplay()
{
}

LinuxDisplay::~LinuxDisplay()
{
}

// LinuxDisplay::init =========================================================
/** Initialize or re-initialize the display system.  Here we need to
  * create our window, and get our 3D hardware setup and online */
//=============================================================================
void LinuxDisplay::init()
{
   Display::init();

   // FIXME: Asset manager gets created here.
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxDisplay::doSmartAssetPurgeAndPreload(const char* usageFileName)
{
   DEBUG_CRASH(("LinuxDisplay::doSmartAssetPurgeAndPreload not yet implemented."));
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(_INTERNAL)
void LinuxDisplay::dumpAssetUsage(const char* mapname)
{
   DEBUG_CRASH(("LinuxDisplay::dumpAssetUsage not yet implemented."));
}
#endif

//============================================================================
// LinuxDisplay::createVideoBuffer
//============================================================================

VideoBuffer* LinuxDisplay::createVideoBuffer(void)
{
   DEBUG_CRASH(("LinuxDisplay::createVideoBuffer not yet implemented."));
   return nullptr;
}

// LinuxDisplay::setClipRegion ================================================
/** Set the clipping region for images.
  @todo: Make this work for all primitives, not just drawImage. */
//=============================================================================
void LinuxDisplay::setClipRegion( IRegion2D *region )
{
   // assign new region
   m_clipRegion = *region;
   m_isClippedEnabled = TRUE;

}  // end setClipRegion

// LinuxDisplay::setTimeOfDay =================================================
/** */
//=============================================================================
void LinuxDisplay::setTimeOfDay(TimeOfDay tod)
{
   DEBUG_CRASH(("LinuxDisplay::setTimeOfDay not yet implemented."));
}

// LinuxDisplay::createLightPulse =============================================
/** Create a "light pulse" which is a dynamic light that grows, decays 
   * and vanishes over several frames */
//=============================================================================
void LinuxDisplay::createLightPulse(const Coord3D *pos, const RGBColor *color, 
   Real innerRadius, Real attenuationWidth, UnsignedInt increaseFrameTime, UnsignedInt decayFrameTime)
{
   DEBUG_CRASH(("LinuxDisplay::createLightPulse not yet implemented."));
}

// LinuxDisplay::drawLine =====================================================
/** draw a line on the display in pixel coordinates with the specified color */
//=============================================================================
void LinuxDisplay::drawLine(Int startX, Int startY, Int endX, Int endY, 
   Real lineWidth, Color lineColor )
{
   DEBUG_CRASH(("LinuxDisplay::drawLine not yet implemented."));
}  // end drawLine

// LinuxDisplay::drawLine =====================================================
/** draw a line on the display in pixel coordinates with the specified color */
//=============================================================================
void LinuxDisplay::drawLine(Int startX, Int startY, Int endX, Int endY, 
   Real lineWidth, Color lineColor1,Color lineColor2)
{
   DEBUG_CRASH(("LinuxDisplay::drawLine not yet implemented."));
}  // end drawLine

// LinuxDisplay::drawOpenRect =================================================
//=============================================================================
void LinuxDisplay::drawOpenRect(Int startX, Int startY, Int width, Int height, Real lineWidth, Color lineColor)
{
   DEBUG_ASSERTCRASH(lineWidth == 1.0f, ("LinuxDisplay::drawOpenRect: One pixel lines only\n"));
   // FIXME: Do I need to worry about m_isClippedEnabled?
   UnsignedByte r {};
   UnsignedByte g {};
   UnsignedByte b {};
   UnsignedByte a {};
   GameGetColorComponents(lineColor, &r, &g, &b, &a);
   SDL_SetRenderDrawColor(renderer, r, g, b, a);
   SDL_FRect frect {};
   frect.x = startX;
   frect.y = startY;
   frect.w = width;
   frect.h = height;
   SDL_RenderRect(renderer, &frect);
}  // end drawOpenRect

// LinuxDisplay::drawFillRect =================================================
//=============================================================================
void LinuxDisplay::drawFillRect(Int startX, Int startY, Int width, Int height, Color color)
{
   UnsignedByte r {};
   UnsignedByte g {};
   UnsignedByte b {};
   UnsignedByte a {};
   GameGetColorComponents(color, &r, &g, &b, &a);
   SDL_SetRenderDrawColor(renderer, r, g, b, a);
   SDL_FRect frect {};
   frect.x = startX;
   frect.y = startY;
   frect.w = width;
   frect.h = height;
   SDL_RenderFillRect(renderer, &frect);
}  // end drawFillRect

void LinuxDisplay::drawRectClock(Int startX, Int startY, Int width, Int height, Int percent, Color color)
{
   DEBUG_CRASH(("LinuxDisplay::drawRectClock not yet implemented."));
}

//--------------------------------------------------------------------------------------------------------------------
// LinuxDisplay::drawRemainingRectClock
// Variation added by Kris -- October 2002
// This version will overlay a clock progress from the specified percentage to 100%. Essentially, this function will
// "reveal" an icon as it progresses towards completion.
//--------------------------------------------------------------------------------------------------------------------
void LinuxDisplay::drawRemainingRectClock(Int startX, Int startY, Int width, Int height, Int percent, Color color)
{
   DEBUG_CRASH(("LinuxDisplay::drawRemainingRectClock not yet implemented."));
}

// LinuxDisplay::drawImage ====================================================
/** Draws an images at the screen coordinates and keeps it within the end
   * screen coords specified */
//=============================================================================
void LinuxDisplay::drawImage( const Image *image, Int startX, Int startY, Int endX, Int endY, Color color, DrawImageMode mode)
{
   DEBUG_CRASH(("LinuxDisplay::drawImage not yet implemented."));
}  // end drawImage

//============================================================================
// LinuxDisplay::drawVideoBuffer
//============================================================================

void LinuxDisplay::drawVideoBuffer(VideoBuffer *buffer, Int startX, Int startY, Int endX, Int endY)
{
   DEBUG_CRASH(("LinuxDisplay::drawVideoBuffer not yet implemented."));
}

//=============================================================================
void LinuxDisplay::setShroudLevel(Int x, Int y, CellShroudStatus setting)
{
   DEBUG_CRASH(("LinuxDisplay::setShroudLevel not yet implemented."));
}

//=============================================================================
/* we don't really need to override this call, since we will soon be called to
   update every shroud cell explicitly...
*/
void LinuxDisplay::clearShroud()
{
   // nothing
}

//=============================================================================
void LinuxDisplay::setBorderShroudLevel(UnsignedByte level)
{
   DEBUG_CRASH(("LinuxDisplay::setBorderShroudLevel not yet implemented."));
}

#if defined(_DEBUG) || defined(_INTERNAL)

//-------------------------------------------------------------------------------------------------
/**  dump all used models/textures to a file.*/
//-------------------------------------------------------------------------------------------------
void LinuxDisplay::dumpModelAssets(const char *path)
{
   DEBUG_CRASH(("LinuxDisplay::dumpModelAssets not yet implemented."));
}

#endif   //only include above code in debug and internal

//-------------------------------------------------------------------------------------------------
/** Preload using the W3D asset manager the model referenced by the string parameter */
//-------------------------------------------------------------------------------------------------
void LinuxDisplay::preloadModelAssets( AsciiString model )
{
   DEBUG_CRASH(("LinuxDisplay::preloadModelAssets not yet implemented."));
}  // end preloadModelAssets

//-------------------------------------------------------------------------------------------------
/** Preload using the W3D asset manager the texture referenced by the string parameter */
//-------------------------------------------------------------------------------------------------
void LinuxDisplay::preloadTextureAssets( AsciiString texture )
{
   DEBUG_CRASH(("LinuxDisplay::preloadTextureAssets not yet implemented."));
}  // end preloadModelAssets

///Save Screen Capture to a file
void LinuxDisplay::takeScreenShot(void)
{
   DEBUG_CRASH(("LinuxDisplay::takeScreenShot not yet implemented."));
}

/** Start/Stop campturing an AVI movie*/
void LinuxDisplay::toggleMovieCapture(void)
{
   DEBUG_CRASH(("LinuxDisplay::toggleMovieCapture not yet implemented."));
}

void LinuxDisplay::toggleLetterBox(void)
{
   DEBUG_CRASH(("LinuxDisplay::toggleLetterBox not yet implemented."));
}

void LinuxDisplay::enableLetterBox(Bool enable)
{
   DEBUG_CRASH(("LinuxDisplay::enableLetterBox not yet implemented."));
}

Real LinuxDisplay::getAverageFPS()
{
   DEBUG_CRASH(("LinuxDisplay::getAverageFPS not yet implemented."));
   return 0.0f;
}

Int LinuxDisplay::getLastFrameDrawCalls()
{
   DEBUG_CRASH(("LinuxDisplay::getLastFrameDrawCalls not yet implemented."));
   return 0;
}
