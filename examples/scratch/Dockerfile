FROM alpine:3.10.3 as builder
RUN apk add --no-cache gcc build-base
COPY main.c /main.c
RUN gcc -o main main.c

FROM scratch
COPY --from=builder /main /main
COPY --from=builder /lib/ld-musl-x86_64.so.1 /lib/ld-musl-x86_64.so.1
CMD ["/main"]
