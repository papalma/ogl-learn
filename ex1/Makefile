# This builds ogl example on Mac 10.14.5
# requires libglfw, libglad

CXX=clang++
CC=clang

GLAD_DIR = "../glad"
GLFW_DIR = "../glfw"

INCLUDE = -I $(GLAD_DIR)/include -I $(GLFW_DIR)/include
CXXFLAGS = $(INCLUDE) -std=c++17
CFLAGS = $(INCLUDE) -Wall

OSX_LIBS = -framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo
LIBS = -lstdc++ $(OSX_LIBS) $(GLFW_DIR)/lib/libglfw3.a $(GLAD_DIR)/src/glad.o

OBJECTS = source.o

result: $(OBJECTS)
	clang -o $@ $^ $(LIBS)

source.o: source.cpp

.PHONE:
clean:
	rm -f *.o result
