# Contributing to H.Y.K - AI Quiz Battle

## Branching

- `main`: production-ready code
- `develop`: integration branch
- feature branches: `feat/<scope>`
- fix branches: `fix/<scope>`

## Commit Convention

- `feat: ...`
- `fix: ...`
- `refactor: ...`
- `docs: ...`
- `test: ...`
- `ci: ...`

## Pull Requests

1. Rebase on latest `develop`
2. Run checks locally:
	- `cd backend && make test`
	- `cd frontend && npm test && npm run build`
3. Fill PR template
4. Request at least one review from team members

## Coding Standards

- Backend C: strict warnings, modular files, defensive error handling
- Frontend: TypeScript strict mode, no any, reusable components
- Tests are mandatory for new logic

## Security

- Never commit secrets
- Use `.env.example` as the contract for required environment variables
