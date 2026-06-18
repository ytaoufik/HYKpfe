#ifndef HYK_ROUTES_H
#define HYK_ROUTES_H

#include "mongoose.h"

/* Health & Stats */
void handle_health(struct mg_connection *c);
void handle_stats(struct mg_connection *c);

/* Quiz */
void handle_generate_quiz(struct mg_connection *c, struct mg_http_message *hm);
void handle_save_quiz(struct mg_connection *c, struct mg_http_message *hm);
void handle_get_quiz(struct mg_connection *c, struct mg_http_message *hm);

/* Score & Leaderboard */
void handle_submit_score(struct mg_connection *c, struct mg_http_message *hm);
void handle_leaderboard(struct mg_connection *c, struct mg_http_message *hm);

/* Session */
void handle_save_session(struct mg_connection *c, struct mg_http_message *hm);
void handle_get_session(struct mg_connection *c, struct mg_http_message *hm);

#endif
