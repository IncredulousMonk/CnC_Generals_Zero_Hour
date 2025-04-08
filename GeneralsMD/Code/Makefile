CXXFLAGS = -std=c++2a -O2 -Wall -Weffc++ -Wextra -Wsign-conversion -Werror -pedantic-errors
LDFLAGS = -LGameEngineDevice/Lib -LGameEngine/Lib
CXXFLAGS += $(shell pkg-config sdl3 --cflags)
LDFLAGS += $(shell pkg-config sdl3 --libs)
INCS = -I. -ILibraries/Include -ILibraries/Source/WWVegas -ILibraries/Source/WWVegas/WWLib -IGameEngine/Include -IGameEngineDevice/Include -IGameEngine/Include/Precompiled
PROG = ../Run/rts
ENGINE_DEVICE_LIB = GameEngineDevice/Lib/GameEngineDevice.a
ENGINE_LIB = GameEngine/Lib/GameEngine.a
GEDO = GameEngineDevice/Obj
GEDSLDC = GameEngineDevice/Source/LinuxDevice/Common
GEO = GameEngine/Obj
GES = GameEngine/Source
CUR_DIR := $(shell pwd)


ifdef DEBUG
CXXFLAGS += -D_DEBUG -O0
endif

.PHONY: run clean

$(PROG): Main/main.cpp $(ENGINE_DEVICE_LIB) $(ENGINE_LIB)
	g++ $(CXXFLAGS) $(INCS) -o $(PROG) Main/main.cpp $(LDFLAGS) -l:GameEngineDevice.a -l:GameEngine.a


# ===== GameEngineDevice =====

ENGINE_DEVICE_OBJS = $(GEDO)/LinuxBIGFile.o $(GEDO)/LinuxBIGFileSystem.o $(GEDO)/LinuxGameEngine.o $(GEDO)/LinuxLocalFile.o $(GEDO)/LinuxLocalFileSystem.o

$(ENGINE_DEVICE_LIB): $(ENGINE_DEVICE_OBJS)
	ar rcs $(ENGINE_DEVICE_LIB)  $(ENGINE_DEVICE_OBJS)

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


# ===== GameEngine =====

ENGINE_OBJS = $(GEO)/GameEngine.o $(GEO)/GameMain.o $(GEO)/GlobalData.o $(GEO)/NameKeyGenerator.o $(GEO)/Version.o $(GEO)/ArchiveFile.o $(GEO)/ArchiveFileSystem.o $(GEO)/INI.o $(GEO)/INIGameData.o $(GEO)/INIWater.o \
$(GEO)/AsciiString.o $(GEO)/CriticalSection.o $(GEO)/Debug.o $(GEO)/File.o $(GEO)/FileSystem.o $(GEO)/GameMemory.o $(GEO)/GameType.o $(GEO)/LocalFileSystem.o $(GEO)/MemoryInit.o \
$(GEO)/RAMFile.o $(GEO)/StreamingArchiveFile.o \
$(GEO)/SubsystemInterface.o $(GEO)/UnicodeString.o $(GEO)/Snow.o $(GEO)/Water.o
#ENGINE_OBJS = $(GEO)/GameEngine.o $(GEO)/GameLOD.o $(GEO)/GameMain.o $(GEO)/GlobalData.o $(GEO)/NameKeyGenerator.o $(GEO)/RandomValue.o $(GEO)/Version.o $(GEO)/AudioEventRTS.o $(GEO)/PlayerList.o $(GEO)/ArchiveFile.o $(GEO)/ArchiveFileSystem.o \
$(GEO)/INI.o $(GEO)/INIGameData.o $(GEO)/INIWater.o $(GEO)/AsciiString.o $(GEO)/CriticalSection.o \
$(GEO)/Debug.o $(GEO)/File.o $(GEO)/FileSystem.o $(GEO)/GameMemory.o $(GEO)/GameType.o $(GEO)/LocalFileSystem.o $(GEO)/MemoryInit.o $(GEO)/MessageStream.o $(GEO)/RAMFile.o $(GEO)/StreamingArchiveFile.o \
$(GEO)/SubsystemInterface.o $(GEO)/UnicodeString.o $(GEO)/Xfer.o $(GEO)/XferCRC.o $(GEO)/GameText.o $(GEO)/Snow.o $(GEO)/Water.o

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

#$(GEO)/MessageStream.o: $(GES)/Common/MessageStream.cpp GameEngine/Include/Common/MessageStream.h
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MessageStream.o $(GES)/Common/MessageStream.cpp

$(GEO)/NameKeyGenerator.o: $(GES)/Common/NameKeyGenerator.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/NameKeyGenerator.o $(GES)/Common/NameKeyGenerator.cpp

#$(GEO)/RandomValue.o: $(GES)/Common/RandomValue.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RandomValue.o $(GES)/Common/RandomValue.cpp

$(GEO)/Version.o: $(GES)/Common/Version.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Version.o $(GES)/Common/Version.cpp

$(GEO)/INI.o: $(GES)/Common/INI/INI.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INI.o $(GES)/Common/INI/INI.cpp

# --- GameEngine/Source/Common/Audio ---
#$(GEO)/AudioEventRTS.o: $(GES)/Common/Audio/AudioEventRTS.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AudioEventRTS.o $(GES)/Common/Audio/AudioEventRTS.cpp

# --- GameEngine/Source/Common/INI ---
$(GEO)/INIGameData.o: $(GES)/Common/INI/INIGameData.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INIGameData.o $(GES)/Common/INI/INIGameData.cpp

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
#$(GEO)/GameText.o: $(GES)/GameClient/GameText.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameText.o $(GES)/GameClient/GameText.cpp

$(GEO)/Snow.o: $(GES)/GameClient/Snow.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Snow.o $(GES)/GameClient/Snow.cpp

$(GEO)/Water.o: $(GES)/GameClient/Water.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Water.o $(GES)/GameClient/Water.cpp


run: $(PROG)
	cd ../Run && ./rts && cd $(CUR_DIR)

clean:
	rm -v $(GEDO)/*.o $(GEO)/*.o $(ENGINE_DEVICE_LIB) $(ENGINE_LIB) $(PROG)

# make &> build.log
