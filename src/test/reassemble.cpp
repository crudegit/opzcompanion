#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLEMIDI_RBUFF_SIZE 32
#include <BLEMidiHelper.h>

#include "util.h"

void process_line(uint8_t *line, size_t len);

int main(int argc, char **argv){
    
    printf("Midi message buffer size is %li\n", sizeof(BMH_::rbuffer));
    
    if(argc < 2){
        printf("Reading from stdin\n");
        read_file_by_line(stdin, &process_line);
    }
    else {
        printf("Opening %s\n", argv[1]);
        read_file_by_line(argv[1], &process_line);
        
    }

    exit(1);
}

void process_midi(uint16_t o, uint16_t l);

void process_line(uint8_t *line, size_t len){
    BLEMidiHelper.process_blemidi(line, len);

    while(BLEMidiHelper.process_next_message(&process_midi)){
    }
}

void process_midi(uint16_t o, uint16_t l){
    printf("MIDI: l=%i %02hhx", l, BLEMidiHelper.gwc(o,0));

    uint8_t lbuff[1024];

    BLEMidiHelper.copy_to_buffer(o, l, lbuff);

    for(uint16_t i=1;i<l;i++){
        printf(" %02hhx/%02hhx", BLEMidiHelper.gwc(o,i), lbuff[i]);
    }
    printf("\n");
    switch(BLEMidiHelper.gwc(o,0) & 0xf0){
        case 0x80:{
            printf("note off %02hhx", BLEMidiHelper.gwc(o,1));
        }break;
        case 0x90:{
            printf("note on %02hhx", BLEMidiHelper.gwc(o,1));
        }break;
        default:{
        }break;
    }
}

