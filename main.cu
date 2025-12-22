#include "examples/examples.h"

#include <dlib/cmd_line_parser.h>
#include <chrono>
#include <iostream>
#include <map>
#include <ranges>

std::map<std::string, std::function<void()>> ACTIONS{
    {"all", nullptr},
    {"c-alloc-gpu", Examples::CUDAC::allocateInGpu},
    {"c-copy-sync", Examples::CUDAC::transferDataPageable},
    {"c-copy-async", Examples::CUDAC::transferDataAsyncPinnedDefaultStream},
    {"c-copy-async-stream", Examples::CUDAC::transferDataAsyncPinnedDedicatedStream},
    {"cpp-alloc-gpu", Examples::CUDACPP::allocateInGpu},
    {"cpp-copy-sync", Examples::CUDACPP::transferDataPageable},
    {"cpp-copy-async", Examples::CUDACPP::transferDataAsyncPinnedDefaultStream},
    {"cpp-copy-async-stream", Examples::CUDACPP::transferDataAsyncPinnedDedicatedStream}
};


int main(int argc, char** argv)
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: ./cudacpp -op <operation_name>" << std::endl;
            return 1;
        }

        dlib::command_line_parser parser{};
        parser.add_option("h", "Display this message.");
        parser.add_option("a", "Action/operation.", 1);
        parser.parse(argc, argv);

        if (parser.option("h"))
        {
            std::cout << "Usage: ./cudacpp -a <operation_name>" << std::endl;
            return 0;
        }

        const std::string action_str{parser.option("a").argument()};

        if (ACTIONS.contains(action_str))
        {
            const auto start = std::chrono::high_resolution_clock::now();
            if (const auto func = ACTIONS.at(action_str))
            {
                func();
            }
            else
            {
                for (const auto& funcs : ACTIONS | std::views::values)
                {
                    if (funcs)
                    {
                        funcs();
                    }
                }
            }
            const auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms = end - start;
            std::cout << "Time taken : " << ms.count() << " ms\n";
        }
        else
        {
            std::cerr << "Invalid action:: " << action_str << '\n';
            std::cerr << "Valid actions are:" << '\n';
            for (const auto& key : ACTIONS | std::views::keys)
            {
                std::cout << key << '\n';
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Caught exception. " << std::endl;
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
