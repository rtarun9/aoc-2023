#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char **argv)
{
    std::ifstream input_file("../../data/day_1_input.txt");
    if (!input_file.is_open())
    {
        std::cerr << "Failed to open day 1 input file.";
        std::exit(-1);
    }

    int32_t result{};

    std::string line_wise_file_content{};
    while (input_file >> line_wise_file_content) 
    {
        // Now, we have to get the first and last numeric digit from line_wise_file_content.
        int first_digit = -1;
        int last_digit = -1;

        std::cout << line_wise_file_content << '\n';

        for (int i = 0, j = line_wise_file_content.size() - 1; j >= 0; ++i, --j) 
        {
            if (first_digit == -1) 
            {
                int d = line_wise_file_content[i] - '0';
                if (d >= 0 && d <= 9)
                {
                    first_digit = d;
                }
            }

            if (last_digit == -1) 
            {
                int d = line_wise_file_content[j] - '0';
                if (d >= 0 && d <= 9)
                {
                    last_digit = d;
                }
            }
        }

        first_digit = std::max(first_digit, 0);
        last_digit = std::max(last_digit, 0);

        result += first_digit * 10 + last_digit;
        std::cout << "Result so far : " << result << '\n';
    }

    std::cout << "The final result : " << result;
}