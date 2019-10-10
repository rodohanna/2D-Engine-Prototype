game:
	g++ -Wall -std=c++14 src/*.cpp -o run -I include -L lib -lSDL2-2.0.0 -lSDL2_ttf-2.0.0 -lSDL2_image-2.0.0

clean:
	rm run