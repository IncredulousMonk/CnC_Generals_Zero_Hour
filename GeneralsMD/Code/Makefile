CXXFLAGS = -std=c++23 -O2 -Wall -Weffc++ -Wextra -Wsign-conversion -Werror -pedantic-errors -D_UNIX
LDPATHS = -LGameEngineDevice/Lib -LGameEngine/Lib -LLibraries/Lib
LDFLAGS = -lavutil -lavcodec -lswscale -lswresample -lGL -lz
CXXFLAGS += $(shell pkg-config sdl3 --cflags)
LDFLAGS += $(shell pkg-config sdl3 --libs) $(shell pkg-config sdl3-ttf --libs) $(shell pkg-config sdl3-sound --libs)
INCS = -I. -ILibraries/Include -ILibraries/Source/WWVegas -ILibraries/Source/WWVegas/WWLib -ILibraries/Source/WWVegas/WWMath -ILibraries/Source/WWVegas/WWSaveLoad -ILibraries/Source/WWVegas/WWDebug \
   -ILibraries/Source/WWVegas/WW3D2 -ILibraries/Source/OGL -ILibraries/Source/Compression -IGameEngine/Include -IGameEngineDevice/Include -IGameEngine/Include/Precompiled
PROG = ../Run/rts
ENGINE_DEVICE_LIB = GameEngineDevice/Lib/GameEngineDevice.a
ENGINE_LIB = GameEngine/Lib/GameEngine.a
COMPRESSION_LIB = Libraries/Lib/Compression.a
OGL_LIB = Libraries/Lib/OGL.a
WWDEBUG_LIB = Libraries/Lib/WWDebug.a
WWLIB_LIB = Libraries/Lib/WWLib.a
WWMATH_LIB = Libraries/Lib/WWMath.a
WW3D2_LIB = Libraries/Lib/WW3D2.a
GEDO = GameEngineDevice/Obj
GEDSLDC = GameEngineDevice/Source/LinuxDevice/Common
GEDSLDF = GameEngineDevice/Source/LinuxDevice/FFmpeg
GEDSLD = GameEngineDevice/Source/LinuxDevice
GEO = GameEngine/Obj
GES = GameEngine/Source
LIBO = Libraries/Obj
COMPS = Libraries/Source/Compression
OGLS = Libraries/Source/OGL
WWDS = Libraries/Source/WWVegas/WWDebug
WWLS = Libraries/Source/WWVegas/WWLib
WWMS = Libraries/Source/WWVegas/WWMath
WW3S = Libraries/Source/WWVegas/WW3D2
CUR_DIR := $(shell pwd)


ifdef DEBUG
CXXFLAGS += -D_DEBUG -O0 -g3
endif

.PHONY: run debug clean

$(PROG): Main/main.cpp $(ENGINE_LIB) $(ENGINE_DEVICE_LIB) $(COMPRESSION_LIB) $(OGL_LIB) $(WWDEBUG_LIB) $(WWLIB_LIB) $(WW3D2_LIB) $(WWMATH_LIB)
	g++ $(CXXFLAGS) $(INCS) -o $(PROG) Main/main.cpp $(LDPATHS) -l:GameEngineDevice.a -l:GameEngine.a -l:GameEngineDevice.a -l:OGL.a -l:WWLib.a -l:WWMath.a -l:WW3D2.a -l:Compression.a $(LDFLAGS)


# ===== Compression =====

COMPRESSION_OBJS = $(LIBO)/CompressionManager.o $(LIBO)/btreedecode.o $(LIBO)/btreeencode.o $(LIBO)/huffdecode.o $(LIBO)/huffencode.o $(LIBO)/refdecode.o $(LIBO)/refencode.o

$(COMPRESSION_LIB): $(COMPRESSION_OBJS)
	ar rcs $(COMPRESSION_LIB)  $(COMPRESSION_OBJS)

$(LIBO)/CompressionManager.o: $(COMPS)/CompressionManager.cpp $(COMPS)/Compression.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/CompressionManager.o $(COMPS)/CompressionManager.cpp

$(LIBO)/btreedecode.o: $(COMPS)/EAC/btreedecode.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/btreedecode.o $(COMPS)/EAC/btreedecode.cpp

$(LIBO)/btreeencode.o: $(COMPS)/EAC/btreeencode.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/btreeencode.o $(COMPS)/EAC/btreeencode.cpp

$(LIBO)/huffdecode.o: $(COMPS)/EAC/huffdecode.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/huffdecode.o $(COMPS)/EAC/huffdecode.cpp

$(LIBO)/huffencode.o: $(COMPS)/EAC/huffencode.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/huffencode.o $(COMPS)/EAC/huffencode.cpp

$(LIBO)/refdecode.o: $(COMPS)/EAC/refdecode.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/refdecode.o $(COMPS)/EAC/refdecode.cpp

$(LIBO)/refencode.o: $(COMPS)/EAC/refencode.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/refencode.o $(COMPS)/EAC/refencode.cpp


# ===== OGL =====

OGL_OBJS = $(LIBO)/OpenGLRenderer.o

$(OGL_LIB): $(OGL_OBJS)
	ar rcs $(OGL_LIB)  $(OGL_OBJS)

$(LIBO)/OpenGLRenderer.o: $(OGLS)/OpenGLRenderer.cpp $(OGLS)/OpenGLRenderer.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/OpenGLRenderer.o $(OGLS)/OpenGLRenderer.cpp


# ===== WWDebug =====

WWDEBUG_OBJS = $(LIBO)/wwdebug.o $(LIBO)/wwmemlog.o $(LIBO)/wwprofile.o

$(WWDEBUG_LIB): $(WWDEBUG_OBJS)
	ar rcs $(WWDEBUG_LIB)  $(WWDEBUG_OBJS)

# --- Libraries/Source/WWVegas/WWDebug ---
$(LIBO)/wwdebug.o: $(WWDS)/wwdebug.cpp $(WWDS)/wwdebug.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/wwdebug.o $(WWDS)/wwdebug.cpp

$(LIBO)/wwmemlog.o: $(WWDS)/wwmemlog.cpp $(WWDS)/wwmemlog.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/wwmemlog.o $(WWDS)/wwmemlog.cpp

$(LIBO)/wwprofile.o: $(WWDS)/wwprofile.cpp $(WWDS)/wwprofile.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/wwprofile.o $(WWDS)/wwprofile.cpp


# ===== WWLib =====

WWLIB_OBJS = $(LIBO)/bufffile.o $(LIBO)/ffactory.o $(LIBO)/mutex.o $(LIBO)/rawfile.o $(LIBO)/refcount.o $(LIBO)/targa.o $(LIBO)/wwfile.o $(LIBO)/wwstring.o

$(WWLIB_LIB): $(WWLIB_OBJS)
	ar rcs $(WWLIB_LIB)  $(WWLIB_OBJS)

# --- Libraries/Source/WWVegas/WWLib ---
$(LIBO)/bufffile.o: $(WWLS)/bufffile.cpp $(WWLS)/bufffile.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/bufffile.o $(WWLS)/bufffile.cpp

$(LIBO)/ffactory.o: $(WWLS)/ffactory.cpp $(WWLS)/ffactory.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/ffactory.o $(WWLS)/ffactory.cpp

$(LIBO)/mutex.o: $(WWLS)/mutex.cpp $(WWLS)/mutex.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/mutex.o $(WWLS)/mutex.cpp

$(LIBO)/rawfile.o: $(WWLS)/rawfile.cpp $(WWLS)/RAWFILE.H
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/rawfile.o $(WWLS)/rawfile.cpp

$(LIBO)/refcount.o: $(WWLS)/refcount.cpp $(WWLS)/refcount.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/refcount.o $(WWLS)/refcount.cpp

$(LIBO)/targa.o: $(WWLS)/TARGA.CPP $(WWLS)/TARGA.H
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/targa.o $(WWLS)/TARGA.CPP

$(LIBO)/wwfile.o: $(WWLS)/wwfile.cpp $(WWLS)/WWFILE.H
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/wwfile.o $(WWLS)/wwfile.cpp

$(LIBO)/wwstring.o: $(WWLS)/wwstring.cpp $(WWLS)/wwstring.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/wwstring.o $(WWLS)/wwstring.cpp


# ===== WWMath =====

WWMATH_OBJS = $(LIBO)/matrix3d.o $(LIBO)/quat.o

$(WWMATH_LIB): $(WWMATH_OBJS)
	ar rcs $(WWMATH_LIB)  $(WWMATH_OBJS)

# --- Libraries/Source/WWVegas/WWMath ---
$(LIBO)/matrix3d.o: $(WWMS)/matrix3d.cpp $(WWMS)/matrix3d.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/matrix3d.o $(WWMS)/matrix3d.cpp

$(LIBO)/quat.o: $(WWMS)/quat.cpp $(WWMS)/quat.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/quat.o $(WWMS)/quat.cpp


# ===== WW3D2 =====

WW3D2_OBJS = $(LIBO)/bitmaphandler.o $(LIBO)/ddsfile.o $(LIBO)/formconv.o $(LIBO)/texture.o $(LIBO)/texturefilter.o $(LIBO)/ww3dformat.o

$(WW3D2_LIB): $(WW3D2_OBJS)
	ar rcs $(WW3D2_LIB)  $(WW3D2_OBJS)

# --- Libraries/Source/WWVegas/WW3D2 ---
$(LIBO)/bitmaphandler.o: $(WW3S)/bitmaphandler.cpp $(WW3S)/bitmaphandler.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/bitmaphandler.o $(WW3S)/bitmaphandler.cpp

$(LIBO)/ddsfile.o: $(WW3S)/ddsfile.cpp $(WW3S)/ddsfile.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/ddsfile.o $(WW3S)/ddsfile.cpp

$(LIBO)/formconv.o: $(WW3S)/formconv.cpp $(WW3S)/formconv.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/formconv.o $(WW3S)/formconv.cpp

$(LIBO)/texture.o: $(WW3S)/texture.cpp $(WW3S)/texture.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/texture.o $(WW3S)/texture.cpp

$(LIBO)/texturefilter.o: $(WW3S)/texturefilter.cpp $(WW3S)/texturefilter.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/texturefilter.o $(WW3S)/texturefilter.cpp

$(LIBO)/ww3dformat.o: $(WW3S)/ww3dformat.cpp $(WW3S)/ww3dformat.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(LIBO)/ww3dformat.o $(WW3S)/ww3dformat.cpp


# ===== GameEngineDevice =====

ENGINE_DEVICE_OBJS = $(GEDO)/SdlAudioManager.o \
$(GEDO)/LinuxBIGFile.o $(GEDO)/LinuxBIGFileSystem.o $(GEDO)/LinuxGameEngine.o $(GEDO)/LinuxLocalFile.o $(GEDO)/LinuxLocalFileSystem.o $(GEDO)/SdlFileStream.o \
$(GEDO)/LinuxFunctionLexicon.o $(GEDO)/LinuxRadar.o \
$(GEDO)/LinuxModuleFactory.o \
$(GEDO)/FFmpegVideoPlayer.o $(GEDO)/FFmpegVideo.o $(GEDO)/PacketQueue.o $(GEDO)/FrameQueue.o $(GEDO)/Decoder.o $(GEDO)/Clock.o \
$(GEDO)/LinuxDisplay.o $(GEDO)/LinuxDisplayString.o $(GEDO)/LinuxDisplayStringManager.o $(GEDO)/LinuxFileSystem.o $(GEDO)/LinuxGameClient.o $(GEDO)/LinuxInGameUI.o \
   $(GEDO)/LinuxParticleSys.o $(GEDO)/LinuxView.o $(GEDO)/SdlKeyboard.o $(GEDO)/SdlMouse.o $(GEDO)/TerrainTex.o $(GEDO)/TileData.o $(GEDO)/WorldHeightMap.o \
