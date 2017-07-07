CC = g++
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -fopenmp
COMP_FLAGS = -Wall -c -std=c++14 -I ./include
LINK_FLAGS = -Wall -std=c++14 -I ./include

SRC_DIR = ./src/
OBJ_DIR = ./obj/
SOURCES := $(shell find $(SRC_DIR) -name *.cpp)
VAR = $(SOURCES:${SRC_DIR}%=%)
DEBUG_OBJ = $(VAR:.cpp=_src_d.o)
REL_OBJ = $(VAR:.cpp=_src.o)

SDL_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx
DEBUG_LIB_FLAGS = -L ./lib ${SDL_FLAGS} -l2D-Engine_debug -lmathLib_debug
RELEASE_LIB_FLAGS = -L ./lib ${SDL_FLAGS} -l2D-Engine_release -lmathLib_release

vpath %.hpp ${SRC_DIR} ${LIB_SRC_DIR}
vpath %.cpp ${SRC_DIR} ${LIB_SRC_DIR}
vpath %.o ${OBJ_DIR} ${LIB_OBJ_DIR}

all: debug

debug: dsrc

release: rsrc

dsrc: ${DEBUG_OBJ}
	${CC} ${DEBUG_FLAGS} $(addprefix ${OBJ_DIR},${DEBUG_OBJ}) -o debug ${DEBUG_LIB_FLAGS}

rsrc: ${REL_OBJ}
	${CC} ${RELEASE_FLAGS} $(addprefix ${OBJ_DIR},${REL_OBJ}) -o release ${RELEASE_LIB_FLAGS}

%_src_d.o: %.cpp %.hpp
	${CC} ${DEBUG_FLAGS} ${COMP_FLAGS} $< -o ${OBJ_DIR}$@

%_src_d.o: %.cpp
	${CC} ${DEBUG_FLAGS} ${COMP_FLAGS} $< -o ${OBJ_DIR}$@

%_src.o: %.cpp %.hpp
	${CC} ${RELEASE_FLAGS} ${COMP_FLAGS} $< -o ${OBJ_DIR}$@

%_src.o: %.cpp
	${CC} ${RELEASE_FLAGS} ${COMP_FLAGS} $< -o ${OBJ_DIR}$@

clean:
	rm -r -f $(shell find ${OBJ_DIR} -name *.o) release debug
