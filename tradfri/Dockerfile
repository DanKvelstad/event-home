FROM libtradfri:latest as builder
COPY src /usr/src
COPY include /usr/include
COPY Makefile /usr
RUN cd /usr && make && make install

FROM debian:stretch-slim
COPY --from=builder /usr/lib/libtradfri.so /usr/lib
COPY --from=builder /usr/bin/tradfri /usr/bin
ENTRYPOINT [ "tradfri" ]