$(GEDO)/LinuxDefaultDraw.o $(GEDO)/LinuxModelDraw.o $(GEDO)/LinuxScienceModelDraw.o \
$(GEDO)/LinuxGameFont.o $(GEDO)/LinuxGameWindow.o $(GEDO)/LinuxGameWindowManager.o \
$(GEDO)/LinuxCheckBox.o $(GEDO)/LinuxHorizontalSlider.o $(GEDO)/LinuxListBox.o $(GEDO)/LinuxProgressBar.o $(GEDO)/LinuxPushButton.o $(GEDO)/LinuxRadioButton.o $(GEDO)/LinuxStaticText.o \
   $(GEDO)/LinuxTextEntry.o $(GEDO)/LinuxVerticalSlider.o \
$(GEDO)/LinuxControlBar.o $(GEDO)/LinuxMainMenu.o $(GEDO)/LinuxMOTD.o \
$(GEDO)/LinuxGameLogic.o $(GEDO)/LinuxTerrainLogic.o

$(ENGINE_DEVICE_LIB): $(ENGINE_DEVICE_OBJS)
	ar rcs $(ENGINE_DEVICE_LIB)  $(ENGINE_DEVICE_OBJS)

# --- GameEngineDevice/Source/LinuxDevice/Audio ---
$(GEDO)/SdlAudioManager.o: $(GEDSLD)/Audio/SdlAudioManager.cpp GameEngineDevice/Include/LinuxDevice/Audio/SdlAudioManager.h GameEngine/Include/Common/AudioEventInfo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/SdlAudioManager.o $(GEDSLD)/Audio/SdlAudioManager.cpp

# --- GameEngineDevice/Source/LinuxDevice/Common ---
$(GEDO)/LinuxBIGFile.o: $(GEDSLDC)/LinuxBIGFile.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxBIGFile.o $(GEDSLDC)/LinuxBIGFile.cpp

$(GEDO)/LinuxBIGFileSystem.o: $(GEDSLDC)/LinuxBIGFileSystem.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxBIGFileSystem.o $(GEDSLDC)/LinuxBIGFileSystem.cpp

$(GEDO)/LinuxGameEngine.o: $(GEDSLDC)/LinuxGameEngine.cpp GameEngineDevice/Include/LinuxDevice/Common/LinuxGameEngine.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameEngine.o $(GEDSLDC)/LinuxGameEngine.cpp

$(GEDO)/LinuxLocalFile.o: $(GEDSLDC)/LinuxLocalFile.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxLocalFile.o $(GEDSLDC)/LinuxLocalFile.cpp

$(GEDO)/LinuxLocalFileSystem.o: $(GEDSLDC)/LinuxLocalFileSystem.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxLocalFileSystem.o $(GEDSLDC)/LinuxLocalFileSystem.cpp

$(GEDO)/SdlFileStream.o: $(GEDSLDC)/SdlFileStream.cpp GameEngineDevice/Include/LinuxDevice/Common/SdlFileStream.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/SdlFileStream.o $(GEDSLDC)/SdlFileStream.cpp

# --- GameEngineDevice/Source/LinuxDevice/Common/System ---
$(GEDO)/LinuxFunctionLexicon.o: $(GEDSLD)/Common/System/LinuxFunctionLexicon.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxFunctionLexicon.o $(GEDSLD)/Common/System/LinuxFunctionLexicon.cpp

$(GEDO)/LinuxRadar.o: $(GEDSLD)/Common/System/LinuxRadar.cpp GameEngineDevice/Include/LinuxDevice/Common/LinuxRadar.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxRadar.o $(GEDSLD)/Common/System/LinuxRadar.cpp

# --- GameEngineDevice/Source/LinuxDevice/Common/Thing ---
$(GEDO)/LinuxModuleFactory.o: $(GEDSLD)/Common/Thing/LinuxModuleFactory.cpp GameEngineDevice/Include/LinuxDevice/Common/LinuxModuleFactory.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxModuleFactory.o $(GEDSLD)/Common/Thing/LinuxModuleFactory.cpp

# --- GameEngineDevice/Source/LinuxDevice/FFmpeg ---
$(GEDO)/FFmpegVideoPlayer.o: $(GEDSLDF)/FFmpegVideoPlayer.cpp GameEngineDevice/Include/LinuxDevice/FFmpeg/FFmpegVideoPlayer.h GameEngineDevice/Include/LinuxDevice/FFmpeg/FFmpegVideo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/FFmpegVideoPlayer.o $(GEDSLDF)/FFmpegVideoPlayer.cpp

$(GEDO)/FFmpegVideo.o: $(GEDSLDF)/FFmpegVideo.cpp GameEngineDevice/Include/LinuxDevice/FFmpeg/FFmpegVideo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/FFmpegVideo.o $(GEDSLDF)/FFmpegVideo.cpp

$(GEDO)/PacketQueue.o: $(GEDSLDF)/PacketQueue.cpp GameEngineDevice/Include/LinuxDevice/FFmpeg/PacketQueue.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/PacketQueue.o $(GEDSLDF)/PacketQueue.cpp

$(GEDO)/FrameQueue.o: $(GEDSLDF)/FrameQueue.cpp GameEngineDevice/Include/LinuxDevice/FFmpeg/FrameQueue.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/FrameQueue.o $(GEDSLDF)/FrameQueue.cpp

$(GEDO)/Decoder.o: $(GEDSLDF)/Decoder.cpp GameEngineDevice/Include/LinuxDevice/FFmpeg/Decoder.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/Decoder.o $(GEDSLDF)/Decoder.cpp

$(GEDO)/Clock.o: $(GEDSLDF)/Clock.cpp GameEngineDevice/Include/LinuxDevice/FFmpeg/Clock.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/Clock.o $(GEDSLDF)/Clock.cpp

# --- GameEngineDevice/Source/LinuxDevice/GameClient ---
$(GEDO)/LinuxDisplay.o: $(GEDSLD)/GameClient/LinuxDisplay.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxDisplay.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxDisplay.o $(GEDSLD)/GameClient/LinuxDisplay.cpp

$(GEDO)/LinuxDisplayString.o: $(GEDSLD)/GameClient/LinuxDisplayString.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxDisplayString.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxDisplayString.o $(GEDSLD)/GameClient/LinuxDisplayString.cpp

$(GEDO)/LinuxDisplayStringManager.o: $(GEDSLD)/GameClient/LinuxDisplayStringManager.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxDisplayStringManager.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxDisplayStringManager.o $(GEDSLD)/GameClient/LinuxDisplayStringManager.cpp

$(GEDO)/LinuxFileSystem.o: $(GEDSLD)/GameClient/LinuxFileSystem.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxFileSystem.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxFileSystem.o $(GEDSLD)/GameClient/LinuxFileSystem.cpp

$(GEDO)/LinuxGameClient.o: $(GEDSLD)/GameClient/LinuxGameClient.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGameClient.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameClient.o $(GEDSLD)/GameClient/LinuxGameClient.cpp

$(GEDO)/LinuxInGameUI.o: $(GEDSLD)/GameClient/LinuxInGameUI.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxInGameUI.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxInGameUI.o $(GEDSLD)/GameClient/LinuxInGameUI.cpp

$(GEDO)/LinuxParticleSys.o: $(GEDSLD)/GameClient/LinuxParticleSys.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxParticleSys.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxParticleSys.o $(GEDSLD)/GameClient/LinuxParticleSys.cpp

$(GEDO)/LinuxView.o: $(GEDSLD)/GameClient/LinuxView.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxView.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxView.o $(GEDSLD)/GameClient/LinuxView.cpp

$(GEDO)/SdlKeyboard.o: $(GEDSLD)/GameClient/SdlKeyboard.cpp GameEngineDevice/Include/LinuxDevice/GameClient/SdlKeyboard.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/SdlKeyboard.o $(GEDSLD)/GameClient/SdlKeyboard.cpp

$(GEDO)/SdlMouse.o: $(GEDSLD)/GameClient/SdlMouse.cpp GameEngineDevice/Include/LinuxDevice/GameClient/SdlMouse.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/SdlMouse.o $(GEDSLD)/GameClient/SdlMouse.cpp

$(GEDO)/TerrainTex.o: $(GEDSLD)/GameClient/TerrainTex.cpp GameEngineDevice/Include/LinuxDevice/GameClient/TerrainTex.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/TerrainTex.o $(GEDSLD)/GameClient/TerrainTex.cpp

$(GEDO)/TileData.o: $(GEDSLD)/GameClient/TileData.cpp GameEngineDevice/Include/LinuxDevice/GameClient/TileData.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/TileData.o $(GEDSLD)/GameClient/TileData.cpp

$(GEDO)/WorldHeightMap.o: $(GEDSLD)/GameClient/WorldHeightMap.cpp GameEngineDevice/Include/LinuxDevice/GameClient/WorldHeightMap.h GameEngine/Include/Common/MapObject.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/WorldHeightMap.o $(GEDSLD)/GameClient/WorldHeightMap.cpp

# --- GameEngineDevice/Source/LinuxDevice/GameClient/Drawable/Draw ---
$(GEDO)/LinuxDefaultDraw.o: $(GEDSLD)/GameClient/Drawable/Draw/LinuxDefaultDraw.cpp GameEngineDevice/Include/LinuxDevice/GameClient/Module/LinuxDefaultDraw.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxDefaultDraw.o $(GEDSLD)/GameClient/Drawable/Draw/LinuxDefaultDraw.cpp

$(GEDO)/LinuxModelDraw.o: $(GEDSLD)/GameClient/Drawable/Draw/LinuxModelDraw.cpp GameEngineDevice/Include/LinuxDevice/GameClient/Module/LinuxModelDraw.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxModelDraw.o $(GEDSLD)/GameClient/Drawable/Draw/LinuxModelDraw.cpp

$(GEDO)/LinuxScienceModelDraw.o: $(GEDSLD)/GameClient/Drawable/Draw/LinuxScienceModelDraw.cpp GameEngineDevice/Include/LinuxDevice/GameClient/Module/LinuxScienceModelDraw.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxScienceModelDraw.o $(GEDSLD)/GameClient/Drawable/Draw/LinuxScienceModelDraw.cpp

# --- GameEngineDevice/Source/LinuxDevice/GameClient/GUI ---
$(GEDO)/LinuxGameFont.o: $(GEDSLD)/GameClient/GUI/LinuxGameFont.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGameFont.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameFont.o $(GEDSLD)/GameClient/GUI/LinuxGameFont.cpp

$(GEDO)/LinuxGameWindow.o: $(GEDSLD)/GameClient/GUI/LinuxGameWindow.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGameWindow.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameWindow.o $(GEDSLD)/GameClient/GUI/LinuxGameWindow.cpp

$(GEDO)/LinuxGameWindowManager.o: $(GEDSLD)/GameClient/GUI/LinuxGameWindowManager.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGameWindowManager.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameWindowManager.o $(GEDSLD)/GameClient/GUI/LinuxGameWindowManager.cpp

# --- GameEngineDevice/Source/LinuxDevice/GameClient/GUI/Gadget ---
$(GEDO)/LinuxCheckBox.o: $(GEDSLD)/GameClient/GUI/Gadget/LinuxCheckBox.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGadget.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxCheckBox.o $(GEDSLD)/GameClient/GUI/Gadget/LinuxCheckBox.cpp

$(GEDO)/LinuxHorizontalSlider.o: $(GEDSLD)/GameClient/GUI/Gadget/LinuxHorizontalSlider.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGadget.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxHorizontalSlider.o $(GEDSLD)/GameClient/GUI/Gadget/LinuxHorizontalSlider.cpp

$(GEDO)/LinuxListBox.o: $(GEDSLD)/GameClient/GUI/Gadget/LinuxListBox.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGadget.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxListBox.o $(GEDSLD)/GameClient/GUI/Gadget/LinuxListBox.cpp

$(GEDO)/LinuxProgressBar.o: $(GEDSLD)/GameClient/GUI/Gadget/LinuxProgressBar.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGadget.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxProgressBar.o $(GEDSLD)/GameClient/GUI/Gadget/LinuxProgressBar.cpp

$(GEDO)/LinuxPushButton.o: $(GEDSLD)/GameClient/GUI/Gadget/LinuxPushButton.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGadget.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxPushButton.o $(GEDSLD)/GameClient/GUI/Gadget/LinuxPushButton.cpp

