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


ifdef DEBUG
CXXFLAGS += -D_DEBUG -O0
endif

.PHONY: clean

$(PROG): Main/main.cpp $(ENGINE_DEVICE_LIB) $(ENGINE_LIB)
	g++ $(CXXFLAGS) $(INCS) -o $(PROG) Main/main.cpp $(LDFLAGS) -l:GameEngineDevice.a -l:GameEngine.a


# GameEngineDevice

$(ENGINE_DEVICE_LIB): $(GEDO)/LinuxGameEngine.o
	ar rcs $(ENGINE_DEVICE_LIB) $(GEDO)/LinuxGameEngine.o

$(GEDO)/LinuxGameEngine.o: $(GEDSLDC)/LinuxGameEngine.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameEngine.o $(GEDSLDC)/LinuxGameEngine.cpp


# GameEngine

$(ENGINE_LIB): $(GEO)/GameEngine.o $(GEO)/GameMain.o $(GEO)/Version.o $(GEO)/AsciiString.o $(GEO)/CriticalSection.o $(GEO)/Debug.o $(GEO)/GameMemory.o $(GEO)/MemoryInit.o $(GEO)/SubsystemInterface.o $(GEO)/UnicodeString.o
	ar rcs $(ENGINE_LIB) $(GEO)/GameEngine.o $(GEO)/GameMain.o $(GEO)/Version.o $(GEO)/AsciiString.o $(GEO)/CriticalSection.o $(GEO)/Debug.o $(GEO)/GameMemory.o $(GEO)/MemoryInit.o $(GEO)/SubsystemInterface.o $(GEO)/UnicodeString.o
#$(ENGINE_LIB): $(GEO)/GameEngine.o $(GEO)/GameMain.o $(GEO)/RandomValue.o $(GEO)/Version.o $(GEO)/INI.o $(GEO)/AsciiString.o $(GEO)/CriticalSection.o $(GEO)/Debug.o $(GEO)/FileSystem.o $(GEO)/GameMemory.o $(GEO)/MemoryInit.o $(GEO)/SubsystemInterface.o $(GEO)/UnicodeString.o $(GEO)/GameText.o
#	ar rcs $(ENGINE_LIB) $(GEO)/GameEngine.o $(GEO)/GameMain.o $(GEO)/RandomValue.o $(GEO)/Version.o $(GEO)/INI.o $(GEO)/AsciiString.o $(GEO)/CriticalSection.o $(GEO)/Debug.o $(GEO)/FileSystem.o $(GEO)/GameMemory.o $(GEO)/MemoryInit.o $(GEO)/SubsystemInterface.o $(GEO)/UnicodeString.o $(GEO)/GameText.o

$(GEO)/GameEngine.o: $(GES)/Common/GameEngine.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameEngine.o $(GES)/Common/GameEngine.cpp

$(GEO)/GameMain.o: $(GES)/Common/GameMain.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameMain.o $(GES)/Common/GameMain.cpp

#$(GEO)/RandomValue.o: $(GES)/Common/RandomValue.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/RandomValue.o $(GES)/Common/RandomValue.cpp

$(GEO)/Version.o: $(GES)/Common/Version.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Version.o $(GES)/Common/Version.cpp

#$(GEO)/INI.o: $(GES)/Common/INI/INI.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/INI.o $(GES)/Common/INI/INI.cpp

$(GEO)/AsciiString.o: $(GES)/Common/System/AsciiString.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/AsciiString.o $(GES)/Common/System/AsciiString.cpp

$(GEO)/CriticalSection.o: $(GES)/Common/System/CriticalSection.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/CriticalSection.o $(GES)/Common/System/CriticalSection.cpp

$(GEO)/Debug.o: $(GES)/Common/System/Debug.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/Debug.o $(GES)/Common/System/Debug.cpp

#$(GEO)/FileSystem.o: $(GES)/Common/System/FileSystem.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/FileSystem.o $(GES)/Common/System/FileSystem.cpp

$(GEO)/GameMemory.o: $(GES)/Common/System/GameMemory.cpp GameEngine/Include/Common/GameMemory.h
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameMemory.o $(GES)/Common/System/GameMemory.cpp

$(GEO)/MemoryInit.o: $(GES)/Common/System/MemoryInit.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/MemoryInit.o $(GES)/Common/System/MemoryInit.cpp

$(GEO)/SubsystemInterface.o: $(GES)/Common/System/SubsystemInterface.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/SubsystemInterface.o $(GES)/Common/System/SubsystemInterface.cpp

$(GEO)/UnicodeString.o: $(GES)/Common/System/UnicodeString.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/UnicodeString.o $(GES)/Common/System/UnicodeString.cpp


#$(GEO)/GameText.o: $(GES)/GameClient/GameText.cpp
#	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameText.o $(GES)/GameClient/GameText.cpp


clean:
	rm -v $(GEDO)/*.o $(GEO)/*.o $(ENGINE_DEVICE_LIB) $(ENGINE_LIB) $(PROG)

# make &> build.log
