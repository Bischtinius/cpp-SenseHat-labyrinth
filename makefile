SOURCES = main.cpp Cell.cpp playground.cpp Player.cpp

all: project

project: main.cpp
	g++ -g -Wall -o labyrinth $(SOURCES) -fpermissive

clean:
	rm -f *.o labyrinth

	
clear: clear.cpp
	g++ clear.cpp -o clear_leds -fpermissive -li2c