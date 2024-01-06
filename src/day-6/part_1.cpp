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

    const auto input = *file_res;
    const auto time_input = common::split_string_by_space(input.at(0));
    const auto distance_input = common::split_string_by_space(input.at(1));

    // The first line is a list of times, second line is a list of distances.
    std::vector<int> times{};
    std::vector<int> distances{};

    for (int i = 1; i < time_input.size(); i++)
    {
        std::cout << i << std::endl;
        times.emplace_back(std::stoi(time_input.at(i)));
        distances.emplace_back(std::stoi(distance_input.at(i)));
    }

    // For part 1, a brute force solution might be possible.
    // Essentially, in O(n) time (where n = time) you can compute the n different paths possible
    // where each path is of the form : hold for j, release for n - j and distance travelled is (n - j) * j.
    // You want to find how many there are such that (n - j) * j is greater than distance.

    int result = 1;
    for (int i = 0; i < times.size(); i++)
    {
        int possible_soln = 0;

        for (int j = 0; j < times[i]; j++)
        {
            if ((times[i] - j) * j > distances[i])
            {
                possible_soln++;
            }
        }

        std::cout << "For :: " << distances[i] << " possible solutions : " << possible_soln << "\n";

        result *= possible_soln;
    }

    std::cout << "Result :: " << result << "\n";
}