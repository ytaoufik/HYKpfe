FROM gcc:13 AS builder

WORKDIR /build

RUN apt-get update && apt-get install -y \
    libsqlite3-dev \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

COPY . .

RUN make all

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    libsqlite3-0 \
    libcurl4 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /build/hyk_backend .
COPY --from=builder /build/schema.sql .
COPY --from=builder /build/seed.sql .
COPY --from=builder /build/migrations/ ./migrations/

RUN mkdir -p /data

ENV PORT=8080
ENV SQLITE_DB_PATH=/data/hyk.db

EXPOSE 8080

HEALTHCHECK --interval=30s --timeout=5s --start-period=10s --retries=3 \
    CMD curl -f http://localhost:8080/health || exit 1

CMD ["./hyk_backend"]
