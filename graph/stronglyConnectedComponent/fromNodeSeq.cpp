#include <vector>
#include <fstream>
#include <iostream>
#include <set>
#include <queue>
#include <algorithm>

class Graph {
private:
    std::vector<std::set<int>> m_graphNeighbors;
public:
    Graph(int size):
        m_graphNeighbors(size,std::set<int>{})
    { }

    void add_edge(int tail, int head) {
        m_graphNeighbors[tail].insert(head);
        m_graphNeighbors[head].insert(tail);
    }

    auto const& visit_neighbors(int node) { return m_graphNeighbors[node]; }
};

int main() {
    int nodes,edges;
    std::ifstream fin{"graph.in"};
    if(fin.is_open()) { fin >> nodes >> edges; }
    Graph g{nodes};

    int node, neighbor;
    for(int i = 0; i < edges; ++i) {
        fin >> node >> neighbor;
        g.add_edge(node,neighbor);
    }

    std::set<int> visited{};
    std::queue<int> frontier{};
    int max_size = 0;
    std::vector<std::vector<int>> stronglyConnectedComponents;

    for(int cur = 0; cur < nodes; ++cur ) {
        if(visited.find(cur) != visited.end()) { continue; }
        std::vector<int> scc{};
        frontier.push(cur);
        visited.insert(cur);
        while(!frontier.empty()) {
            int node = frontier.front();
            for( auto neighbor : g.visit_neighbors(node) ) {
                if(visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    frontier.push(neighbor);
                }
            }
            scc.push_back(node);
            frontier.pop();
        }
        stronglyConnectedComponents.push_back(std::move(scc));
    }

    auto most_elements = [](std::vector<int> lhs,std::vector<int> rhs){ return lhs.size() < rhs.size(); };
    auto greatedscc = std::max_element(stronglyConnectedComponents.begin(),stronglyConnectedComponents.end(),most_elements);
    for( auto& node : *greatedscc ) { std::cout << node << " "; }
}
