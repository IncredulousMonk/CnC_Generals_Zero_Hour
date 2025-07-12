CXXFLAGS = -std=c++23 -O2 -Wall -Weffc++ -Wextra -Wsign-conversion -Werror -pedantic-errors
LDPATHS = -LGameEngineDevice/Lib -LGameEngine/Lib -LLibraries/Lib
LDFLAGS = -lavutil -lavcodec -lswscale -lswresample
CXXFLAGS += $(shell pkg-config sdl3 --cflags)
LDFLAGS += $(shell pkg-config sdl3 --libs) $(shell pkg-config sdl3-ttf --libs) $(shell pkg-config sdl3-image --libs) $(shell pkg-config sdl3-sound --libs)
INCS = -I. -ILibraries/Include -ILibraries/Source/WWVegas -ILibraries/Source/WWVegas/WWLib -ILibraries/Source/WWVegas/WWDebug -IGameEngine/Include -IGameEngineDevice/Include -IGameEngine/Include/Precompiled
PROG = ../Run/rts
ENGINE_DEVICE_LIB = GameEngineDevice/Lib/GameEngineDevice.a
ENGINE_LIB = GameEngine/Lib/GameEngine.a
WWMATH_LIB = Libraries/Lib/WWMath.a
GEDO = GameEngineDevice/Obj
GEDSLDC = GameEngineDevice/Source/LinuxDevice/Common
GEDSLDF = GameEngineDevice/Source/LinuxDevice/FFmpeg
GEDSLD = GameEngineDevice/Source/LinuxDevice
GEO = GameEngine/Obj
GES = GameEngine/Source
WWMO = Libraries/Obj
WWMS = Libraries/Source/WWVegas/WWMath
CUR_DIR := $(shell pwd)


ifdef DEBUG
CXXFLAGS += -D_DEBUG -O0 -g3
endif

.PHONY: run debug clean

$(PROG): Main/main.cpp $(ENGINE_DEVICE_LIB) $(ENGINE_LIB)
	g++ $(CXXFLAGS) $(INCS) -o $(PROG) Main/main.cpp $(LDPATHS) -l:GameEngineDevice.a -l:GameEngine.a $(LDFLAGS)


# ===== WWMath =====

WWMATH_OBJS = $(WWMO)/matrix3d.o $(WWMO)/quat.o

$(WWMATH_LIB): $(WWMATH_OBJS)
	ar rcs $(WWMATH_LIB)  $(WWMATH_OBJS)

# --- Libraries/Source/WWVegas/WWMath ---
$(WWMO)/matrix3d.o: $(WWMS)/matrix3d.cpp $(WWMS)/matrix3d.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(WWMO)/matrix3d.o $(WWMS)/matrix3d.cpp

$(WWMO)/quat.o: $(WWMS)/quat.cpp $(WWMS)/quat.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(WWMO)/quat.o $(WWMS)/quat.cpp


# ===== GameEngineDevice =====

ENGINE_DEVICE_OBJS = $(GEDO)/SdlAudioManager.o \
$(GEDO)/LinuxBIGFile.o $(GEDO)/LinuxBIGFileSystem.o $(GEDO)/LinuxGameEngine.o $(GEDO)/LinuxLocalFile.o $(GEDO)/LinuxLocalFileSystem.o $(GEDO)/SdlFileStream.o \
$(GEDO)/LinuxFunctionLexicon.o \
$(GEDO)/LinuxModuleFactory.o \
$(GEDO)/FFmpegVideoPlayer.o $(GEDO)/FFmpegVideo.o $(GEDO)/PacketQueue.o $(GEDO)/FrameQueue.o $(GEDO)/Decoder.o $(GEDO)/Clock.o \
$(GEDO)/LinuxDisplay.o $(GEDO)/LinuxDisplayString.o $(GEDO)/LinuxDisplayStringManager.o $(GEDO)/LinuxGameClient.o $(GEDO)/LinuxParticleSys.o $(GEDO)/SdlKeyboard.o $(GEDO)/SdlMouse.o \
$(GEDO)/LinuxDefaultDraw.o \
$(GEDO)/LinuxGameFont.o $(GEDO)/LinuxGameWindow.o $(GEDO)/LinuxGameWindowManager.o \
$(GEDO)/LinuxCheckBox.o $(GEDO)/LinuxHorizontalSlider.o $(GEDO)/LinuxListBox.o $(GEDO)/LinuxProgressBar.o $(GEDO)/LinuxPushButton.o $(GEDO)/LinuxRadioButton.o $(GEDO)/LinuxStaticText.o \
   $(GEDO)/LinuxTextEntry.o $(GEDO)/LinuxVerticalSlider.o \
