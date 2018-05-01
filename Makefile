FLAGS := -std=c++17 -Wall -Wextra -pedantic
SFML_STATIC_WIN := -lsfml-graphics-s -lfreetype -ljpeg -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32

all : blocks.exe
	./blocks

blocks.exe : main.o
	g++ $(FLAGS) main.o -o blocks $(SFML_STATIC_WIN)
	
main.o : main.cpp rnd.h
	g++ $(FLAGS) main.cpp -c -o main.o
