// As per the question, the 'strength of cards' are in the following order (from strongest to weakest)
// A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, 2.

// Each hand is a collection of cards (5). The strength depends on the frequency of occurance of each card.
/*
    Five of a kind, where all five cards have the same label: AAAAA
    Four of a kind, where four cards have the same label and one card has a different label: AA8AA
    Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
    Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
    Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
    One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
    High card, where all cards' labels are distinct: 23456

*/
// If two hands have same strength, then from first to last check with individual card strength.

#include <array>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>

#include "../common/file_utils.hpp"

int main()
{
    const auto file_res = common::read_file_by_line("input.txt");
    if (!file_res.has_value())
    {
        return EXIT_FAILURE;
    }

    auto card_strenghts = std::map<char, int>{};
    card_strenghts['A'] = 12;
    card_strenghts['K'] = 11;
    card_strenghts['Q'] = 10;
    card_strenghts['J'] = -1;
    card_strenghts['T'] = 8;
    card_strenghts['9'] = 7;
    card_strenghts['8'] = 6;
    card_strenghts['7'] = 5;
    card_strenghts['6'] = 4;
    card_strenghts['5'] = 3;
    card_strenghts['4'] = 2;
    card_strenghts['3'] = 1;
    card_strenghts['2'] = 0;

    const auto get_hand_type = [&](const auto &hand) -> int
    {
        std::unordered_map<char, int> freq_map{};
        for (const auto &h : hand)
        {
            freq_map[h]++;
        }

        char max_occuring_char = ' ';
        int max = 0;
        for (const auto &[k, v] : freq_map)
        {
            if (v >= max && k != 'J')
            {
                max = v;
                max_occuring_char = k;
            }
        }

        auto s = hand;
        for (int i = 0; i < 5; i++)
            if (s[i] == 'J' && max_occuring_char != 'J')
                s[i] = max_occuring_char;

        freq_map.clear();
        for (const auto &a : s)
            freq_map[a]++;

        if (freq_map.size() == 1)
        {
            return 6;
        }

        if (freq_map.size() == 2)
        {
            for (const auto &[key, value] : freq_map)
            {
                if (value == 4)
                {
                    return 5;
                }

                if (value == 3)
                {
                    return 4;
                }
            }
        }

        if (freq_map.size() == 3)
        {
            for (const auto &[key, value] : freq_map)
            {
                if (value == 3)
                {
                    return 3;
                }

                if (value == 2)
                {
                    return 2;
                }
            }
        }

        if (freq_map.size() == 4)
        {
            return 1;
        }

        return 0;
    };

    // Comparing cards with the same hand.
    const auto comparator = [&](const auto &a, const auto &b) -> bool
    {
        const auto x = get_hand_type(a.first);
        const auto y = get_hand_type(b.first);

        if (x != y)
            return x > y;

        for (int i = 0; i < 5; i++)
        {
            if (a.first[i] == b.first[i])
                continue;

            return card_strenghts[a.first[i]] > card_strenghts[b.first[i]];
        }

        return true;
    };

    // Getting the input.
    std::vector<std::pair<std::string, std::string>> card_bid_pair{};
    for (const auto &ip : *file_res)
    {
        auto _ip = common::split_string_by_space(ip);

        card_bid_pair.push_back(
            {_ip[0], _ip[1]});
    }

    std::sort(card_bid_pair.begin(), card_bid_pair.end(), comparator);

    size_t result{};
    int i = 0;
    for (const auto &a : card_bid_pair)
    {
        result += std::stoi(a.second) * (card_bid_pair.size() - i++);
        std::cout << a.first << " " << a.second << " " << get_hand_type(a.first) << "\n";
    }
    std::cout << "Result :: " << result << "\n";
    std::cout << card_bid_pair.size();
}