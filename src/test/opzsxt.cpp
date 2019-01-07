#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <stdlib.h>
#include <OPZSX.h>

#include "util.h"

void process_line(uint8_t *line, size_t len);
void process_midi(uint16_t o, uint16_t l);

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

void process_line(uint8_t *line, size_t len){
    process_blemidi_line(line, len, &process_midi);
}

void process_midi(uint16_t o, uint16_t l){
    uint8_t lbuff[1024];

    BLEMidiHelper.copy_to_buffer(o, l, lbuff);
    printf("dispatching to sysex\n");
    OPZSX.process_sysex(lbuff, l);
}
