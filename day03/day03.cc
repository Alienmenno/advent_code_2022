#include "gtest/gtest.h"
#include <string>
#include <fstream>
#include <iostream>
#include <charconv>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <filesystem>
#include <cctype>
#include <range/v3/all.hpp>

using RucksackPockets = std::pair<std::string, std::string>;
using Rucksacks = std::vector<std::string>;
using Priority = int;
using Badges = int;

Rucksacks parse_rucksacks(std::filesystem::path rucksacks_inventory)
{
    std::fstream fs;
    fs.open(rucksacks_inventory, std::ios::in);
    assert(fs.is_open());

    Rucksacks rucksacks{};

    for (std::string line; std::getline(fs, line);) {
        rucksacks.push_back(line);
    }
    
    return rucksacks;
}

Priority det_prio(char c) {
    if (std::isupper(c))
        return c - 'A' + 27;
    else
        return c - 'a' + 1;
}

Priority find_priorities(Rucksacks& rucksacks) {
    Priority p = 0;

    for (auto rucksack : rucksacks) {
        auto mid = rucksack.size() / 2;
        auto pocket1 = rucksack.substr(0, mid);
        auto pocket2 = rucksack.substr(mid);
        std::ranges::sort(pocket1);
        std::ranges::sort(pocket2);
        
        std::set<char> intersect;
        std::ranges::set_intersection(
            pocket1,
            pocket2,
            std::inserter(intersect, intersect.begin()));
        
        p += det_prio(*intersect.begin());
    }

    return p;
}

Badges find_badges(Rucksacks& rucksacks) {
    Badges b = 0;

    for (auto elfgroup : rucksacks | ranges::views::chunk(3)) {
        
        std::set<char> intersect;

        std::ranges::set_intersection(
            std::move(elfgroup.at(0)) | ranges::action::sort,
            std::move(elfgroup.at(1)) | ranges::action::sort,
            std::inserter(intersect, intersect.begin()));
        std::set<char> intersect2;
        std::ranges::set_intersection(
            intersect,
            std::move(elfgroup.at(2)) | ranges::action::sort,
            std::inserter(intersect2, intersect2.begin()));

        b += det_prio(*intersect2.begin());
    }

    return b;
}

TEST(Day03, Part1_example) {
    auto rucksacks = parse_rucksacks("day03/example_input.txt");

    ASSERT_EQ(157, find_priorities(rucksacks));
}

TEST(Day03, Part1) {
    auto rucksacks = parse_rucksacks("day03/puzzle_input.txt");

    ASSERT_EQ(7997, find_priorities(rucksacks));
}

TEST(Day03, Part2_example) {
    auto rucksacks = parse_rucksacks("day03/example_input.txt");

    ASSERT_EQ(70, find_badges(rucksacks));
}

TEST(Day03, Part2) {
    auto rucksacks = parse_rucksacks("day03/puzzle_input.txt");

    ASSERT_EQ(2545, find_badges(rucksacks));
}