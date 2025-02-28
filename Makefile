CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: blok

blok: blok.cpp log.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp
	g++ $(CFLAGS) blok.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oblok

clean:
	rm -f blok
	rm -f *.o

