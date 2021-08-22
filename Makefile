CPP_SOURCES = $(wildcard src/*.cpp ./*.cpp)
HEADERS = $(wildcard lib/includes/*.h)

OBJ = ${CPP_SOURCES: .c=.o}

CPPFLAGS = -l ncurses -l pthread

INCLUDES = -I lib

all : main.out

run : main.out
	./main.out

main.out : ${OBJ}
	g++ $^ -o $@ ${INCLUDES} ${CPPFLAGS}

%.o : %.cpp
	g++ $< -o $@ ${INCLUDES} ${CPPFLAGS}

clean :
	rm -rf *.o *.out
	rm -rf src/*.o
