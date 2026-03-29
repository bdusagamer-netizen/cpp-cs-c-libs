#ifndef FILEFLOW_LOADER_HPP
#define FILEFLOW_LOADER_HPP

#include <string>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <vector>

namespace fileflow {
    class Loader {
    public:
        // Loads a file relative to the executable's current working directory
        static std::string load_raw(const std::string& relative_path) {
            std::filesystem::path p = std::filesystem::current_path() / relative_path;
            std::ifstream file(p, std::ios::binary);
            
            if (!file.is_open()) {
                throw std::runtime_error("Loader: Could not find file at " + p.string());
            }

            return std::string((std::istreambuf_iterator<char>(file)), 
                                std::istreambuf_iterator<char>());
        }
    };
}

#endif
