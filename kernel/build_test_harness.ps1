<#
Build helper for the LOOM kernel test harness.

Usage: Open PowerShell (Developer prompt if using MSVC) and run:
  .\build_test_harness.ps1

The script will try MSVC `cl`, then `gcc`, then `clang`. If none are available
it prints guidance on installing Build Tools for Visual Studio or MinGW/WSL.
#>

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$srcDir = Join-Path $root "src"
$outExe = Join-Path $root "loom_test.exe"

Write-Host "LOOM test harness build helper"

function Build-With-Cl {
    Write-Host "Using MSVC cl.exe to build..."
    $cl = Get-Command cl -ErrorAction SilentlyContinue
    if (-not $cl) { return $false }
    Push-Location $srcDir
    $files = "loom_kernel.c loom_tensor.c loom_tensor_metal.c loom_tensor_cuda.c test_harness.c"
    # Basic compile; user should run Developer Command Prompt for cl to be in PATH
    & cl /O2 /Fe:$outExe /I"$srcDir" $files
    $rc = $LASTEXITCODE
    Pop-Location
    return ($rc -eq 0)
}

function Build-With-Gcc {
    Write-Host "Using gcc to build..."
    $gcc = Get-Command gcc -ErrorAction SilentlyContinue
    if (-not $gcc) { return $false }
    Push-Location $srcDir
    $files = "loom_kernel.c loom_tensor.c loom_tensor_metal.c loom_tensor_cuda.c test_harness.c"
    & gcc -std=c11 -O2 -Wall -I"$srcDir" $files -o $outExe
    $rc = $LASTEXITCODE
    Pop-Location
    return ($rc -eq 0)
}

function Build-With-Clang {
    Write-Host "Using clang to build..."
    $clang = Get-Command clang -ErrorAction SilentlyContinue
    if (-not $clang) { return $false }
    Push-Location $srcDir
    $files = "loom_kernel.c loom_tensor.c loom_tensor_metal.c loom_tensor_cuda.c test_harness.c"
    & clang -std=c11 -O2 -Wall -I"$srcDir" $files -o $outExe
    $rc = $LASTEXITCODE
    Pop-Location
    return ($rc -eq 0)
}

if (Build-With-Cl) { Write-Host "Build succeeded: $outExe"; exit 0 }
if (Build-With-Gcc) { Write-Host "Build succeeded: $outExe"; exit 0 }
if (Build-With-Clang) { Write-Host "Build succeeded: $outExe"; exit 0 }

Write-Host "No supported compiler found in PATH." -ForegroundColor Yellow
Write-Host "Options to proceed:" -ForegroundColor Cyan
Write-Host "  - Install Visual Studio Build Tools (select C++ build tools). Then run this script from the 'x64 Native Tools Command Prompt for VS'."
Write-Host "  - Install MinGW-w64 and add its bin directory to PATH (gcc)."
Write-Host "  - Use WSL (Windows Subsystem for Linux) and build there with gcc/clang." 
Write-Host "After installing a compiler re-run this script." -ForegroundColor Cyan
exit 2
