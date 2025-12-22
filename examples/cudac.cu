#include "../utils/error_check.h"

#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>

namespace Examples::CUDAC
{
    void allocateInGpu()
    {
        float* gpu_array{nullptr};
        constexpr size_t num_elements{1u};
        constexpr size_t array_size_bytes{num_elements * sizeof(std::byte)};

        const cudaError_t status{cudaMalloc(reinterpret_cast<void**>(&gpu_array), array_size_bytes)};
        if (status != cudaSuccess)
        {
            throw std::runtime_error{"cudaMalloc failed-" + std::to_string(status) + ": " + cudaGetErrorString(status)};
        }

        // Free the resource.
        cudaFree(gpu_array);
    }

    void transferDataPageable()
    {
        // Allocate input and output host arrays.
        constexpr size_t num_elements{1024u * 1024u};
        constexpr size_t array_size_bytes{num_elements * sizeof(float)};
        std::vector<float> host_array_input(num_elements);
        std::iota(std::begin(host_array_input), std::end(host_array_input), 0.0f);
        std::vector<float> host_array_output(num_elements);

        // Allocate array in GPU.
        float* gpu_array{nullptr};
        cudaError_t status{cudaMalloc(reinterpret_cast<void**>(&gpu_array), array_size_bytes)};
        if (status != cudaSuccess)
        {
            throw std::runtime_error{"cudaMalloc failed " + std::to_string(status) + ": " + cudaGetErrorString(status)};
        }

        // Transfer data from the host to the GPU.
        status = cudaMemcpy(gpu_array, host_array_input.data(), array_size_bytes, cudaMemcpyHostToDevice);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{
                "cudaMemcpyHostToDevice failed-" + std::to_string(status) + ": " +
                cudaGetErrorString(status)
            };
        }

        // Transfer the data from the device to the host.
        status = cudaMemcpy(host_array_output.data(), gpu_array, array_size_bytes, cudaMemcpyDeviceToHost);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{
                "cudaMemcpyDeviceToHost failed " + std::to_string(status) + ": " +
                cudaGetErrorString(status)
            };
        }

        if (std::ranges::equal(host_array_input, host_array_output))
        {
            std::cout << "Data transfer is finished successfully." << std::endl;
        }
        else
        {
            std::cerr << "Error during data transfer." << std::endl;
        }

        // Free the resource.
        cudaFree(gpu_array);
    }

    void transferDataAsyncPinnedDefaultStream()
    {
        // Allocate input and output host arrays.
        constexpr size_t num_elements{1024u * 1024u};
        constexpr size_t array_size_bytes{num_elements * sizeof(float)};
        float* host_array_input{nullptr};
        cudaError_t status{
            cudaHostAlloc(reinterpret_cast<void**>(&host_array_input), array_size_bytes, cudaHostAllocDefault)
        };
        std::iota(host_array_input, host_array_input + num_elements, 0.0f);
        float* host_array_output{nullptr};
        status = cudaHostAlloc(reinterpret_cast<void**>(&host_array_output), array_size_bytes, cudaHostAllocDefault);

        // Allocate array in the GPU.
        float* gpu_array{nullptr};
        status = cudaMalloc(reinterpret_cast<void**>(&gpu_array), array_size_bytes);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{"cudaMalloc failed " + std::to_string(status) + ": " + cudaGetErrorString(status)};
        }

        // Transfer data from the pinned host to the GPU.
        status = cudaMemcpyAsync(gpu_array, host_array_input, array_size_bytes, cudaMemcpyHostToDevice);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{
                "cudaMemcpyHostToDevice failed " + std::to_string(status) + ": " +
                cudaGetErrorString(status)
            };
        }

        // Transfer data from the GPU to the pinned host for output.
        status = cudaMemcpyAsync(host_array_output, gpu_array, array_size_bytes, cudaMemcpyDeviceToHost);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{
                "cudaMemcpyDeviceToHost failed " + std::to_string(status) + ": " +
                cudaGetErrorString(status)
            };
        }

        // Block all works in host until all the previous scheduled operations in the GPU are finished.
        cudaDeviceSynchronize();

        if (std::equal(host_array_input, host_array_input + num_elements, host_array_output))
        {
            std::cout << "Data transfer is finished successfully." << std::endl;
        }
        else
        {
            std::cerr << "Error during data transfer." << std::endl;
        }

        // Free the resources.
        cudaFreeHost(host_array_input);
        cudaFreeHost(host_array_output);
        cudaFree(gpu_array);
    }

    void transferDataAsyncPinnedDedicatedStream()
    {
        // Allocate input and output host arrays.
        constexpr size_t num_elements{1024u * 1024u};
        constexpr size_t array_size_bytes{num_elements * sizeof(float)};
        float* host_array_input{nullptr};
        cudaError_t status{
            cudaHostAlloc(reinterpret_cast<void**>(&host_array_input), array_size_bytes, cudaHostAllocDefault)
        };
        if (status != cudaSuccess)
        {
            throw std::runtime_error{
                "cudaHostAlloc failed " + std::to_string(status) + ": " + cudaGetErrorString(status)
            };
        }

        std::iota(host_array_input, host_array_input + num_elements, 0.0f);
        float* host_array_output{nullptr};
        status = cudaHostAlloc(reinterpret_cast<void**>(&host_array_output), array_size_bytes, cudaHostAllocDefault);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{
                "cudaHostAlloc failed " + std::to_string(status) + ": " + cudaGetErrorString(status)
            };
        }

        // Allocate array in the GPU.
        float* gpu_array{nullptr};
        status = cudaMalloc(reinterpret_cast<void**>(&gpu_array), array_size_bytes);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{"cudaMalloc failed " + std::to_string(status) + ": " + cudaGetErrorString(status)};
        }

        cudaStream_t stream;
        status = cudaStreamCreate(&stream);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{
                "cudaStreamCreate failed " + std::to_string(status) + ": " +
                cudaGetErrorString(status)
            };
        }

        // Transfer data from the pinned host to the GPU.
        status = cudaMemcpyAsync(gpu_array, host_array_input, array_size_bytes, cudaMemcpyDefault, stream);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{
                "cudaMemcpyHostToDevice failed " + std::to_string(status) + ": " +
                cudaGetErrorString(status)
            };
        }

        // Transfer data from the GPU to the pinned host for output.
        status = cudaMemcpyAsync(host_array_output, gpu_array, array_size_bytes, cudaMemcpyDeviceToHost, stream);
        if (status != cudaSuccess)
        {
            throw std::runtime_error{
                "cudaMemcpyDeviceToHost failed " + std::to_string(status) + ": " +
                cudaGetErrorString(status)
            };
        }

        cudaStreamSynchronize(stream);

        if (std::equal(host_array_input, host_array_input + num_elements, host_array_output))
        {
            std::cout << "Data transfer is finished successfully." << std::endl;
        }
        else
        {
            std::cerr << "Error during data transfer." << std::endl;
        }

        // Free the resources.
        cudaFreeHost(host_array_input);
        cudaFreeHost(host_array_output);
        cudaFree(gpu_array);
        cudaStreamDestroy(stream);
    }
} // namespace Examples::CUDAC