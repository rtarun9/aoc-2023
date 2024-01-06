// Explanation of the input.
// Consider the example :
/*
seed-to-soil map:
50 98 2
52 50 48
*/
// Here, Seed number 98 corresponds to soil number 50,
// and seed number 99 corresponds to soil number 51.
// If an number does not have a mapping, consider the identity mapping.

// Naive solution : Basically construct a bunch of range_map's, and go through each of the
// hash maps in the following way.
// Say initial seed is X.
// soln_for_seed = the last element of [X, seed_soil_map[X], some_other_map[seed_soil_map[X]])].
// The final sol = min of all soln_for_seed's.

#include "../common/file_utils.hpp"

#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>

// Rather than using a hash map and manually filling for all entries from source/destination start .. src/dst end,
// for efficiency we can just store the range starts and the range size.
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

    // For the sake of simplicity, I wont even bother storing the names of the range maps like seed to soil, etc.
    // Whenever a new map segment occurs, I'll just add the ranges to a new element in the output range maps vector.
    std::vector<std::vector<RangeMap>> range_maps{};
    std::vector<std::pair<long long, long long>> seed_ranges{};

    // Input processing:
    // It is guaranteed to first have a list of all the seeds.
    // After that, from a x-to-y map: till the next, create a new range map and add the ranges to it.
    const auto &input = file_res.value();

    {
        // Split by space for the first line.
        const auto seed_info = common::split_string_by_space(input[0]);
        for (size_t i = 1; i < seed_info.size(); i += 2)
        {
            seed_ranges.push_back(
                std::make_pair<long long, long long>(
                    std::stoll(seed_info.at(i)),
                    std::stoll(seed_info.at(i)) + std::stoll(seed_info.at(i + 1)) - 1));
        }
    }

    // Now, merge all the seed ranges to optimize the algorithm.
    std::sort(std::begin(seed_ranges), std::end(seed_ranges));
    std::vector<std::pair<long long, long long>> merged_sorted_seed_ranges{seed_ranges.at(0)};

    for (int i = 1; i < seed_ranges.size(); i++)
    {
        if (merged_sorted_seed_ranges.back().second >= seed_ranges.at(i).first)
        {
            merged_sorted_seed_ranges.back().second = std::max(merged_sorted_seed_ranges.back().second, seed_ranges.at(i).second);
        }
        else
        {
            merged_sorted_seed_ranges.push_back(seed_ranges.at(i));
        }
    }

    std::cout << "Merged seed ranges : \n";
    for (const auto& x : merged_sorted_seed_ranges)
    {
        std::cout << x.first << "," << x.second << '\n';
    }

    for (size_t i = 1; i < input.size(); i++)
    {
        // If the current input is not a map, add the ranges to the range map.
        const auto split_input = common::split_string_by_space(input[i]);
        if (!split_input.empty() && split_input.back() != "map:")
        {
            // The input is of the range start_destination start_source step_count.
            const auto destination_start = std::stoll(split_input[0]);
            const auto source_start = std::stoll(split_input[1]);
            const auto range_size = std::stoll(split_input[2]);

            range_maps.back().emplace_back(RangeMap{
                .destination_start = destination_start,
                .source_start = source_start,
                .range_size = range_size,
            });
        }

        else if (!split_input.empty() && split_input.back() == "map:")
        {
            // New map has to be created.
            range_maps.emplace_back(std::vector<RangeMap>{});
        }
    }

    size_t result = INT_MAX;

    for (const auto &seed_range : merged_sorted_seed_ranges)
    {
        for (int seed = seed_range.first; seed <= seed_range.second; seed++)
        {
            size_t seed_location = seed;

            for (const auto &map : range_maps)
            {
                for (const auto &range : map)
                {
                    if (seed_location >= range.source_start && seed_location <= range.source_start + range.range_size)
                    {
                        seed_location = range.destination_start + (seed_location - range.source_start);
                        break;
                    }
                    else
                    {
                        // Seed location is unchanged (i.e identity mapping).
                    }
                }
            }

            result = std::min(result, seed_location);
        }
    }

    std::cout << "Result :: " << result << '\n';
    return EXIT_SUCCESS;
}