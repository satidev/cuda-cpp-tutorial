cmake_minimum_required(VERSION 3.24)

set(CMAKE_CUDA_ARCHITECTURES 86)

project(cudacpp LANGUAGES CXX CUDA)

set(CMAKE_CUDA_STANDARD 20)

add_executable(cudacpp main.cu
        examples_cudac.h
        util.h
        examples_cudac_bugs.h
        examples_cudac_stream.h
        arrays.h
        stream.h
        examples_cudacpp.h
)

# CPU-side flags (g++)
target_compile_options(cudacpp PRIVATE
        -g
        -fno-omit-frame-pointer
)

# CUDA-side flags (nvcc)
target_compile_options(cudacpp PRIVATE
        $<$<COMPILE_LANGUAGE:CUDA>:-g>
        $<$<COMPILE_LANGUAGE:CUDA>:-lineinfo>
        $<$<COMPILE_LANGUAGE:CUDA>:--compiler-options=-fno-omit-frame-pointer>
)


