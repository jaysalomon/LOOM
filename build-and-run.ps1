# Build and run helper for LOOM V1 (Windows PowerShell)
# Usage: .\build-and-run.ps1 -loom examples\hello_hyperedge.loom

param(
    [string]$loom = "examples\hello_hyperedge.loom",
    [string]$out = "build\hello.c",
    [string]$exe = "build\hello.exe"
)

# Create build dir
if (-not (Test-Path build)) { New-Item -ItemType Directory -Path build | Out-Null }

Write-Host "1) Emit C from Loom source: $loom -> $out"
cargo run --manifest-path src/lang/Cargo.toml -- EmitC $loom -o $out
if ($LASTEXITCODE -ne 0) { Write-Error "Emit failed"; exit 1 }

Write-Host "2) Compile kernel + generated C -> $exe"
# Try clang, fallback to gcc
$compiled = $false
try {
    clang -O2 $out kernel\src\loom_kernel.c kernel\src\loom_tensor.c -o $exe
    if ($LASTEXITCODE -eq 0) { $compiled = $true }
} catch {}

if (-not $compiled) {
    try {
        gcc -O2 $out kernel\src\loom_kernel.c kernel\src\loom_tensor.c -o $exe
        if ($LASTEXITCODE -eq 0) { $compiled = $true }
    } catch {}
}

if (-not $compiled) { Write-Error "Compilation failed. Ensure clang or gcc is installed and on PATH."; exit 1 }

Write-Host "3) Run the executable"
& $exe

Write-Host "Done."