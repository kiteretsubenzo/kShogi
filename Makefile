kShogi: main.o board.o ai.o worker.o
	g++ -Wall -O2 -o kShogi main.o board.o ai.o worker.o -pthread

main.o: main.cpp
	g++ -std=c++11 -Wall -O2 -c main.cpp -pthread
	
main.o: board.h AI/ai.h AI/worker.h

board.o: board.cpp
	g++ -std=c++11 -Wall -O2 -c board.cpp -pthread
	
board.o: board.h

ai.o: AI/ai.cpp
	g++ -std=c++11 -Wall -O2 -c AI/ai.cpp -pthread
	
ai.o: AI/ai.h AI/worker.h

worker.o: AI/worker.cpp
	g++ -std=c++11 -Wall -O2 -c AI/worker.cpp -pthread
	
worker.o: AI/worker.h

.PHONY: clean

clean:
	-rm *.o
	-rm kShogi


