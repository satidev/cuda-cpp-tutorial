cmake_minimum_required(VERSION 3.24)

set(CMAKE_CUDA_ARCHITECTURES 86)

project(cudacpp LANGUAGES CXX CUDA)

set(CMAKE_CUDA_STANDARD 20)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/dlib)

add_executable(cudacpp main.cu
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

target_include_directories(cudacpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
target_link_libraries(cudacpp dlib)