$(GEDO)/LinuxRadioButton.o: $(GEDSLD)/GameClient/GUI/Gadget/LinuxRadioButton.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGadget.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxRadioButton.o $(GEDSLD)/GameClient/GUI/Gadget/LinuxRadioButton.cpp

$(GEDO)/LinuxStaticText.o: $(GEDSLD)/GameClient/GUI/Gadget/LinuxStaticText.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGadget.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxStaticText.o $(GEDSLD)/GameClient/GUI/Gadget/LinuxStaticText.cpp

$(GEDO)/LinuxTextEntry.o: $(GEDSLD)/GameClient/GUI/Gadget/LinuxTextEntry.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGadget.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxTextEntry.o $(GEDSLD)/GameClient/GUI/Gadget/LinuxTextEntry.cpp

$(GEDO)/LinuxVerticalSlider.o: $(GEDSLD)/GameClient/GUI/Gadget/LinuxVerticalSlider.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGadget.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxVerticalSlider.o $(GEDSLD)/GameClient/GUI/Gadget/LinuxVerticalSlider.cpp

# --- GameEngineDevice/Source/LinuxDevice/GameClient/GUI/GUICallbacks ---
$(GEDO)/LinuxControlBar.o: $(GEDSLD)/GameClient/GUI/GUICallbacks/LinuxControlBar.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxControlBar.o $(GEDSLD)/GameClient/GUI/GUICallbacks/LinuxControlBar.cpp

$(GEDO)/LinuxMainMenu.o: $(GEDSLD)/GameClient/GUI/GUICallbacks/LinuxMainMenu.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxMainMenu.o $(GEDSLD)/GameClient/GUI/GUICallbacks/LinuxMainMenu.cpp

$(GEDO)/LinuxMOTD.o: $(GEDSLD)/GameClient/GUI/GUICallbacks/LinuxMOTD.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxMOTD.o $(GEDSLD)/GameClient/GUI/GUICallbacks/LinuxMOTD.cpp

# --- GameEngineDevice/Source/LinuxDevice/GameLogic ---
$(GEDO)/LinuxGameLogic.o: $(GEDSLD)/GameLogic/LinuxGameLogic.cpp GameEngineDevice/Include/LinuxDevice/GameLogic/LinuxGameLogic.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameLogic.o $(GEDSLD)/GameLogic/LinuxGameLogic.cpp

$(GEDO)/LinuxTerrainLogic.o: $(GEDSLD)/GameLogic/LinuxTerrainLogic.cpp GameEngineDevice/Include/LinuxDevice/GameLogic/LinuxTerrainLogic.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxTerrainLogic.o $(GEDSLD)/GameLogic/LinuxTerrainLogic.cpp


# ===== GameEngine =====

ENGINE_OBJS = $(GEO)/BitFlags.o $(GEO)/CommandLine.o $(GEO)/crc.o $(GEO)/CRCDebug.o $(GEO)/DamageFX.o $(GEO)/Dict.o $(GEO)/DiscreteCircle.o $(GEO)/GameEngine.o $(GEO)/GameLOD.o \
   $(GEO)/GameMain.o $(GEO)/GlobalData.o $(GEO)/Language.o $(GEO)/MessageStream.o $(GEO)/MultiplayerSettings.o $(GEO)/NameKeyGenerator.o $(GEO)/PartitionSolver.o $(GEO)/RandomValue.o \
   $(GEO)/Recorder.o $(GEO)/StateMachine.o $(GEO)/TerrainTypes.o $(GEO)/UserPreferences.o $(GEO)/Version.o \
$(GEO)/AudioEventRTS.o $(GEO)/AudioRequest.o $(GEO)/DynamicAudioEventInfo.o $(GEO)/GameAudio.o $(GEO)/GameMusic.o $(GEO)/GameSounds.o \
$(GEO)/INI.o $(GEO)/INIAnimation.o $(GEO)/INIAiData.o $(GEO)/INIAudioEventInfo.o $(GEO)/INICommandButton.o $(GEO)/INICrate.o $(GEO)/INIDamageFX.o $(GEO)/INIDrawGroupInfo.o \
   $(GEO)/INIGameData.o $(GEO)/INIMapCache.o $(GEO)/INIMappedImage.o $(GEO)/INIMiscAudio.o $(GEO)/INIMultiplayer.o $(GEO)/INIObject.o $(GEO)/INIParticleSys.o $(GEO)/INISpecialPower.o \
   $(GEO)/INITerrain.o $(GEO)/INITerrainBridge.o $(GEO)/INITerrainRoad.o $(GEO)/INIUpgrade.o $(GEO)/INIVideo.o $(GEO)/INIWater.o $(GEO)/INIWeapon.o \
$(GEO)/ActionManager.o $(GEO)/AcademyStats.o $(GEO)/Energy.o $(GEO)/Handicap.o $(GEO)/MissionStats.o $(GEO)/Money.o $(GEO)/ProductionPrerequisite.o $(GEO)/Player.o $(GEO)/PlayerList.o \
   $(GEO)/PlayerTemplate.o $(GEO)/ResourceGatheringManager.o $(GEO)/Science.o $(GEO)/ScoreKeeper.o $(GEO)/SpecialPower.o $(GEO)/Team.o $(GEO)/TunnelTracker.o \
$(GEO)/ArchiveFile.o $(GEO)/ArchiveFileSystem.o $(GEO)/AsciiString.o $(GEO)/BuildAssistant.o $(GEO)/CriticalSection.o $(GEO)/DataChunk.o $(GEO)/Debug.o $(GEO)/DisabledTypes.o $(GEO)/File.o \
   $(GEO)/FileSystem.o $(GEO)/FunctionLexicon.o $(GEO)/GameCommon.o $(GEO)/GameMemory.o $(GEO)/GameType.o $(GEO)/Geometry.o $(GEO)/KindOf.o $(GEO)/LocalFileSystem.o $(GEO)/MemoryInit.o \
   $(GEO)/ObjectStatusTypes.o $(GEO)/QuotedPrintable.o $(GEO)/Radar.o $(GEO)/RAMFile.o $(GEO)/Snapshot.o $(GEO)/StreamingArchiveFile.o $(GEO)/SubsystemInterface.o $(GEO)/Trig.o \
   $(GEO)/UnicodeString.o $(GEO)/Upgrade.o \
$(GEO)/GameState.o \
$(GEO)/DrawModule.o $(GEO)/Module.o $(GEO)/ModuleFactory.o $(GEO)/Thing.o $(GEO)/ThingFactory.o $(GEO)/ThingTemplate.o \
$(GEO)/Color.o $(GEO)/Credits.o $(GEO)/Display.o $(GEO)/DisplayString.o $(GEO)/DisplayStringManager.o $(GEO)/Drawable.o $(GEO)/DrawGroupInfo.o $(GEO)/FXList.o $(GEO)/GameClient.o \
   $(GEO)/GameClientDispatch.o $(GEO)/GameText.o $(GEO)/GlobalLanguage.o $(GEO)/InGameUI.o $(GEO)/LanguageFilter.o $(GEO)/Line2D.o $(GEO)/MapUtil.o $(GEO)/RadiusDecal.o $(GEO)/Snow.o \
   $(GEO)/Statistics.o $(GEO)/VideoPlayer.o $(GEO)/View.o $(GEO)/Water.o \
$(GEO)/AnimateWindowManager.o $(GEO)/GameFont.o $(GEO)/GameWindow.o $(GEO)/GameWindowGlobal.o $(GEO)/GameWindowManager.o $(GEO)/GameWindowManagerScript.o $(GEO)/GameWindowTransitions.o \
   $(GEO)/GameWindowTransitionsStyles.o $(GEO)/HeaderTemplate.o $(GEO)/IMEManager.o $(GEO)/LoadScreen.o $(GEO)/ProcessAnimateWindow.o $(GEO)/WinInstanceData.o $(GEO)/WindowLayout.o \
   $(GEO)/WindowVideoManager.o \
$(GEO)/ControlBar.o $(GEO)/ControlBarBeacon.o $(GEO)/ControlBarCommand.o $(GEO)/ControlBarCommandProcessing.o $(GEO)/ControlBarMultiSelect.o $(GEO)/ControlBarObserver.o \
   $(GEO)/ControlBarOCLTimer.o $(GEO)/ControlBarPrintPositions.o $(GEO)/ControlBarResizer.o $(GEO)/ControlBarScheme.o $(GEO)/ControlBarStructureInventory.o $(GEO)/ControlBarUnderConstruction.o \
$(GEO)/GadgetCheckBox.o $(GEO)/GadgetComboBox.o $(GEO)/GadgetHorizontalSlider.o $(GEO)/GadgetListBox.o $(GEO)/GadgetProgressBar.o $(GEO)/GadgetPushButton.o $(GEO)/GadgetRadioButton.o \
   $(GEO)/GadgetStaticText.o $(GEO)/GadgetTabControl.o $(GEO)/GadgetTextEntry.o $(GEO)/GadgetVerticalSlider.o \
$(GEO)/ControlBarCallback.o $(GEO)/ControlBarPopupDescription.o $(GEO)/ExtendedMessageBox.o $(GEO)/MessageBox.o \
$(GEO)/CreditsMenu.o $(GEO)/MainMenu.o $(GEO)/OptionsMenu.o \
$(GEO)/Shell.o $(GEO)/ShellMenuScheme.o $(GEO)/Keyboard.o $(GEO)/Mouse.o \
$(GEO)/WindowXlat.o \
$(GEO)/Anim2D.o $(GEO)/CampaignManager.o $(GEO)/Image.o $(GEO)/ParticleSys.o \
$(GEO)/TerrainRoads.o \
$(GEO)/AI.o $(GEO)/AIDock.o $(GEO)/AIGroup.o $(GEO)/AIGuard.o $(GEO)/AIGuardRetaliate.o $(GEO)/AIPathfind.o $(GEO)/AIStates.o $(GEO)/AITNGuard.o $(GEO)/Squad.o $(GEO)/TurretAI.o \
$(GEO)/PolygonTrigger.o $(GEO)/SidesList.o $(GEO)/TerrainLogic.o \
$(GEO)/Armor.o $(GEO)/ExperienceTracker.o $(GEO)/Locomotor.o $(GEO)/Object.o $(GEO)/ObjectCreationList.o $(GEO)/ObjectTypes.o $(GEO)/PartitionManager.o $(GEO)/SimpleObjectIterator.o \
   $(GEO)/Weapon.o $(GEO)/WeaponSet.o \
$(GEO)/AutoHealBehavior.o $(GEO)/BehaviorModule.o $(GEO)/BridgeScaffoldBehavior.o $(GEO)/BridgeTowerBehavior.o $(GEO)/BridgeBehavior.o $(GEO)/GenerateMinefieldBehavior.o \
$(GEO)/BodyModule.o $(GEO)/InactiveBody.o \
$(GEO)/CollideModule.o $(GEO)/SquishCollide.o \
$(GEO)/CrateCollide.o $(GEO)/MoneyCrateCollide.o $(GEO)/SalvageCrateCollide.o $(GEO)/UnitCrateCollide.o $(GEO)/VeterancyCrateCollide.o \
$(GEO)/CaveContain.o $(GEO)/OpenContain.o \
$(GEO)/DamageModule.o \
$(GEO)/DestroyDie.o $(GEO)/DieModule.o $(GEO)/SpecialPowerCompletionDie.o \
$(GEO)/ObjectHelper.o $(GEO)/ObjectDefectionHelper.o \
$(GEO)/AIUpdate.o $(GEO)/CommandButtonHuntUpdate.o $(GEO)/DeletionUpdate.o $(GEO)/LaserUpdate.o $(GEO)/PhysicsUpdate.o $(GEO)/ProneUpdate.o $(GEO)/RadiusDecalUpdate.o \
   $(GEO)/SmartBombTargetHomingUpdate.o $(GEO)/SpecialAbilityUpdate.o $(GEO)/SpecialPowerUpdateModule.o $(GEO)/StealthUpdate.o $(GEO)/StickyBombUpdate.o $(GEO)/UpdateModule.o \
