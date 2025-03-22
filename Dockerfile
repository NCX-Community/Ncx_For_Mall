FROM ubuntu:22.04

# 安装必要的工具和依赖
RUN apt-get update && apt-get install -y \
    git \
    g++ \
    cmake \
    make \
    build-essential \
    libboost-all-dev \
    protobuf-compiler \
    libprotobuf-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# 重新编译proto文件
RUN protoc --cpp_out=./protocol --experimental_allow_proto3_optional ./protobuf/protocol.protoc

RUN rm -rf build && mkdir build && cd build && cmake .. && make

# 将ncxs源文件拷贝的根目录
COPY ./ncxs /app/ncxs

EXPOSE 8888

CMD ["./ncxs ncxs.toml"]