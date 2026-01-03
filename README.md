# Group4Project2

## Video Link
[Group 4](https://www.youtube.com/watch?v=25NMCawHJxI)

## Setup Instructions

We recommend using Docker as it couples the entire build process (frontend & analyzer) into a single container, making it easier to run and use the project. In development and further testing, installing LLVM directly through Homebrew may be needed. Setup instructions for both a local installation and using a Docker container are provided.

### Docker

We provide a Dockerfile that creates a Linux container for running the analyzer and frontend combined:

```
docker build -t cpsc410-project2 .
```

This command will pull a Fedora Linux base image, install the required packages (clang, LLVM, node), and build the project. This will take a few minutes.

Then, you can run the container:

```
docker run -it -v ./analyzer/tests/cases:/usr/src/tests -p 5173:5173 cpsc410-project2
```

This will run the analyzer and start the Vite server to serve the website. To analyze a specific program in mind from `analyzer/tests/cases`, you may specify the file name as an additional argument:

```
docker run -it -v ./analyzer/tests/cases:/usr/src/tests -p 5173:5173 cpsc410-project2 -no_cycle.c
```

Note that the tests that are provided in this folder contain a prefix of `+` or `-` signifying whether or not there is a deadlock in this program or not.

#### Running Custom Tests (outside `/analyzer` directory)

The provided `docker run` commands mount the sample C programs from this repository at`/usr/src/tests` in the container. If you have some other programs you want to run (i.e. `/foo/bar/baz` on your local machine), you can simply change the host path for the volume mapping:

```
docker run -it -v /foo/bar/baz:/usr/src/tests -p 5173:5173 cpsc410-project2 hello_world.c
```

Or alternatively, if you want to run just a single test (i.e. `test.c`):

```
docker run -it -v /foo/bar/test.c:/usr/src/test.c -p 5173:5173 cpsc410-project2 test.c
```

### MacOS

This project requires the LLVM toolchain to be installed on the users machine. Out of the box, LLVM is installed on MacOS, however, a separate installation is required in order to interface with the header files containing AST parsers directly (and the Clang AST headers). To keep this separate from pre-existing configurations, we recommend installing it with homebrew, using the following command:

```
brew install llvm
```

Running this command on a Mac assumes no further setup needed, as the CMake configuration is automatically set to grab Clang/LLVM from the folder Homebrew would install this on. Do not make any edits to any global configuration and keep your computer running the pre-installed LLVM downloaded with Command line Developer Tools on MacOS.

To run, you must the following commands, or use Clion. Clion is recommended as the LLVM header and libraries that are required pulls in a lot of files and having a more full-fledged IDE to provide syntax highlighting, debugging, static analysis is recommended. With Clion, after selecting the `CMakeLists.txt` file, you should be able to run the project directly (remember to set a file name as part of the configuration as an argument with the executable).

Commands:

1. Within `/analyzer`

```
cmake .
```

2. With `/analyzer`

```
make
```

3. You are good to go. The binary executable is saved as `dsa` and within the `bin` folder inside of the analyzer `./analyzer/bin/dsa`

Note, the build size is roughly 75 megabytes with targetting to an ARM executable.

To run on a sample program within the `tests` folder (while the working directory is `./analyzer`):

```
./bin/dsa ./tests/cases/+classic.c
```

Afterwards, the output JSON file can be uploaded to our frontend visualization tool. Since this is decoupled from the analyzer, you would need to build and run the frontend separately. 

Working directory: `/frontend`

```
npm install
npm run dev
```

#### Addendum for Intel Macs

On an Intel-based Mac, you may encounter this error:

```
CMake Error at CMakeLists.txt:17 (find_package):
  Could not find a package configuration file provided by "LLVM" with any of
  the following names:

    LLVMConfig.cmake
    llvm-config.cmake

  Add the installation prefix of "LLVM" to CMAKE_PREFIX_PATH or set
  "LLVM_DIR" to a directory containing one of the above files.  If "LLVM"
  provides a separate development package or SDK, be sure it has been
  installed.
```

To fix this error, change the line `set(LLVM_PATH ...)` in CMakeLists.txt by replacing the dots with the path found by entering `brew info llvm` in the termianl.

### Linux

Install LLVM (maybe from source or GitHub). The CMake file may need to be modified as it is currently only tested under MacOS and the specific installation using Homebrew.

After LLVM is installed, follow the above instructions within the MacOS section to build and run the application.

The setup instructions for the frontend component follows the exact same sequence of steps as MacOS:

Afterwards, the output JSON file can be uploaded to our frontend visualization tool. Since this is decoupled from the analyzer, you would need to build and run the frontend separately. 

Working directory: `/frontend`

```
npm install
npm run dev
```

## Attributions

The JSON library (`analyzer/include/json.hpp`) was obtained from the `nlohmann/json` project: https://github.com/nlohmann/json
