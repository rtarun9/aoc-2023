#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <sstream>
#include <optional>

namespace common
{
    // Returns vector of strings where a space differentiates between two entries.
    std::optional<std::vector<std::string>> read_file_by_word(const std::string_view file_path) noexcept
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

    // Returns vector of strings where a newline (\n) differentiates between two entries.
    std::optional<std::vector<std::string>> read_file_by_line(const std::string_view file_path) noexcept
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
            while (std::getline(file, file_buffer))
            {
                file_contents.emplace_back(file_buffer);
            }
        }

        return file_contents;
    }

    std::vector<std::string> split_string_by_space(const std::string_view input) noexcept
    {
        std::vector<std::string> result{};
        std::stringstream ss{std::string(input)};

        {
            std::string temp_buffer{};
            while (ss >> temp_buffer)
            {
                result.emplace_back(temp_buffer);
            }
        }

        return result;
    }
}