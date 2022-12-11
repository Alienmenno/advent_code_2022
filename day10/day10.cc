#include "gtest/gtest.h"
#include <fstream>
#include <algorithm>
#include <numeric>
#include <filesystem>
#include <ranges>
#include <range/v3/all.hpp>
#include <fmt/ranges.h>

enum OP {
    NOOP,
    ADDX,
};

using Instr = std::pair<OP, int>;
using Program = std::vector<Instr>;

std::istream &operator>>(std::istream &input, OP &o) {
    std::string s;
    input >> s;
    if (s == "noop") o = OP::NOOP;
    if (s == "addx") o = OP::ADDX;
    return input;
}

std::istream &operator>>(std::istream &i, Instr &inst) {
    i >> inst.first >> inst.second;
    return i;
}

auto parse_input(std::filesystem::path cmd_output) {
    Program p;

    std::ifstream fs;
    fs.open(cmd_output, std::ios::in);
    assert(fs.is_open());

    for (std::string line; std::getline(fs, line);) {
        std::istringstream ss(line);
        Instr i;
        ss >> i;
        p.push_back(i);
    }

    return p;
}

auto check_freq(Program& p, std::vector<int> polling_freq) {
    int X = 1;
    int cycle = 1;
    std::vector<int> signal_strengths;
    for (auto [cmd, v] : p) {
        int inst_cycle = cycle + 1;
        if (cmd == OP::ADDX) inst_cycle++;
        for (; cycle < inst_cycle; ++cycle) {
            if (std::ranges::find(polling_freq, cycle) != polling_freq.end()) {
                signal_strengths.push_back(X * cycle);
            }
        }
        X += v;
    }

    return ranges::accumulate(signal_strengths, 0);
}

using CrtOutput = std::vector<std::string>;

auto check_display(Program& p, std::vector<int> polling_freq) {
    int X = 1;
    int cycle = 0;
    CrtOutput crt_display;
    std::string CRT_line;
    for (auto [cmd, v] : p) {
        int inst_cycle = cycle + 1;
        if (cmd == OP::ADDX) inst_cycle++;
        for (; cycle < inst_cycle; ++cycle) {
            if (std::ranges::find(polling_freq, cycle) != polling_freq.end()) {
                crt_display.push_back(CRT_line);
                CRT_line = "";
            }
            if ((cycle % 40) >= (X - 1) && (cycle % 40) <= (X + 1)) CRT_line += '#';
            else CRT_line += '.';
            // fmt::print("c:{:3}, X:{:3} CRT:{}\n", cycle, X, CRT_line);
        }
        X += v; // addx
    }
    crt_display.push_back(CRT_line);
    return crt_display;
}

TEST(day10, Part1_example) {
    auto input = parse_input("day10/example_input.txt");

    ASSERT_EQ(13140, check_freq(input, {20, 60, 100, 140, 180, 220}));
}

TEST(day10, Part1) {
    auto input = parse_input("day10/puzzle_input.txt");
    
    ASSERT_EQ(14340, check_freq(input, {20, 60, 100, 140, 180, 220}));
}

TEST(day10, Part2_example) {
    auto input = parse_input("day10/example_input.txt");

    auto crt_display = check_display(input, {40, 80, 120, 160, 200, 240});

    fmt::print("\n{}\n", fmt::join(crt_display, "\n"));

    CrtOutput exp = {
        "##..##..##..##..##..##..##..##..##..##..",
        "###...###...###...###...###...###...###.",
        "####....####....####....####....####....",
        "#####.....#####.....#####.....#####.....",
        "######......######......######......####",
        "#######.......#######.......#######....."
    };

    ASSERT_EQ(exp, crt_display);
}

TEST(day10, Part2) {
    auto input = parse_input("day10/puzzle_input.txt");

    auto crt_display = check_display(input, {40, 80, 120, 160, 200, 240});

    fmt::print("\n{}\n", fmt::join(crt_display, "\n"));

    CrtOutput exp = {
        "###...##..###....##..##..###..#..#.###..",
        "#..#.#..#.#..#....#.#..#.#..#.#..#.#..#.",
        "#..#.#..#.#..#....#.#....###..####.#..#.",
        "###..####.###.....#.#....#..#.#..#.###..",
        "#....#..#.#....#..#.#..#.#..#.#..#.#....",
        "#....#..#.#.....##...##..###..#..#.#...."
    };

    ASSERT_EQ(exp, crt_display);
}