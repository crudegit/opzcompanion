#include <string.h>
#include "OPZSX.h"

OPZSX_ OPZSX;

#define CAST_MESSAGE(TYP, NAM) const TYP & NAM = (const TYP &)buffer[sizeof(opz_sysex_header)-1]

void OPZSX_::process_sysex(const uint8_t *buffer, int length){
    // TODO: check that first bit of buffer is 'F000207601'
    
    const opz_sysex_header &seh = (const opz_sysex_header&)*buffer;
    
    if(memcmp(OPZ_VENDOR_ID, seh.vendor_id, sizeof(OPZ_VENDOR_ID)) != 0){
        printf("Vendor ID %02hhx:%02hhx:%02hhx is not the expected ID %02hhx:%02hhx:%02hhx\n",seh.vendor_id[0],seh.vendor_id[1],seh.vendor_id[2],OPZ_VENDOR_ID[0],OPZ_VENDOR_ID[1],OPZ_VENDOR_ID[2]);
        return;
    }
    if((seh.protocol_version == 0) || (seh.protocol_version > OPZ_MAX_PROTOCOL_VERSION)){
        printf("Unexpected protocol version %02hhx, was expecting > 0 and <= %02hhx\n", seh.protocol_version, OPZ_MAX_PROTOCOL_VERSION);
        return;
    }
    
    printf("Parm id: %02hhx\n", seh.parm_id);
    
    switch(seh.parm_id){
        case 0x03: {
            const opz_track_info &ti = (const opz_track_info &)buffer[sizeof(opz_sysex_header)-1];
            
            memcpy(&(track_info[ti.track]), &ti, sizeof(opz_track_info));
            #ifdef DOPRINTF
            
            printf("Updated track %i, data %02hhx %02hhx\n", ti.track, track_info[ti.track].value_p1, track_info[ti.track].value_p2);
            
            #endif
        }break;
        case 0x06: {
            CAST_MESSAGE(opz_key_info, ki);
            
            memcpy(&(prev_key_info), &key_info, sizeof(key_info));
            memcpy(&(key_info), &ki, sizeof(key_info));
            
        }
    }

}

