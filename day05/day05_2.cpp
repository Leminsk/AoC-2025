#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>

uint64_t amountOfNumbers(const std::vector<std::vector<uint64_t>>& ranges) {
    uint64_t counter = 0;
    uint64_t ref = 0;

    for(int i=0; i<ranges.size(); ++i) {
        if(ranges[i][0] <= ref && ref <= ranges[i][1]) {
            counter += (ranges[i][1] - ref);
        } else if(ref < ranges[i][0]) {
            counter += (ranges[i][1] - ranges[i][0]) + 1;
        }
        if(ranges[i][1] > ref) {
            ref = ranges[i][1];
        }
    }

    return counter;
}

int main() {
    std::ifstream infile("day05-input.txt");
    std::string line;

    std::vector<std::vector<uint64_t>> ranges = {};

    std::regex re("(\\d+)-(\\d+)");
    std::smatch match;

    while(std::getline(infile, line)) {
        if(line == "") { break; }
        std::regex_search(line.cbegin(), line.cend(), match, re);
        ranges.push_back({ std::stoull(match[1]), std::stoull(match[2]) });
    }

    std::sort(ranges.begin(), ranges.end(),
        [](
            const std::vector<uint64_t>& a,
            const std::vector<uint64_t>& b
        ) {
            if(a[0] != b[0]) {
                return a[0] < b[0];
            }
            return a[1] < b[1];
        }
    );

    uint64_t amount = amountOfNumbers(ranges);

    std::cout << "amount:" << amount << '\n';
    return 0;
}