$(GEDO)/LinuxControlBar.o $(GEDO)/LinuxMainMenu.o $(GEDO)/LinuxMOTD.o \
$(GEDO)/LinuxGameLogic.o

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

$(GEDO)/LinuxGameClient.o: $(GEDSLD)/GameClient/LinuxGameClient.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxGameClient.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameClient.o $(GEDSLD)/GameClient/LinuxGameClient.cpp

$(GEDO)/LinuxParticleSys.o: $(GEDSLD)/GameClient/LinuxParticleSys.cpp GameEngineDevice/Include/LinuxDevice/GameClient/LinuxParticleSys.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxParticleSys.o $(GEDSLD)/GameClient/LinuxParticleSys.cpp

$(GEDO)/SdlKeyboard.o: $(GEDSLD)/GameClient/SdlKeyboard.cpp GameEngineDevice/Include/LinuxDevice/GameClient/SdlKeyboard.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/SdlKeyboard.o $(GEDSLD)/GameClient/SdlKeyboard.cpp

$(GEDO)/SdlMouse.o: $(GEDSLD)/GameClient/SdlMouse.cpp GameEngineDevice/Include/LinuxDevice/GameClient/SdlMouse.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/SdlMouse.o $(GEDSLD)/GameClient/SdlMouse.cpp

# --- GameEngineDevice/Source/LinuxDevice/GameClient/Drawable/Draw ---
$(GEDO)/LinuxDefaultDraw.o: $(GEDSLD)/GameClient/Drawable/Draw/LinuxDefaultDraw.cpp GameEngineDevice/Include/LinuxDevice/GameClient/Module/LinuxDefaultDraw.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxDefaultDraw.o $(GEDSLD)/GameClient/Drawable/Draw/LinuxDefaultDraw.cpp

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


# ===== GameEngine =====

ENGINE_OBJS = $(GEO)/BitFlags.o $(GEO)/CommandLine.o $(GEO)/DamageFX.o $(GEO)/Dict.o $(GEO)/DiscreteCircle.o $(GEO)/GameEngine.o $(GEO)/GameLOD.o $(GEO)/GameMain.o $(GEO)/GlobalData.o \
   $(GEO)/Language.o $(GEO)/MessageStream.o $(GEO)/MultiplayerSettings.o $(GEO)/NameKeyGenerator.o $(GEO)/RandomValue.o $(GEO)/TerrainTypes.o $(GEO)/UserPreferences.o $(GEO)/Version.o \
$(GEO)/AudioEventRTS.o $(GEO)/AudioRequest.o $(GEO)/GameAudio.o $(GEO)/GameMusic.o $(GEO)/GameSounds.o \
$(GEO)/INI.o $(GEO)/INIAnimation.o $(GEO)/INIAudioEventInfo.o $(GEO)/INIDamageFX.o $(GEO)/INIDrawGroupInfo.o $(GEO)/INIGameData.o $(GEO)/INIMappedImage.o $(GEO)/INIMiscAudio.o \
   $(GEO)/INIMultiplayer.o $(GEO)/INIObject.o $(GEO)/INIParticleSys.o $(GEO)/INISpecialPower.o $(GEO)/INITerrain.o $(GEO)/INITerrainBridge.o $(GEO)/INITerrainRoad.o $(GEO)/INIVideo.o \
   $(GEO)/INIWater.o $(GEO)/INIWeapon.o \
$(GEO)/AcademyStats.o $(GEO)/Energy.o $(GEO)/Handicap.o $(GEO)/MissionStats.o $(GEO)/Money.o $(GEO)/ProductionPrerequisite.o $(GEO)/Player.o $(GEO)/PlayerList.o $(GEO)/PlayerTemplate.o \
   $(GEO)/Science.o $(GEO)/ScoreKeeper.o $(GEO)/SpecialPower.o $(GEO)/Team.o $(GEO)/TunnelTracker.o \
