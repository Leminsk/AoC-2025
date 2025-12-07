#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Node {
public:
    std::vector<Node*> parents;
    int line;
    int column;

    Node(const std::vector<Node*> parents, int line, int column) {
        this->parents = parents;
        this->line = line;
        this->column = column;
    }
};

void pushUnique(std::vector<int>& v, int n) {
    std::vector<int>::iterator it;
    it = std::find(v.begin(), v.end(), n);
    if(it == v.end()) {
        v.push_back(n);
    }
}

Node* getNodePointer(const std::vector<Node*>& nodes, int line, int column) {
    int nodes_size = nodes.size();
    for(int i=0; i<nodes_size; ++i) {
        if(line == nodes[i]->line && column == nodes[i]->column) {
            return nodes[i];
        }
    }
    return nullptr;
}

void DFS(std::vector<Node*>& nodes, Node* start, Node* destination, std::vector<Node*> path, uint64_t& distinct_paths) {
    if(start == destination) {
        ++distinct_paths;
    } else {
        for(Node*& p : start->parents) {
            path.push_back(p);
            DFS(nodes, p, destination, path, distinct_paths);
            path.pop_back();
        }
    }
}

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

    Node* root_node = new Node({},          -1, source_index);
    Node* end_node  = new Node({}, line_size+1,           -1);
    std::vector<Node*> nodes = { root_node };
    // initialize all '^' as nodes with no parents
    for(int l=0; l<=last_line; ++l) {
        for(int c=0; c<line_size; ++c) {
            if(lines[l][c] == '^') {
                Node* new_node = new Node({}, l, c);
                nodes.push_back(new_node);
            }
        }
    }

    for(int l=1; l<=last_line; ++l) {
        for(int c=0; c<line_size; ++c) {
            if(lines[l][c] == '^') {
                Node* current_node = getNodePointer(nodes, l, c);
                // edge case for first splitter node
                if(l==1) { current_node->parents.push_back(root_node); }

                const int left = c-1;
                const int right = c+1;
                
                // left side
                int j=l+1;
                for(; j<=last_line; ++j) {
                    if(lines[j][left] == '^') { break; }
                }
                if(j >= last_line) { end_node->parents.push_back(current_node); } 
                else { getNodePointer(nodes, j, left)->parents.push_back(current_node); }

                // right side
                j=l+1;
                for(; j<=last_line; ++j) {
                    if(lines[j][right] == '^') { break; }
                }
                if(j >= last_line) { end_node->parents.push_back(current_node); } 
                else { getNodePointer(nodes, j, right)->parents.push_back(current_node); }
            }
        }
    }

    int i = end_node->parents.size();
    std::cout << "let's count from " << end_node->parents.size() << " parents\n";
    
    Node* previous_entry = nullptr;
    uint64_t distinct_paths = 0;
    uint64_t old_increment = 0;
    for(Node*& entry_point : end_node->parents) {
        printf("%d remaining... ", i);
        if(previous_entry != entry_point) {
            previous_entry = entry_point;            
            std::vector<Node*> path = { entry_point };
            uint64_t current_increment = 0;
            DFS(nodes, entry_point, root_node, path, current_increment);
            distinct_paths += current_increment;
            old_increment = current_increment;
            // std::cout << "{ " << entry_point->line << ',' << entry_point->column << "} current_increment:" << current_increment << '\n';
        } else {
            distinct_paths += old_increment;
        }
        printf("done\n");
        --i;
    }

    // std::cout << "##########################################\n";
    // for(auto& n : nodes) {
    //     std::cout << "{ " << n->line << ',' << n->column << " } ";
    // }std::cout << '\n';

    // std::cout << "##########################################\n";    
    // for(auto& p : end_node->parents) { 
    //     std::cout << "< "; std::cout << p->line << ',' << p->column << " > "; 
    // }std::cout << '\n';
    
    for(Node*& n: nodes) {
        if(n != nullptr) { delete n; }
    }

    std::cout << "distinct_paths:" << distinct_paths<<'\n';

    return 0;
}