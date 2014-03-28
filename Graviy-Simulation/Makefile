COMPILER=clang++

all: Universe Body main
	$(COMPILER) -std=c++11 Universe.o Body.o main.o -lsfml-graphics -lsfml-window -lsfml-system -o gravity

exec: all
	./gravity

clean:
	rm *.o gravity

Universe: Universe.cpp
	$(COMPILER) -c -std=c++11 Universe.cpp -o Universe.o

Body: Body.cpp
	$(COMPILER) -c -std=c++11 Body.cpp -o Body.o

main: main.cpp
	$(COMPILER) -c -std=c++11 main.cpp -o main.o
