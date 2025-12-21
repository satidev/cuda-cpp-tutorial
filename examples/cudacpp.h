#ifndef CUDACPP_EXAMPLES_CUDACPP_H
#define CUDACPP_EXAMPLES_CUDACPP_H

#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/sequence.h>
#include <thrust/universal_vector.h>

namespace Examples::CUDACPP
{
inline void allocationInGpu()
{
    constexpr size_t num_elements{1u};
    thrust::device_vector<float> gpu_array(num_elements);
}

inline void dataTransferPageable()
{
    // Allocate input and output host arrays.
    constexpr size_t num_elements{1024u * 1024u};
    thrust::host_vector<float> host_array_input(num_elements);
    // Generate numbers from 0, 1, ..., num_elements -1
    thrust::sequence(host_array_input.begin(), host_array_input.end(), 0.0f);
    thrust::host_vector<float> host_array_output(num_elements);

    // Allocate array in GPU.
    thrust::device_vector<float> gpu_array(num_elements);

    // Transfer data from the host to the GPU.
    thrust::copy(host_array_input.begin(), host_array_input.end(), gpu_array.begin());

    // Transfer data from the GPU to the host.
    thrust::copy(gpu_array.begin(), gpu_array.end(), host_array_output.begin());

    if (std::equal(host_array_input.begin(), host_array_input.end(), host_array_output.begin()))
    {
        std::cout << "Data transfer is finished successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error during data transfer." << std::endl;
    }
}

inline void dataTransferPinned()
{
    // Allocate input and output host arrays.
    constexpr size_t num_elements{1024u * 1024u};
    thrust::universal_host_pinned_vector<float> host_array_input(num_elements);
    // Generate numbers from 0, 1, ..., num_elements -1
    thrust::sequence(host_array_input.begin(), host_array_input.end(), 0.0f);
    thrust::universal_host_pinned_vector<float> host_array_output(num_elements);

    // Allocate array in GPU.
    thrust::device_vector<float> gpu_array(num_elements);

    // Transfer data from the host to the GPU.
    auto status =
        cudaMemcpyAsync(thrust::raw_pointer_cast(gpu_array.data()), thrust::raw_pointer_cast(host_array_input.data()),
                        num_elements * sizeof(float), cudaMemcpyDefault);
    if (status != cudaSuccess)
    {
        throw std::runtime_error{"cudaMemcpyAsync failed " + std::to_string(status) + ": " +
                                 cudaGetErrorString(status)};
    }

    // Transfer data from the GPU to the host.
    status =
        cudaMemcpyAsync(thrust::raw_pointer_cast(host_array_output.data()), thrust::raw_pointer_cast(gpu_array.data()),
                        num_elements * sizeof(float), cudaMemcpyDefault);
    if (status != cudaSuccess)
    {
        throw std::runtime_error{"cudaMemcpyAsync failed " + std::to_string(status) + ": " +
                                 cudaGetErrorString(status)};
    }

    cudaDeviceSynchronize();

    if (std::equal(host_array_input.begin(), host_array_input.end(), host_array_output.begin()))
    {
        std::cout << "Data transfer is finished successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error during data transfer." << std::endl;
    }
}

inline void dataTransferPinnedStream()
{
    // Allocate input and output host arrays.
    constexpr size_t num_elements{1024u * 1024u};
    thrust::universal_host_pinned_vector<float> host_array_input(num_elements);
    // Generate numbers from 0, 1, ..., num_elements -1
    thrust::sequence(host_array_input.begin(), host_array_input.end(), 0.0f);
    thrust::universal_host_pinned_vector<float> host_array_output(num_elements);

    // Allocate array in GPU.
    thrust::device_vector<float> gpu_array(num_elements);

    cudaStream_t stream;
    cudaStreamCreate(&stream);

    // Transfer data from the host to the GPU.
    auto status =
        cudaMemcpyAsync(thrust::raw_pointer_cast(gpu_array.data()), thrust::raw_pointer_cast(host_array_input.data()),
                        num_elements * sizeof(float), cudaMemcpyDefault, stream);
    if (status != cudaSuccess)
    {
        throw std::runtime_error{"cudaMemcpyAsync failed " + std::to_string(status) + ": " +
                                 cudaGetErrorString(status)};
    }

    // Transfer data from the GPU to the host.
    status =
        cudaMemcpyAsync(thrust::raw_pointer_cast(host_array_output.data()), thrust::raw_pointer_cast(gpu_array.data()),
                        num_elements * sizeof(float), cudaMemcpyDefault, stream);
    if (status != cudaSuccess)
    {
        throw std::runtime_error{"cudaMemcpyAsync failed " + std::to_string(status) + ": " +
                                 cudaGetErrorString(status)};
    }

    cudaStreamSynchronize(stream);
    cudaStreamDestroy(stream);
    if (std::equal(host_array_input.begin(), host_array_input.end(), host_array_output.begin()))
    {
        std::cout << "Data transfer is finished successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error during data transfer." << std::endl;
    }
}
} // namespace Examples::CUDACPP

#endif // CUDACPP_EXAMPLES_CUDACPP_H
