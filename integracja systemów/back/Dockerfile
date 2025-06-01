FROM mcr.microsoft.com/vscode/devcontainers/cpp:ubuntu-22.04

ENV DEBIAN_FRONTEND=noninteractive \
    VCPKG_ROOT=/opt/vcpkg \
    VCPKG_DEFAULT_TRIPLET=x64-linux

RUN apt-get update && apt-get install -y \
    cmake \
    pkg-config \
    git \
    curl \
    build-essential \
    libssl-dev \
    libmysqlcppconn-dev \
    autoconf \
    automake \
    autoconf-archive \
    python3 \
    python3-pip

RUN pip3 install matplotlib

RUN git clone https://github.com/microsoft/vcpkg.git $VCPKG_ROOT \
    && $VCPKG_ROOT/bootstrap-vcpkg.sh

RUN $VCPKG_ROOT/vcpkg install \
    boost-system \
    boost-json \
    boost-beast \
    boost-asio \
    openssl \
    mysql-connector-cpp

ENV CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

WORKDIR /app
COPY . /app

RUN cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
    && cmake --build build

EXPOSE 8080

CMD ["./build/Integracja"]