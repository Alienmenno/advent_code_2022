#include "gtest/gtest.h"
#include <deque>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <ranges>
#include <range/v3/all.hpp>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

struct CraneAction {
    int crate_count;
    int source_id;
    int target_id;
};
std::ostream& operator<< (std::ostream& os, const CraneAction& ca) {
    return os << '{' << ca.crate_count << ',' << ca.source_id << ',' << ca.target_id << '}';
}
template <> struct fmt::formatter<CraneAction> : ostream_formatter {};


using CratesStacks = std::vector<std::deque<char>>;
using Logistics = std::pair<CratesStacks, std::vector<CraneAction>>;

auto parse_logistics_request(std::filesystem::path logistics_order) {
    Logistics logistics;

    std::ifstream fs;
    fs.open(logistics_order, std::ios::in);
    assert(fs.is_open());

    for (std::string line; std::getline(fs, line);) {
        if (!line.empty()) {
            for (auto [i, crate] : ranges::views::enumerate(line | ranges::views::chunk(4))) {
                char c = crate[1];
                if (c == '1') break;
                if (logistics.first.size() <= i)
                    logistics.first.push_back(std::deque<char>());
                if (c != ' ')
                    logistics.first.at(i).push_back(c);
            }
        }
        else break;
    }
    // fmt::print("CratesStacks: {}\n", logistics.first);

    for (std::string line; std::getline(fs, line);) {
        std::istringstream sline(line);
        CraneAction ca;
        sline >> line >> ca.crate_count >> line >> ca.source_id >> line >> ca.target_id;
        logistics.second.push_back(ca);
    }
    // fmt::print("CraneActions: {}\n", logistics.second);

    return logistics;
}

char peek_pop(std::deque<char>& s) {
    char c = s.front();
    s.pop_front();
    return c;
}

std::string perform_logistic_operations_9000(Logistics& logistics) {
    // Execute logistics
    for (auto cmd : logistics.second) {
        for (int i = 0; i < cmd.crate_count; ++i) {
            logistics.first.at(cmd.target_id-1).push_front(
                peek_pop(logistics.first.at(cmd.source_id-1)));
        }
    }
    // fmt::print("CratesStacks: {}\n", logistics.first);

    return ranges::accumulate(
        logistics.first,
        std::string(),
        [](std::string a, auto s) {
            return std::move(a) + s.front();
        });
}

std::string perform_logistic_operations_9001(Logistics& logistics) {
    // Execute logistics
    for (auto cmd : logistics.second) {
        auto& target = logistics.first.at(cmd.target_id-1);
        auto& source = logistics.first.at(cmd.source_id-1);
        target.insert(target.begin(), source.begin(), source.begin() + cmd.crate_count);
        for (int i = 0; i < cmd.crate_count; ++i) {
            source.pop_front();
        }
    }
    // fmt::print("CratesStacks: {}\n", logistics.first);

    return ranges::accumulate(
        logistics.first,
        std::string(),
        [](std::string a, auto s) {
            return std::move(a) + s.front();
        });
}

TEST(Day05, Part1_example) {
    auto logistics = parse_logistics_request("day05/example_input.txt");

    ASSERT_EQ("CMZ", perform_logistic_operations_9000(logistics));
}

TEST(Day05, Part1) {
    auto logistics = parse_logistics_request("day05/puzzle_input.txt");

    ASSERT_EQ("QNHWJVJZW", perform_logistic_operations_9000(logistics));
}

TEST(Day05, Part2_example) {
    auto logistics = parse_logistics_request("day05/example_input.txt");

    ASSERT_EQ("MCD", perform_logistic_operations_9001(logistics));
}

TEST(Day05, Part2) {
    auto logistics = parse_logistics_request("day05/puzzle_input.txt");

    ASSERT_EQ("BPCZJLFJW", perform_logistic_operations_9001(logistics));
}