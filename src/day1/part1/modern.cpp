#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

int main(int argc, char **argv)
{
    std::ifstream input_file("../../data/day_1_input.txt");
    if (!input_file.is_open())
    {
        std::cerr << "Failed to open day 1 input file.";
        std::exit(-1);
    }

    int32_t result{};

    std::string line_wise_file_content;
    std::vector<std::string> file_contents{};

    while (input_file >> line_wise_file_content) 
    {
        file_contents.emplace_back(line_wise_file_content);
    }

    const auto is_digit = [&](const char c) 
    {
        return c >= '0' && c <= '9';
    };

    std::for_each(file_contents.begin(), file_contents.end(), [&](const auto str)
    {
        if (auto it = std::find_if(str.begin(), str.end(), is_digit); it != str.end())
        {
            result += (*it - '0') * 10;
        }

        if (auto it = std::find_if(str.rbegin(), str.rend(), is_digit); it != str.rend())
        {
            result += (*it - '0');
        }
    });

    std::cout << "The final result : " << result;
}