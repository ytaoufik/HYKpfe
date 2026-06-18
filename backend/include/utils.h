#ifndef HYK_UTILS_H
#define HYK_UTILS_H

#include <stdarg.h>

struct mg_connection;
struct mg_str;

void log_info(const char *fmt, ...);
void log_error(const char *fmt, ...);
char *read_file_text(const char *path);
char *json_error(const char *message);
char *url_encode(const char *src);
char *url_decode(const char *src);

/* Mongoose string helpers */
char *dup_mg_string(struct mg_str s);
void respond_json(struct mg_connection *c, int status, const char *json_body);

#endif
