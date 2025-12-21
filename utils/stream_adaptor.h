#ifndef CUDACPP_STREAM_H
#define CUDACPP_STREAM_H

#include "error_check.h"

class StreamAdaptor
{
    cudaStream_t stream;

  public:
    StreamAdaptor()
    {
        checkError(cudaStreamCreate(&stream), "cudaStreamCreate");
    }

    ~StreamAdaptor()
    {
        checkError(cudaStreamDestroy(stream), "cudaStreamDestroy");
    }

    void sync()
    {
        checkError(cudaStreamSynchronize(stream), "cudaStreamSynchronize");
    }

    [[nodiscard]] cudaStream_t getStream() const
    {
        return stream;
    }
};

#endif // CUDACPP_STREAM_H
