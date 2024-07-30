#include "Utils/FileUtils.h"
#include <fstream>
#include <string>
#include <sstream>
#include <spdlog/spdlog.h>


namespace FileUtils {

    bool readFileToString(const std::string& filePath, std::string& content)
    {
        std::ifstream file(filePath, std::ios::in);

        if (!file.is_open()) {
            spdlog::error("Couldn't open the file. File path: {}", filePath);
            return false;
        }

        auto buffer = std::ostringstream{};
        buffer << file.rdbuf();

        if (file.fail()) {
            spdlog::error("Error reading the file. File path: {}", filePath);
            return false;
        }

        content = buffer.str();
        return true;
    }

}