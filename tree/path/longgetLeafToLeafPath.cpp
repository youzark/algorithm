#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <functional>
#include <unordered_map>

class Node{
public:
    int val;
    Node *left;
    Node *right;
    Node(int val):
        val(val),
        left(nullptr),
        right(nullptr) {}
};

class BST {
public:
    Node* root;
    void insert(int val);
    BST();
};

BST::BST(): root(nullptr) {};

void BST::insert(int val) {
    Node* cur = root;
    Node* pre = nullptr;
    if(!root) {
        root = new Node{val};
        return;
    }
    while(cur != nullptr) {
        pre = cur;
        if(val < cur->val) cur = cur->left;
        else if( val > cur->val ) cur = cur->right;
    }
    if(val < pre->val) pre->left = new Node(val);
    else if( val > pre->val ) pre->right = new Node(val);
}

void inTraverse(Node *root) {
    if(!root) return;
    inTraverse(root->left);
    std::cout << root->val << " | ";
    inTraverse(root->right);
}

int main() {
    std::unordered_map<Node*,int> root_to_leaf{};
    std::unordered_map<Node*,int> leaf_to_leaf{};

    std::function<int(Node*)> max_root_to_leaf = [&](Node* root)->int {
        if(!root) return 0;
        if(root_to_leaf.find(root) != root_to_leaf.end()) return root_to_leaf[root];
        int maxLeft = max_root_to_leaf(root->left),maxRight = max_root_to_leaf(root->right);
        return std::max(maxLeft,maxRight) + root->val;
    };

    std::function<int(Node*)> max_leaf_to_leaf = [&](Node* root)->int {
        if(root->left == nullptr || root->right == nullptr ) return 0;
        if(leaf_to_leaf.find(root) != leaf_to_leaf.end()) return leaf_to_leaf[root];
        int containRoot = root->val + max_root_to_leaf(root->left) + max_root_to_leaf(root->right);
        int excludeRoot = max_leaf_to_leaf(root->right);
        return std::max(containRoot,excludeRoot);
    };


    std::ifstream fin{"tree.in"};
    int val;
    BST searchTree;
    if(fin.is_open()) {
        while(fin >> val) { 
            searchTree.insert(val); }
        fin.close();
    }
    std::cout << max_leaf_to_leaf(searchTree.root);
}
