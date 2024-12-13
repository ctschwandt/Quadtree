exe:	main.cpp
	g++ main.cpp src/*.cpp src/*.c classes/*.cpp `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -Iclasses -o quadtree

asan:	main.cpp
	g++ -g -fsanitize=address main.cpp src/*.cpp src/*.c classes/*.cpp `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -Iclasses -o quadtree

leak:	main.cpp
	g++ -g -fsanitize=leak main.cpp src/*.cpp src/*.c classes/*.cpp `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -Iclasses -o quadtree

run:
	./quadtree

r:
	ASAN_OPTIONS=detect_leaks=1 ./quadtree

clean:
	rm quadtree

c:
	rm quadtree
