FROM ubuntu:noble
RUN apt-get update && apt-get install -y vim bash make clang nginx

ARG PROJECT_NAME="Webserv"
ARG PORT="8080"

RUN mkdir -p $PROJECT_NAME/inc 
RUN mkdir -p $PROJECT_NAME/src
RUN mkdir -p $PROJECT_NAME/www

COPY ./ /$PROJECT_NAME
WORKDIR /$PROJECT_NAME

RUN make re

EXPOSE $PORT

CMD ["bash", "-c", "while true; do sleep 3600; done"]
