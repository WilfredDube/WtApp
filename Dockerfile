# GCC support can be specified at major, minor, or micro version
# (e.g. 8, 8.2 or 8.2.0).
# See https://hub.docker.com/r/library/gcc/ for all supported GCC
# tags from Docker Hub.
# See https://docs.docker.com/samples/library/gcc/ for more on how to use this image
# docker run --rm -it  --network host centos:latest
FROM frolvlad/alpine-gxx
# FROM centos

# Install dependences
RUN apk add --no-cache make cmake git sqlite
RUN g++ --version
# RUN git clone https://github.com/emweb/wt.git
RUN apk --update add --no-cache \
    build-base \
    boost boost-dev

RUN apk --update add --no-cache \
    glew-dev \
    openssl graphicsmagick

RUN mkdir -p third_party \
    && cd third_party \
    && git clone https://github.com/emweb/wt.git \
    && ls \
    && cd wt \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && make \
    && make install \
    && make clean


RUN apk --update add --no-cache tcl \
    freetype \
    tcl-dev freetype-dev

RUN apk --update add --no-cache tk tk-dev

WORKDIR /opt/
COPY ./third_party/OCCT /opt/
RUN mkdir -p build \
    && cd build \
    && cmake .. \
    && make \
    && make install \
    && make clean
RUN rm -rf *

# RUN mkdir WtApp && cd WtApp
WORKDIR /WtApp
COPY ./src/* /WtApp/

# RUN mkdir -p third_party \
#     && cd third_party \
#     && git clone https://github.com/Open-Cascade-SAS/OCCT.git \
#     && ls \
#     && cd OCCT \
#     && mkdir -p build \
#     && cd build \
#     && cmake .. \
#     && make \
#     && make install \
#     && make clean


# COPY ./third_party/wt /opt
# RUN git submodule update --init --recursive
# RUN apk add --no-cache boost-dev
# RUN cd wt && mkdir build && cd build && cmake ../ && make && make install && make clean
# RUN rm -rf wt
# These commands copy your files into the specified directory in the image
# and set that as the working location
# COPY ./third_party/opencascade-7.4.0.tgz /opt/
# WORKDIR /opt/docker_test
# COPY ./hello-docker.cpp /opt/docker_test
# COPY ./Makefile /opt/docker_test
# COPY ./CMakeLists.txt /opt/docker_test

# This command compiles your app using GCC, adjust for your source code
# RUN g++ -o hello-docker hello-docker.cpp -lwthttp -lwt

# This command runs your application, comment out this line to compile only
# CMD ["./hello-docker"]

# LABEL Name=hellogrpc Version=0.0.1