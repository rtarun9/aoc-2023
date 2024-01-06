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
// soln_for_seed = last element of [X, seed_soil_map[X], some_other_map[seed_soil_map[X]])].
// The final sol = min of all soln_for_seeds.

#include "../common/file_utils.hpp"

#include <unordered_map>
#include <vector>
#include <iostream>

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
    // Whenever a new map segment occurs, I'll just add the ranges to a new element in the output most vector.
    std::vector<std::vector<RangeMap>> range_maps{};
    std::vector<long long> seeds{};

    // Input processing:
    // It is guaranteed to first have a list of all the seeds.
    // After that, from a x-to-x map: till the next, create a new hash map and add the ranges to it.
    const auto& input = file_res.value();

    {
        // Split by space for the first line.
        const auto seed_info = common::split_string_by_space(input[0]);
        for (size_t i = 1; i < seed_info.size(); i++)
        {
            seeds.emplace_back(std::stoll(seed_info.at(i)));
        }
    }

    for (size_t i = 1; i < input.size(); i++)
    {
        // If the current input is not a map, add the ranges to the hash map.
        const auto split_input = common::split_string_by_space(input[i]);
        if (!split_input.empty() && split_input.back() != "map:")
        {
            // The input is of the range start_destination start_source step_count.
            const auto destination_start = std::stoll(split_input[0]);
            const auto source_start = std::stoll(split_input[1]);
            const auto range_size = std::stoll(split_input[2]);

            range_maps.back().emplace_back(RangeMap{.destination_start = destination_start, .source_start = source_start, .range_size = range_size,});
        }

        else if (!split_input.empty() && split_input.back() == "map:")
        {
            // New map has to be created.
            range_maps.emplace_back(std::vector<RangeMap>{});
        }
    }

    size_t result = INT_MAX;

    for (const auto& seed : seeds)
    {
        size_t seed_location = seed;

        for (const auto& map : range_maps)
        {
            for (const auto& range : map)
            {
                 if (seed_location >= range.source_start && seed_location <= range.source_start + range.range_size)
                 {
                    seed_location = range.destination_start + (seed_location - range.source_start);
                    break;
                 }
                 else
                 {
                    // Seed location is unchanged (i. identity mapping).
                 }
            }
        }    

        std::cout << "Seed " << seed << " -> " << seed_location << '\n';
        result = std::min(result, seed_location);
    }

    std::cout << "Result :: " << result << '\n';
    return EXIT_SUCCESS;
}