#include "gtest/gtest.h"
#include <fstream>
#include <tuple>
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

Marker find_marker(const std::string_view s, const std::size_t w_size = 4) {
    for (auto [i, w] : ranges::views::enumerate(s | ranges::views::sliding(w_size))) {
        if (std::set<char>(w.begin(), w.end()).size() == w_size)
            return i + w_size;
    }
    return 0;
}

//------------------------------------------------------------------------------
using TestSet = std::tuple<int, std::string, std::size_t>;

class Example : public testing::TestWithParam<TestSet> {};
class  Puzzle : public testing::TestWithParam<TestSet> {};

TEST_P(Example, find_marker) {
    auto [answer, packet, width] = GetParam();
    ASSERT_EQ(answer, find_marker(packet, width));
}

TEST_P( Puzzle, find_marker) {
    auto [answer, packet_file, width] = GetParam();

    ASSERT_EQ(answer, find_marker(get_file(packet_file), width));
}

INSTANTIATE_TEST_SUITE_P(
    Day06,
    Example,
    testing::Values(
        // Part 1
        std::tuple( 7, "mjqjpqmgbljsphdztnvjfqwrcgsmlb",     4),
        std::tuple( 5, "bvwbjplbgvbhsrlpgdmjqwftvncz",       4),
        std::tuple( 6, "nppdvjthqldpwncqszvftbrmjlhg",       4),
        std::tuple(10, "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg",  4),
        std::tuple(11, "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw",   4),
        // Part 2
        std::tuple(19, "mjqjpqmgbljsphdztnvjfqwrcgsmlb",    14),
        std::tuple(23, "bvwbjplbgvbhsrlpgdmjqwftvncz",      14),
        std::tuple(23, "nppdvjthqldpwncqszvftbrmjlhg",      14),
        std::tuple(29, "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 14),
        std::tuple(26, "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw",  14)
    )
);

INSTANTIATE_TEST_SUITE_P(
    Day06,
    Puzzle,
    testing::Values(
        std::tuple(1356, "day06/puzzle_input.txt",  4), // Part 1
        std::tuple(2564, "day06/puzzle_input.txt", 14)  // Part 2
    )
);
