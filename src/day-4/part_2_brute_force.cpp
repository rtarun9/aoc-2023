#include "../common/file_utils.hpp"
#include "preprocessing.hpp"

#include <unordered_map>
#include <stack>

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
    // For card X, if we won Y cards, we add to a stack X + 1, X + 2, .... Y.
    // The output of the program is the number of scratch cards. For this, we can use a stack based solution for brute force.
    // For simplicity, items are added in reverse order to the card_data to game 1 lies at the top.
    std::stack<int> card_ids{};

    for (int i = card_datas.size() - 1; i >= 0; --i)
    {
        card_ids.emplace(card_datas.at(i).card_id);
    }

    size_t result = 0;
    while (!card_ids.empty())
    {
        const auto current_card_id = card_ids.top();
        card_ids.pop();

        const auto card_data = card_datas.at(current_card_id - 1);

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

            for (int i = 1; i <= winning_number_matches; i++)
            {
                card_ids.push(current_card_id + i);
            }
        }

        ++result;
    }

    std::cout << "Result :: " << result << '\n';

    return EXIT_SUCCESS;
}