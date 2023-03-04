#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <sstream>

class Node {
public:
    char val;
    Node *left;
    Node *right;
    Node(char val):
        val(val),
        left(nullptr),
        right(nullptr) {}
};

int main() {
    std::ifstream fin{"tree.in"};
    std::unordered_map<char,Node*> val_Nodes;

    auto createIfNotExist = [&](char node_val){
        if( val_Nodes.find(node_val) == val_Nodes.end() ) {
            Node* root = new Node{node_val};
            val_Nodes[node_val] = root;
            return root;
        } else {
            return val_Nodes[node_val];
        }
    };

    if(fin.is_open()) {
        std::string line;
        char root_val,left_val,right_val;
        while(std::getline(fin,line)) {
            std::istringstream ss{line};
            ss >> root_val >> left_val;
            Node* root = createIfNotExist(root_val);
            Node* left = createIfNotExist(left_val);
            Node* right = ss >> right_val ? createIfNotExist(right_val) : nullptr;
            root->left = left;
            root->right = right;
        }
        fin.close();
    }

    int layer = 0;
    int curNodeCount = 0, nextNodeCount = 1;
    Node* cur;
    std::queue<Node*> nodeQ;
    nodeQ.push(val_Nodes['A']);
    std::ofstream fout{"tree.out"};

    while( !nodeQ.empty() ) {
        ++layer;
        std::swap(curNodeCount,nextNodeCount);
        if(layer % 2 == 1) fout << "第 " << layer << " 层结点:";
        while(curNodeCount) {
            cur = nodeQ.front();
            if(cur->left) nextNodeCount++,nodeQ.push(cur->left);
            if(cur->right) nextNodeCount++,nodeQ.push(cur->right);
            if(layer % 2 == 1) {
                if(curNodeCount > 1) fout << cur->val << ",";
                else fout << cur->val;
            }
            curNodeCount--,nodeQ.pop();
        }
        if(layer % 2 == 1) fout << "\n";
    }
}
