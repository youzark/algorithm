#include <vector>
#include <fstream>
#include <ostream>
#include <string>
#include <functional>
#include <stack>
#include <iostream>

class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node(int val):
        val(val),
        left(nullptr),
        right(nullptr) {}
};

class BST {
private:
    Node* root_;
public:
    BST():root_(nullptr){}
    BST(Node* root):root_(root){}

    void insert(int val); 
    Node* root() {
        return root_;
    }
};

void BST::insert(int val) {
    if(root_ == nullptr) { root_ = new Node{val}; }
    Node *pre, *cur = root_;
    while(cur != nullptr) {
        pre = cur;
        if(cur->val == val) return;
        if(cur->val > val ) { cur = cur->left; }
        else { cur = cur->right; }
    }
    if( val < pre->val ) pre->left = new Node{val};
    else pre->right = new Node{val};
}

int main() {
    int num;
    std::ifstream fin{"tree.in"};
    BST searchTree;
    if(fin.is_open()) {
        while(fin >> num && num != 0) {
            searchTree.insert(num);
        }
        fin.close();
    }

    std::ofstream fout{"tree.out"};

    std::function<void(Node*)> postTraverse = [&](Node *root) {
        if(root == nullptr) return;
        postTraverse(root->left);
        postTraverse(root->right);
        fout << root->val << " ";
    };
    if(fout.is_open()) {
        postTraverse(searchTree.root());
    }
}
