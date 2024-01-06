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
    std::string time_str{};
    std::string distance_str{};

    for (int i = 1; i < time_input.size(); i++)
    {
        time_str += time_input.at(i);
        distance_str += distance_input.at(i);
    }
    
    const size_t time = std::stoll(time_str);
    const size_t distance = std::stoll(distance_str);

    std::cout << "Time and distance :: " << time << ", " << distance << "\n";

    // For part 1, a brute force solution might be possible.
    // Essentially, in O(n) time (where n = time) you can compute the n different paths possible
    // where each path is of the form : hold for j, release for n - j and distance travelled is (n - j) * j.
    // You want to find how many there are such that (n - j) * j is greater than distance.

    size_t result = 0;
    for (size_t i = 0; i < time; i++)
    {
        if ((time - i) * i > distance)
        {
            ++result;
        }


    }

    std::cout << "Result :: " << result << "\n";
}