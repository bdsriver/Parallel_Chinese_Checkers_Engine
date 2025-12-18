run:
	g++ -std=c++23 -fopenmp -g src/*.cpp
	./a.out

build:
	g++ -std=c++23 -fopenmp -g src/*.cpp 
	python3 data/graph_plotting.py
	python3 data/old_graph_plotting.py