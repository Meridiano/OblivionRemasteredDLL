@echo off

cd /d "%~dp0"

git init
git submodule add -b main https://github.com/shad0wshayd3-TES4/CommonLibOB64.git lib/commonlibob64
git submodule update --init --remote --recursive

pause
