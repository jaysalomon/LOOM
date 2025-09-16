cmake_minimum_required(VERSION 3.18)
project(loom_kernel C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(LOOM_USE_METAL "Build with Metal backend" OFF)
option(LOOM_USE_CUDA "Build with CUDA backend" OFF)

set(KERNEL_SRC
    loom_kernel.c
    loom_tensor.c
    loom_tensor_metal.c
    loom_tensor_cuda.c
)

if(LOOM_USE_METAL)
    list(APPEND KERNEL_SRC metal/loom_tensor_metal.mm)
    add_definitions(-DLOOM_USE_METAL)
endif()

if(LOOM_USE_CUDA)
    # Note: CUDA support requires enabling CUDA language in top-level CMake
    add_definitions(-DLOOM_USE_CUDA)
endif()

add_library(loom_kernel STATIC ${KERNEL_SRC})
target_include_directories(loom_kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(loom_test ${CMAKE_CURRENT_SOURCE_DIR}/test_harness.c)
target_link_libraries(loom_test PRIVATE loom_kernel)

install(TARGETS loom_kernel loom_test DESTINATION bin)
