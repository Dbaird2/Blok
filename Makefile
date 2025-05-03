all: blok


blok: blok.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp
	g++ -std=c++20 blok.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp libggfonts.a -Wall -lX11 -lGL -pthread -oblok -lalut -lopenal -fopenmp  -D USE_OPENAL_SOUND

clean:
	rm -f blok
	rm -f *.o
