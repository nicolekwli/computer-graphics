PROJECT_NAME = ${FILE}

# Define the names of key files
SOURCE_FILE = $(PROJECT_NAME).cpp
OBJECT_FILE = $(PROJECT_NAME).o
EXECUTABLE = $(PROJECT_NAME).exe
WINDOW_SOURCE = libs/sdw/DrawingWindow.cpp
WINDOW_OBJECT = libs/sdw/DrawingWindow.o

CANVASPOINT_SOURCE = libs/sdw/CanvasPoint.cpp
CANVASPOINT_OBJECT = libs/sdw/CanvasPoint.o

CANVASTRIANGLE_SOURCE = libs/sdw/CanvasTriangle.cpp
CANVASTRIANGLE_OBJECT = libs/sdw/CanvasTriangle.o

MODELTRIANGLE_SOURCE = libs/sdw/ModelTriangle.cpp
MODELTRIANGLE_OBJECT = libs/sdw/ModelTriangle.o

TEXTUREPOINT_SOURCE = libs/sdw/TexturePoint.cpp
TEXTUREPOINT_OBJECT = libs/sdw/TexturePoint.o

COLOUR_SOURCE = libs/sdw/Colour.cpp
COLOUR_OBJECT = libs/sdw/Colour.o

UTILS_SOURCE = libs/sdw/Utils.cpp
UTILS_OBJECT = libs/sdw/Utils.o

PPM_SOURCE = libs/sdw/Ppm.cpp
PPM_OBJECT = libs/sdw/Ppm.o

HELPER_SOURCE = libs/sdw/Helper.cpp
HELPER_OBJECT = libs/sdw/Helper.o

RAYTRACER_SOURCE = libs/sdw/Raytracer.cpp
RAYTRACER_OBJECT = libs/sdw/Raytracer.o

CAMERA_SOURCE = libs/sdw/Camera.cpp
CAMERA_OBJECT = libs/sdw/Camera.o

RASTERISE_SOURCE = libs/sdw/Rasterise.cpp
RASTERISE_OBJECT = libs/sdw/Rasterise.o

MATERIAL_SOURCE = libs/sdw/Material.cpp
MATERIAL_OBJECT = libs/sdw/Material.o

# Build settings
COMPILER = g++
COMPILER_OPTIONS = -c -pipe -Wall -std=c++11
DEBUG_OPTIONS = -ggdb -g3
FUSSY_OPTIONS = -Werror -pedantic
SANITIZER_OPTIONS = -O1 -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer
SPEEDY_OPTIONS = -Ofast -funsafe-math-optimizations -march=native
LINKER_OPTIONS = -mconsole

# Set up flags
SDW_COMPILER_FLAGS := -I./libs/sdw
GLM_COMPILER_FLAGS := -I./libs/glm
# If you have a manual install of SDL, you might not have sdl2-config. Compiler flags should be something like: -I/usr/local/include/SDL2 -D_THREAD_SAFE
SDL_COMPILER_FLAGS := $(shell sdl2-config --cflags)
# If you have a manual install of SDL, you might not have sdl2-config. Linker flags should be something like: -L/usr/local/lib -lSDL2
SDL_LINKER_FLAGS := $(shell sdl2-config --libs)
<<<<<<< HEAD
SDW_LINKER_FLAGS := $(WINDOW_OBJECT) $(HELPER_OBJECT) $(CANVASPOINT_OBJECT) ${TEXTUREPOINT_OBJECT} ${COLOUR_OBJECT} ${CANVASTRIANGLE_OBJECT} ${MODELTRIANGLE_OBJECT} ${UTILS_OBJECT} ${RASTERISE_OBJECT} ${CAMERA_OBJECT} ${RAYTRACER_OBJECT} ${PPM_OBJECT} ${MATERIAL_OBJECT}
=======
SDW_LINKER_FLAGS := $(WINDOW_OBJECT) $(HELPER_OBJECT) $(CANVASPOINT_OBJECT) ${TEXTUREPOINT_OBJECT} ${COLOUR_OBJECT} ${CANVASTRIANGLE_OBJECT} ${MODELTRIANGLE_OBJECT} ${UTILS_OBJECT} ${RASTERISE_OBJECT} ${CAMERA_OBJECT} ${RAYTRACER_OBJECT} ${PPM_OBJECT} ${RENDERER_OBJECT}
>>>>>>> rendering

default: diagnostic

# Rule to help find errors (when you get a segmentation fault)
# NOTE: Needs the "Address Sanitizer" library to be installed in order to work (might not work on lab machines !)
diagnostic: window helper canvaspoint texturepoint colour canvastriangle modeltriangle utils camera rasteriser raytracer ppm material
	$(COMPILER) $(COMPILER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS) 
	./$(EXECUTABLE)

# Rule to compile and link for production release
production: window helper canvaspoint texturepoint colour canvastriangle modeltriangle utils camera rasteriser raytracer ppm material
	$(COMPILER) $(COMPILER_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS) 
	./$(EXECUTABLE)

# Rule to compile and link for use with a debugger
debug: window helper canvaspoint texturepoint colour canvastriangle modeltriangle utils camera rasteriser raytracer ppm material
	$(COMPILER) $(COMPILER_OPTIONS) $(DEBUG_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(DEBUG_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS) 
	./$(EXECUTABLE)

# Rule to build for high performance executable
speedy: window helper canvaspoint texturepoint colour canvastriangle modeltriangle utils camera rasteriser raytracer ppm material
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

modeltriangle:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(MODELTRIANGLE_OBJECT) $(MODELTRIANGLE_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

texturepoint:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(TEXTUREPOINT_OBJECT) $(TEXTUREPOINT_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

colour:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(COLOUR_OBJECT) $(COLOUR_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

utils:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(UTILS_OBJECT) $(UTILS_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

rasteriser:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(RASTERISE_OBJECT) $(RASTERISE_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS)

camera:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(CAMERA_OBJECT) $(CAMERA_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS)

ppm:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(PPM_OBJECT) $(PPM_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS)

raytracer:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(RAYTRACER_OBJECT) $(RAYTRACER_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

material:
	$(COMPILER) $(COMPILER_OPTIONS) -o $(MATERIAL_OBJECT) $(MATERIAL_SOURCE) $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

# Files to remove during clean
clean:
	rm $(OBJECT_FILE)
	rm $(EXECUTABLE)
