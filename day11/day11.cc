#include "gtest/gtest.h"
#include <fstream>
#include <algorithm>
#include <numeric>
#include <filesystem>
#include <ranges>
#include <regex>
#include <range/v3/all.hpp>
#include <fmt/ranges.h>

enum OP {
    ADD,
    MUL,
    SQRT
};

using Items = std::vector<int>;
using OPS = std::pair<OP, int>;

struct Monkey {
    Items items;
    OPS op;
    int tv;
    int t_true;
    int t_false;
    uint i_count;
};

using MonkeyGroup = std::vector<Monkey>;

/**
 * Parsing:
 * Monkey 0:
 *   Starting items: 79, 98
 *   Operation: new = old * 19
 *   Test: divisible by 23
 *     If true: throw to monkey 2
 *     If false: throw to monkey 3
*/
std::istream &operator>>(std::istream &input, Monkey &m) {

    std::string l;
    std::smatch match;

    std::getline(input, l); // Skip First line
    // if (std::regex_search(l, match, std::regex("\\w+ (\\d+):")))
    //     std::cout << "Monkey: " << match[1];

    std::getline(input, l);
    if (std::regex_search(l, match, std::regex("[a-zA-z :]+((?:\\d+,? ?)+)"))) {
        std::istringstream ss(match[1]);
        while(!ss.eof()) {
            int i;
            ss >> i;
            if (!ss.eof()) ss.seekg(2, std::ios_base::cur);
            m.items.push_back(i);
        }
        // fmt::print("{}\n", m.items);
    }

    std::getline(input, l);
    if (std::regex_search(l, match, std::regex("[a-zA-z :=]+(.) (\\S+)"))) {
        // std::cout << match[1] << ' ' << match[2] << '\n';
        if (match[2] == "old") m.op.first = OP::SQRT;
        else if (match[1] == '*') m.op = {OP::MUL, std::stoi(match[2])};
        else m.op = {OP::ADD, std::stoi(match[2])};
        // fmt::print("{}\n", m.op);
    }

    std::getline(input, l);
    if (std::regex_search(l, match, std::regex("[a-zA-z :]+(\\d+)")))
        m.tv = std::stoi(match[1]);

    std::getline(input, l);
    if (std::regex_search(l, match, std::regex("[a-zA-z :]+(\\d+)")))
        m.t_true = std::stoi(match[1]);

    std::getline(input, l);
    if (std::regex_search(l, match, std::regex("[a-zA-z :]+(\\d+)")))
        m.t_false = std::stoi(match[1]);

    std::getline(input, l);

    return input;
}

auto parse_input(std::filesystem::path cmd_output) {
    MonkeyGroup ms;

    std::ifstream fs;
    fs.open(cmd_output, std::ios::in);
    assert(fs.is_open());

    while(!fs.eof()) {
        Monkey m;
        fs >> m;
        ms.push_back(m);
    }

    return ms;
}

void monkeying_around(MonkeyGroup &monkeys) {
    for (auto &m : monkeys) {
        for (auto item : m.items) {
            switch (m.op.first) {
                case OP::ADD:  item += m.op.second; break;
                case OP::MUL:  item *= m.op.second; break;
                case OP::SQRT: item *= item; break;
                default: break;
            }
            item /= 3;
            if ((item % m.tv) == 0) monkeys.at(m.t_true).items.push_back(item);
            else monkeys.at(m.t_false).items.push_back(item);

            m.i_count++;
        }
        m.items.clear();
    }
}

auto check(MonkeyGroup &monkeys, std::size_t n_rounds) {
    for (std::size_t i = 0; i < n_rounds; ++i) {
        monkeying_around(monkeys);
        fmt::print("Round: {}\n", i);
        for (auto m : monkeys) {
            fmt::print("items: {}, i_count: {}\n", m.items, m.i_count);
        }
    }

    std::ranges::sort(monkeys, [](auto a, auto b){return a.i_count > b.i_count;});

    return monkeys.begin()->i_count * (++monkeys.begin())->i_count;
}

TEST(day11, Part1_example) {
    auto input = parse_input("day11/example_input.txt");

    ASSERT_EQ(10605, check(input, 20));
}

TEST(day11, Part1) {
    auto input = parse_input("day11/puzzle_input.txt");
    
    ASSERT_EQ(51075, check(input, 20));
}

TEST(day11, Part2_example) {
    auto input = parse_input("day11/example_input.txt");

    ASSERT_EQ(2713310158, check(input, 10000));
}

// TEST(day11, Part2) {
//     auto input = parse_input("day11/puzzle_input.txt");
    
//     ASSERT_EQ(10605, check(input));
// }