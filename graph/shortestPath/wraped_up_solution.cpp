#include <fstream>
#include <iostream>
#include <ostream>
#include <pthread.h>
#include <sstream>

#include <vector>
#include <string>
#include <tuple>

#include <algorithm>
#include <numeric>
#include <limits>


class Graph {
private:
    using Edge = std::tuple<int,int,int>; // tail -> head : weight
    std::vector<std::vector<Edge>> m_graph;
    std::vector<Edge> m_edges;
    int m_size;

public:
    Graph(int size):
        m_graph(size+1,std::vector<Edge>()),
        m_edges(),
        m_size(size) {}

    void add_edge(int tail, int head, int weight) {
        m_graph[tail].emplace_back(tail,head,weight);
        m_edges.emplace_back(tail,head,weight);
    }

    int get_size() { return m_size; }

    auto& all_edges() {
        return m_edges;
    }
};

class BellmanFordSolver {
private:
    Graph m_graph;
    const int INTMAX = std::numeric_limits<int>::max();
    std::vector<int> m_distance;
    std::vector<int> m_prev_node;
public:
    BellmanFordSolver(Graph&& g):
        m_graph(g),
        m_distance(g.get_size() + 1,INTMAX),
        m_prev_node(g.get_size() + 1,-1) { }

    void solve(int start) {
        std::fill(m_distance.begin(),m_distance.end(),INTMAX);
        std::fill(m_prev_node.begin(),m_prev_node.end(),-1);
        m_distance[start] = 0;

        for( int i = 1; i < m_graph.get_size(); ++i ) {
            for( auto& [tail, head, weight] : m_graph.all_edges() ) {
                if((long)m_distance[tail] + weight < m_distance[head]) {
                    m_distance[head] = m_distance[tail] + weight;
                    m_prev_node[head] = tail;
                }
            }
        }
    }

    int distance(int terminal) { return m_distance[terminal]; }

    std::vector<int> path(int terminal) {
        std::vector<int> route;
        if(m_prev_node[terminal] == -1) return route;
        while(terminal != -1) {
            route.push_back(terminal);
            terminal = m_prev_node[terminal];
        }
        std::reverse(route.begin(),route.end());
        return route;
    }
};

std::ostream& operator<<(std::ostream& stream, std::vector<int> val) {
    for( auto& i : val ) {
        stream << i << " ";
    }
    stream << std::endl;
    return stream;
}

int main() {
    std::vector<std::string> edges;
    int terminal, intermediate;
    int tail, head, weight;
    std::ifstream fin{"input.in"};
    if(fin.is_open()) {
        std::string line;
        std::getline(fin,line);
        terminal = std::stoi(std::string(line.begin()+2,line.end()));
        std::getline(fin,line);
        intermediate = std::stoi(std::string(line.begin()+2,line.end()));
        while(std::getline(fin,line)) {
            edges.push_back(std::move(line));
        }
    }

    Graph g(edges.size());

    for( auto& line : edges ) {
        std::istringstream iss{std::move(line)};
        iss >> tail;
        while(iss >> head >> weight) {
            g.add_edge(tail,head,weight);
        }
    }

    BellmanFordSolver solver(std::move(g));
    solver.solve(1);
    auto to_inter = solver.path(intermediate);
    solver.solve(intermediate);
    auto to_termi = solver.path(terminal);

}