$(GEO)/DeliverPayloadAIUpdate.o $(GEO)/HackInternetAIUpdate.o $(GEO)/RailroadGuideAIUpdate.o $(GEO)/SupplyTruckAIUpdate.o \
$(GEO)/DockUpdate.o $(GEO)/SupplyWarehouseDockUpdate.o \
$(GEO)/UpgradeModule.o \
$(GEO)/ScriptActions.o $(GEO)/ScriptConditions.o $(GEO)/ScriptEngine.o $(GEO)/Scripts.o $(GEO)/VictoryConditions.o \
$(GEO)/CaveSystem.o $(GEO)/CrateSystem.o $(GEO)/Damage.o $(GEO)/GameLogic.o $(GEO)/GameLogicDispatch.o $(GEO)/RankInfo.o \
$(GEO)/GameInfo.o \
$(GEO)/Chat.o

$(ENGINE_LIB): $(ENGINE_OBJS)
	ar rcs $(ENGINE_LIB) $(ENGINE_OBJS)

# --- GameEngine/Source/Common ---
$(GEO)/BitFlags.o: $(GES)/Common/BitFlags.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/BitFlags.o $(GES)/Common/BitFlags.cpp

$(GEO)/CommandLine.o: $(GES)/Common/CommandLine.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CommandLine.o $(GES)/Common/CommandLine.cpp

$(GEO)/crc.o: $(GES)/Common/crc.cpp GameEngine/Include/Common/CRC.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/crc.o $(GES)/Common/crc.cpp

$(GEO)/CRCDebug.o: $(GES)/Common/CRCDebug.cpp GameEngine/Include/Common/CRCDebug.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CRCDebug.o $(GES)/Common/CRCDebug.cpp

$(GEO)/DamageFX.o: $(GES)/Common/DamageFX.cpp GameEngine/Include/Common/DamageFX.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DamageFX.o $(GES)/Common/DamageFX.cpp

$(GEO)/Dict.o: $(GES)/Common/Dict.cpp GameEngine/Include/Common/Dict.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Dict.o $(GES)/Common/Dict.cpp

$(GEO)/DiscreteCircle.o: $(GES)/Common/DiscreteCircle.cpp GameEngine/Include/Common/DiscreteCircle.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DiscreteCircle.o $(GES)/Common/DiscreteCircle.cpp

$(GEO)/GameEngine.o: $(GES)/Common/GameEngine.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameEngine.o $(GES)/Common/GameEngine.cpp

$(GEO)/GameLOD.o: $(GES)/Common/GameLOD.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameLOD.o $(GES)/Common/GameLOD.cpp

$(GEO)/GameMain.o: $(GES)/Common/GameMain.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameMain.o $(GES)/Common/GameMain.cpp

$(GEO)/GlobalData.o: $(GES)/Common/GlobalData.cpp GameEngine/Include/Common/GlobalData.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GlobalData.o $(GES)/Common/GlobalData.cpp

$(GEO)/Language.o: $(GES)/Common/Language.cpp GameEngine/Include/Common/Language.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Language.o $(GES)/Common/Language.cpp

$(GEO)/MessageStream.o: $(GES)/Common/MessageStream.cpp GameEngine/Include/Common/MessageStream.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MessageStream.o $(GES)/Common/MessageStream.cpp

$(GEO)/MultiplayerSettings.o: $(GES)/Common/MultiplayerSettings.cpp GameEngine/Include/Common/MultiplayerSettings.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MultiplayerSettings.o $(GES)/Common/MultiplayerSettings.cpp

$(GEO)/NameKeyGenerator.o: $(GES)/Common/NameKeyGenerator.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/NameKeyGenerator.o $(GES)/Common/NameKeyGenerator.cpp

$(GEO)/PartitionSolver.o: $(GES)/Common/PartitionSolver.cpp GameEngine/Include/Common/PartitionSolver.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/PartitionSolver.o $(GES)/Common/PartitionSolver.cpp

$(GEO)/RandomValue.o: $(GES)/Common/RandomValue.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RandomValue.o $(GES)/Common/RandomValue.cpp

$(GEO)/Recorder.o: $(GES)/Common/Recorder.cpp GameEngine/Include/Common/Recorder.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Recorder.o $(GES)/Common/Recorder.cpp

$(GEO)/StateMachine.o: $(GES)/Common/StateMachine.cpp GameEngine/Include/Common/StateMachine.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/StateMachine.o $(GES)/Common/StateMachine.cpp

$(GEO)/TerrainTypes.o: $(GES)/Common/TerrainTypes.cpp GameEngine/Include/Common/TerrainTypes.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/TerrainTypes.o $(GES)/Common/TerrainTypes.cpp

$(GEO)/UserPreferences.o: $(GES)/Common/UserPreferences.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UserPreferences.o $(GES)/Common/UserPreferences.cpp

$(GEO)/Version.o: $(GES)/Common/Version.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Version.o $(GES)/Common/Version.cpp

# --- GameEngine/Source/Common/Audio ---
$(GEO)/AudioEventRTS.o: $(GES)/Common/Audio/AudioEventRTS.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AudioEventRTS.o $(GES)/Common/Audio/AudioEventRTS.cpp

$(GEO)/AudioRequest.o: $(GES)/Common/Audio/AudioRequest.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AudioRequest.o $(GES)/Common/Audio/AudioRequest.cpp

$(GEO)/DynamicAudioEventInfo.o: $(GES)/Common/Audio/DynamicAudioEventInfo.cpp GameEngine/Include/Common/DynamicAudioEventInfo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DynamicAudioEventInfo.o $(GES)/Common/Audio/DynamicAudioEventInfo.cpp

$(GEO)/GameAudio.o: $(GES)/Common/Audio/GameAudio.cpp GameEngine/Include/Common/AudioEventInfo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameAudio.o $(GES)/Common/Audio/GameAudio.cpp

$(GEO)/GameMusic.o: $(GES)/Common/Audio/GameMusic.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameMusic.o $(GES)/Common/Audio/GameMusic.cpp

$(GEO)/GameSounds.o: $(GES)/Common/Audio/GameSounds.cpp GameEngine/Include/Common/AudioEventInfo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameSounds.o $(GES)/Common/Audio/GameSounds.cpp

# --- GameEngine/Source/Common/INI ---
$(GEO)/INI.o: $(GES)/Common/INI/INI.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INI.o $(GES)/Common/INI/INI.cpp

$(GEO)/INIAnimation.o: $(GES)/Common/INI/INIAnimation.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIAnimation.o $(GES)/Common/INI/INIAnimation.cpp

$(GEO)/INIAiData.o: $(GES)/Common/INI/INIAiData.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIAiData.o $(GES)/Common/INI/INIAiData.cpp

$(GEO)/INIAudioEventInfo.o: $(GES)/Common/INI/INIAudioEventInfo.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIAudioEventInfo.o $(GES)/Common/INI/INIAudioEventInfo.cpp

$(GEO)/INICommandButton.o: $(GES)/Common/INI/INICommandButton.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INICommandButton.o $(GES)/Common/INI/INICommandButton.cpp

$(GEO)/INICrate.o: $(GES)/Common/INI/INICrate.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INICrate.o $(GES)/Common/INI/INICrate.cpp

$(GEO)/INIDamageFX.o: $(GES)/Common/INI/INIDamageFX.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIDamageFX.o $(GES)/Common/INI/INIDamageFX.cpp

$(GEO)/INIDrawGroupInfo.o: $(GES)/Common/INI/INIDrawGroupInfo.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIDrawGroupInfo.o $(GES)/Common/INI/INIDrawGroupInfo.cpp

$(GEO)/INIGameData.o: $(GES)/Common/INI/INIGameData.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIGameData.o $(GES)/Common/INI/INIGameData.cpp

$(GEO)/INIMapCache.o: $(GES)/Common/INI/INIMapCache.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIMapCache.o $(GES)/Common/INI/INIMapCache.cpp

$(GEO)/INIMappedImage.o: $(GES)/Common/INI/INIMappedImage.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIMappedImage.o $(GES)/Common/INI/INIMappedImage.cpp

$(GEO)/INIMiscAudio.o: $(GES)/Common/INI/INIMiscAudio.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIMiscAudio.o $(GES)/Common/INI/INIMiscAudio.cpp

$(GEO)/INIMultiplayer.o: $(GES)/Common/INI/INIMultiplayer.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIMultiplayer.o $(GES)/Common/INI/INIMultiplayer.cpp

$(GEO)/INIObject.o: $(GES)/Common/INI/INIObject.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIObject.o $(GES)/Common/INI/INIObject.cpp

$(GEO)/INIParticleSys.o: $(GES)/Common/INI/INIParticleSys.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIParticleSys.o $(GES)/Common/INI/INIParticleSys.cpp

$(GEO)/INISpecialPower.o: $(GES)/Common/INI/INISpecialPower.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INISpecialPower.o $(GES)/Common/INI/INISpecialPower.cpp

$(GEO)/INITerrain.o: $(GES)/Common/INI/INITerrain.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INITerrain.o $(GES)/Common/INI/INITerrain.cpp

$(GEO)/INITerrainBridge.o: $(GES)/Common/INI/INITerrainBridge.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INITerrainBridge.o $(GES)/Common/INI/INITerrainBridge.cpp

$(GEO)/INITerrainRoad.o: $(GES)/Common/INI/INITerrainRoad.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INITerrainRoad.o $(GES)/Common/INI/INITerrainRoad.cpp

$(GEO)/INIUpgrade.o: $(GES)/Common/INI/INIUpgrade.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIUpgrade.o $(GES)/Common/INI/INIUpgrade.cpp

$(GEO)/INIVideo.o: $(GES)/Common/INI/INIVideo.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIVideo.o $(GES)/Common/INI/INIVideo.cpp

$(GEO)/INIWater.o: $(GES)/Common/INI/INIWater.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIWater.o $(GES)/Common/INI/INIWater.cpp

$(GEO)/INIWeapon.o: $(GES)/Common/INI/INIWeapon.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIWeapon.o $(GES)/Common/INI/INIWeapon.cpp

# --- GameEngine/Source/Common/RTS ---
$(GEO)/ActionManager.o: $(GES)/Common/RTS/ActionManager.cpp GameEngine/Include/Common/ActionManager.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ActionManager.o $(GES)/Common/RTS/ActionManager.cpp

$(GEO)/AcademyStats.o: $(GES)/Common/RTS/AcademyStats.cpp GameEngine/Include/Common/AcademyStats.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AcademyStats.o $(GES)/Common/RTS/AcademyStats.cpp

$(GEO)/Energy.o: $(GES)/Common/RTS/Energy.cpp GameEngine/Include/Common/Energy.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Energy.o $(GES)/Common/RTS/Energy.cpp

$(GEO)/Handicap.o: $(GES)/Common/RTS/Handicap.cpp GameEngine/Include/Common/Handicap.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Handicap.o $(GES)/Common/RTS/Handicap.cpp

$(GEO)/MissionStats.o: $(GES)/Common/RTS/MissionStats.cpp GameEngine/Include/Common/MissionStats.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MissionStats.o $(GES)/Common/RTS/MissionStats.cpp

$(GEO)/Money.o: $(GES)/Common/RTS/Money.cpp GameEngine/Include/Common/Money.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Money.o $(GES)/Common/RTS/Money.cpp

$(GEO)/ProductionPrerequisite.o: $(GES)/Common/RTS/ProductionPrerequisite.cpp GameEngine/Include/Common/ProductionPrerequisite.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ProductionPrerequisite.o $(GES)/Common/RTS/ProductionPrerequisite.cpp

$(GEO)/Player.o: $(GES)/Common/RTS/Player.cpp GameEngine/Include/Common/Player.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Player.o $(GES)/Common/RTS/Player.cpp

$(GEO)/PlayerList.o: $(GES)/Common/RTS/PlayerList.cpp GameEngine/Include/Common/PlayerList.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/PlayerList.o $(GES)/Common/RTS/PlayerList.cpp

$(GEO)/PlayerTemplate.o: $(GES)/Common/RTS/PlayerTemplate.cpp GameEngine/Include/Common/PlayerTemplate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/PlayerTemplate.o $(GES)/Common/RTS/PlayerTemplate.cpp

