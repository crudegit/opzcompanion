
all: reassemble opzsxt

reassemble: reassemble.cpp BLEMidiHelper.o
	g++ -Wall -I../library/BLEMidiHelper/src/ -o reassemble reassemble.cpp BLEMidiHelper.o

BLEMidiHelper.o: ../library/BLEMidiHelper/src/BLEMidiHelper.cpp ../library/BLEMidiHelper/src/BLEMidiHelper.h
	g++ -c -Wall -I../library/BLEMidiHelper/src/ -o BLEMidiHelper.o ../library/BLEMidiHelper/src/BLEMidiHelper.cpp 

opzsxt: opzsxt.cpp BLEMidiHelper.o OPZSX.o
	g++ -Wall -I../library/OPZSysExUtil/src/ -I../library/BLEMidiHelper/src/ -o opzsxt opzsxt.cpp OPZSX.o BLEMidiHelper.o

OPZSX.o: ../library/OPZSysExUtil/src/OPZSX.cpp ../library/OPZSysExUtil/src/OPZSX.h
	g++ -c -Wall -I../library/OPZSysExUtil/src/ -o OPZSX.o ../library/OPZSysExUtil/src/OPZSX.cpp

clean:
	rm -f reassemble opzsxt *.so *.o

.PHONY: clean all

