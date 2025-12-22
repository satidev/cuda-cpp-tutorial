#pragma once

#include <stdexcept>
#include <string>

inline void checkError(const cudaError_t status, const std::string& func)
{
    if (status != cudaSuccess)
    {
        throw std::runtime_error{
            func + " failed. " + std::to_string(status) + ": " + cudaGetErrorString(status)
        };
    }
}
