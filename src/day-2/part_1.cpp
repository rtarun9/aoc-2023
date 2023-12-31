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
    // We have to find some of Game ID's where the given color combinations are possible
    // given there are 12 red, 13 green, and 14 blue cubes.
    // The ; separates the "sets" of a game.

    // Obvious brute force solution :
    // In each set, the count must be less than given parameters.

    int result{};

    for (const auto &line : file_res.value())
    {
        const std::vector<std::string> game_input = preprocessing::preprocess_game_input(line);
        
        // The first entry is "Game", 2nd is game_id, 3rd is :.
        const int game_id = std::stoi(game_input[1]);

        // Now, the sets are split by ';'. The number of a certain color is present before
        // the color name. Ex : 3 green.

        std::unordered_map<std::string, int> freq_map{};

        bool possible_game = true;
        int set_number = 1;

        for (int k = 2; k < game_input.size(); k++)
        {
            if (game_input[k] == ";")
            {
                #if 0
                std::cout << "SET : " << set_number++ << '\n';
                #endif

                #if 0
                for (const auto &[color, count] : freq_map)
                {
                    std::cout << color << " -> " << count << '\n';
                }
                #endif

                // If the code reaches here, then the set is over.
                // Perform the count check here and at the end reset counters to 0.
                if (freq_map["red"] > 12 || freq_map["green"] > 13 || freq_map["blue"] > 14)
                {
                    possible_game = false;
                    break;
                }
                else
                {
                    freq_map.clear();
                }
            }
            else
            {
                // The current input will either be a number or color.

                // If the code reaches here, it is for sure that its either a number of color.
                if (game_input[k] == "red" || game_input[k] == "green" || game_input[k] == "blue")
                {
                    freq_map[game_input[k]] = std::stoi(game_input[k - 1]);
                }
            }
        }

        if (possible_game)
        {
            std::cout << game_id << " is possible.\n";

            result += game_id;
        }
        else
        {
            std::cout << game_id << " is not possible\n";
        }

        std::cout << '\n';
    }

    std::cout << "Result :: " << result << '\n';

    return EXIT_SUCCESS;
}