# H.Y.K - AI Quiz Battle

H.Y.K (How You Know) is an AI-powered multiplayer quiz battle platform built for the ESISA end-of-semester project.

## Core Stack

- Backend: C (GCC), Mongoose HTTP, cJSON, libcurl, SQLite3
- Frontend: Next.js (App Router), TypeScript, Tailwind CSS
- AI: OpenAI API
- Databases: SQLite (local), Supabase PostgreSQL (production)
- Realtime: Supabase Realtime subscriptions
- CI/CD: GitHub Actions
- Deployment: Vercel (frontend), Railway (backend)

## Repository Structure

```text
backend/
frontend/
docs/
.github/
```

## Quick Start

### 1) Environment

Copy `.env.example` and fill values.

### 2) Run Backend

```bash
cd backend
cp .env.example .env
make all
./hyk_backend
```

### 3) Run Frontend

```bash
cd frontend
npm install
npm run dev
```

Frontend URL: `http://localhost:3000`

## Required API Endpoints

- `POST /generate-quiz`
- `POST /save-quiz`
- `GET /quiz/:code`
- `POST /submit-score`
- `GET /leaderboard/:code`
- `GET /health`
- `GET /stats`

## Testing

Backend:

```bash
cd backend
make test
```

Frontend:

```bash
cd frontend
npm test
```

## Deployment

- Frontend: Vercel (`frontend/vercel.json`)
- Backend: Railway (`backend/railway.toml`)
- Docker local stack: `docker-compose.yml`

## Documentation

- `docs/architecture.md`
- `docs/api.md`
- `docs/database.md`
- `docs/testing.md`
- `docs/deployment.md`
- `docs/demo_script.md`
- `docs/presentation.md`

## Team

- Group: project-groupeX-HYK
- Organization: chafik-boulealam-lab
- Supervisor: ch.boulealam@esisa.ac.ma
