#include <iostream>
#include <vector>
#include <string>

#include "../common/file_utils.hpp"

int main()
{
    const auto file_res = common::read_file_by_line("input.txt");
    if (!file_res.has_value())
    {
        return EXIT_FAILURE;
    }

    int result = 0;
    for (const auto& line : file_res.value())
    {
        const auto history = common::split_string_by_space(line);
        std::vector<int> numbers = {};

        for (const auto& hist : history)
        {
            numbers.emplace_back(std::stoi(hist));
        }

        // Brute force solution:
        std::vector<std::vector<int>> history_sequences{};
        history_sequences.push_back(numbers);

        const auto reached_final_sequence = [&](const auto history_sequences)
        {
            if (history_sequences.size() == 0)
            {
                return false;
            }

            for (const auto& num : history_sequences)
            {
                if (num != 0)
                {
                    return false;
                }
            }

            return true;
        };

        while (!reached_final_sequence(history_sequences.back()))
        {
            std::vector<int> num{};
            for (int i = 0; i < history_sequences.back().size() - 1; i++)
            {
                num.push_back(history_sequences.back()[i + 1] - history_sequences.back()[i]);
            }

            history_sequences.emplace_back(num);
        }

        // Now, find the solution.
        int first_value_in_row = 0;
        for (int i = history_sequences.size() - 2; i >= 0; --i)
        {
            first_value_in_row = history_sequences[i][0] - first_value_in_row; 
        }

        result += first_value_in_row;
    }

    std::cout << "Result :: " << result << "\n";

    return EXIT_SUCCESS;
}