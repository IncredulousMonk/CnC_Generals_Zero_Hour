CXXFLAGS = -std=c++2a -O2 -Wall -Weffc++ -Wextra -Wsign-conversion -Werror -pedantic-errors
LDPATHS = -LGameEngineDevice/Lib -LGameEngine/Lib
LDFLAGS = -lavutil -lavcodec -lswscale -lswresample
CXXFLAGS += $(shell pkg-config sdl3 --cflags)
LDFLAGS += $(shell pkg-config sdl3 --libs)
INCS = -I. -ILibraries/Include -ILibraries/Source/WWVegas -ILibraries/Source/WWVegas/WWLib -IGameEngine/Include -IGameEngineDevice/Include -IGameEngine/Include/Precompiled
PROG = ../Run/rts
ENGINE_DEVICE_LIB = GameEngineDevice/Lib/GameEngineDevice.a
ENGINE_LIB = GameEngine/Lib/GameEngine.a
GEDO = GameEngineDevice/Obj
GEDSLDC = GameEngineDevice/Source/LinuxDevice/Common
GEDSLDF = GameEngineDevice/Source/LinuxDevice/FFmpeg
GEDSLD = GameEngineDevice/Source/LinuxDevice
GEDSWD = GameEngineDevice/Source/W3DDevice
GEO = GameEngine/Obj
GES = GameEngine/Source
CUR_DIR := $(shell pwd)


ifdef DEBUG
CXXFLAGS += -D_DEBUG -O0 -g3
endif

.PHONY: run debug clean

$(PROG): Main/main.cpp $(ENGINE_DEVICE_LIB) $(ENGINE_LIB)
	g++ $(CXXFLAGS) $(INCS) -o $(PROG) Main/main.cpp $(LDPATHS) -l:GameEngineDevice.a -l:GameEngine.a $(LDFLAGS)


# ===== GameEngineDevice =====

ENGINE_DEVICE_OBJS = $(GEDO)/SdlAudioManager.o \
$(GEDO)/LinuxBIGFile.o $(GEDO)/LinuxBIGFileSystem.o $(GEDO)/LinuxGameEngine.o $(GEDO)/LinuxLocalFile.o $(GEDO)/LinuxLocalFileSystem.o \
$(GEDO)/FFmpegVideoPlayer.o $(GEDO)/FFmpegVideo.o $(GEDO)/PacketQueue.o $(GEDO)/FrameQueue.o $(GEDO)/Decoder.o $(GEDO)/Clock.o \
$(GEDO)/LinuxDisplay.o $(GEDO)/LinuxGameClient.o $(GEDO)/SdlKeyboard.o $(GEDO)/SdlMouse.o \
$(GEDO)/W3DMOTD.o 

$(ENGINE_DEVICE_LIB): $(ENGINE_DEVICE_OBJS)
	ar rcs $(ENGINE_DEVICE_LIB)  $(ENGINE_DEVICE_OBJS)

# --- GameEngineDevice/Source/LinuxDevice/Audio ---
$(GEDO)/SdlAudioManager.o: $(GEDSLD)/Audio/SdlAudioManager.cpp
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
$(GEDO)/LinuxDisplay.o: $(GEDSLD)/GameClient/LinuxDisplay.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxDisplay.o $(GEDSLD)/GameClient/LinuxDisplay.cpp

$(GEDO)/LinuxGameClient.o: $(GEDSLD)/GameClient/LinuxGameClient.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameClient.o $(GEDSLD)/GameClient/LinuxGameClient.cpp

$(GEDO)/SdlKeyboard.o: $(GEDSLD)/GameClient/SdlKeyboard.cpp GameEngineDevice/Include/LinuxDevice/GameClient/SdlKeyboard.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/SdlKeyboard.o $(GEDSLD)/GameClient/SdlKeyboard.cpp

$(GEDO)/SdlMouse.o: $(GEDSLD)/GameClient/SdlMouse.cpp GameEngineDevice/Include/LinuxDevice/GameClient/SdlMouse.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/SdlMouse.o $(GEDSLD)/GameClient/SdlMouse.cpp

# --- GameEngineDevice/Source/W3DDevice/GameClient/GUI/GUICallbacks ---
$(GEDO)/W3DMOTD.o: $(GEDSWD)/GameClient/GUI/GUICallbacks/W3DMOTD.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/W3DMOTD.o $(GEDSWD)/GameClient/GUI/GUICallbacks/W3DMOTD.cpp


