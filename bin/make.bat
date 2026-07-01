@echo off
SolutionGenerator.exe -bp -make

python "%~dp0patch_vcxproj.py" "%~dp0..\ide\2DPendentLib\2DPendentLib.vcxproj"
python "%~dp0add_opengl_filter.py" "%~dp0..\ide\2DPendentLib\2DPendentLib.vcxproj.filters"
REM python "%~dp0patch_optimization.py" REM = commentaire

PAUSE