#ifndef CUDACPP_STREAM_H
#define CUDACPP_STREAM_H

#include "util.h"

class Stream
{
    cudaStream_t stream;

  public:
    Stream()
    {
        checkError(cudaStreamCreate(&stream), "cudaStreamCreate");
    }

    ~Stream()
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
