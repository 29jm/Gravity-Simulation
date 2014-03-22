all: Universe Body main
	clang++ -std=c++11 Universe.o Body.o main.o -lsfml-graphics -lsfml-window -lsfml-system -o gravity

Universe: Universe.cpp
	clang++ -c -std=c++11 Universe.cpp -o Universe.o

Body: Body.cpp
	clang++ -c -std=c++11 Body.cpp -o Body.o

main: main.cpp
	clang++ -c -std=c++11 main.cpp -o main.o
