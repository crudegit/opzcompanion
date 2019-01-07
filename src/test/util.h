#ifndef __testutil_h__
#define __testutil_h__

#include <BLEMidiHelper.h>

typedef void (*line_processor)(uint8_t *line, size_t len);
typedef void (*midi_processor)(uint16_t offset, uint16_t len);

void read_file_by_line(const char *fname, line_processor lp);
void read_file_by_line(FILE *f, line_processor lp);

void process_blemidi_line(uint8_t *line, size_t len, midi_callback mcb);

#endif // __testutil_h__

#ifdef printf
#undef printf
#endif
