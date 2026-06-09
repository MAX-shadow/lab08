FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y gcc g++ cmake git ca-certificates && \
    rm -rf /var/lib/apt/lists/*

COPY . /app/
WORKDIR /app

RUN cmake -H. -B_build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=_install
RUN cmake --build _build
RUN cmake --build _build --target install

ENV LOG_PATH=/home/logs/log.txt

VOLUME /home/logs

WORKDIR /app/_install/bin

ENTRYPOINT ["./demo"]
