cmake_minimum_required(VERSION 3.18)
project(loom_project C CXX)

# Options for backends
option(LOOM_USE_METAL "Enable Metal backend (macOS only)" OFF)
option(LOOM_USE_CUDA "Enable CUDA backend" OFF)

add_subdirectory(kernel)
