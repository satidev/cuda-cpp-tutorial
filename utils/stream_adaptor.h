#pragma once

#include "error_check.h"

class StreamAdaptor
{
    cudaStream_t stream;

public:
    StreamAdaptor() { checkError(cudaStreamCreate(&stream), "cudaStreamCreate"); }

    ~StreamAdaptor()
    {
        checkError(cudaStreamDestroy(stream), "cudaStreamDestroy");
    }

    void sync()
    {
        checkError(cudaStreamSynchronize(stream), "cudaStreamSynchronize");
    }

    [[nodiscard]] cudaStream_t getStream() const { return stream; }
};
