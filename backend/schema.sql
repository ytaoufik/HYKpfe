CREATE TABLE IF NOT EXISTS users (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	email TEXT UNIQUE NOT NULL,
	display_name TEXT NOT NULL,
	role TEXT NOT NULL DEFAULT 'student',
	created_at TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS quizzes (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	game_code TEXT UNIQUE NOT NULL,
	title TEXT NOT NULL,
	payload_json TEXT NOT NULL,
	created_at TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS questions (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	quiz_id INTEGER NOT NULL,
	question_text TEXT NOT NULL,
	correct_answer INTEGER NOT NULL,
	explanation TEXT NOT NULL,
	FOREIGN KEY (quiz_id) REFERENCES quizzes(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS answers (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	question_id INTEGER NOT NULL,
	option_index INTEGER NOT NULL,
	answer_text TEXT NOT NULL,
	FOREIGN KEY (question_id) REFERENCES questions(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS players (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	game_code TEXT NOT NULL,
	player_name TEXT NOT NULL,
	created_at TEXT NOT NULL,
	UNIQUE(game_code, player_name)
);

CREATE TABLE IF NOT EXISTS scores (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	game_code TEXT NOT NULL,
	player_name TEXT NOT NULL,
	score INTEGER NOT NULL,
	xp INTEGER NOT NULL,
	streak INTEGER NOT NULL,
	lives INTEGER NOT NULL,
	submitted_at TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS sessions (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	game_code TEXT NOT NULL,
	player_name TEXT NOT NULL,
	state_json TEXT NOT NULL,
	updated_at TEXT NOT NULL,
	UNIQUE(game_code, player_name)
);

CREATE TABLE IF NOT EXISTS achievements (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	game_code TEXT NOT NULL,
	player_name TEXT NOT NULL,
	badge_key TEXT NOT NULL,
	unlocked_at TEXT NOT NULL,
	UNIQUE(game_code, player_name, badge_key)
);

CREATE TABLE IF NOT EXISTS leaderboards (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	game_code TEXT NOT NULL,
	snapshot_json TEXT NOT NULL,
	created_at TEXT NOT NULL
);

CREATE INDEX IF NOT EXISTS idx_quizzes_code ON quizzes(game_code);
CREATE INDEX IF NOT EXISTS idx_scores_game_code ON scores(game_code);
CREATE INDEX IF NOT EXISTS idx_scores_player ON scores(player_name);
CREATE INDEX IF NOT EXISTS idx_players_game_code ON players(game_code);
CREATE INDEX IF NOT EXISTS idx_sessions_game_player ON sessions(game_code, player_name);