$(GEO)/ResourceGatheringManager.o: $(GES)/Common/RTS/ResourceGatheringManager.cpp GameEngine/Include/Common/ResourceGatheringManager.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ResourceGatheringManager.o $(GES)/Common/RTS/ResourceGatheringManager.cpp

$(GEO)/Science.o: $(GES)/Common/RTS/Science.cpp GameEngine/Include/Common/Science.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Science.o $(GES)/Common/RTS/Science.cpp

$(GEO)/ScoreKeeper.o: $(GES)/Common/RTS/ScoreKeeper.cpp GameEngine/Include/Common/ScoreKeeper.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ScoreKeeper.o $(GES)/Common/RTS/ScoreKeeper.cpp

$(GEO)/SpecialPower.o: $(GES)/Common/RTS/SpecialPower.cpp GameEngine/Include/Common/SpecialPower.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SpecialPower.o $(GES)/Common/RTS/SpecialPower.cpp

$(GEO)/Team.o: $(GES)/Common/RTS/Team.cpp GameEngine/Include/Common/Team.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Team.o $(GES)/Common/RTS/Team.cpp

$(GEO)/TunnelTracker.o: $(GES)/Common/RTS/TunnelTracker.cpp GameEngine/Include/Common/TunnelTracker.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/TunnelTracker.o $(GES)/Common/RTS/TunnelTracker.cpp

# --- GameEngine/Source/Common/System ---
$(GEO)/ArchiveFile.o: $(GES)/Common/System/ArchiveFile.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ArchiveFile.o $(GES)/Common/System/ArchiveFile.cpp

$(GEO)/ArchiveFileSystem.o: $(GES)/Common/System/ArchiveFileSystem.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ArchiveFileSystem.o $(GES)/Common/System/ArchiveFileSystem.cpp

$(GEO)/AsciiString.o: $(GES)/Common/System/AsciiString.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AsciiString.o $(GES)/Common/System/AsciiString.cpp

$(GEO)/BuildAssistant.o: $(GES)/Common/System/BuildAssistant.cpp GameEngine/Include/Common/BuildAssistant.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/BuildAssistant.o $(GES)/Common/System/BuildAssistant.cpp

$(GEO)/CriticalSection.o: $(GES)/Common/System/CriticalSection.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CriticalSection.o $(GES)/Common/System/CriticalSection.cpp

$(GEO)/DataChunk.o: $(GES)/Common/System/DataChunk.cpp GameEngine/Include/Common/DataChunk.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DataChunk.o $(GES)/Common/System/DataChunk.cpp

$(GEO)/Debug.o: $(GES)/Common/System/Debug.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Debug.o $(GES)/Common/System/Debug.cpp

$(GEO)/DisabledTypes.o: $(GES)/Common/System/DisabledTypes.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DisabledTypes.o $(GES)/Common/System/DisabledTypes.cpp

$(GEO)/File.o: $(GES)/Common/System/File.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/File.o $(GES)/Common/System/File.cpp

$(GEO)/FileSystem.o: $(GES)/Common/System/FileSystem.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/FileSystem.o $(GES)/Common/System/FileSystem.cpp

$(GEO)/FunctionLexicon.o: $(GES)/Common/System/FunctionLexicon.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/FunctionLexicon.o $(GES)/Common/System/FunctionLexicon.cpp

$(GEO)/GameCommon.o: $(GES)/Common/System/GameCommon.cpp GameEngine/Include/Common/GameCommon.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameCommon.o $(GES)/Common/System/GameCommon.cpp

$(GEO)/GameMemory.o: $(GES)/Common/System/GameMemory.cpp GameEngine/Include/Common/GameMemory.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameMemory.o $(GES)/Common/System/GameMemory.cpp

$(GEO)/GameType.o: $(GES)/Common/System/GameType.cpp GameEngine/Include/Common/GameType.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameType.o $(GES)/Common/System/GameType.cpp

$(GEO)/Geometry.o: $(GES)/Common/System/Geometry.cpp GameEngine/Include/Common/Geometry.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Geometry.o $(GES)/Common/System/Geometry.cpp

$(GEO)/KindOf.o: $(GES)/Common/System/KindOf.cpp GameEngine/Include/Common/KindOf.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/KindOf.o $(GES)/Common/System/KindOf.cpp

$(GEO)/LocalFileSystem.o: $(GES)/Common/System/LocalFileSystem.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/LocalFileSystem.o $(GES)/Common/System/LocalFileSystem.cpp

$(GEO)/MemoryInit.o: $(GES)/Common/System/MemoryInit.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MemoryInit.o $(GES)/Common/System/MemoryInit.cpp

$(GEO)/ObjectStatusTypes.o: $(GES)/Common/System/ObjectStatusTypes.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ObjectStatusTypes.o $(GES)/Common/System/ObjectStatusTypes.cpp

$(GEO)/QuotedPrintable.o: $(GES)/Common/System/QuotedPrintable.cpp GameEngine/Include/Common/QuotedPrintable.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/QuotedPrintable.o $(GES)/Common/System/QuotedPrintable.cpp

$(GEO)/Radar.o: $(GES)/Common/System/Radar.cpp GameEngine/Include/Common/Radar.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Radar.o $(GES)/Common/System/Radar.cpp

$(GEO)/RAMFile.o: $(GES)/Common/System/RAMFile.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RAMFile.o $(GES)/Common/System/RAMFile.cpp

$(GEO)/Snapshot.o: $(GES)/Common/System/Snapshot.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Snapshot.o $(GES)/Common/System/Snapshot.cpp

$(GEO)/StreamingArchiveFile.o: $(GES)/Common/System/StreamingArchiveFile.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/StreamingArchiveFile.o $(GES)/Common/System/StreamingArchiveFile.cpp

$(GEO)/SubsystemInterface.o: $(GES)/Common/System/SubsystemInterface.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SubsystemInterface.o $(GES)/Common/System/SubsystemInterface.cpp

$(GEO)/Trig.o: $(GES)/Common/System/Trig.cpp Libraries/Include/Lib/Trig.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Trig.o $(GES)/Common/System/Trig.cpp

$(GEO)/UnicodeString.o: $(GES)/Common/System/UnicodeString.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UnicodeString.o $(GES)/Common/System/UnicodeString.cpp

$(GEO)/Upgrade.o: $(GES)/Common/System/Upgrade.cpp GameEngine/Include/Common/Upgrade.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Upgrade.o $(GES)/Common/System/Upgrade.cpp

#$(GEO)/Xfer.o: $(GES)/Common/System/Xfer.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Xfer.o $(GES)/Common/System/Xfer.cpp

#$(GEO)/XferCRC.o: $(GES)/Common/System/XferCRC.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/XferCRC.o $(GES)/Common/System/XferCRC.cpp

# --- GameEngine/Source/Common/System/SaveGame ---
$(GEO)/GameState.o: $(GES)/Common/System/SaveGame/GameState.cpp GameEngine/Include/Common/GameState.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameState.o $(GES)/Common/System/SaveGame/GameState.cpp

# --- GameEngine/Source/Common/Thing ---
$(GEO)/DrawModule.o: $(GES)/Common/Thing/DrawModule.cpp GameEngine/Include/Common/DrawModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DrawModule.o $(GES)/Common/Thing/DrawModule.cpp

$(GEO)/Module.o: $(GES)/Common/Thing/Module.cpp GameEngine/Include/Common/Module.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Module.o $(GES)/Common/Thing/Module.cpp

$(GEO)/ModuleFactory.o: $(GES)/Common/Thing/ModuleFactory.cpp GameEngine/Include/Common/ModuleFactory.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ModuleFactory.o $(GES)/Common/Thing/ModuleFactory.cpp

$(GEO)/Thing.o: $(GES)/Common/Thing/Thing.cpp GameEngine/Include/Common/Thing.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Thing.o $(GES)/Common/Thing/Thing.cpp

$(GEO)/ThingFactory.o: $(GES)/Common/Thing/ThingFactory.cpp GameEngine/Include/Common/ThingFactory.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ThingFactory.o $(GES)/Common/Thing/ThingFactory.cpp

$(GEO)/ThingTemplate.o: $(GES)/Common/Thing/ThingTemplate.cpp GameEngine/Include/Common/ThingTemplate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ThingTemplate.o $(GES)/Common/Thing/ThingTemplate.cpp

# --- GameEngine/Source/GameClient ---
$(GEO)/Color.o: $(GES)/GameClient/Color.cpp GameEngine/Include/GameClient/Color.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Color.o $(GES)/GameClient/Color.cpp

$(GEO)/Credits.o: $(GES)/GameClient/Credits.cpp GameEngine/Include/GameClient/Credits.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Credits.o $(GES)/GameClient/Credits.cpp

$(GEO)/Display.o: $(GES)/GameClient/Display.cpp GameEngine/Include/GameClient/Display.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Display.o $(GES)/GameClient/Display.cpp

$(GEO)/DisplayString.o: $(GES)/GameClient/DisplayString.cpp GameEngine/Include/GameClient/DisplayString.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DisplayString.o $(GES)/GameClient/DisplayString.cpp

$(GEO)/DisplayStringManager.o: $(GES)/GameClient/DisplayStringManager.cpp GameEngine/Include/GameClient/DisplayStringManager.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DisplayStringManager.o $(GES)/GameClient/DisplayStringManager.cpp

$(GEO)/Drawable.o: $(GES)/GameClient/Drawable.cpp GameEngine/Include/GameClient/Drawable.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Drawable.o $(GES)/GameClient/Drawable.cpp

$(GEO)/DrawGroupInfo.o: $(GES)/GameClient/DrawGroupInfo.cpp GameEngine/Include/GameClient/DrawGroupInfo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DrawGroupInfo.o $(GES)/GameClient/DrawGroupInfo.cpp

$(GEO)/FXList.o: $(GES)/GameClient/FXList.cpp GameEngine/Include/GameClient/FXList.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/FXList.o $(GES)/GameClient/FXList.cpp

$(GEO)/GameClient.o: $(GES)/GameClient/GameClient.cpp GameEngine/Include/GameClient/GameClient.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameClient.o $(GES)/GameClient/GameClient.cpp

$(GEO)/GameClientDispatch.o: $(GES)/GameClient/GameClientDispatch.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameClientDispatch.o $(GES)/GameClient/GameClientDispatch.cpp

$(GEO)/GameText.o: $(GES)/GameClient/GameText.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameText.o $(GES)/GameClient/GameText.cpp

$(GEO)/GlobalLanguage.o: $(GES)/GameClient/GlobalLanguage.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GlobalLanguage.o $(GES)/GameClient/GlobalLanguage.cpp

$(GEO)/InGameUI.o: $(GES)/GameClient/InGameUI.cpp GameEngine/Include/GameClient/InGameUI.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/InGameUI.o $(GES)/GameClient/InGameUI.cpp

$(GEO)/LanguageFilter.o: $(GES)/GameClient/LanguageFilter.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/LanguageFilter.o $(GES)/GameClient/LanguageFilter.cpp

$(GEO)/Line2D.o: $(GES)/GameClient/Line2D.cpp GameEngine/Include/GameClient/Line2D.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Line2D.o $(GES)/GameClient/Line2D.cpp

$(GEO)/MapUtil.o: $(GES)/GameClient/MapUtil.cpp GameEngine/Include/GameClient/MapUtil.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MapUtil.o $(GES)/GameClient/MapUtil.cpp

$(GEO)/RadiusDecal.o: $(GES)/GameClient/RadiusDecal.cpp GameEngine/Include/GameClient/RadiusDecal.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RadiusDecal.o $(GES)/GameClient/RadiusDecal.cpp

$(GEO)/Snow.o: $(GES)/GameClient/Snow.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Snow.o $(GES)/GameClient/Snow.cpp

$(GEO)/Statistics.o: $(GES)/GameClient/Statistics.cpp GameEngine/Include/GameClient/Statistics.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Statistics.o $(GES)/GameClient/Statistics.cpp

$(GEO)/VideoPlayer.o: $(GES)/GameClient/VideoPlayer.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/VideoPlayer.o $(GES)/GameClient/VideoPlayer.cpp

$(GEO)/View.o: $(GES)/GameClient/View.cpp GameEngine/Include/GameClient/View.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/View.o $(GES)/GameClient/View.cpp

