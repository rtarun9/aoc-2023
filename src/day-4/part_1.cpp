#include "../common/file_utils.hpp"
#include "preprocessing.hpp"
#include <unordered_map>

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
    
    size_t result = 0;
    for (const auto& card_data : card_datas)
    {
        size_t winning_number_matches = 0;
        std::unordered_map<int, int> winning_number_to_count_map{}; 
        
        for (const auto& winning_number : card_data.winning_numbers)
        {
            winning_number_to_count_map[winning_number]++;
        }

        for (const auto& card_number : card_data.card_numbers)
        {
            if (winning_number_to_count_map.contains(card_number))
            {
                winning_number_matches++;
            }
        }

        std::cout << winning_number_matches << '\n';
        if (winning_number_matches > 0)
        {
            result += pow(2, winning_number_matches - 1);
        }
    }

    std::cout << "Result :: " << result << '\n';

    return EXIT_SUCCESS;
}