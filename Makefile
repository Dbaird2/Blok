all: blok maphelp #dlvl1


blok: blok.cpp log.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp
	g++ blok.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp log.cpp libggfonts.a -Wall -lX11 -lGL -pthread -oblok -lalut -lopenal -D USE_OPENAL_SOUND

maphelp: blok.cpp log.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp
	g++ blok.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp log.cpp libggfonts.a -Wall -lX11 -lGL -pthread -o maphelp -D MAP_HELP
	
	
	#dlvl1: dasonlvl1.cpp log.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp
	#	g++ dasonlvl1.cpp dbaird.cpp cmorenoyanes.cpp ccontreras2.cpp stoledo.cpp rbarreyro.cpp log.cpp libggfonts.a -Wall -lX11 -lGL -pthread -o dlvl1

clean:
	rm -f blok
	rm -f maphelp
	rm -f *.o
