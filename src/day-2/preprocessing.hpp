#pragma once

#include <string>
#include <vector>

namespace preprocessing
{
    // Performs preprocessing for a single "Game" (i.e line in the input file).
    // It removes any occurrence of ':', which convertes words like 2: into 2.
    // It removes any occurrence of ',', which converts words like red, blue, into red blue etc.
    // It removes any occurrence of ; and adds it as a separate character. I.E, it converts red; blue; into red ; blue ;
    // It additionally also adds a ; at the end of each input to indicate to the algorithm that the final set is completed.
    std::vector<std::string> preprocess_game_input(const std::string &input)
    {
        std::vector<std::string> processed_line{};

        auto line = common::split_string_by_space(input);

        for (auto word : line)
        {
            if (word.back() == ',' || word.back() == ':')
            {
                word.pop_back();
                processed_line.emplace_back(word);
            }
            else if (word.back() == ';')
            {
                word.pop_back();
                processed_line.push_back(word);
                processed_line.emplace_back(";");
            }
            else
            {
                processed_line.emplace_back(word);
            }
        }

        processed_line.push_back(";");

        return processed_line;
    }
}
