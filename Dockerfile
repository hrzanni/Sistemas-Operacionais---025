FROM gcc:latest
WORKDIR /app

RUN mkdir -p /app/dados

COPY code-system-calls /app/code-system-calls
COPY webSite/dados/myFile.txt ./dados/

ARG SOURCE_FILE
RUN sh -c "gcc -o app $SOURCE_FILE"

CMD sh -c "./app; tail -f /dev/null"