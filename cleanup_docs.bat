@echo off
REM LOOM Documentation Cleanup Script for Windows
REM Moves redundant documentation to archive folder

echo LOOM Documentation Cleanup
echo ==========================
echo.
echo This will archive redundant documentation files.
echo The master documentation remains in loom_complete_documentation.tex
echo The Wiki folder contains practical guides.
echo.
echo Files to be archived:
echo - loom_complete_framework.md
echo - loom-master-spec.md
echo - loom_register_architecture.md
echo - loom_mathematics.md
echo - README_NEW.md
echo - FULL_PROJECT_PLAN.md
echo - Kernelidea.md
echo.

set /p confirm="Proceed with cleanup? (y/n): "

if /i "%confirm%"=="y" (
    REM Create archive directory
    if not exist .deprecated_docs mkdir .deprecated_docs

    echo.
    echo Moving files to .deprecated_docs for backup...

    if exist loom_complete_framework.md move loom_complete_framework.md .deprecated_docs\ >nul 2>&1
    if exist loom-master-spec.md move loom-master-spec.md .deprecated_docs\ >nul 2>&1
    if exist loom_register_architecture.md move loom_register_architecture.md .deprecated_docs\ >nul 2>&1
    if exist loom_mathematics.md move loom_mathematics.md .deprecated_docs\ >nul 2>&1
    if exist README_NEW.md move README_NEW.md .deprecated_docs\ >nul 2>&1
    if exist FULL_PROJECT_PLAN.md move FULL_PROJECT_PLAN.md .deprecated_docs\ >nul 2>&1
    if exist Kernelidea.md move Kernelidea.md .deprecated_docs\ >nul 2>&1

    echo.
    echo Files moved to .deprecated_docs\
    echo.
    echo Next steps:
    echo 1. Review .deprecated_docs\ for any unique content
    echo 2. Extract unique content to wiki or .tex file
    echo 3. Once confident, delete .deprecated_docs\
    echo 4. Commit the cleaned structure to GitHub
) else (
    echo Cleanup cancelled.
)

pause