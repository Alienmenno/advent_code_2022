#include "gtest/gtest.h"
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <ranges>
#include <range/v3/all.hpp>
#include <fmt/ranges.h>

using Motions = std::vector<std::pair<char, int>>;

auto parse_input(std::filesystem::path cmd_output) {
    Motions m;

    std::ifstream fs;
    fs.open(cmd_output, std::ios::in);
    assert(fs.is_open());

    for (std::string line; std::getline(fs, line);) {
        std::istringstream ss(line);
        char d;
        int s;
        ss >> d;
        ss >> s;
        m.emplace_back(d, s);
    }

    return m;
}

using Location = std::pair<int,int>;

struct MoveGn {
    MoveGn(const char d, Location& loc):d(d),l(loc){};
    Location operator()() {
        if (d == 'U') l.first  += 1;
        if (d == 'D') l.first  -= 1;
        if (d == 'R') l.second += 1;
        if (d == 'L') l.second -= 1;

        return l;
    }
    const char d;
    Location& l;
};

struct Rope {
    Rope(){};
    Rope(Location loc):h(loc),t(loc){};
    Location operator()(Location m) {
        int r = (m.first - t.first);
        int c = (m.second - t.second);
        if (!(within_equal(r, -1, 1) && within_equal(c, -1, 1))) {
            if (r == 0) move_x(c);
            else if (c == 0) move_y(r);
            else {
                move_x(c);
                move_y(r);
            }
        }
        return t;
    }
    bool within_equal(int a, int b, int c) {return b <= a && a <= c;};
    void move_x(int i) {t.second += std::clamp(i, -1, 1);};
    void move_y(int i) {t.first += std::clamp(i, -1, 1);};
    Location h;
    Location t;
};

template<std::size_t Num>
struct Ropes {
    Ropes(Location loc):ropes(){};
    Location operator()(Location m) {
        fmt::print("\n");
        for (auto& rope : ropes) {
            m = rope(m);
        }

        return m;
    }
    std::array<Rope, Num> ropes;
};

template<std::size_t Num>
auto check_motion(Motions& m) {
    std::set<Location> t_cells;
    Location l = {0, 0};

    Ropes<Num> rope_fn(l);

    for (auto move_cmd : m) {
        std::vector<Location> move_locs(move_cmd.second);
        std::ranges::generate_n(move_locs.begin(), move_cmd.second, MoveGn(move_cmd.first, l));
        for (auto move : move_locs) {
            t_cells.insert(rope_fn(move));
        }
    }

    return t_cells.size();
}

TEST(day09, Part1_example) {
    auto input = parse_input("day09/example_input.txt");

    ASSERT_EQ(13, check_motion<1>(input));
}

TEST(day09, Part1) {
    auto input = parse_input("day09/puzzle_input.txt");
    
    ASSERT_EQ(6236, check_motion<1>(input));
}

TEST(day09, Part2_example) {
    auto input = parse_input("day09/example_input.txt");

    ASSERT_EQ(1, check_motion<9>(input));
}

TEST(day09, Part2_example2) {
    auto input = parse_input("day09/example_input_part2.txt");

    ASSERT_EQ(36, check_motion<9>(input));
}

TEST(day09, Part2) {
    auto input = parse_input("day09/puzzle_input.txt");

    ASSERT_EQ(2449, check_motion<9>(input));
}