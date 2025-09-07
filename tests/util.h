//
// Created by DexrnZacAttack on 9/3/25 using zPc-i2.
//
#pragma once
#include <chrono>
#include <filesystem>
#include <iostream>
#include <utility>

namespace lce::tests::util {
#define _OPEN_FILE(path, out, name)                                            \
    std::ifstream name(util::examples / path, std::ifstream::binary);          \
                                                                               \
    if (!name.is_open())                                                       \
        throw std::ios_base::failure(std::string("Failed to open file ") +     \
                                     (util::examples / path).string());        \
                                                                               \
    std::vector<uint8_t> out(                                                  \
        std::filesystem::file_size(util::examples / path));                    \
    name.read(reinterpret_cast<char *>(out.data()), out.size())

#define _WRITE_FILE(path, data, size, name)                                    \
    std::ofstream name(util::output / path, std::ios::binary);                 \
                                                                               \
    if (!name)                                                                 \
        throw std::ios_base::failure("Failed to open file");                   \
                                                                               \
    name.write(data, size);                                                    \
                                                                               \
    if (!name)                                                                 \
        throw std::ios_base::failure("Failed to write");                       \
                                                                               \
    name.close()

#define OPEN_FILE(path, out) _OPEN_FILE(path, out, in)
#define WRITE_FILE(path, data, size) _WRITE_FILE(path, data, size, out)

#define ADD_TEST(testName, ...)                                                \
    if (testName)                                                              \
    lce::tests::util::runTest(__VA_ARGS__)

#define ADD_TESTS(testName, func, ...)                                         \
    if (testName)                                                              \
    func(__VA_ARGS__)

    const std::filesystem::path examples =
        std::filesystem::weakly_canonical("../../tests/examples");
    const std::filesystem::path output =
        std::filesystem::weakly_canonical("../../tests/output");

    namespace Types {
        static constexpr const char *const FORMATS = "Formats";
        static constexpr const char *const VFS = "VFS";
    } // namespace Types

    template <class... Args>
    void runTest(void (*test)(Args...), const char *const type,
                 const std::string &name, Args... args) {
        // TODO: this counts ifstream r/w time as well, which isn't a good thing
        std::cout << "[" << type << "] Running test \"" << name << "\""
                  << std::endl;

        auto startTime = std::chrono::high_resolution_clock::now();

        try {
            // reset clock since inside try block
            startTime = std::chrono::high_resolution_clock::now();

            test(args...);

            const std::chrono::duration<double, std::milli> duration =
                std::chrono::high_resolution_clock::now() - startTime;

            std::cout << "[" << type << "] \"" << name << "\" finished after "
                      << duration.count() << "ms" << std::endl;
        } catch (const std::exception &e) {
            const std::chrono::duration<double, std::milli> duration =
                std::chrono::high_resolution_clock::now() - startTime;

            std::cerr << "[" << type << "] \"" << name << "\" failed after "
                      << duration.count() << "ms" << " due to " << e.what()
                      << std::endl;
        }
    }
} // namespace lce::tests::util