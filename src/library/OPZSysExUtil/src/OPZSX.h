#ifndef __OPZSX_h__
#define __OPZSX_h__

#include <stdint.h>

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

class OPZSX_ {
    public:
        void process_sysex(const char *buffer, int length);
    protected:
        opz_track_info track_info[16];
};

extern OPZSX_ OPZSX;

#endif // __OPZSX_h__
