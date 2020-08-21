@echo off
setlocal

set BuiltInVsWhereExe="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not defined ProgramFiles(x86) ( set "BuiltInVsWhereExe="%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"" )

if not exist %BuiltInVsWhereExe% (echo 请安装VS2017或者更高版本然后继续！& goto:eof )

for /f "tokens=*" %%i in ('%BuiltInVsWhereExe% -latest -prerelease -property installationPath') do ( set LatestVisualStudioRoot=%%i)


pushd "%~dp0"

md "%~dp0objs"

setlocal
call "%LatestVisualStudioRoot%\VC\Auxiliary\Build\vcvarsamd64_x86.bat"

md "%~dp0objs\%Platform%"
cl /O1 /Os /Oi /GS- /arch:IA32 /Z7 /MT /Fo"objs\%Platform%\CRT_parameter_handler_Thunks.obj" /Zl /c /D "NDEBUG" "%~dp0src\CRT_parameter_handler_Thunks.cpp"
endlocal

setlocal
call "%LatestVisualStudioRoot%\VC\Auxiliary\Build\vcvars64.bat"

md "%~dp0objs\%Platform%"
cl /O1 /Os /Oi /GS- /Z7 /MT /Fo"objs\%Platform%\CRT_parameter_handler_Thunks.obj" /Zl /c /D "NDEBUG" "%~dp0src\CRT_parameter_handler_Thunks.cpp"
endlocal

setlocal
call "%LatestVisualStudioRoot%\VC\Auxiliary\Build\vcvarsamd64_arm.bat"

md "%~dp0objs\%Platform%"
cl /O1 /Os /Oi /GS- /Z7 /MT /Fo"objs\%Platform%\CRT_parameter_handler_Thunks.obj" /Zl /c /D "NDEBUG" "%~dp0src\CRT_parameter_handler_Thunks.cpp"
endlocal

setlocal
call "%LatestVisualStudioRoot%\VC\Auxiliary\Build\vcvarsamd64_arm64.bat"

md "%~dp0objs\%Platform%"
cl /O1 /Os /Oi /GS- /Z7 /MT /Fo"objs\%Platform%\CRT_parameter_handler_Thunks.obj" /Zl /c /D "NDEBUG" "%~dp0src\CRT_parameter_handler_Thunks.cpp"
endlocal

pause

goto:eof