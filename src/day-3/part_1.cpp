#include "../common/file_utils.hpp"

int main()
{
    const auto file_res = common::read_file_by_word("input.txt");
    if (!file_res.has_value())
    {
        return EXIT_FAILURE;
    }

    const auto &input = file_res.value();

    // The logic : Construct a matrix of dimensions (w, h) = (input[0].size(), input.size()).
    // In this matrix, go row wise (visiting each matrix entry) and do the following:
    // Check for Symbol above and bottom, left and right, and diagonal.
    // Take a 2 pointer approach since the number's digit is in different matrix entries.

    // Create and fill in the input matrix.
    std::vector<std::vector<char>> input_matrix{};
    input_matrix.reserve(input.size());

    for (const auto &line : input)
    {
        std::vector<char> matrix_row{};
        matrix_row.reserve(line.size() + 1);

        // Add the end of each line I have added a '.' just for edge case where number is at the very end of a line.
        for (const auto &c : line)
        {
            matrix_row.emplace_back(c);
        }
        matrix_row.emplace_back('.');

        input_matrix.emplace_back(matrix_row);
    }

    int result = 0;


    const auto is_symbol = [&](char c) -> bool
    {
        return (c != '.') && (c < '0' || c > '9');
    };

    const auto is_adjacent_to_symbol = [&](const int i, const int j) -> bool
    {
        const auto above = (i > 0 && is_symbol(input[i - 1][j]));
        const auto below = (i + 1 <= input.size() - 1 && is_symbol(input[i + 1][j]));
        const auto right = (j + 1 <= input[0].size() - 1 && is_symbol(input[i][j + 1]));
        const auto left = (j > 0 && is_symbol(input[i][j - 1]));

        const auto diag_above_right = (i > 0 && j + 1 <= input[0].size() - 1&& is_symbol(input[i - 1][j + 1]));
        const auto diag_above_left = (i > 0 && j > 0 && is_symbol(input[i - 1][j - 1]));
        const auto diag_bottom_right = (i + 1 <= input.size() - 1 && j + 1 <= input[0].size() - 1 && is_symbol(input[i + 1][j + 1]));
        const auto diag_bottom_left = (i + 1 <= input.size() - 1 && j > 0 && is_symbol(input[i + 1][j - 1]));

        return above || below || right || left || diag_above_left || diag_above_right || diag_bottom_left || diag_bottom_right;
    };

    // Traversing the matrix row wise.
    for (int i = 0; i < input_matrix.size(); i++)
    {
        // In each row, find the find 'number' and check if its engine part.
        int word_left = -1;
        int word_right = -1;

        for (int j = 0; j < input_matrix[i].size(); j++)
        {
            if (input_matrix[i][j] >= '0' && input_matrix[i][j] <= '9')
            {
                // Compute left and right pointer.
                if (word_left == -1)
                {
                    word_left = j;
                    word_right = j;
                }
                else
                {
                    word_right++;
                }
            }
            else
            {
                // If the code reaches here, the number is either complete or input
                // just a symbol or '.'.
                // This can be verified by checking word_left. If it is -1, then no number has been encountered.
                if (word_right == -1)
                {
                    continue;
                }
                else
                {
                    // The word is complete, check adjacency with symbol.
                    // Note that this computation is done for EACH digit of the number.
                    // Not mentioned in question, but as a edge case.
                    for (int x = word_left; x <= word_right; x++)
                    {
                        if (is_adjacent_to_symbol(i, x))
                        {
                            // Compute the number.
                            int number = 0;
                            for (int y = word_right; y >= word_left; --y)
                            {
                                number += (input_matrix[i][y] - '0') * pow(10, word_right - y);
                            }

                            std::cout << number << " is a engine part!\n";

                            result += number;
                            break;
                        }
                    }

                    word_right = -1;
                    word_left = -1;
                }
            }
        }
    }

    std::cout << "Result :: " << result << '\n';

    return EXIT_SUCCESS;
}