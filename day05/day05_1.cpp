#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

int isFresh(uint64_t id, const std::vector<std::vector<uint64_t>>& ranges) {
    for(const std::vector<uint64_t>& r : ranges) {
        if(r[0] <= id && id <= r[1]) {
            return 1;
        }
    }
    return 0;
}

int main() {
    std::ifstream infile("day05-input.txt");
    std::string line;

    std::vector<std::vector<uint64_t>> ranges = {};

    std::regex re("(\\d+)-(\\d+)");
    std::smatch match;

    bool ranges_done = false;
    uint64_t fresh_counter = 0;
    
    while(std::getline(infile, line)) {
        if(line == "") { 
            ranges_done = true; 
            continue; 
        }

        if(ranges_done) {
            fresh_counter += isFresh(std::stoull(line), ranges);
        } else {
            std::regex_search(line.cbegin(), line.cend(), match, re);
            ranges.push_back({ std::stoull(match[1]), std::stoull(match[2]) });
        }
    }

    std::cout << "fresh_counter:" << fresh_counter << '\n';
    return 0;
}