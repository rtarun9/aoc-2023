#include "../common/file_utils.hpp"
#include "preprocessing.hpp"

#include <unordered_map>
#include <algorithm>
#include <numeric>

int main()
{
    const auto file_res = common::read_file_by_line("input.txt");
    if (!file_res.has_value())
    {
        return EXIT_FAILURE;
    }

    const auto card_datas = preprocessing::preprocess_input(file_res.value());

    // Logic : Use a hashmap based approach. First add all the winning numbers to the hash map,
    // and then find number of matches in the card numbers present in the card.
    // Compared to the brute force solution, I will be used a single vector of card_ids.
    // Note that once card_id 'x' has been traversed, it will not be travered in the future again.
    // So, for card_id 'x', if Y matches are found, we can just traverse from index x to y, and add freq[x].
    // This will make the solution O(n) + O(n * n), which is bad but atleast inplace.
    std::vector<int> cards(card_datas.size(), 1);

    for (int i = 0; i < cards.size(); i++)
    {
        const auto current_card_id = i;

        const auto card_data = card_datas.at(current_card_id);

        {
            size_t winning_number_matches = 0;
            std::unordered_map<int, int> winning_number_to_count_map{};

            for (const auto &winning_number : card_data.winning_numbers)
            {
                winning_number_to_count_map[winning_number]++;
            }

            for (const auto &card_number : card_data.card_numbers)
            {
                if (winning_number_to_count_map.contains(card_number))
                {
                    winning_number_matches++;
                }
            }

            if (i != cards.size() - 1)
            {
                for (int k = 1; k <= winning_number_matches; k++)
                {
                    cards[current_card_id + k] += cards[current_card_id];
                }
            }
        }
    }

    size_t result = std::accumulate(std::begin(cards), std::end(cards), (size_t)0);
    std::cout << "Result :: " << result << '\n';

    return EXIT_SUCCESS;
}