# ===== GameEngine =====

ENGINE_OBJS = $(GEO)/GameEngine.o $(GEO)/GameMain.o $(GEO)/GlobalData.o $(GEO)/Language.o $(GEO)/MessageStream.o $(GEO)/NameKeyGenerator.o $(GEO)/UserPreferences.o $(GEO)/Version.o \
$(GEO)/AudioEventRTS.o $(GEO)/GameAudio.o \
$(GEO)/INI.o $(GEO)/INIGameData.o $(GEO)/INIVideo.o $(GEO)/INIWater.o \
$(GEO)/ArchiveFile.o $(GEO)/ArchiveFileSystem.o $(GEO)/AsciiString.o $(GEO)/CriticalSection.o $(GEO)/Debug.o $(GEO)/File.o $(GEO)/FileSystem.o $(GEO)/FunctionLexicon.o $(GEO)/GameMemory.o \
   $(GEO)/GameType.o $(GEO)/LocalFileSystem.o $(GEO)/MemoryInit.o $(GEO)/RAMFile.o $(GEO)/Snapshot.o $(GEO)/StreamingArchiveFile.o $(GEO)/SubsystemInterface.o $(GEO)/UnicodeString.o \
$(GEO)/Display.o $(GEO)/DisplayStringManager.o $(GEO)/GameClient.o $(GEO)/GameClientDispatch.o $(GEO)/GameText.o $(GEO)/GlobalLanguage.o $(GEO)/LanguageFilter.o $(GEO)/Snow.o \
   $(GEO)/VideoPlayer.o $(GEO)/Water.o \
$(GEO)/GameWindow.o $(GEO)/GameWindowManager.o $(GEO)/GameWindowManagerScript.o $(GEO)/IMEManager.o $(GEO)/WinInstanceData.o $(GEO)/WindowLayout.o \
$(GEO)/GadgetCheckBox.o $(GEO)/GadgetComboBox.o $(GEO)/GadgetHorizontalSlider.o $(GEO)/GadgetListBox.o $(GEO)/GadgetProgressBar.o $(GEO)/GadgetPushButton.o $(GEO)/GadgetRadioButton.o \
   $(GEO)/GadgetStaticText.o $(GEO)/GadgetTabControl.o $(GEO)/GadgetTextEntry.o $(GEO)/GadgetVerticalSlider.o \
$(GEO)/ExtendedMessageBox.o $(GEO)/MessageBox.o \
$(GEO)/OptionsMenu.o \
$(GEO)/Shell.o $(GEO)/Keyboard.o $(GEO)/Mouse.o

$(ENGINE_LIB): $(ENGINE_OBJS)
	ar rcs $(ENGINE_LIB) $(ENGINE_OBJS)

# --- GameEngine/Source/Common ---
$(GEO)/GameEngine.o: $(GES)/Common/GameEngine.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameEngine.o $(GES)/Common/GameEngine.cpp

#$(GEO)/GameLOD.o: $(GES)/Common/GameLOD.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameLOD.o $(GES)/Common/GameLOD.cpp

$(GEO)/GameMain.o: $(GES)/Common/GameMain.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameMain.o $(GES)/Common/GameMain.cpp

$(GEO)/GlobalData.o: $(GES)/Common/GlobalData.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GlobalData.o $(GES)/Common/GlobalData.cpp

$(GEO)/Language.o: $(GES)/Common/Language.cpp GameEngine/Include/Common/Language.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Language.o $(GES)/Common/Language.cpp

$(GEO)/MessageStream.o: $(GES)/Common/MessageStream.cpp GameEngine/Include/Common/MessageStream.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MessageStream.o $(GES)/Common/MessageStream.cpp

$(GEO)/NameKeyGenerator.o: $(GES)/Common/NameKeyGenerator.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/NameKeyGenerator.o $(GES)/Common/NameKeyGenerator.cpp

#$(GEO)/RandomValue.o: $(GES)/Common/RandomValue.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RandomValue.o $(GES)/Common/RandomValue.cpp

$(GEO)/UserPreferences.o: $(GES)/Common/UserPreferences.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UserPreferences.o $(GES)/Common/UserPreferences.cpp

