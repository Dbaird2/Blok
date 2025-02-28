CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk2

walk2: walk2.cpp log.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp
	g++ $(CFLAGS) walk2.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk2

clean:
	rm -f walk2
	rm -f *.o

