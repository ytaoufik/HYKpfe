FROM ubuntu:24.04 AS build
WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
	build-essential \
	ca-certificates \
	curl \
	libsqlite3-dev \
	libcurl4-openssl-dev && rm -rf /var/lib/apt/lists/*

COPY . .
RUN make all

FROM ubuntu:24.04 AS runtime
WORKDIR /app
RUN apt-get update && apt-get install -y --no-install-recommends \
	libsqlite3-0 \
	libcurl4 \
	ca-certificates && rm -rf /var/lib/apt/lists/*

COPY --from=build /app/hyk_backend ./hyk_backend
COPY --from=build /app/schema.sql ./schema.sql
COPY --from=build /app/seed.sql ./seed.sql
ENV PORT=8080
EXPOSE 8080
CMD ["./hyk_backend"]
