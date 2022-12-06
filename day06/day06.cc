#include "gtest/gtest.h"
#include <fstream>
#include <filesystem>
#include <range/v3/all.hpp>

using Marker = int;

std::string get_file(std::filesystem::path file_path) {
    std::ifstream fs;
    fs.open(file_path, std::ios::in);
    assert(fs.is_open());

    std::string s;
    std::getline(fs, s);
    return s;
}

Marker find_marker(std::string_view s, std::size_t w_size = 4) {
    for (auto [i, w] : ranges::views::enumerate(s | ranges::views::sliding(w_size))) {
        if (std::set<char>(w.begin(), w.end()).size() == w_size)
            return i + w_size;
    }
    return 0;
}

TEST(Day06, Part1_example_1) {
    ASSERT_EQ(7, find_marker("mjqjpqmgbljsphdztnvjfqwrcgsmlb"));
}

TEST(Day06, Part1_example_2) {
    ASSERT_EQ(5, find_marker("bvwbjplbgvbhsrlpgdmjqwftvncz"));
}

TEST(Day06, Part1_example_3) {
    ASSERT_EQ(6, find_marker("nppdvjthqldpwncqszvftbrmjlhg"));
}

TEST(Day06, Part1_example_4) {
    ASSERT_EQ(10, find_marker("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg"));
}

TEST(Day06, Part1_example_5) {
    ASSERT_EQ(11, find_marker("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"));
}

TEST(Day06, Part1) {
    std::string s = get_file("day06/puzzle_input.txt");

    ASSERT_EQ(1356, find_marker(s));
}

TEST(Day06, Part2_example_1) {
    ASSERT_EQ(19, find_marker("mjqjpqmgbljsphdztnvjfqwrcgsmlb", 14));
}

TEST(Day06, Part2_example_2) {
    ASSERT_EQ(23, find_marker("bvwbjplbgvbhsrlpgdmjqwftvncz", 14));
}

TEST(Day06, Part2_example_3) {
    ASSERT_EQ(23, find_marker("nppdvjthqldpwncqszvftbrmjlhg", 14));
}

TEST(Day06, Part2_example_4) {
    ASSERT_EQ(29, find_marker("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 14));
}

TEST(Day06, Part2_example_5) {
    ASSERT_EQ(26, find_marker("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 14));
}

TEST(Day06, Part2) {
    std::string s = get_file("day06/puzzle_input.txt");

    ASSERT_EQ(2564, find_marker(s, 14));
}