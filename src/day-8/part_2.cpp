#include <vector>
#include <string>
#include <unordered_map>
#include <numeric>

#include "../common/file_utils.hpp"

// For part 2, we need to find steps such that all the nodes ending with A will end up at nodes ending with Z.
// As cycles are present, you cant do all the processing simultaneously.
// A better approach would be to find number of steps required by each node that ends with A (i.e your starting point),
// and then take a LCM of them. The LCM would be the final results.
int main()
{
    const auto file_res = common::read_file_by_line("input.txt");
    if (!file_res.has_value())
    {
        return EXIT_FAILURE;
    }

    std::string instructions = file_res.value()[0];
    std::unordered_map<std::string, std::vector<std::string>> nodes{};

    for (int i = 2; i < file_res.value().size(); i++)
    {
        const auto l = common::split_string_by_space(file_res.value()[i]);

        const auto source = l[0];
        auto left = l[2];
        left.pop_back();
        left.erase(0, 1);

        auto right = l[3];
        right.pop_back();

        nodes[source] = std::vector{left, right};
    }

    std::vector<std::string> current = {};
    for (const auto& [key, value] : nodes)
    {
        if (key[2] == 'A')
        {
            current.emplace_back(key);
        }
    }

    const auto steps_to_reach_z = [&](const auto start_point ) -> uint64_t
    {
        uint64_t steps = 0;

        auto c = start_point; 
        while (c.back() != 'Z')
        {
            char move_direction = instructions.at(steps % (instructions.size()));
            if (move_direction == 'L')
            {
                c = nodes[c][0];
            }
            else
            {
                c = nodes[c][1];
            }

            ++steps;
        }
        std::cout << "Steps :: " << steps << std::endl;

        return steps;
    };

    std::vector<uint64_t> steps_to_reach_z_for_all_start_points{};
    for (const auto& start : current)
    {
        steps_to_reach_z_for_all_start_points.emplace_back(steps_to_reach_z(start));
    }

    uint64_t steps = 1;
    for (const auto& s : steps_to_reach_z_for_all_start_points)
    {
        steps = std::lcm(steps, s);
    }

    std::cout << "Steps :: " << steps << std::endl;
    
    return EXIT_SUCCESS;
}