#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <optional>

namespace common
{
    std::optional<std::vector<std::string>> read_file(const std::string_view file_path) noexcept
    {
        std::ifstream file = std::ifstream(std::string(file_path));
        if (!file.is_open())
        {
            std::cerr << "Could not open file with path : " << file_path << '\n';
            return std::nullopt;
        }

        std::cout << "Successfully read file with path : " << file_path << '\n';
        
        std::vector<std::string> file_contents{};

        {
            std::string file_buffer{};
            while (file >> file_buffer)
            {
                file_contents.emplace_back(file_buffer);
            }
        }

        return file_contents;
    }
}