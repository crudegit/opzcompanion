#ifndef __reassemble_h__
#define __reassemble_h__

#include <stdint.h>

#ifndef BLEMIDI_RBUFF_SIZE
#define BLEMIDI_RBUFF_SIZE 4096
#endif // BLEMIDI_RBUFF_SIZE

uint8_t rbuffer[BLEMIDI_RBUFF_SIZE];

typedef void (*midi_callback)(uint16_t offset, uint16_t length);

uint8_t process_blemidi(char *msg, int len);
uint8_t process_next_message(midi_callback);

#define gwc(o,l) rbuffer[(o+l)%BLEMIDI_RBUFF_SIZE]

#endif // __reassemble_h__
