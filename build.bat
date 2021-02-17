@echo off

rem set VTK_DIR=D:/tmp/VTK-5.10.1-msvc2013-x86_amd64
set VTK_DIR=D:/VTK-5.10.1-msvc2013-x86_amd64
rem set VTK_DIR=D:/VTK/VTK-8.2.0

rd /Q /S bin
rd /Q /S bld
rd /Q /S kit
mkdir bld

pushd bld
cmake -G "Visual Studio 15 2017 Win64" ^
	-D CMAKE_RUNTIME_OUTPUT_DIRECTORY=..//bin ^
	-D VTK_DIR:PATH=%VTK_DIR% ^
	-D CMAKE_INSTALL_PREFIX=..//kit ..
popd