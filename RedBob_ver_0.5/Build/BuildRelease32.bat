@echo off

REM THIS IS RELEASE WIN32 BUILD SCRIPT
REM Set the path to the Visual Studio environment variables
REM call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"

set SDL2_PATH=C:\dev\SDL2
set GLM_PATH=..\
set IMGUI_PATH=..\imgui
set IMGUI_BACKEND_PATH=..\imgui\backends

set SOURCE_FILES=..\src\*.cpp ..\imgui\*.cpp ..\imgui\backends\imgui_impl_sdl2.cpp ..\imgui\backends\imgui_impl_sdlrenderer2.cpp
set OUTPUT_EXE=RedBobVer02-Win32.exe

REM /MT	Causes the application to use the multithread, static version of the run-time library. Defines _MT and causes the compiler to place the library name LIBCMT.lib into the .obj file so that the linker will use LIBCMT.lib to resolve external symbols.
REM Set additional compiler and linker options if needed
set COMPILER_OPTIONS=/EHsc /Zi /MT
set LINKER_OPTIONS=/SUBSYSTEM:WINDOWS Shell32.lib

REM Set library paths and libraries
set LIB_PATHS=C:\dev\SDL2\lib\x86
set LIBRARIES=SDL2main.lib SDL2_ttf.lib SDL2.lib SDL2_image.lib SDL2_mixer.lib

REM Compile source files
cl %COMPILER_OPTIONS% %SOURCE_FILES% /Fe%OUTPUT_EXE% /I %SDL2_PATH% /I %GLM_PATH% /I %IMGUI_PATH% /I %IMGUI_BACKEND_PATH% /link /LIBPATH:%LIB_PATHS% %LIBRARIES% %LINKER_OPTIONS%

REM Clean up unnecessary files
del *.ilk *.obj *.pdb

REM Display any error messages
if errorlevel 1 (
    echo Compilation failed.
) else (
    echo Compilation successful.
)