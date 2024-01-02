#include "../common/file_utils.hpp"

#include <stack>
#include <array>

int main()
{
    const auto file_res = common::read_file_by_line("input.txt");
    if (!file_res.has_value())
    {
        return EXIT_FAILURE;
    }

    // Note : Inorder to prevent doing multiple boundary checks, the input matrix is padding.
    // Row is adding to top and bottom of matrix, and also to left and right.
    // This means I can just loop over matrix with start index of 1 and width / height - 1.
    const int num_rows = file_res.value().size() + 2;
    const int num_columns = file_res.value().at(0).size() + 2;

    std::vector<std::vector<char>> matrix{};
    matrix.reserve(num_rows);

    // Padding (top of matrix).
    matrix.emplace_back(std::vector<char>(num_columns, '.'));
    for (const auto row : file_res.value())
    {
        std::vector<char> row_to_insert{};
        row_to_insert.reserve(num_columns);

        row_to_insert.emplace_back('.');
        row_to_insert.insert(std::end(row_to_insert), row.begin(), row.end());
        row_to_insert.emplace_back('.');

       matrix.emplace_back(row_to_insert); 
    }
    matrix.emplace_back(std::vector<char>(num_columns, '.'));

    // Using a different logic compared to part_1.
    // First I will go through the matrix, and whenever I come across a special symbol, I will add those coordinates to a stack.
    // Then at the end, find 2 numbers adjacent to a stack element (i.e special symbol coordinate) and process.
    // I.E if 2 adjacent numbers are found multiply them and accumulate.

    std::stack<std::pair<int, int>> special_symbol_coordinate{};

    const auto is_special = [&](const int i, const int j)
    {
        const int x = matrix[i][j];
        return x == '*';
    };

    for (int i = 1; i < num_rows - 1; i++)
    {
        for (int j = 1; j < num_columns - 1; j++)
        {   
            if (is_special(i, j))
            {
                special_symbol_coordinate.push({i, j});
            }
        }
    }

    size_t result = 0;
    while (!special_symbol_coordinate.empty())
    {
        const auto top = special_symbol_coordinate.top();
        special_symbol_coordinate.pop();

        // In a 3x3 region around the symbol, find numbers.
        // Keep in mind that number is a collection of digits (always horizontal).

        constexpr auto neighbour_extents = std::array<int, 3u>{-1, 0, 1};

        std::vector<std::pair<int, int>> neighbour_number_indices{};

        for (const auto i : neighbour_extents)
        {
            for (const auto j : neighbour_extents)
            {
                if (i == j && i == 0)
                {
                    continue;
                }

                // Check if there is a number at neighbour offset i, j.
                // If yes, add the index to the vector. 
                const char elem = matrix[i + top.first][j + top.second]; 
                if (elem >= '0' && elem <= '9')
                {
                    const int r = i + top.first;
                    const int c = j + top.second;

                    // Check if the current element is adjacent to the previous element. If so, remove previous element.            
                    if (!neighbour_number_indices.empty())
                    {
                        if (neighbour_number_indices.back().first == r && neighbour_number_indices.back().second + 1 == c)
                        {
                            std::cout << "POPPED : " << matrix[neighbour_number_indices.back().first][neighbour_number_indices.back().second] << '\n';
                            neighbour_number_indices.pop_back();
                        }
                    }

                    neighbour_number_indices.emplace_back(std::pair<int, int>{r, c});    
                }
            }
        }

        // Get the two adjacent numbers.
        std::vector<int> nums{};
        for (const auto& x : neighbour_number_indices)  
        {
            // Construct number from a single digit index.
            int k = x.second;

            // Find leftmost digit.
            while (isdigit(matrix[x.first][k - 1]))
            {
                k--;
            }

            // Go from right to left and construct number.
            int number = 0;
            while (isdigit(matrix[x.first][k]))
            {
                number = number * 10 + int(matrix[x.first][k++] - '0');
            }

            nums.emplace_back(number);
        }

        std::cout << "Neighbor numbers : \n";
        for (const auto& n : nums)
        {
            std::cout << n << " ";
        }
        std::cout << '\n';

        // If number > 2, ignore. Else multiply and accumulate.
        if (nums.size() == 2)
        {
            result += size_t(nums[0]) * size_t(nums[1]);
        }
    }
    
    std::cout << "Result :: " << result << '\n';
}