$(GEO)/Water.o: $(GES)/GameClient/Water.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Water.o $(GES)/GameClient/Water.cpp

# --- GameEngine/Source/GameClient/GUI ---
$(GEO)/AnimateWindowManager.o: $(GES)/GameClient/GUI/AnimateWindowManager.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AnimateWindowManager.o $(GES)/GameClient/GUI/AnimateWindowManager.cpp

$(GEO)/GameFont.o: $(GES)/GameClient/GUI/GameFont.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameFont.o $(GES)/GameClient/GUI/GameFont.cpp

$(GEO)/GameWindow.o: $(GES)/GameClient/GUI/GameWindow.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameWindow.o $(GES)/GameClient/GUI/GameWindow.cpp

$(GEO)/GameWindowGlobal.o: $(GES)/GameClient/GUI/GameWindowGlobal.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameWindowGlobal.o $(GES)/GameClient/GUI/GameWindowGlobal.cpp

$(GEO)/GameWindowManager.o: $(GES)/GameClient/GUI/GameWindowManager.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameWindowManager.o $(GES)/GameClient/GUI/GameWindowManager.cpp

$(GEO)/GameWindowManagerScript.o: $(GES)/GameClient/GUI/GameWindowManagerScript.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameWindowManagerScript.o $(GES)/GameClient/GUI/GameWindowManagerScript.cpp

$(GEO)/GameWindowTransitions.o: $(GES)/GameClient/GUI/GameWindowTransitions.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameWindowTransitions.o $(GES)/GameClient/GUI/GameWindowTransitions.cpp

$(GEO)/GameWindowTransitionsStyles.o: $(GES)/GameClient/GUI/GameWindowTransitionsStyles.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameWindowTransitionsStyles.o $(GES)/GameClient/GUI/GameWindowTransitionsStyles.cpp

$(GEO)/HeaderTemplate.o: $(GES)/GameClient/GUI/HeaderTemplate.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/HeaderTemplate.o $(GES)/GameClient/GUI/HeaderTemplate.cpp

$(GEO)/IMEManager.o: $(GES)/GameClient/GUI/IMEManager.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/IMEManager.o $(GES)/GameClient/GUI/IMEManager.cpp

$(GEO)/LoadScreen.o: $(GES)/GameClient/GUI/LoadScreen.cpp GameEngine/Include/GameClient/LoadScreen.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/LoadScreen.o $(GES)/GameClient/GUI/LoadScreen.cpp

$(GEO)/ProcessAnimateWindow.o: $(GES)/GameClient/GUI/ProcessAnimateWindow.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ProcessAnimateWindow.o $(GES)/GameClient/GUI/ProcessAnimateWindow.cpp

$(GEO)/WinInstanceData.o: $(GES)/GameClient/GUI/WinInstanceData.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/WinInstanceData.o $(GES)/GameClient/GUI/WinInstanceData.cpp

$(GEO)/WindowLayout.o: $(GES)/GameClient/GUI/WindowLayout.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/WindowLayout.o $(GES)/GameClient/GUI/WindowLayout.cpp

$(GEO)/WindowVideoManager.o: $(GES)/GameClient/GUI/WindowVideoManager.cpp GameEngine/Include/GameClient/WindowVideoManager.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/WindowVideoManager.o $(GES)/GameClient/GUI/WindowVideoManager.cpp

# --- GameEngine/Source/GameClient/GUI/ControlBar ---
$(GEO)/ControlBar.o: $(GES)/GameClient/GUI/ControlBar/ControlBar.cpp GameEngine/Include/GameClient/ControlBar.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBar.o $(GES)/GameClient/GUI/ControlBar/ControlBar.cpp

$(GEO)/ControlBarBeacon.o: $(GES)/GameClient/GUI/ControlBar/ControlBarBeacon.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarBeacon.o $(GES)/GameClient/GUI/ControlBar/ControlBarBeacon.cpp

$(GEO)/ControlBarCommand.o: $(GES)/GameClient/GUI/ControlBar/ControlBarCommand.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarCommand.o $(GES)/GameClient/GUI/ControlBar/ControlBarCommand.cpp

$(GEO)/ControlBarCommandProcessing.o: $(GES)/GameClient/GUI/ControlBar/ControlBarCommandProcessing.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarCommandProcessing.o $(GES)/GameClient/GUI/ControlBar/ControlBarCommandProcessing.cpp

$(GEO)/ControlBarMultiSelect.o: $(GES)/GameClient/GUI/ControlBar/ControlBarMultiSelect.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarMultiSelect.o $(GES)/GameClient/GUI/ControlBar/ControlBarMultiSelect.cpp

$(GEO)/ControlBarObserver.o: $(GES)/GameClient/GUI/ControlBar/ControlBarObserver.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarObserver.o $(GES)/GameClient/GUI/ControlBar/ControlBarObserver.cpp

$(GEO)/ControlBarOCLTimer.o: $(GES)/GameClient/GUI/ControlBar/ControlBarOCLTimer.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarOCLTimer.o $(GES)/GameClient/GUI/ControlBar/ControlBarOCLTimer.cpp

$(GEO)/ControlBarPrintPositions.o: $(GES)/GameClient/GUI/ControlBar/ControlBarPrintPositions.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarPrintPositions.o $(GES)/GameClient/GUI/ControlBar/ControlBarPrintPositions.cpp

$(GEO)/ControlBarResizer.o: $(GES)/GameClient/GUI/ControlBar/ControlBarResizer.cpp GameEngine/Include/GameClient/ControlBarResizer.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarResizer.o $(GES)/GameClient/GUI/ControlBar/ControlBarResizer.cpp

$(GEO)/ControlBarScheme.o: $(GES)/GameClient/GUI/ControlBar/ControlBarScheme.cpp GameEngine/Include/GameClient/ControlBarScheme.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarScheme.o $(GES)/GameClient/GUI/ControlBar/ControlBarScheme.cpp

$(GEO)/ControlBarStructureInventory.o: $(GES)/GameClient/GUI/ControlBar/ControlBarStructureInventory.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarStructureInventory.o $(GES)/GameClient/GUI/ControlBar/ControlBarStructureInventory.cpp

$(GEO)/ControlBarUnderConstruction.o: $(GES)/GameClient/GUI/ControlBar/ControlBarUnderConstruction.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarUnderConstruction.o $(GES)/GameClient/GUI/ControlBar/ControlBarUnderConstruction.cpp

# --- GameEngine/Source/GameClient/GUI/Gadget ---
$(GEO)/GadgetCheckBox.o: $(GES)/GameClient/GUI/Gadget/GadgetCheckBox.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetCheckBox.o $(GES)/GameClient/GUI/Gadget/GadgetCheckBox.cpp

$(GEO)/GadgetComboBox.o: $(GES)/GameClient/GUI/Gadget/GadgetComboBox.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetComboBox.o $(GES)/GameClient/GUI/Gadget/GadgetComboBox.cpp

$(GEO)/GadgetHorizontalSlider.o: $(GES)/GameClient/GUI/Gadget/GadgetHorizontalSlider.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetHorizontalSlider.o $(GES)/GameClient/GUI/Gadget/GadgetHorizontalSlider.cpp

$(GEO)/GadgetListBox.o: $(GES)/GameClient/GUI/Gadget/GadgetListBox.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetListBox.o $(GES)/GameClient/GUI/Gadget/GadgetListBox.cpp

$(GEO)/GadgetProgressBar.o: $(GES)/GameClient/GUI/Gadget/GadgetProgressBar.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetProgressBar.o $(GES)/GameClient/GUI/Gadget/GadgetProgressBar.cpp

$(GEO)/GadgetPushButton.o: $(GES)/GameClient/GUI/Gadget/GadgetPushButton.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetPushButton.o $(GES)/GameClient/GUI/Gadget/GadgetPushButton.cpp

$(GEO)/GadgetRadioButton.o: $(GES)/GameClient/GUI/Gadget/GadgetRadioButton.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetRadioButton.o $(GES)/GameClient/GUI/Gadget/GadgetRadioButton.cpp

$(GEO)/GadgetStaticText.o: $(GES)/GameClient/GUI/Gadget/GadgetStaticText.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetStaticText.o $(GES)/GameClient/GUI/Gadget/GadgetStaticText.cpp

$(GEO)/GadgetTabControl.o: $(GES)/GameClient/GUI/Gadget/GadgetTabControl.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetTabControl.o $(GES)/GameClient/GUI/Gadget/GadgetTabControl.cpp

$(GEO)/GadgetTextEntry.o: $(GES)/GameClient/GUI/Gadget/GadgetTextEntry.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetTextEntry.o $(GES)/GameClient/GUI/Gadget/GadgetTextEntry.cpp

$(GEO)/GadgetVerticalSlider.o: $(GES)/GameClient/GUI/Gadget/GadgetVerticalSlider.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GadgetVerticalSlider.o $(GES)/GameClient/GUI/Gadget/GadgetVerticalSlider.cpp

# --- GameEngine/Source/GameClient/GUI/GUICallbacks ---
$(GEO)/ControlBarCallback.o: $(GES)/GameClient/GUI/GUICallbacks/ControlBarCallback.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarCallback.o $(GES)/GameClient/GUI/GUICallbacks/ControlBarCallback.cpp

$(GEO)/ControlBarPopupDescription.o: $(GES)/GameClient/GUI/GUICallbacks/ControlBarPopupDescription.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ControlBarPopupDescription.o $(GES)/GameClient/GUI/GUICallbacks/ControlBarPopupDescription.cpp

$(GEO)/ExtendedMessageBox.o: $(GES)/GameClient/GUI/GUICallbacks/ExtendedMessageBox.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ExtendedMessageBox.o $(GES)/GameClient/GUI/GUICallbacks/ExtendedMessageBox.cpp

$(GEO)/MessageBox.o: $(GES)/GameClient/GUI/GUICallbacks/MessageBox.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MessageBox.o $(GES)/GameClient/GUI/GUICallbacks/MessageBox.cpp

# --- GameEngine/Source/GameClient/GUI/GUICallbacks/Menus ---
$(GEO)/CreditsMenu.o: $(GES)/GameClient/GUI/GUICallbacks/Menus/CreditsMenu.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CreditsMenu.o $(GES)/GameClient/GUI/GUICallbacks/Menus/CreditsMenu.cpp

$(GEO)/MainMenu.o: $(GES)/GameClient/GUI/GUICallbacks/Menus/MainMenu.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MainMenu.o $(GES)/GameClient/GUI/GUICallbacks/Menus/MainMenu.cpp

$(GEO)/OptionsMenu.o: $(GES)/GameClient/GUI/GUICallbacks/Menus/OptionsMenu.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/OptionsMenu.o $(GES)/GameClient/GUI/GUICallbacks/Menus/OptionsMenu.cpp

# --- GameEngine/Source/GameClient/GUI/Shell ---
$(GEO)/Shell.o: $(GES)/GameClient/GUI/Shell/Shell.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Shell.o $(GES)/GameClient/GUI/Shell/Shell.cpp

$(GEO)/ShellMenuScheme.o: $(GES)/GameClient/GUI/Shell/ShellMenuScheme.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ShellMenuScheme.o $(GES)/GameClient/GUI/Shell/ShellMenuScheme.cpp

# --- GameEngine/Source/GameClient/Input ---
$(GEO)/Keyboard.o: $(GES)/GameClient/Input/Keyboard.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Keyboard.o $(GES)/GameClient/Input/Keyboard.cpp

$(GEO)/Mouse.o: $(GES)/GameClient/Input/Mouse.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Mouse.o $(GES)/GameClient/Input/Mouse.cpp

# --- GameEngine/Source/GameClient/MessageStream ---
$(GEO)/WindowXlat.o: $(GES)/GameClient/MessageStream/WindowXlat.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/WindowXlat.o $(GES)/GameClient/MessageStream/WindowXlat.cpp

# --- GameEngine/Source/GameClient/System ---
$(GEO)/Anim2D.o: $(GES)/GameClient/System/Anim2D.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Anim2D.o $(GES)/GameClient/System/Anim2D.cpp

