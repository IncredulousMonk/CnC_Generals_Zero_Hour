CXXFLAGS = -std=c++23 -O2 -Wall -Weffc++ -Wextra -Wsign-conversion -Werror -pedantic-errors -D_UNIX
INCS = -I. -I../WWLib -I../../../GameEngine/Include
OGL_LIB = ../../Lib/OGL.a


ifdef DEBUG
CXXFLAGS += -D_DEBUG -O0 -g3
endif

.PHONY: clean


OGL_OBJS = OpenGLRenderer.o

$(OGL_LIB): $(OGL_OBJS)
	ar rcs $(OGL_LIB)  $(OGL_OBJS)

OpenGLRenderer.o: OpenGLRenderer.cpp OpenGLRenderer.h
	g++ $(CXXFLAGS) $(INCS) -c -o OpenGLRenderer.o OpenGLRenderer.cpp


clean:
	rm -v *.o $(OGL_LIB)
