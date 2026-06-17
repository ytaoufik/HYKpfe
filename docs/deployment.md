# Deployment

## Frontend (Vercel)

1. Import repository in Vercel
2. Root directory: `frontend`
3. Set env vars:
	- `NEXT_PUBLIC_API_URL`
	- `NEXT_PUBLIC_SUPABASE_URL`
	- `NEXT_PUBLIC_SUPABASE_ANON_KEY`
4. Deploy and verify:
	- `/`
	- `/join`
	- `/teacher`

## Backend (Railway)

1. Create Railway project from GitHub repo
2. Set service root to repository root (Dockerfile path uses `backend/Dockerfile`)
3. Set env vars:
	- `PORT`
	- `SQLITE_DB_PATH`
	- `OPENAI_API_KEY`
	- `OPENAI_MODEL`
	- `SUPABASE_URL`
	- `SUPABASE_KEY`
4. Validate endpoints:
	- `/health`
	- `/stats`

## Local Docker Compose

```bash
docker compose up --build
```
