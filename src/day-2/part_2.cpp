#include "../common/file_utils.hpp"
#include "preprocessing.hpp"

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
        const std::vector<std::string> game_input = preprocessing::preprocess_game_input(line);

        // The first entry is "Game", 2nd is game_id, 3rd is :.
        const int game_id = std::stoi(game_input[1]);

        // Now, the sets are split by ';'. The number of a certain color is present before
        // the color name. Ex : 3 green.

        std::unordered_map<std::string, int> freq_map{};

        int set_number = 1;

        for (int k = 2; k < game_input.size(); k++)
        {
            // The current input will either be a ;, number, or color.
            if (game_input[k] == ";")
            {
                continue;
            }

            // If the code reaches here, it is for sure that its either a number of color.
            if (game_input[k] == "red" || game_input[k] == "green" || game_input[k] == "blue")
            {
                freq_map[game_input[k]] = std::max(freq_map[game_input[k]], std::stoi(game_input[k - 1]));
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