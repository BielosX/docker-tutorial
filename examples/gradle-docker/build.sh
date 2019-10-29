#!/bin/bash

docker run --mount type=bind,src=${PWD},dst=/workspace -w /workspace gradle:5.6.3-jdk gradle shadowJar
docker build . -f Dockerfile.jre -t gradle-docker:0.1
