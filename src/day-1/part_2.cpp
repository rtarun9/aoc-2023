#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std::string_literals;

#include "../common/file_utils.hpp"

int main()
{
    const auto file_result = common::read_file_by_word("input.txt");
    if (!file_result.has_value())
    {
        return EXIT_FAILURE;
    }

    // Logic. 2 pointer approach.
    // Once the first digit is found, set second digit to 0 and continue the algorithm on the entire string.
    // In part_2 the digit can be of the form '1', '2', ... or 'one', 'two', etc.
    // Time complexity : O(n) where n is number of characters in a word.
    // Space complexity : O(1) since I use a single variable to compute running sum.

    // There are 2 code paths that the code can take : Either find a digit or a word.
    // Find the first occurrence of a substring matching either a number of a word.
    // This map can be used to convert a number in word form to corresponding number.
    const auto word_to_number_map = std::unordered_map<std::string, int>{
        {"zero", 0},
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9},
        {"0", 0},
        {"1", 1},
        {"2", 2},
        {"3", 3},
        {"4", 4},
        {"5", 5},
        {"6", 6},
        {"7", 7},
        {"8", 8},
        {"9", 9},
    };

    const auto word_number_match_patterns = std::vector<std::string>{
        "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    // Return index into the word_number_match_patterns array.
    const auto get_first_and_last_matches = [&](const std::string &line)
    {
        std::pair<int, int> occurance_pos = {INT_MAX, -1};
        std::pair<std::string, std::string> matched_patterns{};

        for (const auto &pattern : word_number_match_patterns)
        {
            if (const int first = line.find(pattern); first != std::string::npos)
            {
                if (first < occurance_pos.first)
                {
                    occurance_pos.first = first;
                    matched_patterns.first = pattern;
                }
            }

            if (const int last = line.rfind(pattern); last != std::string::npos)
            {
                if (last > occurance_pos.second)
                {
                    occurance_pos.second = last;
                    matched_patterns.second = pattern;
                }
            }
        }

        if (occurance_pos.second == -1)
        {
            occurance_pos.second = occurance_pos.first;
        }

        return matched_patterns;
    };

    int result = 0;
    for (const std::string &line : file_result.value())
    {
        const auto matches = get_first_and_last_matches(line);

        int first_digit = word_to_number_map.at(matches.first);
        int last_digit = word_to_number_map.at(matches.second);

        result += first_digit * 10 + last_digit;
    }

    std::cout << "Result : " << result << '\n';

    return EXIT_SUCCESS;
}