#include "../common/file_utils.hpp"

#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>

struct RangeMap
{
    long long destination_start{};
    long long source_start{};
    long long range_size{};
};

int main()
{
    const auto file_res = common::read_file_by_line("input.txt");
    if (!file_res.has_value())
    {
        return EXIT_FAILURE;
    }

    std::vector<std::vector<RangeMap>> range_maps{};
    std::vector<std::pair<long long, long long>> seed_ranges{};

    const auto &input = file_res.value();

    {
        const auto seed_info = common::split_string_by_space(input[0]);
        for (size_t i = 1; i < seed_info.size(); i += 2) 
        {
            seed_ranges.push_back(
                std::make_pair<long long, long long>(
                    std::stoll(seed_info.at(i)),
                    std::stoll(seed_info.at(i)) + std::stoll(seed_info.at(i + 1)) - 1));
        }
    }

    for (size_t i = 1; i < input.size(); i++)
    {
        const auto split_input = common::split_string_by_space(input[i]);
        if (!split_input.empty() && split_input.back() != "map:")
        {
            const auto destination_start = std::stoll(split_input[0]);
            const auto source_start = std::stoll(split_input[1]);
            const auto range_size = std::stoll(split_input[2]);

            range_maps.back().emplace_back(RangeMap{
                .destination_start = destination_start,
                .source_start = source_start,
                .range_size = range_size - 1,
            });
        }
        else if (!split_input.empty() && split_input.back() == "map:")
        {
            range_maps.emplace_back(std::vector<RangeMap>{});
        }
    }

    for (const auto &map : range_maps)
    {
        std::vector<std::pair<long long, long long>> new_range_maps{};
        while (!seed_ranges.empty())
        {
            const auto top = seed_ranges.back();
            seed_ranges.pop_back();

            bool overlap_found = false;

            for (const auto &m : map)
            {
                const auto overlapping_region = std::pair{
                    std::max(top.first, m.source_start),
                    std::min(top.second, m.source_start + m.range_size)};

                if (overlapping_region.first <= overlapping_region.second)
                {
                    if (top.first < overlapping_region.first)
                    {
                        seed_ranges.emplace_back(std::pair{top.first, overlapping_region.first - 1});
                    }

                    new_range_maps.push_back({
                        overlapping_region.first - m.source_start + m.destination_start,
                        overlapping_region.second - m.source_start + m.destination_start});

                    if (top.second > overlapping_region.second)
                    {
                        seed_ranges.push_back({overlapping_region.second + 1, top.second});
                    }

                    overlap_found = true;
                    break;
                }
            }

            if (!overlap_found)
            {
                new_range_maps.push_back(top);
            }
        }

        seed_ranges = new_range_maps;
    }

    std::for_each(std::begin(seed_ranges), std::end(seed_ranges), [&](const auto& p) {std::cout << p.first << ", " << p.second << '\n';});

    std::cout << "Result :: " << std::min_element(seed_ranges.begin(), seed_ranges.end())->first << "\n";

    return EXIT_SUCCESS;
}
