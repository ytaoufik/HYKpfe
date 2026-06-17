# API Specification

Base URL (local): `http://localhost:8080`

## POST /generate-quiz

Request:

```json
{
	"input": "lesson text"
}
```

Response:

```json
{
	"ok": true,
	"game_code": "ABC123",
	"quiz": {
		"title": "...",
		"questions": [
			{
				"question": "...",
				"options": ["A", "B", "C", "D"],
				"correct_answer": 0,
				"explanation": "..."
			}
		]
	}
}
```

## POST /save-quiz

Request:

```json
{
	"game_code": "ABC123",
	"quiz": { "title": "...", "questions": [] }
}
```

## GET /quiz/:code

Fetches persisted quiz payload by game code.

## POST /submit-score

Request:

```json
{
	"game_code": "ABC123",
	"player_name": "Lina",
	"score": 1400,
	"xp": 320,
	"streak": 4,
	"lives": 2
}
```

## GET /leaderboard/:code

Returns top players ordered by score then XP.

## GET /health

Service liveness endpoint.

## GET /stats

Returns aggregate counts (quizzes, players, scores).
