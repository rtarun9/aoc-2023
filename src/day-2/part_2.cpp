#include "../common/file_utils.hpp"

#include <iostream>
#include <unordered_map>

int main()
{
    const auto file_res = common::read_file_by_line("input.txt");
    if (!file_res.has_value())
    {
        return EXIT_FAILURE;
    }

    // A single input line is of the format :
    // Game ID : x red, y blue; a red, b gree, g blue; i green etc.
    // We have to find minimum number of colors of each cube to make game possible.
    // Solution : For all sets in the game, find max of each color and multiply. That is the 'power' of game.
    // The ; separates the "sets" of a game.

    int result{};

    for (const auto &line : file_res.value())
    {
        std::vector<std::string> split_line = common::split_string_by_space(line);
        // Adding a ; to the end of the list for ease in data processing.
        split_line.back().push_back(';');

        // The first entry is "Game", 2nd is game_id, 3rd is :.
        const int game_id = std::stoi(split_line[1]);

        // Now, the sets are split by ';'. The number of a certain color is present before
        // the color name. Ex : 3 green.

        std::unordered_map<std::string, int> freq_map{};

        int set_number = 1;

        for (int k = 2; k < split_line.size(); k++)
        {
            if (split_line[k] == ";" || split_line[k].back() == ';')
            {
                // First add to the feq map. This is required because if string is red;, we have to add
                // red cube information as well.
                auto color = split_line[k];
                color.pop_back();

                freq_map[color] = std::max(freq_map[color], std::stoi(split_line[k - 1]));

                // std::cout << "SET : " << set_number++ << '\n';

                // for (const auto &[color, count] : freq_map)
                // {
                //     std::cout << color << " -> " << count << '\n';
                // }
            }
            else
            {
                // The current input will either be a color_comma, number, or color.
                if (split_line[k].back() == ',')
                {
                    split_line[k].pop_back();
                }

                // If the code reaches here, it is for sure that its either a number of color.
                if (split_line[k] == "red" || split_line[k] == "green" || split_line[k] == "blue")
                {
                    freq_map[split_line[k]] = std::max(freq_map[split_line[k]], std::stoi(split_line[k - 1]));
                }
            }
        }

        int power = 1;
        for (const auto &[color, count] : freq_map)
        {
            power *= count;
        }

        result += power;
    }

    std::cout << "Result :: " << result << '\n';

    return EXIT_SUCCESS;
}