$(GEO)/CampaignManager.o: $(GES)/GameClient/System/CampaignManager.cpp GameEngine/Include/GameClient/CampaignManager.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CampaignManager.o $(GES)/GameClient/System/CampaignManager.cpp

$(GEO)/Image.o: $(GES)/GameClient/System/Image.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Image.o $(GES)/GameClient/System/Image.cpp

$(GEO)/ParticleSys.o: $(GES)/GameClient/System/ParticleSys.cpp GameEngine/Include/GameClient/ParticleSys.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ParticleSys.o $(GES)/GameClient/System/ParticleSys.cpp

# --- GameEngine/Source/GameClient/Terrain ---
$(GEO)/TerrainRoads.o: $(GES)/GameClient/Terrain/TerrainRoads.cpp GameEngine/Include/GameClient/TerrainRoads.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/TerrainRoads.o $(GES)/GameClient/Terrain/TerrainRoads.cpp

# --- GameEngine/Source/GameLogic/AI ---
$(GEO)/AI.o: $(GES)/GameLogic/AI/AI.cpp GameEngine/Include/GameLogic/AI.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AI.o $(GES)/GameLogic/AI/AI.cpp

$(GEO)/AIDock.o: $(GES)/GameLogic/AI/AIDock.cpp GameEngine/Include/GameLogic/AIDock.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AIDock.o $(GES)/GameLogic/AI/AIDock.cpp

$(GEO)/AIGroup.o: $(GES)/GameLogic/AI/AIGroup.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AIGroup.o $(GES)/GameLogic/AI/AIGroup.cpp

$(GEO)/AIGuard.o: $(GES)/GameLogic/AI/AIGuard.cpp GameEngine/Include/GameLogic/AIGuard.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AIGuard.o $(GES)/GameLogic/AI/AIGuard.cpp

$(GEO)/AIGuardRetaliate.o: $(GES)/GameLogic/AI/AIGuardRetaliate.cpp GameEngine/Include/GameLogic/AIGuardRetaliate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AIGuardRetaliate.o $(GES)/GameLogic/AI/AIGuardRetaliate.cpp

$(GEO)/AIPathfind.o: $(GES)/GameLogic/AI/AIPathfind.cpp GameEngine/Include/GameLogic/AIPathfind.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AIPathfind.o $(GES)/GameLogic/AI/AIPathfind.cpp

$(GEO)/AIStates.o: $(GES)/GameLogic/AI/AIStates.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AIStates.o $(GES)/GameLogic/AI/AIStates.cpp

$(GEO)/AITNGuard.o: $(GES)/GameLogic/AI/AITNGuard.cpp GameEngine/Include/GameLogic/AITNGuard.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AITNGuard.o $(GES)/GameLogic/AI/AITNGuard.cpp

$(GEO)/Squad.o: $(GES)/GameLogic/AI/Squad.cpp GameEngine/Include/GameLogic/Squad.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Squad.o $(GES)/GameLogic/AI/Squad.cpp

$(GEO)/TurretAI.o: $(GES)/GameLogic/AI/TurretAI.cpp GameEngine/Include/GameLogic/TurretAI.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/TurretAI.o $(GES)/GameLogic/AI/TurretAI.cpp

# --- GameEngine/Source/GameLogic/Map ---
$(GEO)/PolygonTrigger.o: $(GES)/GameLogic/Map/PolygonTrigger.cpp GameEngine/Include/GameLogic/PolygonTrigger.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/PolygonTrigger.o $(GES)/GameLogic/Map/PolygonTrigger.cpp

$(GEO)/SidesList.o: $(GES)/GameLogic/Map/SidesList.cpp GameEngine/Include/GameLogic/SidesList.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SidesList.o $(GES)/GameLogic/Map/SidesList.cpp

$(GEO)/TerrainLogic.o: $(GES)/GameLogic/Map/TerrainLogic.cpp GameEngine/Include/GameLogic/TerrainLogic.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/TerrainLogic.o $(GES)/GameLogic/Map/TerrainLogic.cpp

# --- GameEngine/Source/GameLogic/Object ---
$(GEO)/Armor.o: $(GES)/GameLogic/Object/Armor.cpp GameEngine/Include/GameLogic/Armor.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Armor.o $(GES)/GameLogic/Object/Armor.cpp

$(GEO)/ExperienceTracker.o: $(GES)/GameLogic/Object/ExperienceTracker.cpp GameEngine/Include/GameLogic/ExperienceTracker.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ExperienceTracker.o $(GES)/GameLogic/Object/ExperienceTracker.cpp

$(GEO)/Locomotor.o: $(GES)/GameLogic/Object/Locomotor.cpp GameEngine/Include/GameLogic/Locomotor.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Locomotor.o $(GES)/GameLogic/Object/Locomotor.cpp

$(GEO)/Object.o: $(GES)/GameLogic/Object/Object.cpp GameEngine/Include/GameLogic/Object.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Object.o $(GES)/GameLogic/Object/Object.cpp

$(GEO)/ObjectCreationList.o: $(GES)/GameLogic/Object/ObjectCreationList.cpp GameEngine/Include/GameLogic/ObjectCreationList.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ObjectCreationList.o $(GES)/GameLogic/Object/ObjectCreationList.cpp

$(GEO)/ObjectTypes.o: $(GES)/GameLogic/Object/ObjectTypes.cpp GameEngine/Include/GameLogic/ObjectTypes.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ObjectTypes.o $(GES)/GameLogic/Object/ObjectTypes.cpp

$(GEO)/PartitionManager.o: $(GES)/GameLogic/Object/PartitionManager.cpp GameEngine/Include/GameLogic/PartitionManager.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/PartitionManager.o $(GES)/GameLogic/Object/PartitionManager.cpp

$(GEO)/SimpleObjectIterator.o: $(GES)/GameLogic/Object/SimpleObjectIterator.cpp GameEngine/Include/GameLogic/ObjectIter.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SimpleObjectIterator.o $(GES)/GameLogic/Object/SimpleObjectIterator.cpp

$(GEO)/Weapon.o: $(GES)/GameLogic/Object/Weapon.cpp GameEngine/Include/GameLogic/Weapon.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Weapon.o $(GES)/GameLogic/Object/Weapon.cpp

$(GEO)/WeaponSet.o: $(GES)/GameLogic/Object/WeaponSet.cpp GameEngine/Include/GameLogic/WeaponSet.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/WeaponSet.o $(GES)/GameLogic/Object/WeaponSet.cpp

# --- GameEngine/Source/GameLogic/Object/Behavior ---
$(GEO)/AutoHealBehavior.o: $(GES)/GameLogic/Object/Behavior/AutoHealBehavior.cpp GameEngine/Include/GameLogic/Module/AutoHealBehavior.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AutoHealBehavior.o $(GES)/GameLogic/Object/Behavior/AutoHealBehavior.cpp

$(GEO)/BehaviorModule.o: $(GES)/GameLogic/Object/Behavior/BehaviorModule.cpp GameEngine/Include/GameLogic/Module/BehaviorModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/BehaviorModule.o $(GES)/GameLogic/Object/Behavior/BehaviorModule.cpp

$(GEO)/BridgeBehavior.o: $(GES)/GameLogic/Object/Behavior/BridgeBehavior.cpp GameEngine/Include/GameLogic/Module/BridgeBehavior.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/BridgeBehavior.o $(GES)/GameLogic/Object/Behavior/BridgeBehavior.cpp

$(GEO)/BridgeScaffoldBehavior.o: $(GES)/GameLogic/Object/Behavior/BridgeScaffoldBehavior.cpp GameEngine/Include/GameLogic/Module/BridgeScaffoldBehavior.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/BridgeScaffoldBehavior.o $(GES)/GameLogic/Object/Behavior/BridgeScaffoldBehavior.cpp

$(GEO)/BridgeTowerBehavior.o: $(GES)/GameLogic/Object/Behavior/BridgeTowerBehavior.cpp GameEngine/Include/GameLogic/Module/BridgeTowerBehavior.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/BridgeTowerBehavior.o $(GES)/GameLogic/Object/Behavior/BridgeTowerBehavior.cpp

$(GEO)/GenerateMinefieldBehavior.o: $(GES)/GameLogic/Object/Behavior/GenerateMinefieldBehavior.cpp GameEngine/Include/GameLogic/Module/GenerateMinefieldBehavior.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GenerateMinefieldBehavior.o $(GES)/GameLogic/Object/Behavior/GenerateMinefieldBehavior.cpp

# --- GameEngine/Source/GameLogic/Object/Body ---
$(GEO)/BodyModule.o: $(GES)/GameLogic/Object/Body/BodyModule.cpp GameEngine/Include/GameLogic/Module/BodyModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/BodyModule.o $(GES)/GameLogic/Object/Body/BodyModule.cpp

$(GEO)/InactiveBody.o: $(GES)/GameLogic/Object/Body/InactiveBody.cpp GameEngine/Include/GameLogic/Module/InactiveBody.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/InactiveBody.o $(GES)/GameLogic/Object/Body/InactiveBody.cpp

# --- GameEngine/Source/GameLogic/Object/Collide ---
$(GEO)/CollideModule.o: $(GES)/GameLogic/Object/Collide/CollideModule.cpp GameEngine/Include/GameLogic/Module/CollideModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CollideModule.o $(GES)/GameLogic/Object/Collide/CollideModule.cpp

$(GEO)/SquishCollide.o: $(GES)/GameLogic/Object/Collide/SquishCollide.cpp GameEngine/Include/GameLogic/Module/SquishCollide.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SquishCollide.o $(GES)/GameLogic/Object/Collide/SquishCollide.cpp

# --- GameEngine/Source/GameLogic/Object/Collide/CrateCollide ---
$(GEO)/CrateCollide.o: $(GES)/GameLogic/Object/Collide/CrateCollide/CrateCollide.cpp GameEngine/Include/GameLogic/Module/CrateCollide.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CrateCollide.o $(GES)/GameLogic/Object/Collide/CrateCollide/CrateCollide.cpp

$(GEO)/MoneyCrateCollide.o: $(GES)/GameLogic/Object/Collide/CrateCollide/MoneyCrateCollide.cpp GameEngine/Include/GameLogic/Module/MoneyCrateCollide.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MoneyCrateCollide.o $(GES)/GameLogic/Object/Collide/CrateCollide/MoneyCrateCollide.cpp

$(GEO)/SalvageCrateCollide.o: $(GES)/GameLogic/Object/Collide/CrateCollide/SalvageCrateCollide.cpp GameEngine/Include/GameLogic/Module/SalvageCrateCollide.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SalvageCrateCollide.o $(GES)/GameLogic/Object/Collide/CrateCollide/SalvageCrateCollide.cpp

$(GEO)/UnitCrateCollide.o: $(GES)/GameLogic/Object/Collide/CrateCollide/UnitCrateCollide.cpp GameEngine/Include/GameLogic/Module/UnitCrateCollide.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UnitCrateCollide.o $(GES)/GameLogic/Object/Collide/CrateCollide/UnitCrateCollide.cpp

$(GEO)/VeterancyCrateCollide.o: $(GES)/GameLogic/Object/Collide/CrateCollide/VeterancyCrateCollide.cpp GameEngine/Include/GameLogic/Module/VeterancyCrateCollide.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/VeterancyCrateCollide.o $(GES)/GameLogic/Object/Collide/CrateCollide/VeterancyCrateCollide.cpp

# --- GameEngine/Source/GameLogic/Object/Contain ---
$(GEO)/CaveContain.o: $(GES)/GameLogic/Object/Contain/CaveContain.cpp GameEngine/Include/GameLogic/Module/CaveContain.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CaveContain.o $(GES)/GameLogic/Object/Contain/CaveContain.cpp

$(GEO)/OpenContain.o: $(GES)/GameLogic/Object/Contain/OpenContain.cpp GameEngine/Include/GameLogic/Module/OpenContain.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/OpenContain.o $(GES)/GameLogic/Object/Contain/OpenContain.cpp

# --- GameEngine/Source/GameLogic/Object/Damage ---
$(GEO)/DamageModule.o: $(GES)/GameLogic/Object/Damage/DamageModule.cpp GameEngine/Include/GameLogic/Module/DamageModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DamageModule.o $(GES)/GameLogic/Object/Damage/DamageModule.cpp

