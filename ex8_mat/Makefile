# This builds ogl example on Mac 10.14.5
# requires libglfw, libglad

CXX=clang++
CC=clang

GLAD_DIR = ../glad
GLAD_LIB = $(GLAD_DIR)/src/glad.o
GLFW_DIR = ../glfw/macos
GLM_DIR = ../glm

INCLUDE = -I $(GLM_DIR) -I $(GLAD_DIR)/include -I $(GLFW_DIR)/include -I ../include
CXXFLAGS = $(INCLUDE) -std=c++17 -Wall
CFLAGS = $(INCLUDE) -Wall

OSX_LIBS = -framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo
LIBS = -lstdc++ $(OSX_LIBS) $(GLFW_DIR)/lib/libglfw3.a

OBJECTS = source.o

all: result

result: $(OBJECTS) $(GLAD_LIB)
	clang -o $@ $^ $(LIBS)

$(GLAD_LIB):
	$(MAKE) -C $(GLAD_DIR)

source.o: source.cpp

.PHONY: clean
clean:
	rm -f *.o result
