# First stage: Build LLVM & Clang in a clean environment
FROM fedora:latest AS build

# Install necessary dependencies
RUN dnf install -y \
    clang \
    llvm-devel \
    clang-devel \
    libffi-devel \
    libxml2-devel \
    cmake \
    make \
    gcc-c++ \
    nodejs\
    && dnf clean all

# Set working directory inside the container
WORKDIR /usr/src/app

# Copy project files (from ./analyzer)
COPY ./analyzer ./analyzer

# Move into the analyzer directory and build the project
WORKDIR /usr/src/app/analyzer

# Clean previous CMake builds
RUN rm -rf .cmake .bin .cmake-build-debug CMakeCache.txt CMakeFiles build Testing cmake_install.cmake compile_commands.json Makefile \
    && mkdir build && cd build \
    && cmake .. && make -j$(nproc)

# Setup the frontend
WORKDIR /usr/src/app
COPY ./frontend ./frontend
WORKDIR /usr/src/app/frontend

RUN rm -rf dist node_modules && npm install && npm run-script build

WORKDIR /usr/src/app

# Setup script to run on startup
COPY ./start.sh ./start.sh
RUN chmod +x ./start.sh
ENTRYPOINT ["./start.sh"]
CMD ["+classic.c"]
