#!/bin/bash

docker run --mount type=bind,src=${PWD},dst=/workspace -w /workspace gradle:5.6.3-jdk gradle shadowJar
docker build -t gradle-docker:0.1 -f- . <<EOF
FROM openjdk:8-jre-alpine3.9

COPY build/libs/gradle-docker-all.jar /app/gradle-docker-all.jar
CMD ["java", "-jar", "/app/gradle-docker-all.jar"]
EOF
