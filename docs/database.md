# Database

## Development Database

- Engine: SQLite
- Files: `backend/schema.sql`, `backend/seed.sql`

## Production Database

- Engine: Supabase PostgreSQL
- Realtime channels subscribed from frontend for score events

## Tables

- `users`
- `quizzes`
- `questions`
- `answers`
- `players`
- `scores`
- `sessions`
- `achievements`
- `leaderboards`

## Key Indexes

- `idx_quizzes_code`
- `idx_scores_game_code`
- `idx_scores_player`
- `idx_players_game_code`
- `idx_sessions_game_player`

## Migration Strategy

- Initial schema: `migrations/001_init.sql`
- Runtime bootstrap in backend executes schema then seed
