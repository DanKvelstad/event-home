FROM gcc:latest as builder
COPY src /usr/src
COPY include /usr/include
COPY Makefile /usr
RUN cd /usr && make && make install

FROM gcc:latest
COPY include /usr/include
COPY --from=builder /usr/lib/libtradfri.so /usr/lib