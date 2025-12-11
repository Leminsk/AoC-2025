#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>

// I stole it from here: https://www.sourcecodeexamples.net/2024/01/combinations-cpp-solution.html
void findCombinations(
    const std::vector<int>& arr, int k, 
    std::vector<int>& curr, int start, 
    std::set<std::vector<int>>& combinations
) {
    if (curr.size() == k) {
        combinations.insert(curr);
        return;
    }

    for (int i = start; i < arr.size(); ++i) {
        curr.push_back(arr[i]);
        findCombinations(arr, k, curr, i + 1, combinations);
        curr.pop_back(); // Backtrack
    }
}
// I stole it from here: https://www.sourcecodeexamples.net/2024/01/combinations-cpp-solution.html
std::set<std::vector<int>> generateCombinations(const std::vector<int>& arr, int k) {
    std::set<std::vector<int>> combinations;
    std::vector<int> curr;
    findCombinations(arr, k, curr, 0, combinations);
    return combinations;
}


bool isZeroVector(const std::vector<bool>& v) {
    for(const bool& e : v) {
        if(e) { return false; }
    }
    return true;
}

std::vector<bool> xorVecs(const std::vector<bool>& a, const std::vector<bool>& b) {
    int limit = a.size();
    std::vector<bool> res = std::vector<bool>(limit);
    for(int i=0; i<limit; ++i) {
        res[i] = a[i] ^ b[i];
    }
    return res;
}

int main() {
    std::ifstream infile("day10-input.txt");
    std::string line;
    std::string num_str;
    std::vector<std::vector<bool>> light_targets = {};
    std::vector< std::vector<std::vector<bool>> > all_buttons = {};
    std::vector< int > all_buttons_sizes = {};
    while(std::getline(infile, line)) {
        std::vector<bool> light_target = {};
        int i=1;
        for(; line[i] != ']'; ++i) { light_target.push_back( line[i] == '#' ); }
        light_targets.push_back(light_target);
        i+=2;
        std::vector<std::vector<bool>> buttons = {};
        while(true) { 
            std::vector<bool> button = std::vector<bool>(light_target.size(), false);
            for(; line[i] != ')'; ++i) {
                if(line[i]==',' || line[i]=='(') { continue; }
                button[line[i] - '0'] = true;
            }
            buttons.push_back(button);
            // skip joltages
            if(line[i+2] == '{') { break; }
            i+=3;
        }
        all_buttons.push_back(buttons);
        all_buttons_sizes.push_back(buttons.size());
    }

    std::vector<int> presses_per_line = std::vector<int>(light_targets.size(), 0);

    for(int i=0; i<all_buttons.size(); ++i) {
        std::vector<int> possible_values = {};
        for(int j=0; j<all_buttons_sizes[i]; ++j) { 
            possible_values.push_back(j); 
        }

        for(int length=1; length<=all_buttons_sizes[i]; ++length) {
            bool got_it = false;
            std::set<std::vector<int>> combinations = generateCombinations(possible_values, length);
            for(auto& comb : combinations) {
                std::vector<bool> curr_xor = xorVecs(light_targets[i], all_buttons[i][ comb[0] ]);
                for (int ci=1; ci<comb.size(); ++ci) {
                    curr_xor = xorVecs(curr_xor, all_buttons[i][ comb[ci] ]);
                }
                if(isZeroVector(curr_xor)) {
                    presses_per_line[i] = comb.size();
                    got_it = true;
                    break;
                }
            }
            if(got_it) { 
                break; 
            }
        }
    }

    int sum = 0;
    for(const int& i : presses_per_line) { sum += i; }

    std::cout << "sum:" << sum << '\n';

    


    return 0;
}