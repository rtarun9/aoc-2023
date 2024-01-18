#include <array>
#include <iostream>
#include <queue>
#include <unordered_map>

#include "../common/file_utils.hpp"

// Idea : Perform a search for all loops and mark them first.
// Then, perform a floodfill and do the search.
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
    const auto input_width = (file_res.value()[0]).size() + 2;

    const auto v = std::vector<char>(input_width, '.');
    input.emplace_back(v);
    for (const auto &line : *file_res)
    {
        std::vector<char> ip{};
        ip.push_back('.');
        for (const auto &l : line)
        {
            ip.push_back(l);
        }
        input.emplace_back(ip);
    }
    input.emplace_back(v);

    std::cout << "Input (after padding)" << std::endl;

    for (const auto &a : input)
    {
        for (const auto &b : a)
        {
            std::cout << b;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

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
    // bfs queue has the elements :  current cell x and y.
    std::vector<std::vector<char>> step_map(input.size(), std::vector<char>(input.at(0).size(), '.'));
    std::queue<std::array<int, 2>> bfs_queue{};

    // The logic is simple : Start from S, and do a bfs.
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input.size(); j++)
        {
            if (input[i][j] == 'S')
            {
                bfs_queue.push({i, j});
                break;
            }
        }
    }

    while (!bfs_queue.empty())
    {
        const auto top = bfs_queue.front();
        bfs_queue.pop();

        const auto &[curr_y, curr_x] = top;

        // Add the neighbouring cells to the queue.
        const auto directions = pipe_end_map[input[curr_y][curr_x]];
        const auto first_dir = directions.at(0);
        const auto second_dir = directions.at(1);

        if (input[curr_y][curr_x] == '.')
        {
            continue;
        }

        if (input[curr_y][curr_x] != '.' && step_map[curr_y][curr_x] == '.')
        {
            step_map[curr_y][curr_x] = input[curr_y][curr_x];
        }
        else if (step_map[curr_y][curr_x] != '.')
        {
            continue;
        }

        // Check neighboring cells and add to queue accordingly.
        const auto move_possiblities = is_move_possible(curr_y, curr_x);

        if (move_possiblities[0])
            bfs_queue.push({curr_y, curr_x - 1});
        if (move_possiblities[1])
            bfs_queue.push({curr_y - 1, curr_x});
        if (move_possiblities[2])
            bfs_queue.push({curr_y, curr_x + 1});
        if (move_possiblities[3])
            bfs_queue.push({curr_y + 1, curr_x});

        // If the current char is S, then based on move possiblities replace S with the appropriate pipe.
        if (input[curr_y][curr_x] == 'S')
        {
            if (move_possiblities[0] && move_possiblities[1])
            {
                // Can move left and up.
                step_map[curr_y][curr_x] = 'J';
            }
            else if (move_possiblities[0] && move_possiblities[2])
            {
                // Can move left and right.
                step_map[curr_y][curr_x] = '-';
            }
            else if (move_possiblities[0] && move_possiblities[3])
            {
                // Can move left and down.
                step_map[curr_y][curr_x] = '7';
            }
            else if (move_possiblities[1] && move_possiblities[2])
            {
                // Can move up and right.
                step_map[curr_y][curr_x] = 'L';
            }
            else if (move_possiblities[1] && move_possiblities[3])
            {
                // Can move up down.
                step_map[curr_y][curr_x] = '|';
            }
            else if (move_possiblities[2] && move_possiblities[3])
            {
                // Can move right down.
                step_map[curr_y][curr_x] = 'F';
            }
        }

    }

    std::cout << "The loops in the grid are as follows : " << std::endl;
    for (int i = 0; i < step_map.size(); i++)
    {
        for (int j = 0; j < step_map[i].size(); j++)
        {
            std::cout << step_map[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    // Now that the loop is made, we can first do a flood fill to find the possible regions.
    const std::array<char, 7> obstacles = {'7', '|', '-', 'L', 'J', 'F', 'S'};

    const auto is_num = [&](const char c) { return (c - '0' >= 0 && c - '0' <= 9); };
    const auto can_be_part_of_region = [&](const char c) { return c == '.'; };

    // Locations with region 0 belong to the background, 1 represents the pixels worth checking at.
    int current_region = 0;
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            if (can_be_part_of_region(step_map[i][j]))
            {
                std::queue<std::pair<int, int>> q{};
                q.push({i, j});

                while (!q.empty())
                {
                    const auto top = q.front();
                     std::cout << current_region << " for i and j " << top.first << ", "
                     << top.second  << " and value : " << step_map[top.first][top.second] << std::endl;
                    q.pop();

                    step_map[top.first][top.second] = current_region + '0';

                    // Check for neighbouring regions to see if they are part of current
                    // region.
                    if (top.first + 1 < step_map.size() && can_be_part_of_region(step_map[top.first + 1][top.second]))
                    {
                        step_map[top.first + 1][top.second] = current_region + '0';
                        q.push({top.first + 1, top.second});
                    }

                    if (top.first - 1 >= 0 && can_be_part_of_region(step_map[top.first - 1][top.second]))
                    {
                        step_map[top.first - 1][top.second] = current_region + '0';
                        q.push({top.first - 1, top.second});
                    }

                    if (top.second + 1 < step_map[0].size() && can_be_part_of_region(step_map[top.first][top.second + 1]))
                    {
                        step_map[top.first][top.second + 1] = current_region + '0';
                        q.push({top.first, top.second + 1});
                    }

                    if (top.second - 1 >= 0 && can_be_part_of_region(step_map[top.first][top.second - 1]))
                    {
                        step_map[top.first][top.second - 1] = current_region + '0';
                        q.push({top.first, top.second - 1});
                    }
                }

                if (current_region == 0)
                {
                    current_region = 1;
                }
            }
        }
    }

    // Now, check for all points in the loop. This can be done using the point in polygon algorithm.
    // https://en.wikipedia.org/wiki/Point_in_polygon.
    // for each point that is inside the loop, shoot rays to the left and see the number of intersections.
    // Now, if you encounter two obstacles that point up / both point down, it is not a intersection as you can just
    // cruise through the common side (Like L and J both have a horizontal side). So, check if both are pointing in same
    // direction. If yes, then increment intersection count. Else, decrease intersection count but keep track of last
    // hit char.
    const auto is_same_direction = [&](const auto a, const auto b) -> bool
    {
        return (a == 'L' && b == 'J' || a == 'F' && b == '7' || a == '|' || b == '|');
    };

    int result = 0;
    for (int i = 0; i < step_map.size(); i++)
    {
        for (int j = 0; j < step_map[i].size(); j++)
        {
            if (step_map[i][j] - '0' == 1)
            {
                int num_intersections = 0;

                for (int x = j - 1; x >= 0; --x)
                {
                    // Why only L, J, and |?
                    // Because, a L 7 crossing is a single crossing and not 2 individual ones.
                    // The if statement below can either have L J or 7 F, doesn't matter.
                    if (step_map[i][x] == 'L' || step_map[i][x] == 'J' || step_map[i][x] == '|')
                    ++num_intersections;
                }
                if (num_intersections != 0 && num_intersections % 2 != 0)
                {
                    result++;
                }
            }
        }
    }

    std::cout << "Result :: " << result << std::endl;


    return EXIT_SUCCESS;
}
