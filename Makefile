kShogi: main.o board.o
	g++ -Wall -O2 -o kShogi main.o board.o

main.o: main.cpp
	g++ -std=c++11 -Wall -O2 -c main.cpp
	
main.o: board.h

board.o: board.cpp
	g++ -std=c++11 -Wall -O2 -c board.cpp
	
board.o: board.h

.PHONY: clean

clean:
	-rm *.o


