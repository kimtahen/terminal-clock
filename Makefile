all : main.out

run : main.out
	./main.out

clean :
	rm main.out


main.out : main.cpp
	g++ main.cpp -o main.out -l ncurses 
