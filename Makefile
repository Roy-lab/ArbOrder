CC = g++
CFLAGS = -std=c++17 -O0

SRC = Framework.cpp ArbAssignments.cpp Graph.cpp Matrix.cpp Node.cpp

arbAssigner: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o arbAssigner

clean:
	rm -f arbAssigner
