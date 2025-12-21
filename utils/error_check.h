#ifndef CUDACPP_UTIL_H
#define CUDACPP_UTIL_H

#include <stdexcept>
#include <string>

inline void checkError(const cudaError_t status, const std::string &func)
{
    if (status != cudaSuccess)
    {
        throw std::runtime_error{func + " failed. " + std::to_string(status) + ": " + cudaGetErrorString(status)};
    }
}

#endif // CUDACPP_UTIL_H
