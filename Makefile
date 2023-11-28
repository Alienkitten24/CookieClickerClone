OBJS = main.cpp

INCLUDE_PATHS = -IC:/mingw_dev_lib/SDL2-2.28.2/include/SDL2 -IC:/mingw_dev_lib/SDL2_image-2.6.3/include/SDL2

LIBRARY_PATHS = -LC:/mingw_dev_lib/SDL2-2.28.2/lib -LC:/mingw_dev_lib/SDL2_image-2.6.3/lib

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

all: $(OBJS)
	g++ $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o game