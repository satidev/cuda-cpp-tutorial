#include "examples_cudac.h"
#include "examples_cudac_bugs.h"
#include "examples_cudac_stream.h"
#include "examples_cudacpp.h"
#include <chrono>
#include <iostream>

int main()
{
    try
    {
        const auto start = std::chrono::high_resolution_clock::now();
        Examples::CUDAC::singleStream();
        const auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = end - start;
        std::cout << "Time taken : " << ms.count() << " ms\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught exception. " << std::endl;
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
