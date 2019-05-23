@echo off

pushd .
call "c:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
popd

set compiler_flags=-GR- -EHa- -O2 -Oi -nologo -fp:fast -fp:except- -FC -WX -W4 -wd4201 -wd4100 -wd4189 /I ../libs /Z7 -DCONSOLE 
set linker_flags=-incremental:no -opt:ref user32.lib kernel32.lib opengl32.lib gdi32.lib shlwapi.lib

IF NOT EXIST bin mkdir bin

pushd bin
IF EXIST *.pdb DEL *.pdb

cl %compiler_flags% ../ch_test.cpp /link %linker_flags%

popd