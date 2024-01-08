#include <vector>
#include <string>
#include <unordered_map>

#include "../common/file_utils.hpp"

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

    int steps = 0;
    std::vector<std::string> current = {};
    for (const auto& [key, value] : nodes)
    {
        if (key[2] == 'A')
        {
            current.emplace_back(key);
        }
    }

    const auto reached_all_zs = [&](const auto& current)
    {
        for (const auto& c : current)
        {
            if (c.back() != 'Z')
            {
                return false;
            }
        }

        return true;
    };

    while (!reached_all_zs(current))
    {
        char move_direction = instructions.at(steps % (instructions.size()));
        if (move_direction == 'L')
        {
            const auto dup = current;
            current.clear();

            for (const auto& c : dup)
            {
                current.emplace_back(nodes[c][0]);
            }
        }
        else
        {

            const auto dup = current;
            current.clear();

            for (const auto& c : dup)
            {
                current.emplace_back(nodes[c][1]);
            }
        }

        ++steps; 
    }

    std::cout << "Steps :: " << steps << std::endl;
    
    return EXIT_SUCCESS;
}