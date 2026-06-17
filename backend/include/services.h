#ifndef HYK_SERVICES_H
#define HYK_SERVICES_H

#include <stdbool.h>

char *gemini_generate_quiz_json(const char *input_text, int retries);
bool quiz_validate_payload(const char *quiz_json, char *error_buf, int error_buf_len);
char *quiz_generate_code(void);
int quiz_save(const char *game_code, const char *quiz_json);
char *quiz_fetch_by_code(const char *game_code);
int score_submit(const char *body_json);
char *leaderboard_fetch(const char *game_code);
char *stats_fetch_json(void);

#endif
