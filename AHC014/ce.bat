echo off
set USR_INPUT_FILE=
set /P USR_INPUT_FILE="execute file name (without extension): "
g++ -ftrapv -D_GLIBCXX_DEBUG -std=c++17 -I . ./%USR_INPUT_FILE%.cpp -o %USR_INPUT_FILE%