run:
	g++ -std=c++23 src/eval.cpp src/play.cpp src/main.cpp src/transpositionTable.cpp src/board.cpp
	./a.out

build:
	g++ -std=c++23 -g g++ -std=c++23 src/eval.cpp src/play.cpp src/main.cpp src/transpositionTable.cpp src/board.cpp
