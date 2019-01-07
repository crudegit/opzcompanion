#include <string.h>
#include "OPZSX.h"

OPZSX_ OPZSX;

void OPZSX_::process_sysex(const uint8_t *buffer, int length){
    // TODO: check that first bit of buffer is 'F000207601'
    
    const opz_sysex_header &seh = (const opz_sysex_header&)*buffer;
    
    printf("Parm id: %02hhx\n", seh.parm_id);
    
    if(seh.parm_id == 0x03){
        const opz_track_info &ti = (const opz_track_info &)buffer[sizeof(opz_sysex_header)-1];
        
        memcpy(&(track_info[ti.track]), &ti, sizeof(opz_track_info));
        #ifdef DOPRINTF
        
        printf("Updated track %i, data %02hhx %02hhx\n", ti.track, track_info[ti.track].value_p1, track_info[ti.track].value_p2);
        
        #endif
    }

}

