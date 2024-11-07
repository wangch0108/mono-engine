@echo off
pushd ..\
call External\premake\bin\premake5.exe --file=Tools/premake5.lua vs2022
popd
pause