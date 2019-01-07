#ifndef __testutil_h__
#define __testutil_h__

typedef void (*line_processor)(uint8_t *line, size_t len);

void read_file_by_line(const char *fname, line_processor lp);
void read_file_by_line(FILE *f, line_processor lp);

#endif // __testutil_h__