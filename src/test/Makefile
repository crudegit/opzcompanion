
all: reassemble

reassemble: reassemble.cpp
	g++ -I../library/BLEMidiHelper/src/ -o reassemble reassemble.cpp ../library/BLEMidiHelper/src/BLEMidiHelper.cpp

clean:
	rm -f reassemble *.so *.so

.PHONY: clean all

