#ifndef __BLEMidiHelper_h__
#define __BLEMidiHelper_h__

#include <stdint.h>

#ifndef BLEMIDI_RBUFF_SIZE
#define BLEMIDI_RBUFF_SIZE 4096
#endif // BLEMIDI_RBUFF_SIZE


typedef void (*midi_callback)(uint16_t offset, uint16_t length);

class BMH_ {
	public:
		uint8_t rbuffer[BLEMIDI_RBUFF_SIZE];
		uint8_t process_blemidi(const uint8_t *msg, int len);
		uint8_t process_next_message(midi_callback);
		uint8_t gwc(uint16_t o, uint16_t l);
	private:
		uint16_t head = 0;
		uint16_t tail = 0;
};

extern BMH_ BLEMidiHelper;

#endif // __BLEMidiHelper_h__