$(GEO)/Version.o: $(GES)/Common/Version.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Version.o $(GES)/Common/Version.cpp

# --- GameEngine/Source/Common/Audio ---
$(GEO)/AudioEventRTS.o: $(GES)/Common/Audio/AudioEventRTS.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AudioEventRTS.o $(GES)/Common/Audio/AudioEventRTS.cpp

$(GEO)/GameAudio.o: $(GES)/Common/Audio/GameAudio.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameAudio.o $(GES)/Common/Audio/GameAudio.cpp

# --- GameEngine/Source/Common/INI ---
$(GEO)/INI.o: $(GES)/Common/INI/INI.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INI.o $(GES)/Common/INI/INI.cpp

$(GEO)/INIGameData.o: $(GES)/Common/INI/INIGameData.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIGameData.o $(GES)/Common/INI/INIGameData.cpp

$(GEO)/INIVideo.o: $(GES)/Common/INI/INIVideo.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIVideo.o $(GES)/Common/INI/INIVideo.cpp

$(GEO)/INIWater.o: $(GES)/Common/INI/INIWater.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIWater.o $(GES)/Common/INI/INIWater.cpp

# --- GameEngine/Source/Common/RTS ---
#$(GEO)/PlayerList.o: $(GES)/Common/RTS/PlayerList.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/PlayerList.o $(GES)/Common/RTS/PlayerList.cpp

# --- GameEngine/Source/Common/System ---
$(GEO)/ArchiveFile.o: $(GES)/Common/System/ArchiveFile.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ArchiveFile.o $(GES)/Common/System/ArchiveFile.cpp

$(GEO)/ArchiveFileSystem.o: $(GES)/Common/System/ArchiveFileSystem.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/ArchiveFileSystem.o $(GES)/Common/System/ArchiveFileSystem.cpp

$(GEO)/AsciiString.o: $(GES)/Common/System/AsciiString.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AsciiString.o $(GES)/Common/System/AsciiString.cpp

$(GEO)/CriticalSection.o: $(GES)/Common/System/CriticalSection.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CriticalSection.o $(GES)/Common/System/CriticalSection.cpp

$(GEO)/Debug.o: $(GES)/Common/System/Debug.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Debug.o $(GES)/Common/System/Debug.cpp

$(GEO)/File.o: $(GES)/Common/System/File.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/File.o $(GES)/Common/System/File.cpp

$(GEO)/FileSystem.o: $(GES)/Common/System/FileSystem.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/FileSystem.o $(GES)/Common/System/FileSystem.cpp

$(GEO)/FunctionLexicon.o: $(GES)/Common/System/FunctionLexicon.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/FunctionLexicon.o $(GES)/Common/System/FunctionLexicon.cpp

$(GEO)/GameMemory.o: $(GES)/Common/System/GameMemory.cpp GameEngine/Include/Common/GameMemory.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameMemory.o $(GES)/Common/System/GameMemory.cpp

$(GEO)/GameType.o: $(GES)/Common/System/GameType.cpp GameEngine/Include/Common/GameType.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameType.o $(GES)/Common/System/GameType.cpp

$(GEO)/LocalFileSystem.o: $(GES)/Common/System/LocalFileSystem.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/LocalFileSystem.o $(GES)/Common/System/LocalFileSystem.cpp

$(GEO)/MemoryInit.o: $(GES)/Common/System/MemoryInit.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MemoryInit.o $(GES)/Common/System/MemoryInit.cpp

$(GEO)/RAMFile.o: $(GES)/Common/System/RAMFile.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RAMFile.o $(GES)/Common/System/RAMFile.cpp

$(GEO)/Snapshot.o: $(GES)/Common/System/Snapshot.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Snapshot.o $(GES)/Common/System/Snapshot.cpp

$(GEO)/StreamingArchiveFile.o: $(GES)/Common/System/StreamingArchiveFile.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/StreamingArchiveFile.o $(GES)/Common/System/StreamingArchiveFile.cpp

$(GEO)/SubsystemInterface.o: $(GES)/Common/System/SubsystemInterface.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SubsystemInterface.o $(GES)/Common/System/SubsystemInterface.cpp

