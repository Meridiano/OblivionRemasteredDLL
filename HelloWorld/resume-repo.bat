@echo off

cd /d "%~dp0"

git init
git submodule add -b main https://github.com/shad0wshayd3/CommonLibOBR.git lib/commonlibobr
git submodule update --init --remote --recursive

pause
