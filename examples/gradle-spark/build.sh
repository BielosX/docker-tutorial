gradle distTar
docker build -t gradle-spark:0.1 -f- . <<EOF
FROM openjdk:8-jre-alpine3.9

COPY build/distributions/gradle-spark.tar /gradle-spark.tar
RUN mkdir build && tar -xf /gradle-spark.tar -C /build && \
    mkdir -p /gradle-spark/lib && \
    cp /build/gradle-spark/lib/gradle-spark.jar /gradle-spark && \
    cp /build/gradle-spark/lib/*.jar /gradle-spark/lib
WORKDIR /gradle-spark
CMD ["java", "-cp", "gradle-spark.jar:lib/*", "Main"]
EOF
