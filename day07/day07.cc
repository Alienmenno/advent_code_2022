#include "gtest/gtest.h"
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <ranges>
#include <range/v3/all.hpp>

using FileData = std::pair<std::string, std::size_t>;
using Files = std::vector<FileData>;

struct DirStructure {
    std::string name;
    DirStructure *parent;
    std::vector<DirStructure> dir_list;
    Files file_list;
};

auto parse_input(std::filesystem::path cmd_output) {
    DirStructure dir {
        "/",
        nullptr,
        {},
        {}
    };

    std::ifstream fs;
    fs.open(cmd_output, std::ios::in);
    assert(fs.is_open());

    // Start at root /
    DirStructure *current_dir = &dir;
    for (std::string line; std::getline(fs, line);) {
        if (line.front() == '$') {
            std::istringstream ss(line);
            ss.seekg(2);
            std::string cmd;
            std::string target;
            ss >> cmd;
            ss >> target;

            if (cmd == "cd" && target == "..") {
                current_dir = current_dir->parent;
            }
            else if (cmd == "cd" && target == "/") {
                current_dir = &dir;
            }
            else if (cmd == "cd") {
                auto r = std::ranges::find_if(current_dir->dir_list, [target](auto& d) {
                    return d.name == target;
                });

                if (r !=  current_dir->dir_list.end()) {
                    current_dir = &(*r);
                }
            }
        }
        else if (line.starts_with("dir")) {
            DirStructure d;
            std::istringstream ss(line);
            ss.seekg(4);
            ss >> d.name;
            d.parent = current_dir;
            current_dir->dir_list.push_back(d);
        }
        else { // Assume file
            std::istringstream ss(line);
            FileData f;
            ss >> f.second;
            ss >> f.first;
            current_dir->file_list.push_back(f);
        }
    }

    return dir;
}

std::size_t recur_dir_find_size(DirStructure& dir, std::size_t limit, auto& m) {
    std::size_t f_size = ranges::accumulate(dir.file_list, 0,
        [](auto acc, auto f) {
            return acc + f.second;
        });

    std::size_t d_size = ranges::accumulate(dir.dir_list | std::views::transform([limit, &m](auto& d) {
        return recur_dir_find_size(d, limit, m);
    }), 0);

    std::size_t t_size = f_size + d_size;

    if (t_size <= limit) {
        m.insert({dir.name, t_size});
    }

    return t_size;
}

std::size_t find_sum_files_under_limit(DirStructure& dir, std::size_t limit) {
    std::multimap<std::string, std::size_t> dir_under_limit;

    recur_dir_find_size(dir, 100000, dir_under_limit);

    return ranges::accumulate(dir_under_limit, 0, [](auto acc, auto& elm) {
        return acc + elm.second;
    });
}

std::size_t find_smallest_delete(DirStructure& dir, std::size_t limit) {
    std::multimap<std::string, std::size_t> dir_under_limit;

     std::size_t req = recur_dir_find_size(dir, 70000000, dir_under_limit) - limit;

    return std::ranges::max_element(dir_under_limit, [req](auto& a, auto& b) {
        return a.second > b.second && b.second > req;
    })->second;
}

TEST(Day07, Part1_example) {
    auto input = parse_input("day07/example_input.txt");

    ASSERT_EQ(95437, find_sum_files_under_limit(input, 100000));
}

TEST(Day07, Part1) {
    auto input = parse_input("day07/puzzle_input.txt");
    
    ASSERT_EQ(1315285, find_sum_files_under_limit(input, 100000));
}

TEST(Day07, Part2_example) {
    auto input = parse_input("day07/example_input.txt");

    ASSERT_EQ(24933642, find_smallest_delete(input, 70000000 - 30000000));
}

TEST(Day07, Part2) {
    auto input = parse_input("day07/puzzle_input.txt");

    ASSERT_EQ(9847279, find_smallest_delete(input, 70000000 - 30000000));
}