$(GEO)/ArchiveFile.o $(GEO)/ArchiveFileSystem.o $(GEO)/AsciiString.o $(GEO)/CriticalSection.o $(GEO)/Debug.o $(GEO)/DisabledTypes.o $(GEO)/File.o $(GEO)/FileSystem.o \
   $(GEO)/FunctionLexicon.o $(GEO)/GameCommon.o $(GEO)/GameMemory.o $(GEO)/GameType.o $(GEO)/Geometry.o $(GEO)/KindOf.o $(GEO)/LocalFileSystem.o $(GEO)/MemoryInit.o \
   $(GEO)/ObjectStatusTypes.o $(GEO)/RAMFile.o $(GEO)/Snapshot.o $(GEO)/StreamingArchiveFile.o $(GEO)/SubsystemInterface.o $(GEO)/Trig.o $(GEO)/UnicodeString.o $(GEO)/Upgrade.o \
$(GEO)/GameState.o \
$(GEO)/DrawModule.o $(GEO)/Module.o $(GEO)/ModuleFactory.o $(GEO)/Thing.o $(GEO)/ThingFactory.o $(GEO)/ThingTemplate.o \
$(GEO)/Color.o $(GEO)/Credits.o $(GEO)/Display.o $(GEO)/DisplayString.o $(GEO)/DisplayStringManager.o $(GEO)/Drawable.o $(GEO)/DrawGroupInfo.o $(GEO)/FXList.o $(GEO)/GameClient.o \
   $(GEO)/GameClientDispatch.o $(GEO)/GameText.o $(GEO)/GlobalLanguage.o $(GEO)/InGameUI.o $(GEO)/LanguageFilter.o $(GEO)/Line2D.o $(GEO)/RadiusDecal.o $(GEO)/Snow.o \
   $(GEO)/VideoPlayer.o $(GEO)/Water.o \
$(GEO)/AnimateWindowManager.o $(GEO)/GameFont.o $(GEO)/GameWindow.o $(GEO)/GameWindowGlobal.o $(GEO)/GameWindowManager.o $(GEO)/GameWindowManagerScript.o $(GEO)/GameWindowTransitions.o \
   $(GEO)/GameWindowTransitionsStyles.o $(GEO)/HeaderTemplate.o $(GEO)/IMEManager.o $(GEO)/LoadScreen.o $(GEO)/ProcessAnimateWindow.o $(GEO)/WinInstanceData.o $(GEO)/WindowLayout.o \
$(GEO)/GadgetCheckBox.o $(GEO)/GadgetComboBox.o $(GEO)/GadgetHorizontalSlider.o $(GEO)/GadgetListBox.o $(GEO)/GadgetProgressBar.o $(GEO)/GadgetPushButton.o $(GEO)/GadgetRadioButton.o \
   $(GEO)/GadgetStaticText.o $(GEO)/GadgetTabControl.o $(GEO)/GadgetTextEntry.o $(GEO)/GadgetVerticalSlider.o \
$(GEO)/ExtendedMessageBox.o $(GEO)/MessageBox.o \
$(GEO)/CreditsMenu.o $(GEO)/MainMenu.o $(GEO)/OptionsMenu.o \
$(GEO)/Shell.o $(GEO)/ShellMenuScheme.o $(GEO)/Keyboard.o $(GEO)/Mouse.o \
$(GEO)/WindowXlat.o \
$(GEO)/Anim2D.o $(GEO)/CampaignManager.o $(GEO)/Image.o $(GEO)/ParticleSys.o \
$(GEO)/TerrainRoads.o \
$(GEO)/Squad.o \
$(GEO)/PolygonTrigger.o $(GEO)/SidesList.o $(GEO)/TerrainLogic.o \
$(GEO)/Armor.o $(GEO)/Object.o $(GEO)/PartitionManager.o $(GEO)/SimpleObjectIterator.o $(GEO)/Weapon.o $(GEO)/WeaponSet.o \
$(GEO)/AutoHealBehavior.o $(GEO)/BehaviorModule.o \
$(GEO)/BodyModule.o $(GEO)/InactiveBody.o \
$(GEO)/CollideModule.o \
$(GEO)/DamageModule.o \
$(GEO)/DestroyDie.o $(GEO)/DieModule.o \
$(GEO)/StealthUpdate.o $(GEO)/UpdateModule.o \
$(GEO)/UpgradeModule.o \
$(GEO)/CaveSystem.o $(GEO)/Damage.o $(GEO)/GameLogic.o $(GEO)/GameLogicDispatch.o $(GEO)/RankInfo.o \
$(GEO)/Chat.o

