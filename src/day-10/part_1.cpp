#include <array>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "../common/file_utils.hpp"

int main()
{
    const auto file_res = common::read_file_by_line("input.txt");
    if (!file_res.has_value())
    {
        return EXIT_FAILURE;
    }

    // Parse the input into a matrix of chars.
    // Also, pad the input to prevent having to do boundary checks.
    std::vector<std::vector<char>> input{};
    const auto input_width = (*file_res).size() + 2;

    const auto v = std::vector<char>(input_width, '.');
    input.emplace_back(v);
    for (const auto &line : *file_res)
    {
        std::vector<char> ip{};
        ip.push_back('.');
        for (const auto &l : line)
        {
            ip.emplace_back(l);
        }
        ip.push_back('.');
        input.emplace_back(ip);
    }
    input.emplace_back(v);

    for (const auto &a : input)
    {
        for (const auto &b : a)
        {
            std::cout << b;
        }
        std::cout << std::endl;
    }

    // A pipe can be represented as a pair.
    // Pair of numbers, where each number represents offsets from the current cell to the cells it connects.
    // The first element of pair is x offset, and second is the y offset.

    // up : 0 -1
    // down : 0 1
    // right : 1 0
    // left : -1 0
    constexpr auto up = std::array{-1, 0};
    constexpr auto down = std::array{1, 0};
    constexpr auto right = std::array{0, 1};
    constexpr auto left = std::array{0, -1};

    std::unordered_map<char, std::array<std::array<int, 2>, 2>> pipe_end_map{};
    pipe_end_map['|'] = std::array{up, down};
    pipe_end_map['-'] = std::array{right, left};
    pipe_end_map['L'] = std::array{up, right};
    pipe_end_map['J'] = std::array{up, left};
    pipe_end_map['7'] = std::array{down, left};
    pipe_end_map['F'] = std::array{down, right};

    const auto has_end = [&](const auto current_input, const auto direction) {
        if (pipe_end_map[current_input][0] == direction || pipe_end_map[current_input][1] == direction ||
            current_input == 'S')
            return true;

        return false;
    };

    const auto is_move_possible = [&](const auto i, const auto j) {
        const auto right_move = has_end(input[i][j], right) && has_end(input[i][j + 1], left);
        const auto left_move = has_end(input[i][j], left) && has_end(input[i][j - 1], right);
        const auto up_move = has_end(input[i][j], up) && has_end(input[i - 1][j], down);
        const auto down_move = has_end(input[i][j], down) && has_end(input[i + 1][j], up);

        return std::array{left_move, up_move, right_move, down_move};
    };

    // 1 -> unvisited location.
    // bfs queue has the elements : steps to reach parent, current cell x and y.
    std::vector<std::vector<int>> step_map(input.size(), std::vector<int>(input.at(0).size(), -1));
    std::queue<std::array<int, 3>> bfs_queue{};

    // The logic is simple : Start from S, and do a bfs.
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input.size(); j++)
        {
            if (input[i][j] == 'S')
            {
                bfs_queue.push({-1, i, j});
            }
        }
    }

    while (!bfs_queue.empty())
    {
        const auto top = bfs_queue.front();
        bfs_queue.pop();

        const auto &[steps_to_reach_parent, curr_y, curr_x] = top;

        // Add the neighbouring cells to the queue.
        const auto directions = pipe_end_map[input[curr_y][curr_x]];
        const auto first_dir = directions.at(0);
        const auto second_dir = directions.at(1);

        if (input[curr_y][curr_x] == '.')
        {
            continue;
        }

        if (step_map[curr_y][curr_x] == -1)
        {
            step_map[curr_y][curr_x] = steps_to_reach_parent + 1;
        }
        else
        {
            // Only modify if the new step is less than previously discovered route.
            if (step_map[curr_y][curr_x] < steps_to_reach_parent + 1)
            {
                continue;
            }

            step_map[curr_y][curr_x] = steps_to_reach_parent + 1;
        }

        const auto steps_to_reach_current = step_map[curr_y][curr_x];

        // Check neighboring cells and add to queue accordingly.
        const auto move_possiblities = is_move_possible(curr_y, curr_x);

        if (move_possiblities[0])
            bfs_queue.push({steps_to_reach_current, curr_y, curr_x - 1});
        if (move_possiblities[1])
            bfs_queue.push({steps_to_reach_current, curr_y - 1, curr_x});
        if (move_possiblities[2])
            bfs_queue.push({steps_to_reach_current, curr_y, curr_x + 1});
        if (move_possiblities[3])
            bfs_queue.push({steps_to_reach_current, curr_y + 1, curr_x});
    }

    int result = 0;
    std::cout << "Step map :: " << std::endl;
    for (int i = 0; i < step_map.size(); i++)
    {
        for (int j = 0; j < step_map.size(); j++)
        {
            if (step_map[i][j] == -1)
                std::cout << '.';
            else
                std::cout << step_map[i][j];

            result = std::max(result, step_map[i][j]);
        }
        std::cout << std::endl;
    }

    std::cout << "Result :: " << result << std::endl;
    return EXIT_SUCCESS;
}
