#include "gtest/gtest.h"
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <ranges>
#include <range/v3/all.hpp>

using Trees = std::vector<std::vector<uint>>;

auto parse_input(std::filesystem::path cmd_output) {
    Trees t;

    std::ifstream fs;
    fs.open(cmd_output, std::ios::in);
    assert(fs.is_open());

    for (std::string line; std::getline(fs, line);) {
        t.emplace_back();
        for (int c : line) {
            t.back().push_back(c-48);
        }
    }

    return t;
}

bool is_visible(Trees& t, std::size_t i, std::size_t j) {
    std::size_t up    = i;
    std::size_t left  = j;
    std::size_t down  = t.size()-i-1;
    std::size_t right = t.front().size()-j-1;

    std::vector<uint> tree_line_up(up);
    std::vector<uint> tree_line_left(left);
    std::vector<uint> tree_line_down(down);
    std::vector<uint> tree_line_right(right);

    std::ranges::generate_n(tree_line_up.begin(),    up,    [&t, i, j]() mutable { return t[--i][j];});
    std::ranges::generate_n(tree_line_left.begin(),  left,  [&t, i, j]() mutable { return t[i][--j];});
    std::ranges::generate_n(tree_line_down.begin(),  down,  [&t, i, j]() mutable { return t[++i][j];});
    std::ranges::generate_n(tree_line_right.begin(), right, [&t, i, j]() mutable { return t[i][++j];});

    return (t[i][j] > *std::ranges::max_element(tree_line_up))
        || (t[i][j] > *std::ranges::max_element(tree_line_left))
        || (t[i][j] > *std::ranges::max_element(tree_line_down))
        || (t[i][j] > *std::ranges::max_element(tree_line_right));
}

uint count_trees(Trees& t) {
    uint outside_trees_count = t.size()*2 + t.front().size()*2 - 4;
    uint inside_trees_count = 0;

    for (std::size_t i = 1; i < t.size() - 1; ++i) {
        for (std::size_t j = 1; j < t.front().size() - 1; ++j) {
            if (is_visible(t, i, j))
                inside_trees_count++;
        }
    }
    return outside_trees_count + inside_trees_count;
}

struct count_until {
    count_until(uint l):stop(false),l(l){};
    bool operator()(uint t) {
        if (stop) return false;
        if (t >= l) stop = true;
        return true;
    }
private:
    bool stop;
    uint l;
};

uint det_vis(Trees& t, std::size_t i, std::size_t j) {
    std::size_t up    = i;
    std::size_t left  = j;
    std::size_t down  = t.size()-i-1;
    std::size_t right = t.front().size()-j-1;

    std::vector<uint> tree_line_up(up);
    std::vector<uint> tree_line_left(left);
    std::vector<uint> tree_line_down(down);
    std::vector<uint> tree_line_right(right);

    std::ranges::generate_n(tree_line_up.begin(),    up,    [&t, i, j]() mutable { return t[--i][j];});
    std::ranges::generate_n(tree_line_left.begin(),  left,  [&t, i, j]() mutable { return t[i][--j];});
    std::ranges::generate_n(tree_line_down.begin(),  down,  [&t, i, j]() mutable { return t[++i][j];});
    std::ranges::generate_n(tree_line_right.begin(), right, [&t, i, j]() mutable { return t[i][++j];});

    auto th = t[i][j];

    return std::ranges::count_if(tree_line_up   , count_until(th))
         * std::ranges::count_if(tree_line_left , count_until(th))
         * std::ranges::count_if(tree_line_down , count_until(th))
         * std::ranges::count_if(tree_line_right, count_until(th));
}

uint check_visiblity(Trees& t) {
    std::vector<uint> tree_visiblity;
    for (std::size_t i = 1; i < t.size() - 1; ++i) {
        for (std::size_t j = 1; j < t.front().size() - 1; ++j) {
            tree_visiblity.push_back(det_vis(t, i, j));
        }
    }

    return *std::ranges::max_element(tree_visiblity);
}

TEST(Day08, Part1_example) {
    auto input = parse_input("day08/example_input.txt");

    ASSERT_EQ(21, count_trees(input));
}

TEST(Day08, Part1) {
    auto input = parse_input("day08/puzzle_input.txt");
    
    ASSERT_EQ(1679, count_trees(input));
}

TEST(Day08, Part2_example) {
    auto input = parse_input("day08/example_input.txt");

    ASSERT_EQ(8, check_visiblity(input));
}

TEST(Day08, Part2) {
    auto input = parse_input("day08/puzzle_input.txt");

    ASSERT_EQ(536625, check_visiblity(input));
}