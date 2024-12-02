#!/bin/bash

# define variables
PROJECT_NAME=${PWD##*/}
BUILD_DIR="./build"
INCLUDE_IN_BUILD="./assets"

# compilation targets
args_linux86_64()
{
    ARCHITECTURE="linux-86_64"
    COMPILER="/bin/gcc"
    ARGS="-lSDL2"
    FILE_EXSTENSION=""
}

args_win86-64()
{
    ARCHITECTURE="win-86_64"
    COMPILER="/usr/bin/x86_64-w64-mingw32-gcc"
    ARGS="-lmingw32 -lSDL2main -lSDL2 -mwindows"
    INCLUDE_IN_BUILD="$INCLUDE_IN_BUILD /usr/x86_64-w64-mingw32/bin/SDL2.dll"
    FILE_EXSTENSION=".exe"
}

args_emscripten()
{
    ARCHITECTURE="web"
    COMPILER="emcc"         # just make sure it's in $PATH (it's a pain to install)
    ARGS="-s USE_SDL=2"
    FILE_EXSTENSION=".html"
}

# handle arguments
if [ "$1" = "linux86_64" ]; then args_linux86_64
elif [ "$1" = "win86_64" ]; then args_win86-64
elif [ "$1" = "web" ]; then args_emscripten
else echo -e "\033[91mdidn't include any arguments! D:\033[0m" && exit 1
fi

# check whether $BUILD_DIR or $ARCHITECTURE isn't set
if [[ -z $BUILD_DIR ]] || [[ -z "$ARCHITECTURE" ]]; then
    echo -e "\033[91mBUILD_DIR or ARCHITECTURE not set D:\033[0m"
    exit 1
fi


# make (and clear) the build directory
mkdir -p "$BUILD_DIR/$ARCHITECTURE"
rm -rf "${BUILD_DIR:?}/$ARCHITECTURE/*"

# copy included files or directories to the build directory
if [[ -n $INCLUDE_IN_BUILD ]]; then
    cp -r "$INCLUDE_IN_BUILD" "$BUILD_DIR/$ARCHITECTURE"
fi

# get the executable path
EXE_PATH=$BUILD_DIR/$ARCHITECTURE/$PROJECT_NAME$FILE_EXSTENSION
echo "building at: $EXE_PATH"

# check whether the compiler can actually be executed
if [ ! -x "$COMPILER" ] && ! command -v "$COMPILER" > /dev/null; then
    echo -e "\033[91mCouldn't find an executable at path: \033[0m $COMPILER"
    exit 1
fi

# compile the code
COMMAND="$COMPILER $(find ./src -name "*.c") -o $EXE_PATH -Wall -g -lm $ARGS"
echo "using command: $COMMAND"
$COMMAND
exit $?
