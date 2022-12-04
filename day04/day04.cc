#include "gtest/gtest.h"
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

struct CleaningJob {
    int min_id;
    int max_id;
};

std::ostream& operator<< (std::ostream& os, const CleaningJob& j) {
    return os << '{' << j.min_id << '-' << j.max_id << '}';
}
template <> struct fmt::formatter<CleaningJob> : ostream_formatter {};

using CleaningJobs = std::vector<std::pair<CleaningJob, CleaningJob>>;

auto parse_job(std::fstream& fs) {
    int min_id, max_id;

    if (fs.peek() == ',')
        fs.ignore();

    fs >> min_id;
    fs.ignore(); // Ignore '-'
    fs >> max_id;

    return CleaningJob {
        min_id,
        max_id
    };
}

auto parse_cleaning_jobs(std::filesystem::path cleaning_job_list)
{
    std::fstream fs;
    fs.open(cleaning_job_list, std::ios::in);
    assert(fs.is_open());

    CleaningJobs cleaning_jobs{};

    while(!fs.eof()) {
        cleaning_jobs.emplace_back(parse_job(fs), parse_job(fs));
    }
    
    return cleaning_jobs;
}

auto check_for_total_overlap(CleaningJobs& jobs) {
    return std::ranges::count_if(jobs, [](auto& job_pair) {
        return (job_pair.first.min_id <= job_pair.second.min_id
             && job_pair.first.max_id >= job_pair.second.max_id)
            || (job_pair.second.min_id <= job_pair.first.min_id
             && job_pair.second.max_id >= job_pair.first.max_id);
    });
}

auto check_for_partial_overlap(CleaningJobs& jobs) {
    return std::ranges::count_if(jobs, [](auto& job_pair) {
        return !(job_pair.first.max_id < job_pair.second.min_id
              || job_pair.second.max_id < job_pair.first.min_id);
    });
}

TEST(Day04, Part1_example) {
    auto jobs = parse_cleaning_jobs("day04/example_input.txt");

    ASSERT_EQ(2, check_for_total_overlap(jobs));
}

TEST(Day04, Part1) {
    auto jobs = parse_cleaning_jobs("day04/puzzle_input.txt");

    ASSERT_EQ(534, check_for_total_overlap(jobs));
}

TEST(Day04, Part2_example) {
    auto jobs = parse_cleaning_jobs("day04/example_input.txt");

    ASSERT_EQ(4, check_for_partial_overlap(jobs));
}

TEST(Day04, Part2) {
    auto jobs = parse_cleaning_jobs("day04/puzzle_input.txt");

    ASSERT_EQ(841, check_for_partial_overlap(jobs));
}