# --- GameEngine/Source/GameLogic/Object/Die ---
$(GEO)/DestroyDie.o: $(GES)/GameLogic/Object/Die/DestroyDie.cpp GameEngine/Include/GameLogic/Module/DestroyDie.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DestroyDie.o $(GES)/GameLogic/Object/Die/DestroyDie.cpp

$(GEO)/DieModule.o: $(GES)/GameLogic/Object/Die/DieModule.cpp GameEngine/Include/GameLogic/Module/DieModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DieModule.o $(GES)/GameLogic/Object/Die/DieModule.cpp

$(GEO)/SpecialPowerCompletionDie.o: $(GES)/GameLogic/Object/Die/SpecialPowerCompletionDie.cpp GameEngine/Include/GameLogic/Module/SpecialPowerCompletionDie.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SpecialPowerCompletionDie.o $(GES)/GameLogic/Object/Die/SpecialPowerCompletionDie.cpp

# --- GameEngine/Source/GameLogic/Object/Helper ---
$(GEO)/ObjectHelper.o: $(GES)/GameLogic/Object/Helper/ObjectHelper.cpp GameEngine/Include/GameLogic/Module/ObjectHelper.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ObjectHelper.o $(GES)/GameLogic/Object/Helper/ObjectHelper.cpp

$(GEO)/ObjectDefectionHelper.o: $(GES)/GameLogic/Object/Helper/ObjectDefectionHelper.cpp GameEngine/Include/GameLogic/Module/ObjectDefectionHelper.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ObjectDefectionHelper.o $(GES)/GameLogic/Object/Helper/ObjectDefectionHelper.cpp

# --- GameEngine/Source/GameLogic/Object/Update ---
$(GEO)/AIUpdate.o: $(GES)/GameLogic/Object/Update/AIUpdate.cpp GameEngine/Include/GameLogic/Module/AIUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AIUpdate.o $(GES)/GameLogic/Object/Update/AIUpdate.cpp

$(GEO)/CommandButtonHuntUpdate.o: $(GES)/GameLogic/Object/Update/CommandButtonHuntUpdate.cpp GameEngine/Include/GameLogic/Module/CommandButtonHuntUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CommandButtonHuntUpdate.o $(GES)/GameLogic/Object/Update/CommandButtonHuntUpdate.cpp

$(GEO)/DeletionUpdate.o: $(GES)/GameLogic/Object/Update/DeletionUpdate.cpp GameEngine/Include/GameLogic/Module/DeletionUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DeletionUpdate.o $(GES)/GameLogic/Object/Update/DeletionUpdate.cpp

$(GEO)/LaserUpdate.o: $(GES)/GameLogic/Object/Update/LaserUpdate.cpp GameEngine/Include/GameLogic/Module/LaserUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/LaserUpdate.o $(GES)/GameLogic/Object/Update/LaserUpdate.cpp

$(GEO)/PhysicsUpdate.o: $(GES)/GameLogic/Object/Update/PhysicsUpdate.cpp GameEngine/Include/GameLogic/Module/PhysicsUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/PhysicsUpdate.o $(GES)/GameLogic/Object/Update/PhysicsUpdate.cpp

$(GEO)/ProneUpdate.o: $(GES)/GameLogic/Object/Update/ProneUpdate.cpp GameEngine/Include/GameLogic/Module/ProneUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ProneUpdate.o $(GES)/GameLogic/Object/Update/ProneUpdate.cpp

$(GEO)/RadiusDecalUpdate.o: $(GES)/GameLogic/Object/Update/RadiusDecalUpdate.cpp GameEngine/Include/GameLogic/Module/RadiusDecalUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RadiusDecalUpdate.o $(GES)/GameLogic/Object/Update/RadiusDecalUpdate.cpp

$(GEO)/SmartBombTargetHomingUpdate.o: $(GES)/GameLogic/Object/Update/SmartBombTargetHomingUpdate.cpp GameEngine/Include/GameLogic/Module/SmartBombTargetHomingUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SmartBombTargetHomingUpdate.o $(GES)/GameLogic/Object/Update/SmartBombTargetHomingUpdate.cpp

$(GEO)/SpecialAbilityUpdate.o: $(GES)/GameLogic/Object/Update/SpecialAbilityUpdate.cpp GameEngine/Include/GameLogic/Module/SpecialAbilityUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SpecialAbilityUpdate.o $(GES)/GameLogic/Object/Update/SpecialAbilityUpdate.cpp

$(GEO)/SpecialPowerUpdateModule.o: $(GES)/GameLogic/Object/Update/SpecialPowerUpdateModule.cpp GameEngine/Include/GameLogic/Module/SpecialPowerUpdateModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SpecialPowerUpdateModule.o $(GES)/GameLogic/Object/Update/SpecialPowerUpdateModule.cpp

$(GEO)/StealthUpdate.o: $(GES)/GameLogic/Object/Update/StealthUpdate.cpp GameEngine/Include/GameLogic/Module/StealthUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/StealthUpdate.o $(GES)/GameLogic/Object/Update/StealthUpdate.cpp

$(GEO)/StickyBombUpdate.o: $(GES)/GameLogic/Object/Update/StickyBombUpdate.cpp GameEngine/Include/GameLogic/Module/StickyBombUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/StickyBombUpdate.o $(GES)/GameLogic/Object/Update/StickyBombUpdate.cpp

$(GEO)/UpdateModule.o: $(GES)/GameLogic/Object/Update/UpdateModule.cpp GameEngine/Include/GameLogic/Module/UpdateModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UpdateModule.o $(GES)/GameLogic/Object/Update/UpdateModule.cpp

# --- GameEngine/Source/GameLogic/Object/Update/AIUpdate ---
$(GEO)/DeliverPayloadAIUpdate.o: $(GES)/GameLogic/Object/Update/AIUpdate/DeliverPayloadAIUpdate.cpp GameEngine/Include/GameLogic/Module/DeliverPayloadAIUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DeliverPayloadAIUpdate.o $(GES)/GameLogic/Object/Update/AIUpdate/DeliverPayloadAIUpdate.cpp

$(GEO)/HackInternetAIUpdate.o: $(GES)/GameLogic/Object/Update/AIUpdate/HackInternetAIUpdate.cpp GameEngine/Include/GameLogic/Module/HackInternetAIUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/HackInternetAIUpdate.o $(GES)/GameLogic/Object/Update/AIUpdate/HackInternetAIUpdate.cpp

$(GEO)/RailroadGuideAIUpdate.o: $(GES)/GameLogic/Object/Update/AIUpdate/RailroadGuideAIUpdate.cpp GameEngine/Include/GameLogic/Module/RailroadGuideAIUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RailroadGuideAIUpdate.o $(GES)/GameLogic/Object/Update/AIUpdate/RailroadGuideAIUpdate.cpp

$(GEO)/SupplyTruckAIUpdate.o: $(GES)/GameLogic/Object/Update/AIUpdate/SupplyTruckAIUpdate.cpp GameEngine/Include/GameLogic/Module/SupplyTruckAIUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SupplyTruckAIUpdate.o $(GES)/GameLogic/Object/Update/AIUpdate/SupplyTruckAIUpdate.cpp

# --- GameEngine/Source/GameLogic/Object/Update/DockUpdate ---
$(GEO)/DockUpdate.o: $(GES)/GameLogic/Object/Update/DockUpdate/DockUpdate.cpp GameEngine/Include/GameLogic/Module/DockUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DockUpdate.o $(GES)/GameLogic/Object/Update/DockUpdate/DockUpdate.cpp

$(GEO)/SupplyWarehouseDockUpdate.o: $(GES)/GameLogic/Object/Update/DockUpdate/SupplyWarehouseDockUpdate.cpp GameEngine/Include/GameLogic/Module/SupplyWarehouseDockUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SupplyWarehouseDockUpdate.o $(GES)/GameLogic/Object/Update/DockUpdate/SupplyWarehouseDockUpdate.cpp

# --- GameEngine/Source/GameLogic/Object/Upgrade ---
$(GEO)/UpgradeModule.o: $(GES)/GameLogic/Object/Upgrade/UpgradeModule.cpp GameEngine/Include/GameLogic/Module/UpgradeModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UpgradeModule.o $(GES)/GameLogic/Object/Upgrade/UpgradeModule.cpp

# --- GameEngine/Source/GameLogic/ScriptEngine ---
$(GEO)/ScriptActions.o: $(GES)/GameLogic/ScriptEngine/ScriptActions.cpp GameEngine/Include/GameLogic/ScriptActions.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ScriptActions.o $(GES)/GameLogic/ScriptEngine/ScriptActions.cpp

$(GEO)/ScriptConditions.o: $(GES)/GameLogic/ScriptEngine/ScriptConditions.cpp GameEngine/Include/GameLogic/ScriptConditions.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ScriptConditions.o $(GES)/GameLogic/ScriptEngine/ScriptConditions.cpp

$(GEO)/ScriptEngine.o: $(GES)/GameLogic/ScriptEngine/ScriptEngine.cpp GameEngine/Include/GameLogic/ScriptEngine.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ScriptEngine.o $(GES)/GameLogic/ScriptEngine/ScriptEngine.cpp

$(GEO)/Scripts.o: $(GES)/GameLogic/ScriptEngine/Scripts.cpp GameEngine/Include/GameLogic/Scripts.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Scripts.o $(GES)/GameLogic/ScriptEngine/Scripts.cpp

$(GEO)/VictoryConditions.o: $(GES)/GameLogic/ScriptEngine/VictoryConditions.cpp GameEngine/Include/GameLogic/VictoryConditions.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/VictoryConditions.o $(GES)/GameLogic/ScriptEngine/VictoryConditions.cpp

# --- GameEngine/Source/GameLogic/System ---
$(GEO)/CaveSystem.o: $(GES)/GameLogic/System/CaveSystem.cpp GameEngine/Include/GameLogic/CaveSystem.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CaveSystem.o $(GES)/GameLogic/System/CaveSystem.cpp

$(GEO)/CrateSystem.o: $(GES)/GameLogic/System/CrateSystem.cpp GameEngine/Include/GameLogic/CrateSystem.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CrateSystem.o $(GES)/GameLogic/System/CrateSystem.cpp

$(GEO)/Damage.o: $(GES)/GameLogic/System/Damage.cpp GameEngine/Include/GameLogic/Damage.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Damage.o $(GES)/GameLogic/System/Damage.cpp

$(GEO)/GameLogic.o: $(GES)/GameLogic/System/GameLogic.cpp GameEngine/Include/GameLogic/GameLogic.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameLogic.o $(GES)/GameLogic/System/GameLogic.cpp

$(GEO)/GameLogicDispatch.o: $(GES)/GameLogic/System/GameLogicDispatch.cpp GameEngine/Include/GameLogic/GameLogic.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameLogicDispatch.o $(GES)/GameLogic/System/GameLogicDispatch.cpp

$(GEO)/RankInfo.o: $(GES)/GameLogic/System/RankInfo.cpp GameEngine/Include/GameLogic/RankInfo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RankInfo.o $(GES)/GameLogic/System/RankInfo.cpp

# --- GameEngine/Source/GameNetwork ---
$(GEO)/GameInfo.o: $(GES)/GameNetwork/GameInfo.cpp GameEngine/Include/GameNetwork/GameInfo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameInfo.o $(GES)/GameNetwork/GameInfo.cpp

# --- GameEngine/Source/GameNetwork/GameSpy ---
$(GEO)/Chat.o: $(GES)/GameNetwork/GameSpy/Chat.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Chat.o $(GES)/GameNetwork/GameSpy/Chat.cpp


run: $(PROG)
	cd ../Run && ./rts && cd $(CUR_DIR)

debug: $(PROG)
	cd ../Run && gdb -q ./rts && cd $(CUR_DIR)

clean:
	rm -v $(GEDO)/*.o $(GEO)/*.o $(LIBO)/*.o $(ENGINE_DEVICE_LIB) $(ENGINE_LIB) Libraries/Lib/*.a $(PROG)

# make DEBUG=1 &> build.log
