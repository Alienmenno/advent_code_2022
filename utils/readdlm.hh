#pragma once

#include <tuple>
#include <filesystem>
#include <vector>
#include <fstream>
#include <ranges>

namespace utils
{

// auto t = utils::readdlm<int, double, int, int>("day05/example_input.txt");
// fmt::print("t: {}\n", t);

void handle_dlm(std::istream& stream, std::string_view dlims) {
    if (dlims.find(stream.peek()) != dlims.npos)
        stream.ignore();
}

template<typename T>
auto parse_type(std::istream& stream) -> T {
    T elm;
    stream >> elm;
    return elm;
}

template<>
auto parse_type(std::istream& stream) -> bool {
    bool elm;
    stream >> std::boolalpha >> elm;
    return elm;
}

// template<class... Args>
// auto parse_type(std::istream& stream) -> std::vector<std::tuple<Args...>> {
//     return parsedlm<Args...>(stream, ",\n");
// }

template<typename T>
auto parse_elm(std::stringstream& stream, std::string_view dlims) -> T {
    handle_dlm(stream, dlims);
    return parse_type<T>(stream);
}

template<class... Args>
auto parsedlm(std::istream& is, std::string_view dlims = ",\n") {
    std::vector<std::tuple<Args...>> elms;

    while(!is.eof()) {
        std::string line;
        std::getline(is, line);
        std::ranges::reverse(line);
        std::stringstream sline(line);
        elms.emplace_back(parse_elm<Args>(sline, dlims)...);
    }

    return elms;
}

// Only works on well formed data, empty fields dont work
template<class... Args>
auto readdlm(std::filesystem::path file_path, std::string_view dlims = ",\n") {
    std::ifstream fs;
    fs.open(file_path, std::ios::in);
    assert(fs.is_open());

    return parsedlm<Args...>(fs, dlims);
}

}