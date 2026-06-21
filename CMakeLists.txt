cmake_minimum_required(VERSION 3.24)

# ---------------------------------------------------------------------------
# GPU architecture selection
#
# Defaults to "native", which auto-detects the GPU(s) present on the build
# machine (requires CMake 3.24+ and CUDA 11.6+).
#
# To target a specific architecture (or a comma-separated list), pass it on
# the cmake command line:
#
#   cmake -DCMAKE_CUDA_ARCHITECTURES="75;86;89;90" ..
#
# Supported values for CUDA 13.3 (Turing and newer):
#   75  – Turing    (RTX 20xx, T4)
#   80  – Ampere    (A100, RTX 30xx)
#   86  – Ampere    (RTX 30xx laptop / desktop)
#   87  – Ampere    (Jetson Orin)
#   89  – Ada Lovelace (RTX 40xx)
#   90  – Hopper    (H100)
#   100 – Blackwell (RTX 50xx / B100)
# ---------------------------------------------------------------------------
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES native)
endif()

project(cudacpp LANGUAGES CXX CUDA)

# ---------------------------------------------------------------------------
# Language standards
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/dlib)

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------
add_executable(cudacpp
        main.cu
        examples/cudac.h
        examples/cudac_bugs.h
        examples/cudac_stream.h
        utils/arrays.h
        utils/stream_adaptor.h
        examples/cudacpp.h
        examples/examples.h
        examples/cudac.cu
        examples/cudac_bugs.cu
        examples/cudacpp.cu
)

# ---------------------------------------------------------------------------
# Compile options
# ---------------------------------------------------------------------------

# Compile flags – routed per language via generator expressions
target_compile_options(cudacpp PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-g>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-omit-frame-pointer>
        $<$<COMPILE_LANGUAGE:CUDA>:-g>
        $<$<COMPILE_LANGUAGE:CUDA>:-lineinfo>
        $<$<COMPILE_LANGUAGE:CUDA>:--compiler-options=-fno-omit-frame-pointer>
)

# ---------------------------------------------------------------------------
# Include directories and linking
# ---------------------------------------------------------------------------
target_include_directories(cudacpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_link_libraries(cudacpp dlib)