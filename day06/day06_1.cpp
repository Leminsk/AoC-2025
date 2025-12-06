#include <iostream>
#include <fstream>
#include <vector>
#include <regex>


int main() {
    std::ifstream infile("day06-input.txt");
    std::string line;
    
    std::vector<std::vector<int>> lines = {};
    std::string operations = "";

    std::regex re("\\d+");
    std::smatch match;
    std::string::const_iterator begin;
    std::string::const_iterator end;
    
    while(std::getline(infile, line)) {
        if(line[0] != ' ' && line[0] < '0') { // last line: operations
            int line_size = line.size();
            for(int i=0; i<line_size; ++i) {
                if(line[i] != ' ') { operations += line[i]; }
            }
        } else { // operands
            std::vector<int> new_line = {};
            begin = line.cbegin();
            end = line.cend();
            while(std::regex_search(begin, end, match, re)) {
                new_line.push_back(std::stoi(match[0]));
                begin = match.suffix().first;
            }
            lines.push_back(new_line);
        }
    }

    int lines_amount = lines.size();
    int columns_amount = operations.size();
    uint64_t sum = 0;

    char op;
    uint64_t column_res;
    for(int i=0; i<columns_amount; ++i) {
        op = operations[i];
        if(op == '+') {
            column_res = 0;
            for(int j=0; j<lines_amount; ++j) { column_res += lines[j][i]; }
        } else {
            column_res = 1;
            for(int j=0; j<lines_amount; ++j) { column_res *= lines[j][i]; }
        }
        sum += column_res;
    }


    std::cout << "sum:" << sum << '\n';
    return 0;
}