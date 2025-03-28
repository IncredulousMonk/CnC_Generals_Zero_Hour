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


# GameEngineDevice

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


# GameEngine

ENGINE_OBJS = $(GEO)/GameEngine.o $(GEO)/GameMain.o $(GEO)/NameKeyGenerator.o $(GEO)/Version.o $(GEO)/ArchiveFile.o $(GEO)/ArchiveFileSystem.o $(GEO)/AsciiString.o $(GEO)/CriticalSection.o \
$(GEO)/Debug.o $(GEO)/File.o $(GEO)/FileSystem.o $(GEO)/GameMemory.o $(GEO)/LocalFileSystem.o $(GEO)/MemoryInit.o $(GEO)/RAMFile.o $(GEO)/StreamingArchiveFile.o $(GEO)/SubsystemInterface.o $(GEO)/UnicodeString.o
#ENGINE_OBJS = $(GEO)/GameEngine.o $(GEO)/GameMain.o $(GEO)/NameKeyGenerator.o $(GEO)/RandomValue.o $(GEO)/Version.o $(GEO)/ArchiveFile.o $(GEO)/ArchiveFileSystem.o $(GEO)/INI.o $(GEO)/AsciiString.o $(GEO)/CriticalSection.o \
$(GEO)/Debug.o $(GEO)/File.o $(GEO)/FileSystem.o $(GEO)/GameMemory.o $(GEO)/LocalFileSystem.o $(GEO)/MemoryInit.o $(GEO)/RAMFile.o $(GEO)/StreamingArchiveFile.o $(GEO)/SubsystemInterface.o $(GEO)/UnicodeString.o $(GEO)/GameText.o

$(ENGINE_LIB): $(ENGINE_OBJS)
	ar rcs $(ENGINE_LIB) $(ENGINE_OBJS)

$(GEO)/GameEngine.o: $(GES)/Common/GameEngine.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameEngine.o $(GES)/Common/GameEngine.cpp

$(GEO)/GameMain.o: $(GES)/Common/GameMain.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameMain.o $(GES)/Common/GameMain.cpp

$(GEO)/NameKeyGenerator.o: $(GES)/Common/NameKeyGenerator.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/NameKeyGenerator.o $(GES)/Common/NameKeyGenerator.cpp

#$(GEO)/RandomValue.o: $(GES)/Common/RandomValue.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RandomValue.o $(GES)/Common/RandomValue.cpp

$(GEO)/Version.o: $(GES)/Common/Version.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Version.o $(GES)/Common/Version.cpp

#$(GEO)/INI.o: $(GES)/Common/INI/INI.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INI.o $(GES)/Common/INI/INI.cpp

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


#$(GEO)/GameText.o: $(GES)/GameClient/GameText.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameText.o $(GES)/GameClient/GameText.cpp


run: $(PROG)
	cd ../Run && ./rts && cd $(CUR_DIR)

clean:
	rm -v $(GEDO)/*.o $(GEO)/*.o $(ENGINE_DEVICE_LIB) $(ENGINE_LIB) $(PROG)

# make &> build.log
