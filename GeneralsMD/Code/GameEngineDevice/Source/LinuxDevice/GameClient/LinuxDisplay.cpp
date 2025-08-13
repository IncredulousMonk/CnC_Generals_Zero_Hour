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
#include "GameClient/Image.h"
#include "Common/FileSystem.h"
#include "LinuxDevice/Common/SdlFileStream.h"
#include <SDL3_image/SDL_image.h>

// DEFINES ////////////////////////////////////////////////////////////////////
#define LOCALISED_TGA_PATH "Data/English/Art/Textures/"

// EXTERNALS //////////////////////////////////////////////////////////////////////////////////////
extern SDL_Renderer* renderer;

//-------------------------------------------------------------------------------------------------
LinuxDisplay::LinuxDisplay()
{
}

LinuxDisplay::~LinuxDisplay()
{
   for (const auto& [key, value] : m_textureCache) {
      if (value.loaded) {
         SDL_DestroyTexture(value.texture);
      }
   }
}

// LinuxDisplay::init =========================================================
/** Initialize or re-initialize the display system.  Here we need to
  * create our window, and get our 3D hardware setup and online */
//=============================================================================
void LinuxDisplay::init()
{
   Display::init();

   // FIXME: Asset manager gets created here.

   auto storeTexturePath {
      [this](Image* image) {
         Texture texture {};
         AsciiString filename {LOCALISED_TGA_PATH};
         filename.concat(image->getFilename());
         if (TheFileSystem->doesFileExist(filename.str())) {
            texture.path = filename;
         } else {
            filename.set(TGA_DIR_PATH);
            filename.concat(image->getFilename());
            if (TheFileSystem->doesFileExist(filename.str())) {
               texture.path = filename;
            } else {
               // Where are they?
               DEBUG_LOG(("Image file not found: %s\n", image->getFilename().str()));
               texture.path = AsciiString::TheEmptyString;
            }
         }
         m_textureCache[image->getFilename()] = texture;
      }
   };

   TheMappedImageCollection->iterate(storeTexturePath);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void LinuxDisplay::doSmartAssetPurgeAndPreload(const char* usageFileName)
{
   (void) usageFileName;
   DEBUG_CRASH(("LinuxDisplay::doSmartAssetPurgeAndPreload not yet implemented."));
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(_INTERNAL)
void LinuxDisplay::dumpAssetUsage(const char* mapname)
{
   (void) mapname;
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
   (void) tod;
   DEBUG_CRASH(("LinuxDisplay::setTimeOfDay not yet implemented."));
}

// LinuxDisplay::createLightPulse =============================================
/** Create a "light pulse" which is a dynamic light that grows, decays 
   * and vanishes over several frames */
//=============================================================================
void LinuxDisplay::createLightPulse(const Coord3D *pos, const RGBColor *color, 
   Real innerRadius, Real attenuationWidth, UnsignedInt increaseFrameTime, UnsignedInt decayFrameTime)
{
   (void) pos;
   (void) color;
   (void) innerRadius;
   (void) attenuationWidth;
   (void) increaseFrameTime;
   (void) decayFrameTime;
   DEBUG_CRASH(("LinuxDisplay::createLightPulse not yet implemented."));
}

// LinuxDisplay::drawLine =====================================================
/** draw a line on the display in pixel coordinates with the specified color */
//=============================================================================
void LinuxDisplay::drawLine(Int startX, Int startY, Int endX, Int endY, 
   Real lineWidth, Color lineColor )
{
   DEBUG_ASSERTCRASH(lineWidth == 1.0f, ("LinuxDisplay::drawLine: One pixel lines only\n"));
   UnsignedByte r {};
   UnsignedByte g {};
   UnsignedByte b {};
   UnsignedByte a {};
   GameGetColorComponents(lineColor, &r, &g, &b, &a);
   SDL_SetRenderDrawColor(renderer, r, g, b, a);
   SDL_RenderLine(renderer, startX, startY, endX, endY);
}  // end drawLine

// LinuxDisplay::drawLine =====================================================
/** draw a line on the display in pixel coordinates with the specified color */
//=============================================================================
void LinuxDisplay::drawLine(Int startX, Int startY, Int endX, Int endY, 
   Real lineWidth, Color lineColor1,Color lineColor2)
{
   (void) startX;
   (void) startY;
   (void) endX;
   (void) endY;
   (void) lineWidth;
   (void) lineColor1;
   (void) lineColor2;
   DEBUG_CRASH(("LinuxDisplay::drawLine two colours not yet implemented."));
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
   (void) startX;
   (void) startY;
   (void) width;
   (void) height;
   (void) percent;
   (void) color;
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
   (void) startX;
   (void) startY;
   (void) width;
   (void) height;
   (void) percent;
   (void) color;
   DEBUG_CRASH(("LinuxDisplay::drawRemainingRectClock not yet implemented."));
}

// LinuxDisplay::drawImage ====================================================
/** Draws an images at the screen coordinates and keeps it within the end
   * screen coords specified */
//=============================================================================
void LinuxDisplay::drawImage( const Image *image, Int startX, Int startY, Int endX, Int endY, Color color, DrawImageMode mode)
{
   (void) color;
   (void) mode;
   if (!image) {
      return;
   }
   // DEBUG_LOG(("LinuxDisplay::drawImage: %s\n", image->getFilename().str()));
   if (!m_textureCache.contains(image->getFilename())) {
      DEBUG_CRASH(("LinuxDisplay::drawImage: Texture not found for image called '%s'\n", image->getFilename().str()));
   }
   Texture tex {m_textureCache[image->getFilename()]};
   if (!tex.loaded) {
      DEBUG_LOG(("Loading texture %s (%s)\n", image->getFilename().str(), tex.path.str()));
      File* textureFile {TheFileSystem->openFile(tex.path.str())};
      SdlFileStream fileStream {textureFile};
      SDL_IOStream* sdlStream {SDL_OpenIO(fileStream.interface(), &fileStream)};
      DEBUG_ASSERTCRASH(sdlStream, (("SDL stream is NULL\n")));
      tex.texture = IMG_LoadTextureTyped_IO(renderer, sdlStream, true, "TGA");
      DEBUG_ASSERTCRASH(tex.texture, ("Texture is NULL: %s\n", SDL_GetError()));
      tex.loaded = TRUE;
      m_textureCache[image->getFilename()] = tex;
   }

   SDL_FRect src {};
   src.x = image->getImageOrigin()->x;
   src.y = image->getImageOrigin()->y;
   src.w = image->getImageSize()->x;
   src.h = image->getImageSize()->y;
   SDL_FRect dest {};
   dest.x = startX;
   dest.y = startY;
   dest.w = endX - startX;
   dest.h = endY - startY;
   SDL_RenderTexture(renderer, tex.texture, &src, &dest);
}  // end drawImage

//============================================================================
// LinuxDisplay::drawVideoBuffer
//============================================================================

void LinuxDisplay::drawVideoBuffer(VideoBuffer *buffer, Int startX, Int startY, Int endX, Int endY)
{
   (void) buffer;
   (void) startX;
   (void) startY;
   (void) endX;
   (void) endY;
   DEBUG_CRASH(("LinuxDisplay::drawVideoBuffer not yet implemented."));
}

//=============================================================================
void LinuxDisplay::setShroudLevel(Int x, Int y, CellShroudStatus setting)
{
   (void) x;
   (void) y;
   (void) setting;
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
   (void) level;
   DEBUG_CRASH(("LinuxDisplay::setBorderShroudLevel not yet implemented."));
}

#if defined(_DEBUG) || defined(_INTERNAL)

//-------------------------------------------------------------------------------------------------
/**  dump all used models/textures to a file.*/
//-------------------------------------------------------------------------------------------------
void LinuxDisplay::dumpModelAssets(const char *path)
{
   (void) path;
   DEBUG_CRASH(("LinuxDisplay::dumpModelAssets not yet implemented."));
}

#endif   //only include above code in debug and internal

//-------------------------------------------------------------------------------------------------
/** Preload using the W3D asset manager the model referenced by the string parameter */
//-------------------------------------------------------------------------------------------------
void LinuxDisplay::preloadModelAssets( AsciiString model )
{
   (void) model;
   DEBUG_CRASH(("LinuxDisplay::preloadModelAssets not yet implemented."));
}  // end preloadModelAssets

//-------------------------------------------------------------------------------------------------
/** Preload using the W3D asset manager the texture referenced by the string parameter */
//-------------------------------------------------------------------------------------------------
void LinuxDisplay::preloadTextureAssets( AsciiString texture )
{
   (void) texture;
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
   (void) enable;
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
