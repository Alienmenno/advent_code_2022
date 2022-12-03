#include "gtest/gtest.h"
#include <string>
#include <fstream>
#include <iostream>
#include <charconv>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <filesystem>

using FoodItem = int;
using Snacks = std::vector<FoodItem>;

struct ElfBackpack {
    Snacks food;
};

using ElfGroup = std::vector<ElfBackpack>;

ElfGroup gather_food_items(std::filesystem::path list_of_food)
{
    std::fstream fs;
    fs.open(list_of_food, std::ios::in);
    assert(fs.is_open());

    ElfGroup elfs{};
    Snacks snacks_for_elf{};

    for (std::string line; std::getline(fs, line);) {
        if (line == "") {
            elfs.push_back(ElfBackpack{snacks_for_elf});
            snacks_for_elf.clear();
        }
        else {
            int i = 0;
            std::from_chars(line.data(), line.data() + line.size(), i);
            snacks_for_elf.push_back(i);
        }
    }

    // add last elf
    elfs.push_back(ElfBackpack{snacks_for_elf});

    return elfs;
}

int find_most_calories(ElfGroup& elfs) {
    return std::ranges::max(elfs | std::views::transform([](ElfBackpack elf) {
        return std::accumulate(elf.food.begin(), elf.food.end(), 0);
    }));
}

int find_top_3_callories(ElfGroup& elfs) {
    std::vector<int> callorie_counts;
    std::ranges::transform(elfs, std::back_inserter(callorie_counts), [](ElfBackpack elf) {
        return std::accumulate(elf.food.begin(), elf.food.end(), 0);
    });
    std::ranges::sort(callorie_counts, std::ranges::greater());
    auto top3 = std::ranges::take_view{callorie_counts, 3};
    return std::accumulate(top3.begin(), top3.end(), 0);
}

TEST(Day01, Part1_example) {
    auto elf_group = gather_food_items("day01/example_input.txt");

    ASSERT_EQ(24000, find_most_calories(elf_group));
}

TEST(Day01, Part1) {
    auto elf_group = gather_food_items("day01/puzzle_input.txt");

    ASSERT_EQ(72070, find_most_calories(elf_group));
}

TEST(Day01, Part2_example) {
    auto elf_group = gather_food_items("day01/example_input.txt");

    ASSERT_EQ(45000, find_top_3_callories(elf_group));
}

TEST(Day01, Part2) {
    auto elf_group = gather_food_items("day01/puzzle_input.txt");

    ASSERT_EQ(211805, find_top_3_callories(elf_group));
}