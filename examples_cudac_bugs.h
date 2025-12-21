#ifndef CUDACPP_EXAMPLES_CUDAC_BUGS_H
#define CUDACPP_EXAMPLES_CUDAC_BUGS_H

#include "util.h"

#include <vector>

namespace Bugs::CUDAC
{
inline void allocationInGpu()
{
    float *gpu_array{nullptr};
    constexpr size_t array_size{1u};
    constexpr size_t array_size_bytes{array_size * sizeof(std::byte)};

    const cudaError_t status{cudaMalloc(reinterpret_cast<void **>(&gpu_array), array_size_bytes)};
    if (status != cudaSuccess)
    {
        throw std::runtime_error{"cudaMalloc failed-" + std::to_string(status) + ": " + cudaGetErrorString(status)};
    }
}
} // namespace Bugs::CUDAC

#endif // CUDACPP_EXAMPLES_CUDAC_BUGS_H
