#ifndef HYK_MODELS_H
#define HYK_MODELS_H

typedef struct QuizPayload {
	char *title;
	char *questions_json;
} QuizPayload;

typedef struct ScorePayload {
	char game_code[16];
	char player_name[128];
	int score;
	int xp;
	int streak;
	int lives;
} ScorePayload;

#endif