$(GEO)/UnicodeString.o: $(GES)/Common/System/UnicodeString.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UnicodeString.o $(GES)/Common/System/UnicodeString.cpp

#$(GEO)/Xfer.o: $(GES)/Common/System/Xfer.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Xfer.o $(GES)/Common/System/Xfer.cpp

#$(GEO)/XferCRC.o: $(GES)/Common/System/XferCRC.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/XferCRC.o $(GES)/Common/System/XferCRC.cpp

# --- GameEngine/Source/GameClient ---
$(GEO)/Display.o: $(GES)/GameClient/Display.cpp GameEngine/Include/GameClient/Display.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Display.o $(GES)/GameClient/Display.cpp

$(GEO)/DisplayStringManager.o: $(GES)/GameClient/DisplayStringManager.cpp GameEngine/Include/GameClient/DisplayStringManager.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/DisplayStringManager.o $(GES)/GameClient/DisplayStringManager.cpp

$(GEO)/GameClient.o: $(GES)/GameClient/GameClient.cpp GameEngine/Include/GameClient/GameClient.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameClient.o $(GES)/GameClient/GameClient.cpp

$(GEO)/GameClientDispatch.o: $(GES)/GameClient/GameClientDispatch.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameClientDispatch.o $(GES)/GameClient/GameClientDispatch.cpp

$(GEO)/GameText.o: $(GES)/GameClient/GameText.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameText.o $(GES)/GameClient/GameText.cpp

$(GEO)/GlobalLanguage.o: $(GES)/GameClient/GlobalLanguage.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GlobalLanguage.o $(GES)/GameClient/GlobalLanguage.cpp

$(GEO)/LanguageFilter.o: $(GES)/GameClient/LanguageFilter.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/LanguageFilter.o $(GES)/GameClient/LanguageFilter.cpp

$(GEO)/Snow.o: $(GES)/GameClient/Snow.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Snow.o $(GES)/GameClient/Snow.cpp

$(GEO)/VideoPlayer.o: $(GES)/GameClient/VideoPlayer.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/VideoPlayer.o $(GES)/GameClient/VideoPlayer.cpp

$(GEO)/Water.o: $(GES)/GameClient/Water.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Water.o $(GES)/GameClient/Water.cpp

# --- GameEngine/Source/GameClient/GUI ---
$(GEO)/GameWindow.o: $(GES)/GameClient/GUI/GameWindow.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameWindow.o $(GES)/GameClient/GUI/GameWindow.cpp

$(GEO)/GameWindowManager.o: $(GES)/GameClient/GUI/GameWindowManager.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameWindowManager.o $(GES)/GameClient/GUI/GameWindowManager.cpp

$(GEO)/GameWindowManagerScript.o: $(GES)/GameClient/GUI/GameWindowManagerScript.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameWindowManagerScript.o $(GES)/GameClient/GUI/GameWindowManagerScript.cpp

$(GEO)/IMEManager.o: $(GES)/GameClient/GUI/IMEManager.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/IMEManager.o $(GES)/GameClient/GUI/IMEManager.cpp

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
$(GEO)/OptionsMenu.o: $(GES)/GameClient/GUI/GUICallbacks/Menus/OptionsMenu.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/OptionsMenu.o $(GES)/GameClient/GUI/GUICallbacks/Menus/OptionsMenu.cpp

# --- GameEngine/Source/GameClient/GUI/Shell ---
$(GEO)/Shell.o: $(GES)/GameClient/GUI/Shell/Shell.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Shell.o $(GES)/GameClient/GUI/Shell/Shell.cpp

# --- GameEngine/Source/GameClient/Input ---
$(GEO)/Keyboard.o: $(GES)/GameClient/Input/Keyboard.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Keyboard.o $(GES)/GameClient/Input/Keyboard.cpp

$(GEO)/Mouse.o: $(GES)/GameClient/Input/Mouse.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Mouse.o $(GES)/GameClient/Input/Mouse.cpp


run: $(PROG)
	cd ../Run && ./rts && cd $(CUR_DIR)

debug: $(PROG)
	cd ../Run && gdb -q ./rts && cd $(CUR_DIR)

clean:
	rm -v $(GEDO)/*.o $(GEO)/*.o $(ENGINE_DEVICE_LIB) $(ENGINE_LIB) $(PROG)

# make &> build.log
