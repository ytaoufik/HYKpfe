#ifndef HYK_SERVICES_H
#define HYK_SERVICES_H

#include <stdbool.h>

/* AI Provider (unified) */
char *ai_generate_quiz_json(const char *input_text, int retries);

/* OpenAI */
char *openai_generate_quiz_json(const char *input_text, int retries);

/* Gemini */
char *gemini_generate_quiz_json(const char *input_text, int retries);

/* Quiz */
bool quiz_validate_payload(const char *quiz_json, char *error_buf, int error_buf_len);
char *quiz_generate_code(void);
int quiz_save(const char *game_code, const char *quiz_json);
char *quiz_fetch_by_code(const char *game_code);

/* Score / Leaderboard */
int score_submit(const char *body_json);
char *leaderboard_fetch(const char *game_code);

/* Stats */
char *stats_fetch_json(void);

/* Session persistence */
int session_save(const char *game_code, const char *player_name, const char *state_json);
char *session_fetch(const char *game_code, const char *player_name);

/* Achievements */
int achievement_save(const char *game_code, const char *player_name, const char *badge_key);
char *achievements_fetch(const char *game_code, const char *player_name);

#endif
