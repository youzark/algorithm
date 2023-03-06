#include <fstream>
#include <iostream>

#include <limits>
#include <vector>
#include <string>
#include <tuple>
#include <stack>

#include <utility>
#include <algorithm>
#include <numeric>

class Graph {
private:
    using Edge = std::pair<int,int>;
    std::vector<std::vector<Edge>> m_graph;
    std::vector<std::tuple<int,int,int>> m_edges;
    int m_size;
public:
    Graph(int size):
        m_graph(size+1,std::vector<Edge>{}),
        m_size(size) {}

    void add_edge(int tail, int head, int weight) {
        m_graph[tail].emplace_back(head,weight);
        m_edges.emplace_back(tail,head,weight);
    }

    std::vector<Edge>& out_edges(int tail) {
        return m_graph[tail];
    }

    auto& all_edges() { return m_edges; }
};

int main() {
    int const MAXINT = std::numeric_limits<int>::max();

    int node_count, edge_count, start, destination;
    int tail, head ,weight;
    std::ifstream fin{"graph.in"};
    if(fin.is_open()) { fin >> node_count >> edge_count >> start >> destination; } 
    std::vector<int> cur_dist(node_count+1,MAXINT); // current visible shortest path from source to other nodes
    cur_dist[start] = 0;
    std::vector<int> prev_node(node_count+1,-1);
    Graph g(node_count);
    if(fin.is_open()) {
        while(fin >> tail >> head >> weight) {
            g.add_edge(tail,head,weight);
            g.add_edge(head,tail,weight);
        }
        fin.close();
    }

    // relax all the edges
    for(int i = 1; i < node_count; ++i) { 
        for( auto [tail,head,weight] : g.all_edges()) {
            std::cout << tail << " -> " << head << " : " << weight << std::endl;
            if(cur_dist[tail] + weight < cur_dist[head]) {
                cur_dist[head] = cur_dist[tail] + weight;
                prev_node[head] = tail;
            }
        }
    }

    if(prev_node[destination] == -1) std::cout << "can't arrive" << std::endl;
    else {
        std::stack<int> node_track{};
        start = destination;
        while(start != -1) {
            node_track.push(start);
            start = prev_node[start];
        }
        while(!node_track.empty()) {
            std::cout << node_track.top() << " ";
            node_track.pop();
        }
    }
}
