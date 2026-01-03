cmake_minimum_required(VERSION 3.10)
project(DeadlockStaticAnalyzer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/main.cpp src/Visitor.cpp src/Detector.cpp)

# Detect OS
if(APPLE)
    set(LLVM_PATH /opt/homebrew/opt/llvm)
    set(LLVM_LIB_DIR ${LLVM_PATH}/lib)

    find_package(LLVM REQUIRED CONFIG PATHS ${LLVM_LIB_DIR}/cmake/llvm NO_DEFAULT_PATH)
    find_package(Clang REQUIRED CONFIG PATHS ${LLVM_LIB_DIR}/cmake/clang NO_DEFAULT_PATH)

    add_executable(dsa ${SOURCES})
    target_include_directories(dsa PRIVATE include lib ${CLANG_INCLUDE_DIRS} ${LLVM_PATH}/include/c++/v1)
    target_link_libraries(dsa PRIVATE clangTooling)
    target_compile_options(dsa PRIVATE -isystem /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include)
    target_link_options(dsa PRIVATE -Wl,-dead_strip)
    add_custom_command(TARGET dsa POST_BUILD COMMAND strip ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dsa)
elseif(UNIX) # Note: needs testing with other Linux distros other than Fedora/RHEL/CentOS
    set(LLVM_DIR /usr/lib64)
    set(CLANG_DIR /usr/lib64)
    
    find_package(LLVM REQUIRED CONFIG PATHS ${LLVM_DIR})
    find_package(Clang REQUIRED CONFIG PATHS ${CLANG_DIR})

    include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})

    # Get LLVM compile & link flags
    execute_process(COMMAND llvm-config --cxxflags OUTPUT_VARIABLE LLVM_CXXFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND llvm-config --libs --system-libs OUTPUT_VARIABLE LLVM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Find libclang-cpp
    find_library(CLANG_CPP_LIBRARY clang-cpp REQUIRED)

    add_executable(dsa ${SOURCES})

    # Link against libclang-cpp instead of individual libraries
    target_link_libraries(dsa PRIVATE ${CLANG_CPP_LIBRARY} ${LLVM_LIBS} ${LLVM_LDFLAGS})
    target_include_directories(dsa PRIVATE include)

else()
    message(FATAL_ERROR "Unsupported OS")
endif()