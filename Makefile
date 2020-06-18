PROJECT_NAME = Main

# Define the names of key files
SOURCE_FILE = $(PROJECT_NAME).cpp
OBJECT_FILE = $(PROJECT_NAME).o
EXECUTABLE = $(PROJECT_NAME)
WINDOW_SOURCE = libs/sdw/DrawingWindow.cpp
WINDOW_OBJECT = libs/sdw/DrawingWindow.o

CANVASPOINT_SOURCE = libs/sdw/CanvasPoint.cpp
CANVASPOINT_OBJECT = libs/sdw/CanvasPoint.o

CANVASTRIANGLE_SOURCE = libs/sdw/CanvasTriangle.cpp
CANVASTRIANGLE_OBJECT = libs/sdw/CanvasTriangle.o

TEXTUREPOINT_SOURCE = libs/sdw/TexturePoint.cpp
TEXTUREPOINT_OBJECT = libs/sdw/TexturePoint.o

COLOUR_SOURCE = libs/sdw/Colour.cpp
COLOUR_OBJECT = libs/sdw/Colour.o

HELPER_SOURCE = libs/sdw/Helper.cpp
HELPER_OBJECT = libs/sdw/Helper.o

# Build settings
COMPILER = g++
COMPILER_OPTIONS = -c -pipe -Wall -std=c++11
DEBUG_OPTIONS = -ggdb -g3
FUSSY_OPTIONS = -Werror -pedantic
SANITIZER_OPTIONS = -O1 -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer
SPEEDY_OPTIONS = -Ofast -funsafe-math-optimizations -march=native
LINKER_OPTIONS =

# Set up flags
SDW_COMPILER_FLAGS := -I./libs/sdw
GLM_COMPILER_FLAGS := -I./libs/glm
# If you have a manual install of SDL, you might not have sdl2-config. Compiler flags should be something like: -I/usr/local/include/SDL2 -D_THREAD_SAFE
SDL_COMPILER_FLAGS := $(shell sdl2-config --cflags)
# If you have a manual install of SDL, you might not have sdl2-config. Linker flags should be something like: -L/usr/local/lib -lSDL2
SDL_LINKER_FLAGS := $(shell sdl2-config --libs)
SDW_LINKER_FLAGS := $(WINDOW_OBJECT) $(HELPER_OBJECT) $(CANVASPOINT_OBJECT) ${TEXTUREPOINT_OBJECT} ${COLOUR_OBJECT} ${CANVASTRIANGLE_OBJECT}


default: diagnostic

# Rule to help find errors (when you get a segmentation fault)
# NOTE: Needs the "Address Sanitizer" library to be installed in order to work (might not work on lab machines !)
diagnostic: window helper canvaspoint texturepoint colour canvastriangle
	$(COMPILER) $(COMPILER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS) 
	./$(EXECUTABLE)

# Rule to compile and link for production release
production: window helper canvaspoint texturepoint colour canvastriangle
	$(COMPILER) $(COMPILER_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS) 
	./$(EXECUTABLE)

# Rule to compile and link for use with a debugger
debug: window helper canvaspoint texturepoint colour canvastriangle
	$(COMPILER) $(COMPILER_OPTIONS) $(DEBUG_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(DEBUG_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS) 
	./$(EXECUTABLE)

# Rule to build for high performance executable
speedy: window helper canvaspoint texturepoint colour canvastriangle
	$(COMPILER) $(COMPILER_OPTIONS) $(SPEEDY_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(SPEEDY_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule for building the DisplayWindow
window:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(WINDOW_OBJECT) $(WINDOW_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

helper:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(HELPER_OBJECT) $(HELPER_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

canvaspoint:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(CANVASPOINT_OBJECT) $(CANVASPOINT_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

canvastriangle:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(CANVASTRIANGLE_OBJECT) $(CANVASTRIANGLE_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

texturepoint:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(TEXTUREPOINT_OBJECT) $(TEXTUREPOINT_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

colour:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(COLOUR_OBJECT) $(COLOUR_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

# Files to remove during clean
clean:
	rm $(OBJECT_FILE)
	rm $(EXECUTABLE)