$(ENGINE_LIB): $(ENGINE_OBJS)
	ar rcs $(ENGINE_LIB) $(ENGINE_OBJS)

# --- GameEngine/Source/Common ---
$(GEO)/BitFlags.o: $(GES)/Common/BitFlags.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/BitFlags.o $(GES)/Common/BitFlags.cpp

$(GEO)/CommandLine.o: $(GES)/Common/CommandLine.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CommandLine.o $(GES)/Common/CommandLine.cpp

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

$(GEO)/RandomValue.o: $(GES)/Common/RandomValue.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RandomValue.o $(GES)/Common/RandomValue.cpp

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

$(GEO)/INIAudioEventInfo.o: $(GES)/Common/INI/INIAudioEventInfo.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIAudioEventInfo.o $(GES)/Common/INI/INIAudioEventInfo.cpp

$(GEO)/INIDamageFX.o: $(GES)/Common/INI/INIDamageFX.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIDamageFX.o $(GES)/Common/INI/INIDamageFX.cpp

$(GEO)/INIDrawGroupInfo.o: $(GES)/Common/INI/INIDrawGroupInfo.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIDrawGroupInfo.o $(GES)/Common/INI/INIDrawGroupInfo.cpp

$(GEO)/INIGameData.o: $(GES)/Common/INI/INIGameData.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIGameData.o $(GES)/Common/INI/INIGameData.cpp

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

$(GEO)/INIVideo.o: $(GES)/Common/INI/INIVideo.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIVideo.o $(GES)/Common/INI/INIVideo.cpp

$(GEO)/INIWater.o: $(GES)/Common/INI/INIWater.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIWater.o $(GES)/Common/INI/INIWater.cpp

$(GEO)/INIWeapon.o: $(GES)/Common/INI/INIWeapon.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIWeapon.o $(GES)/Common/INI/INIWeapon.cpp

# --- GameEngine/Source/Common/RTS ---
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

$(GEO)/CriticalSection.o: $(GES)/Common/System/CriticalSection.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CriticalSection.o $(GES)/Common/System/CriticalSection.cpp

#$(GEO)/DataChunk.o: $(GES)/Common/System/DataChunk.cpp GameEngine/Include/Common/DataChunk.h
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DataChunk.o $(GES)/Common/System/DataChunk.cpp

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

#$(GEO)/MapUtil.o: $(GES)/GameClient/MapUtil.cpp GameEngine/Include/GameClient/MapUtil.h
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MapUtil.o $(GES)/GameClient/MapUtil.cpp

$(GEO)/RadiusDecal.o: $(GES)/GameClient/RadiusDecal.cpp GameEngine/Include/GameClient/RadiusDecal.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RadiusDecal.o $(GES)/GameClient/RadiusDecal.cpp

$(GEO)/Snow.o: $(GES)/GameClient/Snow.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Snow.o $(GES)/GameClient/Snow.cpp

$(GEO)/VideoPlayer.o: $(GES)/GameClient/VideoPlayer.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/VideoPlayer.o $(GES)/GameClient/VideoPlayer.cpp

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
$(GEO)/Squad.o: $(GES)/GameLogic/AI/Squad.cpp GameEngine/Include/GameLogic/Squad.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Squad.o $(GES)/GameLogic/AI/Squad.cpp

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

$(GEO)/Object.o: $(GES)/GameLogic/Object/Object.cpp GameEngine/Include/GameLogic/Object.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Object.o $(GES)/GameLogic/Object/Object.cpp

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

# --- GameEngine/Source/GameLogic/Object/Body ---
$(GEO)/BodyModule.o: $(GES)/GameLogic/Object/Body/BodyModule.cpp GameEngine/Include/GameLogic/Module/BodyModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/BodyModule.o $(GES)/GameLogic/Object/Body/BodyModule.cpp

$(GEO)/InactiveBody.o: $(GES)/GameLogic/Object/Body/InactiveBody.cpp GameEngine/Include/GameLogic/Module/InactiveBody.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/InactiveBody.o $(GES)/GameLogic/Object/Body/InactiveBody.cpp

