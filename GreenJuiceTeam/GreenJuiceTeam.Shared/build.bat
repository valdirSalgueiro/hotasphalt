@echo off

call "%VS140COMNTOOLS%vsvars32.bat"

rem emscripten configuration
call D:\_Downloads\emsdk-portable-64bit\emscripten\1.37.22\emcmake cmake -G "MinGW Makefiles"

rem build
nmake

D:\_Downloads\emsdk-portable-64bit\emscripten\1.37.22\emcc -O2 fmod.bc libGreenJuiceTeam.bc -s TOTAL_MEMORY=167772160 -o final.html --preload-file ../res@/ --shell-file shell_minimal.html