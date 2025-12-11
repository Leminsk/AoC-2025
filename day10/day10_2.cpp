#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>
#include <iomanip>
#include <highs/Highs.h> // yes I had to install it together with CMake

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

    std::vector<std::vector<int>> joltage_targets = {};
    std::vector< std::vector<std::vector<bool>> > all_buttons = {};
    std::vector< std::vector<std::vector<int>> > all_buttons_int = {};
    std::vector< int > all_buttons_sizes = {};

    while(std::getline(infile, line)) {
        std::vector<bool> light_target = {};
        int i=1;
        for(; line[i] != ']'; ++i) { light_target.push_back( line[i] == '#' ); }
        i+=2;
        std::vector<std::vector<bool>> buttons = {};
        std::vector<std::vector<int>> buttons_int = {};
        std::vector<int> joltages = {};
        while(true) { 
            std::vector<bool> button = std::vector<bool>(light_target.size(), false);
            std::vector<int> button_int = {};
            for(; line[i] != ')'; ++i) {
                if(line[i]==',' || line[i]=='(') { continue; }
                int num = line[i] - '0';
                button[num] = true;
                button_int.push_back(num);
            }
            buttons.push_back(button);
            buttons_int.push_back(button_int);
            // skip joltages
            if(line[i+2] == '{') { 
                i+=3;
                std::string num_str = "";
                for(; line[i] != '}'; ++i) {
                    if(line[i]==',') { 
                        joltages.push_back( std::stoi(num_str) );
                        num_str = "";
                        continue; 
                    }
                    num_str += line[i];
                }
                joltages.push_back( std::stoi(num_str) );
                break;
            }
            i+=3;
        }
        joltage_targets.push_back(joltages);
        all_buttons.push_back(buttons);
        all_buttons_int.push_back(buttons_int);
        all_buttons_sizes.push_back(buttons.size());
    }

    int64_t total_button_presses = 0;

    for(int line=0; line<joltage_targets.size(); ++line) {
        std::vector<int>& current_joltages = joltage_targets[line];
        std::vector< std::vector<int> >& current_buttons_ints = all_buttons_int[line];

        Highs highs;
        HighsModel model;
        HighsLp &lp = model.lp_;

        lp.num_col_ = current_buttons_ints.size(); // amount of buttons
        lp.num_row_ = current_joltages.size();     // amount of joltage requirements
        lp.col_cost_ = std::vector<double>(lp.num_col_, 1.0);
        lp.col_lower_ = std::vector<double>(lp.num_col_, 0.0);
        lp.col_upper_ = std::vector<double>(lp.num_col_, 1e6);

        std::vector<double> lp_row_lower = {};
        for(int& j : current_joltages) { lp_row_lower.push_back(static_cast<double>(j)); }
        lp.row_lower_ = lp_row_lower;
        lp.row_upper_ = lp.row_lower_;

        lp.a_matrix_.format_ = MatrixFormat::kColwise;
        // cumulative of nonzeros per column/button (starting at zero)
        std::vector<int> matrix_start = {0};
        std::vector<int> matrix_index = {};
        int col_sum = 0;
        for(std::vector<int>& b : current_buttons_ints) { 
            col_sum += b.size();
            matrix_start.push_back(col_sum); 
            for(int& bi : b) { matrix_index.push_back(bi); }
        }
        lp.a_matrix_.start_ = matrix_start;

        // which positions are toggle per each button (in order)
        lp.a_matrix_.index_ = matrix_index;
        lp.a_matrix_.value_ = std::vector<double>(lp.a_matrix_.index_.size(), 1.0);
        // only integer solutions
        lp.integrality_.assign(lp.num_col_, HighsVarType::kInteger);

        highs.passModel(model);
        highs.setOptionValue("output_flag", "false");
        HighsStatus status = highs.run();
        if(status != HighsStatus::kOk) {
            std::cerr << "HiGHS failed to solve model (status " << int(status) << ")\n";
            return 1;
        }
        const std::vector<double> &x = highs.getSolution().col_value;
        int64_t buttom_presses = 0;
        for(auto& e : x) { buttom_presses += std::llround(e); }
        total_button_presses += buttom_presses;
    }

    std::cout << "total_button_presses:" << total_button_presses << '\n';


    return 0;
}