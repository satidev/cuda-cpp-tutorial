#ifndef CUDACPP_ARRAYS_H
#define CUDACPP_ARRAYS_H

#include "stream.h"

#include <optional>

template <typename ElemType> class Array
{
  protected:
    ElemType *ptr_{nullptr};
    size_t num_elements_{0u};

  public:
    virtual ~Array() = 0;

    [[nodiscard]] size_t getNumElements() const
    {
        return num_elements_;
    }
    [[nodiscard]] size_t getNumBytes() const
    {
        return num_elements_ * sizeof(ElemType);
    }
    ElemType *getPointer() const
    {
        return ptr_;
    }
    ElemType *getPointer()
    {
        return ptr_;
    }

    // Iterators
    ElemType *begin()
    {
        return ptr_;
    }
    ElemType *end()
    {
        return ptr_ + num_elements_;
    }

    const ElemType *begin() const
    {
        return ptr_;
    }
    const ElemType *end() const
    {
        return ptr_ + num_elements_;
    }

    const ElemType *cbegin() const
    {
        return ptr_;
    }
    const ElemType *cend() const
    {
        return ptr_ + num_elements_;
    }
};

template <typename ElemType> Array<ElemType>::~Array() = default;

template <typename ElemType> class HostArray : public Array<ElemType>
{
  public:
    HostArray() = default;

    explicit HostArray(size_t num_elements);

    ~HostArray() override;
};

template <typename ElemType> class GpuArray : public Array<ElemType>
{
  public:
    GpuArray() = default;

    explicit GpuArray(size_t num_elements);

    ~GpuArray() override;
};

template <typename ElemType> HostArray<ElemType>::HostArray(size_t num_elements)
{
    this->num_elements_ = num_elements;
    checkError(cudaHostAlloc(reinterpret_cast<void **>(&this->ptr_), this->getNumBytes(), cudaHostAllocDefault),
               "cudaHostAlloc");
}

template <typename ElemType> HostArray<ElemType>::~HostArray()
{
    if (this->ptr_)
    {
        checkError(cudaFreeHost(this->ptr_), "cudaFreeHost");
    }
}

template <typename ElemType> GpuArray<ElemType>::GpuArray(size_t num_elements)
{
    this->num_elements_ = num_elements;
    checkError(cudaMalloc(reinterpret_cast<void **>(&this->ptr_), this->getNumBytes()), "cudaMalloc");
}

template <typename ElemType> GpuArray<ElemType>::~GpuArray()
{
    if (this->ptr_)
    {
        checkError(cudaFree(this->ptr_), "cudaFree");
    }
}

template <typename ElemType>
void copy(const Array<ElemType> &src, Array<ElemType> &dst, Stream &stream,
          std::optional<size_t> num_elements_copy = std::nullopt, size_t num_elements_skip_src = 0,
          size_t num_elements_skip_dst = 0)
{
    const size_t transfer_size{num_elements_copy.has_value() ? num_elements_copy.value() : src.getNumElements()};
    if (transfer_size + num_elements_skip_src > src.getNumElements())
    {
        throw std::invalid_argument{"Source array is not large enough for the requested transfer."};
    }
    if (transfer_size + num_elements_skip_dst > dst.getNumElements())
    {
        throw std::invalid_argument{"Destination array is not large enough for the requested transfer."};
    }
    checkError(
        cudaMemcpyAsync(dst.begin(), src.begin(), transfer_size * sizeof(float), cudaMemcpyDefault, stream.getStream()),
        "cudaMemcpyAsync");
}
#endif // CUDACPP_ARRAYS_H
