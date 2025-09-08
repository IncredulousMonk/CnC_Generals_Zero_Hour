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

///// LinuxDisplay.h /////////////////////////////
// Matthew Gill, April 2025
//////////////////////////////////////////////////

#pragma once

#ifndef __LINUX_DISPLAY_H
#define __LINUX_DISPLAY_H

#include "GameClient/Display.h"
#include "LinuxDevice/GameClient/LinuxFileSystem.h"
#include <SDL3/SDL.h>

struct Texture {
   GameFileClass* gameFile {};
   SDL_Texture* texture {};
   Bool loaded {};
   Bool dds {};
};

using TextureCache = std::unordered_map<AsciiString, Texture, rts::hash<AsciiString>, rts::equal_to<AsciiString>>;

struct DisplayMode {
   int xres;
   int yres;

   bool operator<(const DisplayMode& other) const {
      // Uses > to sort in descending order.
      return (xres > other.xres) || (xres == other.xres && yres > other.yres);
   }
};

class LinuxDisplay: public Display {
public:
   LinuxDisplay();
   virtual ~LinuxDisplay();

   virtual void init();    ///< initialize or re-initialize the system
   // virtual void reset();   ///< Reset system

   // virtual void setWidth(UnsignedInt width);
   // virtual void setHeight(UnsignedInt height);
   virtual Bool setDisplayMode(UnsignedInt xres, UnsignedInt yres, UnsignedInt bitdepth, Bool windowed);
   virtual Int getDisplayModeCount();  ///<return number of display modes/resolutions supported by video card.
   virtual void getDisplayModeDescription(Int modeIndex, UnsignedInt *xres, UnsignedInt *yres, UnsignedInt *bitDepth);  ///<return description of mode
   // virtual void setGamma(Real gamma, Real bright, Real contrast, Bool calibrate);
   virtual void doSmartAssetPurgeAndPreload(const char* usageFileName);
#if defined(_DEBUG) || defined(_INTERNAL)
   virtual void dumpAssetUsage(const char* mapname);
#endif
   virtual VideoBuffer*	createVideoBuffer(void);   ///< Create a video buffer that can be used for this display

   //---------------------------------------------------------------------------------------
   // Drawing management
   virtual void setClipRegion(IRegion2D* region);  ///< Set clip rectangle for 2D draw operations.
   virtual Bool isClippingEnabled(void) { return m_isClippedEnabled; }
   virtual void enableClipping(Bool onoff) { m_isClippedEnabled = onoff; }
	virtual void draw();  ///< redraw the entire display
   virtual void setTimeOfDay(TimeOfDay tod);
   /// @todo Replace these light management routines with a LightManager singleton
   virtual void createLightPulse(const Coord3D* pos, const RGBColor* color, Real innerRadius, Real outerRadius, 
      UnsignedInt increaseFrameTime, UnsignedInt decayFrameTime);
   /// draw a line on the display in screen coordinates
   virtual void drawLine(Int startX, Int startY, Int endX, Int endY, Real lineWidth, Color lineColor);
   /// draw a line on the display in screen coordinates
   virtual void drawLine(Int startX, Int startY, Int endX, Int endY, Real lineWidth, Color lineColor1, Color lineColor2);
   /// draw a rect border on the display in pixel coordinates with the specified color
   virtual void drawOpenRect(Int startX, Int startY, Int width, Int height, Real lineWidth, Color lineColor);
   /// draw a filled rect on the display in pixel coords with the specified color
   virtual void drawFillRect(Int startX, Int startY, Int width, Int height, Color color);
   /// Draw a percentage of a rectangle, much like a clock (0 to x%)
   virtual void drawRectClock(Int startX, Int startY, Int width, Int height, Int percent, Color color);
   /// Draw's the remaining percentage of a rectangle (x% to 100)
   virtual void drawRemainingRectClock(Int startX, Int startY, Int width, Int height, Int percent, Color color);
   /// draw an image fit within the screen coordinates
   virtual void drawImage(const Image *image, Int startX, Int startY,
      Int endX, Int endY, Color color = 0xFFFFFFFF, DrawImageMode mode=DRAW_IMAGE_ALPHA);
   /// draw a video buffer fit within the screen coordinates
   virtual void drawVideoBuffer(VideoBuffer *buffer, Int startX, Int startY, Int endX, Int endY);
   virtual void setShroudLevel(Int x, Int y, CellShroudStatus setting);
   virtual void clearShroud();
   virtual void setBorderShroudLevel(UnsignedByte level);   ///<color that will appear in unused border terrain.
#if defined(_DEBUG) || defined(_INTERNAL)
   virtual void dumpModelAssets(const char *path); ///< dump all used models/textures to a file.
#endif
   virtual void preloadModelAssets(AsciiString model);      ///< preload model asset
   virtual void preloadTextureAssets(AsciiString texture);  ///< preload texture asset
   virtual void takeScreenShot(void);           //save screenshot to file
   virtual void toggleMovieCapture(void);       //enable AVI or frame capture mode.
   virtual void toggleLetterBox(void);          ///<enabled letter-boxed display
   virtual void enableLetterBox(Bool enable);   ///<forces letter-boxed display on/off
   virtual Real getAverageFPS(void);            ///< return the average FPS.
   virtual Int getLastFrameDrawCalls(void);     ///< returns the number of draw calls issued in the previous frame

protected:
   IRegion2D m_clipRegion {}; ///< the clipping region for images
   Bool m_isClippedEnabled {};
   TextureCache m_textureCache {};
   std::vector<DisplayMode> m_displayModes {};
};

#endif // __LINUX_DISPLAY_H
