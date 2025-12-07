#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main() {
    std::ifstream infile("day07-input.txt");
    std::string line;

    int source_index;
    std::vector<std::string> lines = {};
    
    while(std::getline(infile, line)) {
        bool skip_line = false;
        for(int i=0; i<line.size(); ++i) {
            if(line[i] == 'S') { 
                source_index = i;
                skip_line = true;
                break;
            }
        }
        if(!skip_line) {
            lines.push_back(line);
        }
    }

    
    int last_line = lines.size()-1;
    int line_size = lines[0].size();
    uint64_t paths_counter = 0;

    std::vector<uint64_t> columns_counters = std::vector<uint64_t>(line_size, 0);
    columns_counters[source_index] = 1;
    uint64_t increment;
    for(int l=0; l<=last_line; ++l) {
        for(int c=0; c<line_size; ++c) {
            if(lines[l][c] == '^') {
                increment = columns_counters[c];
                columns_counters[c-1] += increment;
                columns_counters[c+1] += increment;
                columns_counters[c] = 0;
            }
        }
    }
    for(int i=0; i<line_size; ++i) {
        paths_counter += columns_counters[i];
    }

    std::cout << "paths_counter:" << paths_counter << '\n';
    

    return 0;
}