# --- GameEngine/Source/GameLogic/Object/Collide ---
$(GEO)/CollideModule.o: $(GES)/GameLogic/Object/Collide/CollideModule.cpp GameEngine/Include/GameLogic/Module/CollideModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CollideModule.o $(GES)/GameLogic/Object/Collide/CollideModule.cpp

# --- GameEngine/Source/GameLogic/Object/Damage ---
$(GEO)/DamageModule.o: $(GES)/GameLogic/Object/Damage/DamageModule.cpp GameEngine/Include/GameLogic/Module/DamageModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DamageModule.o $(GES)/GameLogic/Object/Damage/DamageModule.cpp

# --- GameEngine/Source/GameLogic/Object/Die ---
$(GEO)/DestroyDie.o: $(GES)/GameLogic/Object/Die/DestroyDie.cpp GameEngine/Include/GameLogic/Module/DestroyDie.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DestroyDie.o $(GES)/GameLogic/Object/Die/DestroyDie.cpp

$(GEO)/DieModule.o: $(GES)/GameLogic/Object/Die/DieModule.cpp GameEngine/Include/GameLogic/Module/DieModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DieModule.o $(GES)/GameLogic/Object/Die/DieModule.cpp

# --- GameEngine/Source/GameLogic/Object/Update ---
$(GEO)/StealthUpdate.o: $(GES)/GameLogic/Object/Update/StealthUpdate.cpp GameEngine/Include/GameLogic/Module/StealthUpdate.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/StealthUpdate.o $(GES)/GameLogic/Object/Update/StealthUpdate.cpp

$(GEO)/UpdateModule.o: $(GES)/GameLogic/Object/Update/UpdateModule.cpp GameEngine/Include/GameLogic/Module/UpdateModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UpdateModule.o $(GES)/GameLogic/Object/Update/UpdateModule.cpp

# --- GameEngine/Source/GameLogic/Object/Upgrade ---
$(GEO)/UpgradeModule.o: $(GES)/GameLogic/Object/Upgrade/UpgradeModule.cpp GameEngine/Include/GameLogic/Module/UpgradeModule.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UpgradeModule.o $(GES)/GameLogic/Object/Upgrade/UpgradeModule.cpp

# --- GameEngine/Source/GameLogic/System ---
$(GEO)/CaveSystem.o: $(GES)/GameLogic/System/CaveSystem.cpp GameEngine/Include/GameLogic/CaveSystem.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CaveSystem.o $(GES)/GameLogic/System/CaveSystem.cpp

$(GEO)/Damage.o: $(GES)/GameLogic/System/Damage.cpp GameEngine/Include/GameLogic/Damage.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Damage.o $(GES)/GameLogic/System/Damage.cpp

$(GEO)/GameLogic.o: $(GES)/GameLogic/System/GameLogic.cpp GameEngine/Include/GameLogic/GameLogic.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameLogic.o $(GES)/GameLogic/System/GameLogic.cpp

$(GEO)/GameLogicDispatch.o: $(GES)/GameLogic/System/GameLogicDispatch.cpp GameEngine/Include/GameLogic/GameLogic.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameLogicDispatch.o $(GES)/GameLogic/System/GameLogicDispatch.cpp

$(GEO)/RankInfo.o: $(GES)/GameLogic/System/RankInfo.cpp GameEngine/Include/GameLogic/RankInfo.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RankInfo.o $(GES)/GameLogic/System/RankInfo.cpp

# --- GameEngine/Source/GameNetwork/GameSpy ---
$(GEO)/Chat.o: $(GES)/GameNetwork/GameSpy/Chat.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Chat.o $(GES)/GameNetwork/GameSpy/Chat.cpp


run: $(PROG)
	cd ../Run && ./rts && cd $(CUR_DIR)

debug: $(PROG)
	cd ../Run && gdb -q ./rts && cd $(CUR_DIR)

clean:
	rm -v $(GEDO)/*.o $(GEO)/*.o $(WWMO)/*.o $(ENGINE_DEVICE_LIB) $(ENGINE_LIB) $(WWMATH_LIB) $(PROG)

# make DEBUG=1 &> build.log
