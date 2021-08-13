CPP_SOURCES = $(wildcard src/*.cpp ./*.cpp)
HEADERS = $(wildcard lib/includes/*.h)

OBJ = ${CPP_SOURCES: .c=.o}

CPPFLAGS = -l ncurses

INCLUDES = -I lib

all : main.out

run : main.out
	./main.out

main.out : ${OBJ}
	g++ ${CPPFLAGS} ${INCLUDES} $^ -o $@

%.o : %.cpp
	g++ ${CPPFLAGS} ${INCLUDES} $< -o $@

clean :
	rm -rf *.o *.out
	rm -rf src/*.o
