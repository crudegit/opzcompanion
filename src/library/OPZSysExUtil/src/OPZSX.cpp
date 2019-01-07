#include <string.h>
#include "OPZSX.h"

OPZSX_ OPZSX;

void OPZSX_::process_sysex(const char *buffer, int length){
    // TODO: check that first bit of buffer is 'F000207601'
    
    uint8_t parm_id = buffer[5];

    if(parm_id == 0x03){
        uint8_t track = buffer[9];
        memcpy(&(track_info[track]), &(buffer[5]), sizeof(opz_track_info));
    }

}

