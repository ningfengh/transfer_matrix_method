CC=g++

CFLAGS=-c -std=c++11 -O3 -Wall

all: tmm

tmm: tmm_main.o ellipsometry.o simulation.o materail.o layer.o
	$(CC) tmm_main.o ellipsometry.o simulation.o material.o layer.o -o tmm

tmm_main.o: tmm_main.cpp
	$(CC) $(CFLAGS) tmm_main.cpp

ellipsometry.o: ellipsometry.cpp
	$(CC) $(CFLAGS) ellipsometry.cpp

simulation.o: simulation.cpp
	$(CC) $(CFLAGS) simulation.cpp

materail.o: material.cpp
	$(CC) $(CFLAGS) material.cpp

layer.o: layer.cpp
	$(CC) $(CFLAGS) layer.cpp

clean:
	rm *o tmm
