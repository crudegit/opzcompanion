#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLEMIDI_RBUFF_SIZE 32
#include <BLEMidiHelper.h>

void process_line(char *line, size_t len);

int main(int argc, char **argv){
    FILE *f;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if(argc < 2){
        printf("Reading from stdin\n");
        f = stdin;
    }
    else {
        printf("Opening %s\n", argv[1]);
        f = fopen(argv[1], "r");
    }

    printf("Midi message buffer size is %li\n", sizeof(BMH_::rbuffer));

    if(f == NULL){
        exit(EXIT_FAILURE);
    }
    
    while((read = getline(&line, &len, f)) != -1){
        //printf("Read (%i): %s", len, line);
        printf("-------------------------------------------------------------\n");
        process_line(line, len);
    }

    fclose(f);

    if(line){
        free(line);
    }

    exit(1);
}

uint8_t linebuff[1024];

void process_midi(uint16_t o, uint16_t l);

void process_line(char *line, size_t len){
    int o = 0;
    char rb[] = {'0','0',0};
    if(line[0] == '#'){
        return;
    }

    for(size_t i=0;i<len;i++){
        if(line[i] == '\n'){
            break;
        }
        rb[0] = line[i];
        rb[1] = line[i+1];
        linebuff[o++] = (uint8_t)(strtol(rb, NULL, 16) & 0xff);
        //printf("rb: %s => %02hhx\n", rb, linebuff[o-1]);
        printf("%02hhx", linebuff[o-1]);
        i++;
    }
    printf("\n");
    BLEMidiHelper.process_blemidi(linebuff, o);

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

