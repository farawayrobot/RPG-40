a.out: main.cc map.h
	g++ main.cc -lncurses

clean:
	rm -rf a.out core *.o
