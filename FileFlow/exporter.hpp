#ifndef FILEFLOW_EXPORTER_HPP
#define FILEFLOW_EXPORTER_HPP

#include <string>
#include <fstream>
#include <vector>

namespace fileflow {
    class Exporter {
    public:
        // Template to handle char, int, double, etc.
        template<typename T>
        static void to_file(const std::string& path, const T& data) {
            std::ofstream file(path);
            if (file.is_open()) {
                file << data;
            }
        }

        // Specifically for raw byte buffers
        static void export_bytes(const std::string& path, const std::vector<char>& buffer) {
            std::ofstream file(path, std::ios::binary);
            file.write(buffer.data(), buffer.size());
        }
    };
}

#endif
