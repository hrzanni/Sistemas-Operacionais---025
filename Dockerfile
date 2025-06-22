FROM gcc:latest
WORKDIR /app

RUN mkdir -p /app/dados

COPY . .

ARG SOURCE_FILE
RUN if echo "$SOURCE_FILE" | grep -qE '\.cpp$'; then \
      g++ -o app "$SOURCE_FILE" -lpthread; \
    else \
      gcc -o app "$SOURCE_FILE" -lpthread; \
    fi
