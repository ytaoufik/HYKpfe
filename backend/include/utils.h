#ifndef HYK_UTILS_H
#define HYK_UTILS_H

#include <stdarg.h>

void log_info(const char *fmt, ...);
void log_error(const char *fmt, ...);
char *read_file_text(const char *path);
char *json_error(const char *message);

#endif
