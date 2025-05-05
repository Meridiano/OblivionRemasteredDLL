@echo off

cd /d "%~dp0"

git init
git submodule add -b main https://github.com/libxse/commonlibob64.git lib/commonlibob64
git submodule update --init --remote --recursive

pause
