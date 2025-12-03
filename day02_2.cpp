#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

bool hasRepeatingSequence(uint64_t number) {
    std::string s = std::to_string(number);
    const size_t s_size = s.size();
    if(s_size == 1) {
        return false;
    }
    static const std::regex r("^(\\d+?)\\1+$");
    return std::regex_match(s, r);
}

int main() {
    std::ifstream infile("day02-input.txt");
    std::string ranges_line;
    std::getline(infile, ranges_line);

    std::vector<std::vector<std::string>> ranges = {};
    std::regex re("(\\d+)-(\\d+)");
    std::smatch match;
    std::string::const_iterator begin = ranges_line.cbegin();
    std::string::const_iterator end = ranges_line.cend();
    
    while(std::regex_search(begin, end, match, re)) {
        ranges.push_back({ match[1].str(), match[2].str() });
        begin = match.suffix().first;
    }

    uint64_t low, high;
    uint64_t accumulator = 0;
    for(const std::vector<std::string>& range : ranges) {
        low = std::stoull(range[0]);
        high = std::stoull(range[1]);
        for(uint64_t i=low; i<=high; ++i) {
            if(hasRepeatingSequence(i)) {
                accumulator += i;
            }
        }
    }

    std::cout << "sum:" << accumulator << '\n';

    return 0;
}