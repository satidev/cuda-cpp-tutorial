#ifndef CUDACPP_EXAMPLES_CUDAC_STREAM_H
#define CUDACPP_EXAMPLES_CUDAC_STREAM_H

#include "arrays.h"
#include "stream.h"

#include <algorithm>
#include <numeric>

namespace Examples::CUDAC
{
struct StreamExamples
{
    static constexpr size_t array_size{1024u * 1024u};
    HostArray<float> input{array_size};
    HostArray<float> output{array_size};
    GpuArray<float> gpu{array_size};

    StreamExamples()
    {
        std::iota(input.begin(), input.end(), 0.0f);
    }
    [[nodiscard]] bool checkResult() const
    {
        return std::ranges::equal(input, output);
    }
};

inline void singleStream()
{
    StreamExamples obj;
    Stream stream{};
    copy(obj.input, obj.gpu, stream);
    copy(obj.gpu, obj.output, stream);
    stream.sync();
    if (obj.checkResult())
    {
        std::cout << "Data transfer is finished successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error during data transfer." << std::endl;
    }
}
} // namespace Examples::CUDAC

#endif // CUDACPP_EXAMPLES_CUDAC_STREAM_H
