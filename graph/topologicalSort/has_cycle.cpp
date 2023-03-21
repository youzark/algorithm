#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <functional>

class Graph {
public:
    std::vector<std::vector<int>> m_graph;

    Graph(int size):
        m_graph(size+1,std::vector<int>{}) {};

    void add_edge(int tail, int head) {
        m_graph[tail].push_back(head);
    }

    std::vector<int> &all_neighbor(int tail) {
        return m_graph[tail];
    }
};


int main() {
    std::ifstream fin{"di_graph.in"};
    int num, tail, head;
    fin >> num;
    Graph g{num};
    std::vector<int> topo_seq;
    std::vector<bool> inTopo(num+1,false);
    std::vector<bool> visited(num+1,false);
    while(fin >> tail >> head) {
        g.add_edge(tail,head);
    }

    std::function<void(int node)> find_topo = 
        [&](int node) {
            for( int neighbor : g.all_neighbor(node) ) {
                if(!visited[neighbor]) {
                    visited[neighbor] = true;
                    find_topo(neighbor);
                } else if(!inTopo[neighbor]) return;
            }
            topo_seq.push_back(node);
            inTopo[node] = true;
        };

    for(int i = 1; i <= num; ++ i) {
        if(!visited[i]) find_topo(i);
    }


    for(int i = 1; i <= num; ++i) {
        if(!inTopo[i]) {
            std::cout << "has cycle";
            return 1;
        } 
    }
    std::reverse(topo_seq.begin(),topo_seq.end());
    for(auto n : topo_seq) std::cout << n << " ";
}
