# Testing

## Backend

Commands:

```bash
cd backend
make test
make coverage
```

Coverage includes quiz validation, OpenAI error-path behavior, and DB integration checks.

## Frontend

Commands:

```bash
cd frontend
npm test
```

Recommended:

- Add tests for gameplay transitions
- Add mocked API tests for latency and failure modes
