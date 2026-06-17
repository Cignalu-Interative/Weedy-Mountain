@echo off
SolutionGenerator.exe -bp -make

python "%~dp0patch_vcxproj.py" "%~dp0..\ide\2DPendentLib\2DPendentLib.vcxproj"

PAUSE