#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <span>

namespace preprocessing
{
    namespace internal
    {
        std::vector<std::string> split_by_space(const std::string_view input)
        {
            std::vector<std::string> output{};
            std::stringstream ss(input.data());

            {
                std::string temp{};
                while (ss >> temp)
                {
                    output.emplace_back(temp);
                }
            }

            return output;
        }
    }

    struct CardData
    {
        int card_id{};
        std::vector<int> winning_numbers{};
        std::vector<int> card_numbers{};
    };

    // The function returns a vector of CardData struct's.
    std::vector<CardData> preprocess_input(const std::span<std::string const> input)
    {
        std::vector<CardData> card_datas{};
        for (const auto &line : input)
        {
            // Split the line by space.
            const auto line_split_by_space = internal::split_by_space(line);

            // Example input : Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
            CardData data = {};
            auto card_id = line_split_by_space.at(1);
            card_id.pop_back();
            data.card_id = stoi(card_id);

            // card_divider_found is set to true when a | is encountered.
            bool card_divider_found{false};
            for (int i = 2; i < line_split_by_space.size(); i++)
            {
                if (line_split_by_space.at(i) == "|")
                {
                    card_divider_found = true;
                    continue;
                }

                if (card_divider_found)
                {
                    data.card_numbers.emplace_back(stoi(line_split_by_space.at(i)));
                }
                else
                {
                    data.winning_numbers.emplace_back(stoi(line_split_by_space.at(i)));
                }
            }

            card_datas.emplace_back(data);
        }

        return card_datas;
    }
}