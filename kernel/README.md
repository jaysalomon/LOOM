# LOOM Kernel - Build Instructions

This directory contains the LOOM kernel sources and a small test harness.

Quick build (cross-platform, using CMake):

On macOS (with Xcode and CMake):

```bash
mkdir -p build && cd build
cmake -DLOOM_USE_METAL=ON ..
cmake --build . --config Release
./loom_test
```

On Windows (MSVC Developer Command Prompt):

```powershell
mkdir build; cd build
cmake ..
cmake --build . --config Release
.\Release\loom_test.exe
```

Notes:
- Metal backend code is in `kernel/metal/loom_tensor_metal.mm` (Objective-C++). It is a skeleton and will be further implemented on macOS.
- CUDA support is planned and will be enabled via `-DLOOM_USE_CUDA=ON` once CUDA code and toolchain are added.
- If you use a non-Microsoft toolchain on Windows (MinGW), point CMake to the correct generator or use WSL.
