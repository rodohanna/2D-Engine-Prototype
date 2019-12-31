#mingw32-make

#OBJS specifies which files to compile as part of the project
OBJS = src/Assets.cpp src/Entity.cpp src/Input.cpp src/main.cpp \
		src/ProcGen.cpp src/Render.cpp src/SDLWrapper.cpp src/Window.cpp \
		src/Physics.cpp src/Zone.cpp src/Order.cpp src/MessageBus.cpp src/UI.cpp \
		src/BottomBar.cpp src/GUI.cpp src/BuildMenu.cpp src/Build.cpp

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