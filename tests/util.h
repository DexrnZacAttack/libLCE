//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#pragma once
#include <chrono>
#include <filesystem>
#include <iostream>

namespace lce::tests::util {
    const std::filesystem::path examples = std::filesystem::weakly_canonical("../../tests/examples");
    const std::filesystem::path output = std::filesystem::weakly_canonical("../../tests/output");

    template <class... Args>
    void runTest(void (*test)(Args...), const std::string name, Args... args) {
        // TODO: this counts ifstream r/w time as well, which isn't a good thing
        std::cout << "Running test \"" << name << "\"" << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();

        try {
            // reset clock since inside try block
            startTime = std::chrono::high_resolution_clock::now();

            test(args...);

            const std::chrono::duration<double, std::milli> duration =
            std::chrono::high_resolution_clock::now() - startTime;

            std::cout << "\"" << name << "\" finished after " << duration.count() << "ms" << std::endl;
        } catch (const std::exception &e) {
            const std::chrono::duration<double, std::milli> duration =
            std::chrono::high_resolution_clock::now() - startTime;

            std::cerr << "\"" << name << "\" failed after " << duration.count() << "ms" << " due to " << e.what() << std::endl;
        }
    }
} // namespace lce::tests::util