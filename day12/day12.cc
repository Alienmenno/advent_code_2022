#include "gtest/gtest.h"
#include <fstream>
#include <algorithm>
#include <numeric>
#include <filesystem>
#include <ranges>
#include <regex>
#include <functional>
#include <range/v3/all.hpp>
#include <fmt/ranges.h>

using HeightMap = std::vector<std::vector<char>>;
using Coord = std::pair<std::size_t, std::size_t>;
using Path = std::vector<Coord>;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

auto parse_input(std::filesystem::path cmd_output) {
    HeightMap hm;

    std::ifstream fs;
    fs.open(cmd_output, std::ios::in);
    assert(fs.is_open());

    for (std::string line; std::getline(fs, line);) {
        hm.emplace_back(line.begin(), line.end());
    }

    return hm;
}

Coord det_pos(const HeightMap &hm, const char T) {
    for (auto [i, row] : ranges::views::enumerate(hm)) {
        auto it = std::ranges::find(row, T);

        if (it != row.end())
            return {i, std::distance(row.begin(), it)};
    }

    return {0, 0};
}

std::pair<Coord, Coord> det_start_end(const HeightMap &hm) {
    // Determine Start by 'S' and end by 'E'
    return {det_pos(hm, 'S'),  det_pos(hm, 'E')};
}

bool possible_path(const HeightMap &hm, const Coord c, const Direction dir) {
    const char cur = hm[c.first][c.second];
    bool start = (cur == 'S');
    const std::size_t rl = hm.size() - 1;
    const std::size_t cl = hm.front().size() - 1;
    switch (dir) {
        case UP:
            return (c.first  < rl) && (start || (hm[c.first+1][c.second  ] <= (cur + 1)));
        break;
        case DOWN:
            return (c.first  > 0 ) && (start || (hm[c.first-1][c.second  ] <= (cur + 1)));
        break;
        case LEFT:
            return (c.second > 0 ) && (start || (hm[c.first  ][c.second-1] <= (cur + 1)));
        break;
        case RIGHT:
            return (c.second < cl) && (start || (hm[c.first  ][c.second+1] <= (cur + 1)));
        break;
        default: return false;
    }
    // return d <= (c + 1) || hm[c.first][c.second] == 'S';
}

auto det_shortest_path(HeightMap hm, Coord s, Coord e) -> Path {
    // fmt::print("{}\n", possible_path('z', 'E'));

    if (s == e) return {s};

    std::array<Path, 4> options;
    HeightMap nhm = hm;
    nhm[s.first][s.second] = '~';

    if (possible_path(hm, s, RIGHT)) {
        options[0] = det_shortest_path(nhm, {s.first,   s.second+1}, e);
    }
    if (possible_path(hm, s, UP)) {
        options[1] = det_shortest_path(nhm, {s.first+1, s.second  }, e);
    }
    if (possible_path(hm, s, DOWN)) {
        options[2] = det_shortest_path(nhm, {s.first-1, s.second  }, e);
    }
    if (possible_path(hm, s, LEFT)) {
        options[3] = det_shortest_path(nhm, {s.first,   s.second-1}, e);
    }

    // fmt::print("cur: {}, e: {}\n", s, e);
    // fmt::print("{}\n", fmt::join(hm, "\n"));
    // fmt::print("Options:\n{}\n", fmt::join(options, "\n"));

    auto p = std::ranges::min_element(options, [e](Path &a, Path &b) {
        return (a.size() != 0 && a.front() == e && a.size() < b.size()) || b.size() == 0 || b.front() != e;
    });
    p->push_back(s);
    // fmt::print("path: {}\n", *p);
    return *p;
}

auto check(HeightMap &hm) {
    auto [s, e] = det_start_end(hm);

    fmt::print("S: {}-{}, E: {}-{}\n", s, hm[s.first][s.second], e, hm[e.first][e.second]);
    hm[e.first][e.second] = '{';

    auto path = det_shortest_path(hm, s, e);

    fmt::print("path: {}\n", path);

    return path.size() - 1;
}

TEST(day12, Part1_example) {
    auto input = parse_input("day12/example_input.txt");

    fmt::print("{}\n", fmt::join(input, "\n"));

    ASSERT_EQ(31, check(input));
}

TEST(day12, Part1) {
    auto input = parse_input("day12/puzzle_input.txt");
    
    ASSERT_EQ(31, check(input));
}

// TEST(day12, Part2_example) {
//     auto input = parse_input("day12/example_input.txt");

//     ASSERT_EQ(31, check(input));
// }

// TEST(day12, Part2) {
//     auto input = parse_input("day12/puzzle_input.txt");

//     ASSERT_EQ(31, check(input));
// }