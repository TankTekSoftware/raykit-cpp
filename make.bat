
@echo off
setlocal enabledelayedexpansion

rem Default preset (matches Makefile default)
set "PRESET=linux-debug"

rem Parse arguments: action (build/configure/clean/rebuild) and KEY=VALUE pairs
set "ACTION="
:parse_loop
if "%~1"=="" goto parsed
if "%~1"=="-h" goto usage
if "%~1"=="--help" goto usage

echo "%~1" | findstr "=" >nul
if %ERRORLEVEL% EQU 0 (
	for /f "tokens=1* delims==" %%A in ("%~1") do (
		if /I "%%A"=="PRESET" (
			set "PRESET=%%B"
		) else (
			set "%%A=%%B"
		)
	)
) else (
	if not defined ACTION set "ACTION=%~1"
)

shift
goto parse_loop

:parsed
if not defined ACTION set "ACTION=build"

if /I "%ACTION%"=="all" set "ACTION=build"

if /I "%ACTION%"=="configure" (
	cmake --preset=%PRESET%
	exit /b %ERRORLEVEL%
)

if /I "%ACTION%"=="build" (
	cmake --preset=%PRESET%
	if errorlevel 1 exit /b %ERRORLEVEL%
	cmake --build --preset=%PRESET%
	exit /b %ERRORLEVEL%
)

if /I "%ACTION%"=="clean" (
	cmake --build --preset=%PRESET% --target clean
	exit /b %ERRORLEVEL%
)

if /I "%ACTION%"=="rebuild" (
	cmake --build --preset=%PRESET% --target clean
	if errorlevel 1 exit /b %ERRORLEVEL%
	cmake --preset=%PRESET%
	if errorlevel 1 exit /b %ERRORLEVEL%
	cmake --build --preset=%PRESET%
	exit /b %ERRORLEVEL%
)

echo Unknown action "%ACTION%".
goto usage

:usage
echo Usage: %~n0 [action] [KEY=VALUE]...
echo.
echo Actions:
echo   build       Configure and build (default)
echo   configure   Run cmake --preset=PRESET
echo   clean       Run cmake --build --preset=PRESET --target clean
echo   rebuild     Clean + configure + build
echo.
echo Examples:
echo   %~n0 build PRESET=desktop-debug
exit /b 1
