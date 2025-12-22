#pragma once

namespace Examples::CUDACPP
{
    // Allocates 1 floating number in GPU and free the memory at the end.
    void allocateInGpu();

    // Allocates an array of floating point numbers in pageable memory and do the host-device and device-host transfers.
    // Synchronous data transfer where the host operations are blocked during transfer.
    void transferDataPageable();

    // Allocates a floating point array in pinned memory and do the host-device and device-host transfers.
    // Default stream is used for asynchronous data transfer.
    void transferDataAsyncPinnedDefaultStream();

    // Allocates a floating point array in pinned memory and do the host-device and device-host transfers.
    // Dedicated stream is used for asynchronous data transfer.
    void transferDataAsyncPinnedDedicatedStream();
} // namespace Examples::CUDACPP
