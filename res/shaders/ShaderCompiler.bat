@echo off
setlocal enabledelayedexpansion

set SHADERCROSS=exec\shadercross\shadercross.exe
set RAW_DIR=raw
set OUT_SPIRV=Compiled\spirv
set OUT_DXIL=Compiled\dxil
set OUT_MSL=Compiled\msl

if not exist "%OUT_SPIRV%" mkdir "%OUT_SPIRV%"
if not exist "%OUT_DXIL%" mkdir "%OUT_DXIL%"
if not exist "%OUT_MSL%" mkdir "%OUT_MSL%"

echo Compilation des shaders
echo.

for %%f in ("%RAW_DIR%\*.vert" "%RAW_DIR%\*.frag" "%RAW_DIR%\*.comp") do (

	set "filename=%%~nf"
	set "ext=%%~xf"
	echo -- %%~nxf

	if "!ext!"==".vert" (
		set STAGE=vertex
	) else if "!ext!"==".frag" (
		set STAGE=fragment
	)else if "!ext!"==".comp" (
		set STAGE=compute
	)

	%SHADERCROSS% "%%f" -s HLSL -t !STAGE! -o "%OUT_SPIRV%\!filename!!ext!.spv"
	if errorlevel 1 (
		echo [ERREUR] SPRIV : %%~nxf
	) else (
		echo OK - SPIRV
	)

	%SHADERCROSS% "%%f" -s HLSL -t !STAGE! -o "%OUT_DXIL%\!filename!!ext!.dxil"
	if errorlevel 1 (
		echo [ERREUR] DXIL : %%~nxf
	) else (
		echo OK - DXIL
	)

	%SHADERCROSS% "%%f" -s HLSL -t !STAGE! -o "%OUT_MSL%\!filename!!ext!.msl"
	if errorlevel 1 (
		echo [ERREUR] MSL : %%~nxf
	) else (
		echo OK - MSL
	)
)
echo Termine.
pause


