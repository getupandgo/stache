CC := g++
CFLAGS := `pkg-config --static --libs opencv` -O3

BOARD_TAG      = userspace

USERSPACE_MAKE = TRUE

all: stache tracker

stache: stache.cpp Makefile
	$(CC) $(CFLAGS) -o ./build/stache stache.cpp

tracker: tracker.cpp Makefile
	$(CC) $(CFLAGS) -o ./build/tracker tracker.cpp

clean:
	rm -f ./build/stache ./build/tracker

install: stache
	cp stache.desktop $(HOME)/Desktop/
