CXXFLAGS = -std=c++2a -O2 -Wall -Weffc++ -Wextra -Wsign-conversion -Werror -pedantic-errors
LDFLAGS = -LGameEngineDevice/Lib -LGameEngine/Lib
CXXFLAGS += $(shell pkg-config sdl3 --cflags)
LDFLAGS += $(shell pkg-config sdl3 --libs)
INCS = -ILibraries/Include -ILibraries/Source/WWVegas -ILibraries/Source/WWVegas/WWLib -IGameEngine/Include -IGameEngineDevice/Include -IGameEngine/Include/Precompiled
PROG = ../Run/rts
ENGINE_DEVICE_LIB = GameEngineDevice/Lib/GameEngineDevice.a
ENGINE_LIB = GameEngine/Lib/GameEngine.a
GEDO = GameEngineDevice/Obj
GEDSLDC = GameEngineDevice/Source/LinuxDevice/Common
GEO = GameEngine/Obj
GES = GameEngine/Source

.PHONY: clean

$(PROG): Main/main.cpp $(ENGINE_DEVICE_LIB) $(ENGINE_LIB)
	g++ $(CXXFLAGS) $(INCS) -o $(PROG) Main/main.cpp $(LDFLAGS) -l:GameEngineDevice.a -l:GameEngine.a


# GameEngineDevice

$(ENGINE_DEVICE_LIB): $(GEDO)/LinuxGameEngine.o
	ar rcs $(ENGINE_DEVICE_LIB) $(GEDO)/LinuxGameEngine.o

$(GEDO)/LinuxGameEngine.o: $(GEDSLDC)/LinuxGameEngine.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEDO)/LinuxGameEngine.o $(GEDSLDC)/LinuxGameEngine.cpp


# GameEngine

$(ENGINE_LIB): $(GEO)/GameEngine.o $(GEO)/GameMain.o
	ar rcs $(ENGINE_LIB) $(GEO)/GameEngine.o $(GEO)/GameMain.o

$(GEO)/GameEngine.o: $(GES)/Common/GameEngine.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameEngine.o $(GES)/Common/GameEngine.cpp

$(GEO)/GameMain.o: $(GES)/Common/GameMain.cpp
	g++ $(CXXFLAGS) $(INCS) -c -o $(GEO)/GameMain.o $(GES)/Common/GameMain.cpp


clean:
	rm -v $(GEDO)/*.o $(GEO)/*.o $(ENGINE_DEVICE_LIB) $(ENGINE_LIB) $(PROG)

# make &> build.log
