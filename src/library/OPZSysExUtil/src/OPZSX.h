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
    
    // F000207601 06 01 40 1F 1F 02 F7
    
    uint8_t key11 : 1;
    uint8_t record : 1;
    uint8_t mixer : 1;
    uint8_t key14 : 1;
    
    uint8_t key15 : 1;
    uint8_t key16 : 1;
    uint8_t key17 : 1;
    uint8_t key18 : 1;
    
    uint8_t key21 : 1;
    uint8_t key22 : 1;
    uint8_t key23 : 1;
    uint8_t key24 : 1;
    
    uint8_t key25 : 1;
    uint8_t key26 : 1;
    uint8_t key27 : 1;
    uint8_t key28 : 1;
    
    uint8_t key31 : 1;
    uint8_t metronome : 1;
    uint8_t key33 : 1;
    uint8_t key34 : 1;
    
    uint8_t key35 : 1;
    uint8_t key36 : 1;
    uint8_t key37 : 1;
    uint8_t key38 : 1;
    
    uint8_t key41 : 1;
    uint8_t stop : 1;
    uint8_t screen : 1;
    uint8_t key44 : 1;
    
    uint8_t key45 : 1;
    uint8_t key46 : 1;
    uint8_t key47 : 1;
    uint8_t key48 : 1;
    
    uint8_t key51 : 1;
    uint8_t key52 : 1;
    uint8_t key53 : 1;
    uint8_t key54 : 1;
    
    uint8_t key55 : 1;
    uint8_t key56 : 1;
    uint8_t project : 1;
    uint8_t track : 1;
    
} opz_key_info, *p_opz_key_info;

const uint8_t OPZ_VENDOR_ID[3] = {0x00, 0x20, 0x76};
const uint8_t OPZ_MAX_PROTOCOL_VERSION = 0x01;

class OPZSX_ {
    public:
        void process_sysex(const uint8_t *buffer, int length);
    protected:
        opz_track_info track_info[16];
        opz_key_info key_info;
        opz_key_info prev_key_info;
};

extern OPZSX_ OPZSX;

#endif // __OPZSX_h__
