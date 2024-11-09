@echo off
setlocal enabledelayedexpansion

set CC=gcc
set exec=val.exe
set flags=-g
set installDir="C:\Program Files\val\"

if "%1"=="make" (
    echo Compiling source files...
    for %%f in (src\*.c) do (
        set obj=%%~dpnf.o
        set objects=!objects! !obj!
        %CC% -c %flags% %%f -o !obj!
    )

    echo Linking object files...
    %CC% !objects! %flags% -o %exec%
    echo Build complete: %exec%
    exit /b 0
)

if "%1"=="install" (
    if exist %exec% (
        echo Installing %exec% to %installDir%...
        copy %exec% %installDir%
        echo Installation complete.
        exit /b 0
    ) else (
        echo Error: %exec% does not exist. Please build it first.
        exit /b 1
    )
)

if "%1"=="clean" (
    echo Cleaning up...
    del /Q %exec%
    for %%o in (src\*.o) do (
        del /Q %%o
    )
    echo Cleanup complete.
    exit /b 0
)

endlocal
