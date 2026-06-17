# Architecture

## Overview

H.Y.K is a distributed quiz game platform with a C backend and Next.js frontend.

Flow:

1. Teacher submits lesson/topic text.
2. Backend calls OpenAI to generate exactly 10 MCQs.
3. Quiz is validated and stored with a game code.
4. Students join and play in web UI.
5. Scores are autosaved after each answer.
6. Leaderboard updates in real time.

## Backend Layers

- `api/`: HTTP server and endpoint dispatch (Mongoose)
- `services/`: business rules (quiz generation, validation, scoring)
- `database/`: SQLite bootstrap + migration handling
- `models/`: domain helpers
- `utils/`: logging, file, and JSON helpers

## Frontend Layers

- `app/`: App Router pages (teacher, join, play, results, leaderboard)
- `components/`: reusable gameplay and UI components
- `lib/`: API client, scoring logic, Supabase realtime client

## Data Stores

- Local development and tests: SQLite
- Production: Supabase PostgreSQL + Realtime

## Reliability

- JSON schema validation for generated quizzes
- OpenAI retry and timeout strategy
- Session auto-save and restore after refresh
- CI pipeline with backend/frontend tests
