COMPILER=clang++
OPTIONS=-std=c++11 -Wall -Wextra

ifdef SystemRoot
	COMPILER = g++
endif

all: Universe Body main
	$(COMPILER) $(OPTIONS) Universe.o Body.o main.o -lsfml-graphics -lsfml-window -lsfml-system -o gravity

debug: OPTIONS += -g
debug: all

release: OPTIONS += -O3
release: all

exec: all
	./gravity

clean:
	rm *.o gravity

Universe: Universe.cpp
	$(COMPILER) $(OPTIONS) -c Universe.cpp -o Universe.o

Body: Body.cpp
	$(COMPILER) $(OPTIONS) -c Body.cpp -o Body.o

main: main.cpp
	$(COMPILER) $(OPTIONS) -c main.cpp -o main.o
