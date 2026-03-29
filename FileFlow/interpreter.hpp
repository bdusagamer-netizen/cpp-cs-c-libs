#ifndef FILEFLOW_INTERPRETER_HPP
#define FILEFLOW_INTERPRETER_HPP

#include "loader.hpp"
#include <functional>
#include <map>

namespace fileflow {
    
    class Interpreter {
    public:
        // The core logic block that receives a "query-able" data map
        using Logic = std::function<void(const std::map<std::string, std::string>&)>;

        static void interpret(const std::string& filename, Logic logic) {
            std::string content = Loader::load_raw(filename);

            // Mock "JSON" parser: In a real scenario, integrate nlohmann/json here.
            // For now, we simulate finding the "save/save-code" key.
            std::map<std::string, std::string> data;
            
            if (content.find("Something") != std::string::npos) {
                data["save/save-code"] = "Something";
            }

            // Execute the user's logic
            logic(data);
        }
    };
}

#endif
