
STDARGS:=-Wall -D DOPRINTF
#STDARGS:=-Wall
STDINCS:=-I../library/BLEMidiHelper/src/ -I../library/OPZSysExUtil/src/
CFLAGS:=$(STDARGS) $(STDINCS)
CC:=g++

all: reassemble opzsxt

build:
	mkdir $@
	touch $@

build/%.o: %.c
	$(CC) -c $(STDARGS) $(STDINCS) -o $@ $<

build/xlib.a: build/util.o build/conamidi.o
	ar rcs $@ $^

reassemble: reassemble.cpp build/BLEMidiHelper.o build/xlib.a
	$(CC) $(CFLAGS) -o $@ $^

build/BLEMidiHelper.o: ../library/BLEMidiHelper/src/BLEMidiHelper.cpp 
	$(CC) $(CFLAGS) -c -o $@ $^

opzsxt: opzsxt.cpp build/BLEMidiHelper.o build/OPZSX.o build/xlib.a
	$(CC) $(CFLAGS) -o $@ $^

build/OPZSX.o: ../library/OPZSysExUtil/src/OPZSX.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f reassemble opzsxt
	rm -f build/*

.PHONY: clean all

