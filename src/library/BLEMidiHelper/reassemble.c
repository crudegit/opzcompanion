#include <reassemble.h>
#include <stdint.h>

#define printf(x,y) "";

uint16_t head = 0;
uint16_t tail = 0;

midi_callback process_midi(uint16_t o, uint16_t l);

uint8_t process_blemidi(char *msg, int len){
    uint16_t last_tail = tail;
    if( ((last_tail+BLEMIDI_RBUFF_SIZE-1-head) % BLEMIDI_RBUFF_SIZE) < len){
        return 0;
    }
    
    uint8_t tslNext = 0;
    for(int i=1;i<len;i++){
        if(msg[i] & 0x80){
            if(!tslNext){
                tslNext = 1;
                continue;
            }
            else {
                tslNext = 0;
            }
        }
        rbuffer[head] = msg[i];
        printf("%02hhx", msg[i]);
        head = (head + 1) % BLEMIDI_RBUFF_SIZE;
    }
    printf("\nHead at %i\n", head);
    return 1;
}


uint8_t process_next_message(midi_callback cb){
    if(tail == head){
        return 0;
    }
    
    uint8_t fb = rbuffer[tail];
    uint8_t is_sysex = (fb == 0xf0) ? 1 : 0;
    uint16_t mmlen = 0;
    uint16_t cnt = 0;

    if(!is_sysex){
        if(fb == 0xf7){
            mmlen = 1;
        }
        else {
            mmlen = 3;
        }
    }

    for(uint16_t i=tail;i!=head;i=(i+1)%BLEMIDI_RBUFF_SIZE){
        cnt++;
        if(is_sysex){
            if(cnt > 1 && rbuffer[i] & 0x80){
                cb(tail, cnt);
                tail = i;
                return 1;
            }
        }
        else {
            if(cnt == mmlen){
                cb(tail, mmlen);
                tail += cnt;
                return 1;
            }
        }
    }

    return 0;
}

