#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "util.h"

void parse_line(char *line, size_t len, line_processor lp);

void process_blemidi_line(uint8_t *line, size_t len, midi_callback mcb){
    BLEMidiHelper.process_blemidi(line, len);

    while(BLEMidiHelper.process_next_message(mcb)){
    }
}

void read_file_by_line(const char *fname, line_processor lp){
    FILE *f;
    
    f = fopen(fname, "r");
    
    if(f == NULL){
        exit(EXIT_FAILURE);
    }
    
    read_file_by_line(f, lp);
    
    fclose(f);
}

void read_file_by_line(FILE *f, line_processor lp){
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while((read = getline(&line, &len, f)) != -1){
        printf("Read (%li): %s", len, line);
        printf("-------------------------------------------------------------\n");
        parse_line(line, len, lp);
    }
    
    if(line){
        free(line);
    }

}

void parse_line(char *line, size_t len, line_processor lp){
    uint8_t linebuff[1024];

    int o = 0;
    char rb[] = {'0','0',0};
    if(line[0] == '#'){
        return;
    }

    for(size_t i=0;i<len;i++){
        if(line[i] == '\n'){
            break;
        }
        if((line[i] == ' ') | (line[i] == '\t')){
            continue;
        }
            
        rb[0] = line[i];
        rb[1] = line[i+1];
        linebuff[o++] = (uint8_t)(strtol(rb, NULL, 16) & 0xff);
        printf("rb: %s => %02hhx\n", rb, linebuff[o-1]);
        printf("%02hhx", linebuff[o-1]);
        i++;
    }
    printf("\n");
    lp(linebuff, o);
}