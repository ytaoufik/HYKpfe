#!/bin/bash
set -e

echo "Starting HYK Backend..."

# Set default environment variables if not already set
export PORT=${PORT:-8080}
export SQLITE_DB_PATH=${SQLITE_DB_PATH:-./hyk.db}
export AI_PROVIDER=${AI_PROVIDER:-openai}

# Make sure the executable has execute permissions
chmod +x ./hyk_backend

# Start the backend service
exec ./hyk_backend
