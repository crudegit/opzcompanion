
STDARGS:=-Wall -D DOPRINTF
#STDARGS:=-Wall
STDINCS:=-I../library/BLEMidiHelper/src/ -I../library/OPZSysExUtil/src/
CFLAGS:=$(STDARGS) $(STDINCS)
XLIBS:=util conamidi
CC:=g++


all: reassemble opzsxt

build/%.o: %.c
	$(CC) -c $(STDARGS) $(STDINCS) -o $@ $<

build:
	mkdir build
	touch build

build/xlib.a: build/util.o build/conamidi.o
	ar rcs build/xlib.a $^

reassemble: reassemble.cpp build/BLEMidiHelper.o build/xlib.a
	$(CC) $(CFLAGS) -o reassemble reassemble.cpp build/BLEMidiHelper.o build/xlib.a

build/BLEMidiHelper.o: ../library/BLEMidiHelper/src/BLEMidiHelper.cpp ../library/BLEMidiHelper/src/BLEMidiHelper.h
	$(CC) $(CFLAGS) -c -o build/BLEMidiHelper.o ../library/BLEMidiHelper/src/BLEMidiHelper.cpp 

opzsxt: opzsxt.cpp build/BLEMidiHelper.o build/OPZSX.o build/xlib.a
	$(CC) $(CFLAGS) -o opzsxt opzsxt.cpp build/OPZSX.o build/BLEMidiHelper.o build/xlib.a

build/OPZSX.o: ../library/OPZSysExUtil/src/OPZSX.cpp ../library/OPZSysExUtil/src/OPZSX.h
	$(CC) $(CFLAGS) -c -o build/OPZSX.o ../library/OPZSysExUtil/src/OPZSX.cpp

clean:
	rm -f reassemble opzsxt *.so *.o *.a
	rm -f build/*

.PHONY: clean all

