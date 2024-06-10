FROM ubuntu:noble
RUN apt-get update && apt-get install -y vim gcc bash
ARG PROJECT_NAME="Webserv"
RUN mkdir -p $PROJECT_NAME/inc 
RUN mkdir -p $PROJECT_NAME/src
RUN mkdir -p $PROJECT_NAME/www

COPY ./ /$PROJECT_NAME

WORKDIR /$PROJECT_NAME

CMD ["bash", "-c", "while true; do sleep 3600; done"]
