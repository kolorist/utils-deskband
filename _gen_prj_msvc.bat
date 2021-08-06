@echo off

if not exist "msvc" (
mkdir msvc
)

pushd %~dp0
cd msvc
call cmake -DUSE_MSVC_PROJECT=TRUE -DTARGET_PLATFORM="x64" -G "Visual Studio 16 2019" -A x64 ..
popd