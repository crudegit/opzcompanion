#ifndef __OPZSX_h__
#define __OPZSX_h__

#ifndef DOPRINTF
#ifndef printf
#define printf(...) ;
#endif
#else
#include <stdio.h>
#endif

#include <stdint.h>

#pragma pack(1)
typedef struct {
    uint8_t sysex_id;
    uint8_t vendor_id[3];
    uint8_t protocol_version;
    uint8_t parm_id;
} opz_sysex_header, *p_opz_sysex_header;

#pragma pack(1)
typedef struct {
    uint8_t parm_id;
    uint8_t value_p1;
    uint8_t value_p2;
    uint8_t track;
} opz_track_info, *p_opz_track_info;

#pragma pack(1)
typedef struct {
    uint8_t parm_id;
} opz_encoder_info, *p_opz_encoder_info;

#pragma pack(1)
typedef struct {
    uint8_t parm_id;
    
} opz_key_info, *p_opz_key_info;


class OPZSX_ {
    public:
        void process_sysex(const uint8_t *buffer, int length);
    protected:
        opz_track_info track_info[16];
        opz_key_info key_info;
};

extern OPZSX_ OPZSX;

#endif // __OPZSX_h__
