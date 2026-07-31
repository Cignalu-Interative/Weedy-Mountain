@echo off
SolutionGenerator.exe -bp -make

python "%~dp0patch_vcxproj.py" "%~dp0..\ide\2DPendentLib\2DPendentLib.vcxproj"
python "%~dp0add_opengl_filter.py" "%~dp0..\ide\2DPendentLib\2DPendentLib.vcxproj.filters"
python "%~dp0deploy_sdl3_dlls.py"
python "%~dp0..\res\Game\REPERTORY\scan_assets.py"
REM python "%~dp0patch_optimization.py" REM = commentaire

PAUSE