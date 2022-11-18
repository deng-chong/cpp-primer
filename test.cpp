#include<iostream>
#include<string>
#include<functional>

class BinStrTree;

class TreeNode {
    friend class BinStrTree;
private:
    std::string value;
    int *count;
    TreeNode *left;
    TreeNode *right;
public:
    TreeNode(const std::string& s="", TreeNode *lchild=nullptr, TreeNode *rchild=nullptr):
        value(s), count(new int(1)), left(lchild), right(rchild){
            std::cout << value << " constructed.\n";
        }
    
    ~TreeNode() {
        std::cout << value << " going to be destroyed.\n";
        if (--*count == 0) {//引用计数归零后，将左右子节点都删除
            delete count;
            delete left;
            delete right;
        }
        std::cout << value << " destroyed.\n";
    }
    TreeNode(const TreeNode& rhs) {
        value = rhs.value;
        count = rhs.count;
        left = rhs.left;
        right = rhs.right;
        ++*count;
    }
    TreeNode& operator=(const TreeNode& rhs) {
        if (this != &rhs) {
            auto tmp(rhs);
            std::swap(value, tmp.value);
            std::swap(count, tmp.count);
            std::swap(left, tmp.left);
            std::swap(right, tmp.right);
        }
        return *this;
    }
};

class BinStrTree {
private:
    TreeNode *root;
    int *count;
public:
    BinStrTree(TreeNode *p=nullptr): root(p), count(new int(1)) {}
    ~BinStrTree() {
        if (--*count == 0) {
            delete root;
            delete count;
        }
    }
    BinStrTree(const BinStrTree& rhs) {
        root = rhs.root;
        count = rhs.count;
        ++*count;
    }
    BinStrTree& operator=(const BinStrTree& rhs) {
        if (this != &rhs) {
            BinStrTree tmp(rhs);
            std::swap(root, tmp.root);
            std::swap(count, tmp.count);
        }
        return *this;
    }

    void show() const {
        std::function<void(TreeNode*, int)> help = [&](TreeNode* p, int dep) {
            if (!p) {
                for (int i = 0; i < dep; ++i) std::cout << "    ";
                std::cout << "null\n";
                return;
            }
            if (p->left || p->right) help(p->right, dep + 1);
            for (int i = 0; i < dep; ++i) std::cout << "    ";
            std::cout << p->value << "\n";
            if (p->left || p->right) help(p->left, dep + 1);
        };

        if (!root) std::cout << "An empty Binary String Tree.\n";
        else help(root, 0);
    }
};

int main() {
    TreeNode *G = new TreeNode("G");
    TreeNode *D = new TreeNode("D"), *E = new TreeNode("E"), *F = new TreeNode("F", nullptr, G);
    TreeNode *B = new TreeNode("B", D, E), *C = new TreeNode("C", nullptr, F), *A = new TreeNode("A", B, C);
    BinStrTree bst(A);
    bst.show();
/*
    std::cout << "\n";

    *F = *B;
    std::cout << "\n";

    delete A;
    std::cout << "\n";

    delete F;
    std::cout << "\n";
*/
}