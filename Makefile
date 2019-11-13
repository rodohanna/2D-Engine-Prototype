#mingw32-make

#OBJS specifies which files to compile as part of the project
OBJS = src/main.cpp src/EventBus.cpp src/InputSystem.cpp \
 src/Player.cpp src/RenderSystem.cpp src/SDLWrapper.cpp src/Assets.cpp \
 src/Scenery.cpp src/MapGen.cpp src/Camera.cpp src/Physics.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\mingw_dev_lib\SDL2\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\mingw_dev_lib\SDL2\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -Wall enables all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -Wall -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lwinmm

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = game

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)