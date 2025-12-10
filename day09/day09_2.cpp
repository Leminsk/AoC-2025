#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <stack>
#include <map>

class Tile {
public:
    int64_t id, x, y;

    Tile(int64_t id, int64_t x, int64_t y) {
        this->id = id;
        this->x = x;
        this->y = y;
    }

    friend bool operator==(const Tile& a, const Tile& b) {
        return a.id == b.id;
    }
    friend std::ostream& operator<<(std::ostream& stream, const Tile& b) {
        stream << '[' << b.id << "]_{" << b.x << ',' << b.y << '}';
        return stream;
    }
};

int64_t rectArea(const Tile& a, const Tile& b) {
    int64_t x = (a.x > b.x) ? a.x - b.x : b.x - a.x;
    int64_t y = (a.y > b.y) ? a.y - b.y : b.y - a.y;
    return (x+1) * (y+1);
}

bool rectangleOK(const std::vector<std::vector<bool>>& grid, int ax, int ay, int bx, int by) {
    int min_x = std::min(ax, bx); int max_x = std::max(ax, bx);
    int min_y = std::min(ay, by); int max_y = std::max(ay, by);
    for(int y=min_y; y<=max_y; ++y) {
        for(int x=min_x; x<=max_x; ++x) {
            if(!grid[y][x]) { return false; }
        }
    }
    return true;
}

int main() {
    std::ifstream infile("day09-input.txt");
    std::string line;

    std::regex re("\\d+");
    std::smatch match;
    std::string::const_iterator begin;
    std::string::const_iterator end;
    
    std::vector<Tile*> tiles = {};

    int64_t max_width = 0;
    int64_t max_height = 0;
    int id_counter = 0;
    while(std::getline(infile, line)) {
        std::vector<int> new_line = {};
        begin = line.cbegin();
        end = line.cend();
        while(std::regex_search(begin, end, match, re)) {
            new_line.push_back(std::stoi(match[0]));
            begin = match.suffix().first;
        }
        Tile* tile = new Tile(id_counter, new_line[0], new_line[1]);
        tiles.push_back(tile);
        if(new_line[0] > max_width) { max_width = new_line[0]; }
        if(new_line[1] > max_height) { max_height = new_line[1]; }
        ++id_counter;
    }

    // compress 
    std::map<int64_t, int> mapx, mapy; 
    for(Tile* t : tiles) { mapx[t->x] = 0; mapy[t->y] = 0; } 
    std::vector<int64_t> compressed_to_original_x, compressed_to_original_y; 
    int idx = 0; 
    for(auto& kv : mapx) { 
        compressed_to_original_x.push_back(kv.first); 
        kv.second = idx; ++idx; 
    } 
    idx = 0; 
    for(auto& kv : mapy) { 
        compressed_to_original_y.push_back(kv.first); 
        kv.second = idx; 
        ++idx; 
    }

    // compress that bastard
    int mapy_size = mapy.size(); 
    int mapx_size = mapx.size(); 
    std::vector<std::vector<bool>> grid(mapy_size, std::vector<bool>(mapx_size, false));

    for(int i=0; i<id_counter; ++i) {
        int neighbor = (i+1) % id_counter;
        Tile* v1 = tiles[i]; Tile* v2 = tiles[neighbor];
        int x1 = mapx[v1->x]; int y1 = mapy[v1->y]; 
        int x2 = mapx[v2->x]; int y2 = mapy[v2->y];
        if(x1 == x2) { // vertical
            int min_y = std::min(y1, y2);
            int max_y = std::max(y1, y2);
            for(int y=min_y; y<=max_y; ++y) { grid[y][x1] = true; }
        } else { // horizontal (must share y)
            int min_x = std::min(x1, x2);
            int max_x = std::max(x1, x2);
            for(int x=min_x; x<=max_x; ++x) { grid[y1][x] = true; }
        }
    }

    // ayyyy lmao cut it in haaalf
    int seed_x = mapx_size >> 1;
    // cut it half of the haaalf
    int seed_y = mapy_size >> 2;

    // flood fill
    std::stack<std::vector<int64_t>> s;
    s.push({seed_x,seed_y});
    while(!s.empty()) {
        std::vector<int64_t> top = s.top();
        s.pop();
        if(!grid[ top[1] ][ top[0] ]) {
            grid[ top[1] ][ top[0] ] = true;
            s.push({ top[0]-1, top[1]   });
            s.push({ top[0]+1, top[1]   });
            s.push({ top[0]  , top[1]+1 });
            s.push({ top[0]  , top[1]-1 });
        }
    }

    // debug draw the pacman
    // for(int y=0; y<mapy_size; ++y) {
    //     for(int x=0; x<mapx_size; ++x) {
    //         std::cout << (grid[y][x] ? '#' : '-');
    //     }std::cout << '\n';
    // }std::cout << '\n';

    int64_t max_area = 0;
    for(auto& t : tiles) {
        for(auto& p : tiles) {
            if(p == t || p->x == t->x || p->y == t->y) { continue; }
            int64_t area = rectArea(*t, *p);
            if(area > max_area) {
                if(rectangleOK(grid, mapx[t->x], mapy[t->y], mapx[p->x], mapy[p->y])) {
                    max_area = area;
                }
            }
        }
    }
    std::cout << "largest area: " << max_area << '\n';

    return 0;
}