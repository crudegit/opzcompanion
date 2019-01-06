
all: reassemble

reassemble: reassemble.c
	gcc -I../library/BLEMidiHelper/ -o reassemble reassemble.c ../library/BLEMidiHelper/reassemble.c

clean:
	rm -f reassemble *.so *.so

.PHONY: clean all

