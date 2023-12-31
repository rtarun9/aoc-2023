#include <iostream>
#include <algorithm>

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
    // Time complexity : O(n) where n is number of characters in a word.
    // Space complexity : O(1) since I use a single variable to compute running sum.
    
    int result = 0;
    for (const auto& line : file_result.value())
    {
        int first_digit = -1;
        int last_digit = -1;

        for (const auto& ch : line)
        {            
            if (isdigit(ch))
            {
                if (first_digit == -1)
                {
                    first_digit = ch - '0';
                }
                else
                {
                    last_digit = ch - '0';
                }
            }
        }

        // If last_digit was not found, set it to the first_digit.
        if (last_digit == -1)
        {
            last_digit = first_digit;
        }
        
        result += first_digit * 10 + last_digit;
    }

    std::cout << "Result : " << result << '\n';

    return EXIT_SUCCESS;
}