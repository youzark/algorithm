#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ostream>
#include <functional>

class Node {
public:
    int val;
    Node *left;
    Node *right;
    Node(int val): 
        val(val),
        left(nullptr),
        right(nullptr) {}
};



int main() {
    std::ifstream fin{"tree.in"};
    int num;
    std::vector<Node*> nodes;
    if(fin.is_open()) {
        fin >> num;
        nodes.reserve(num);
        for(int i = 0; i < num; ++i) {
            nodes.push_back(new Node{i+1});
        }
        int root,left,right;
        for( int i = 0; i < num; ++i ) { 
            fin >> root >> left >>right;
            if(left) { nodes[root-1]->left = nodes[left-1]; }
            if(right) { nodes[root-1]->right = nodes[right-1]; }
        }
        fin.close();
    }

    std::ofstream fout{"tree.out"};
    std::function<void(Node*)> postOrder = [&](Node* root) {
        if(!root) return;
        postOrder(root->left);
        postOrder(root->right);
        fout << root->val << " ";
    };
    if(fout.is_open()) {
        postOrder(nodes[0]);
        fout.close();
    }
}
