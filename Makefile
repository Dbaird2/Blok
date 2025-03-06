
all: blok lab6 #dlvl1

lab6: waterlab6.cpp
	g++ waterlab6.cpp -Wall -lX11 -lGL libggfonts.a -o lab6

blok: blok.cpp log.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp
	g++ blok.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp log.cpp libggfonts.a -Wall -lX11 -lGL -pthread -oblok

#dlvl1: dasonlvl1.cpp log.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp
#	g++ dasonlvl1.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp log.cpp libggfonts.a -Wall -lX11 -lGL -pthread -o dlvl1

clean:
	rm -f blok
	rm -f *.o

