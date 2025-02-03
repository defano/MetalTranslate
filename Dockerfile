FROM ubuntu:22.04 AS builder

    WORKDIR /app

    RUN apt update && apt install -y git build-essential cmake libdnnl-dev git pkg-config libicu-dev wget unzip

    COPY . /app/
    RUN ./init

    RUN cd /app/ && ./build.sh

