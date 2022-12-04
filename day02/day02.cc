#include "gtest/gtest.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <filesystem>

enum GameOption {
    Rock = 1,
    Paper = 2,
    Siccors = 3,
};

enum GameRoundResult {
    Loss = 0,
    Draw = 3,
    Win = 6,
};

using GameRound = std::pair<GameOption, GameOption>;
using GameStrategy = std::vector<GameRound>;
using GameResult = int;

GameOption read_game_option(char c) {
    switch (c) {
        case 'A': case 'X': return Rock;
        case 'B': case 'Y': return Paper;
        case 'C': case 'Z': return Siccors;
        default: assert(false);
    }
}


GameStrategy read_game_strategy(std::filesystem::path strategy_file)
{
    std::fstream fs;
    fs.open(strategy_file, std::ios::in);
    assert(fs.is_open());

    GameStrategy strategy{};

    for (std::string line; std::getline(fs, line);) {
        strategy.emplace_back(read_game_option(line.front()), read_game_option(line.back()));
    }

    return strategy;
}

GameResult play_game(GameStrategy& strategy) {
    GameResult r = 0;

    for (auto round : strategy) {
        if (round.first == round.second)
            r += Draw;
        else if ((round.first == Rock && round.second == Paper)
              || (round.first == Paper && round.second == Siccors)
              || (round.first == Siccors && round.second == Rock))
            r += Win;
        else
            r += Loss;

        r += round.second;
    }

    return r;
}

TEST(Day02, Part1_example) {
    auto strategy = read_game_strategy("day02/example_input.txt");

    ASSERT_EQ(15, play_game(strategy));
}

TEST(Day02, Part1) {
    auto strategy = read_game_strategy("day02/puzzle_input.txt");

    ASSERT_EQ(12772, play_game(strategy));
}

// ----------------------------------------------------------------------------

using GameRound2 = std::pair<GameOption, GameRoundResult>;
using GameStrategy2 = std::vector<GameRound2>;

GameRoundResult read_game_option_result(char c) {
    switch (c) {
        case 'X': return Loss;
        case 'Y': return Draw;
        case 'Z': return Win;
        default: assert(false);
    }
}

GameStrategy2 read_game_strategy2(std::filesystem::path strategy_file)
{
    std::fstream fs;
    fs.open(strategy_file, std::ios::in);
    assert(fs.is_open());

    GameStrategy2 strategy{};

    for (std::string line; std::getline(fs, line);) {
        strategy.emplace_back(read_game_option(line.front()), read_game_option_result(line.back()));
    }

    return strategy;
}

GameResult play_game2(GameStrategy2& strategy) {
    GameResult r = 0;

    for (auto round : strategy) {
        switch (round.second) {
            case Loss:
                r += round.first - 1;
                if (round.first == Rock) r += 3;
                break;
            case Draw:
                r += round.first;
                break;
            case Win:
                r += (round.first % 3) + 1;
                break;
            default:
                assert(false);
        }
        r += round.second;
    }

    return r;
}

TEST(Day02, Part2_example) {
    auto strategy = read_game_strategy2("day02/example_input.txt");

    ASSERT_EQ(12, play_game2(strategy));
}

TEST(Day02, Part2) {
    auto strategy = read_game_strategy2("day02/puzzle_input.txt");

    ASSERT_EQ(11618, play_game2(strategy));
}