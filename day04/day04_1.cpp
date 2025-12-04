#include <iostream>
#include <fstream>
#include <vector>

int getRollsOfPaper(const std::vector<std::string>& grid, int line_limit, int column_limit, int x, int y) {
    if(x == 0) {
        if(y == 0) {
            return (
                                      (grid[y+1][x  ] == '@') + (grid[y+1][x+1] == '@') +
                                                                (grid[y  ][x+1] == '@')
                
            );
        } else if(y == column_limit) {
            return (
                
                                                                (grid[y  ][x+1] == '@') +
                                      (grid[y-1][x  ] == '@') + (grid[y-1][x+1] == '@')
            );
        }
        return (
                                      (grid[y+1][x  ] == '@') + (grid[y+1][x+1] == '@') +
                                                                (grid[y  ][x+1] == '@') +
                                      (grid[y-1][x  ] == '@') + (grid[y-1][x+1] == '@')
        );

    } else if (x == line_limit) {
        if(y == 0) {
            return (
                (grid[y+1][x-1] == '@') + (grid[y+1][x  ] == '@') +
                (grid[y  ][x-1] == '@')
                
            );
        } else if(y == column_limit) {
            return (
                (grid[y  ][x-1] == '@') +
                (grid[y-1][x-1] == '@') + (grid[y-1][x  ] == '@')

            );
        }
        return (
                (grid[y+1][x-1] == '@') + (grid[y+1][x  ] == '@') +
                (grid[y  ][x-1] == '@') +                          
                (grid[y-1][x-1] == '@') + (grid[y-1][x  ] == '@')
        );
    }
    
    if(y == 0) {
        return (
            (grid[y+1][x-1] == '@') + (grid[y+1][x  ] == '@') + (grid[y+1][x+1] == '@') +
            (grid[y  ][x-1] == '@') +                           (grid[y  ][x+1] == '@')

        );
    } else if(y == column_limit) {
        return (
            
            (grid[y  ][x-1] == '@') +                           (grid[y  ][x+1] == '@') +
            (grid[y-1][x-1] == '@') + (grid[y-1][x  ] == '@') + (grid[y-1][x+1] == '@')
        );
    }
    return (
            (grid[y+1][x-1] == '@') + (grid[y+1][x  ] == '@') + (grid[y+1][x+1] == '@') +
            (grid[y  ][x-1] == '@') +                           (grid[y  ][x+1] == '@') +
            (grid[y-1][x-1] == '@') + (grid[y-1][x  ] == '@') + (grid[y-1][x+1] == '@')
    );
}

int main() {
    std::ifstream infile("day04-input.txt");
    std::string line;
    std::vector<std::string> grid = {};
    while(std::getline(infile, line)) {
        grid.push_back(line);
    }
    uint64_t counter = 0;
    int column_limit = grid.size()-1;
    int line_limit = grid[0].size()-1;
    for(int y=0; y<=column_limit; ++y) {
        for(int x=0; x<=line_limit; ++x) {
            if(grid[y][x] == '@' && getRollsOfPaper(grid, line_limit, column_limit, x, y) < 4) {
                ++counter;
            }
        }
    }
    std::cout << "counter:" << counter << '\n';
    return 0;
}