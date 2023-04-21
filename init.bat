@echo off

set VCPKG_REPO_URL="https://github.com/microsoft/vcpkg.git"
set "SCRIPT_DIR=%~dp0"
set "VCPKG_DIR=%SCRIPT_DIR%\vcpkg"

where git >nul 2>nul || (
    echo git command not found. Please install git and try again.
    exit /b 1
)

if not exist "%VCPKG_DIR%" (
    echo vcpkg not found. Cloning vcpkg repository...
    git clone %VCPKG_REPO_URL% %VCPKG_DIR%
)

echo Building vcpkg...
cd /d "%VCPKG_DIR%"
.\bootstrap-vcpkg.bat

echo Done!
