#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* =========================================================
   PHẦN 1: CÂY NHỊ PHÂN LƯU TRỮ MÓC NỐI
   ========================================================= */

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

void preorder(Node* root) {
    if (root == nullptr) return;

    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node* root) {
    if (root == nullptr) return;

    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

void postorder(Node* root) {
    if (root == nullptr) return;

    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}

/*
    Cây đầy đủ:

              1
            /   \
           2     3
          / \   / \
         4   5 6   7
*/
Node* createFullTreeLinked() {
    Node* root = new Node(1);

    root->left = new Node(2);
    root->right = new Node(3);

    root->left->left = new Node(4);
    root->left->right = new Node(5);

    root->right->left = new Node(6);
    root->right->right = new Node(7);

    return root;
}

/*
    Cây lệch trái:

        1
       /
      2
     /
    3
   /
  4
 /
5
*/
Node* createLeftSkewedTreeLinked() {
    Node* root = new Node(1);

    root->left = new Node(2);
    root->left->left = new Node(3);
    root->left->left->left = new Node(4);
    root->left->left->left->left = new Node(5);

    return root;
}

/*
    Cây lệch phải:

    1
     \
      2
       \
        3
         \
          4
           \
            5
*/
Node* createRightSkewedTreeLinked() {
    Node* root = new Node(1);

    root->right = new Node(2);
    root->right->right = new Node(3);
    root->right->right->right = new Node(4);
    root->right->right->right->right = new Node(5);

    return root;
}

/*
    Cây zigzac:

        1
       /
      2
       \
        3
       /
      4
       \
        5
*/
Node* createZigZagTreeLinked() {
    Node* root = new Node(1);

    root->left = new Node(2);
    root->left->right = new Node(3);
    root->left->right->left = new Node(4);
    root->left->right->left->right = new Node(5);

    return root;
}

void printLinkedTreeTraversal(Node* root) {
    cout << "Duyet truoc  NLR: ";
    preorder(root);
    cout << endl;

    cout << "Duyet giua   LNR: ";
    inorder(root);
    cout << endl;

    cout << "Duyet sau    LRN: ";
    postorder(root);
    cout << endl;
}

/* =========================================================
   PHẦN 2: CÂY NHỊ PHÂN LƯU TRỮ TUẦN TỰ BẰNG MẢNG
   Quy ước:
   - Gốc ở vị trí i = 1
   - Con trái của i là 2 * i
   - Con phải của i là 2 * i + 1
   - Nếu vị trí không có node thì dùng -1
   ========================================================= */

const int EMPTY = -1;

void preorderArray(const vector<int>& tree, int i) {
    if (i >= tree.size() || tree[i] == EMPTY) return;

    cout << tree[i] << " ";
    preorderArray(tree, 2 * i);
    preorderArray(tree, 2 * i + 1);
}

void inorderArray(const vector<int>& tree, int i) {
    if (i >= tree.size() || tree[i] == EMPTY) return;

    inorderArray(tree, 2 * i);
    cout << tree[i] << " ";
    inorderArray(tree, 2 * i + 1);
}

void postorderArray(const vector<int>& tree, int i) {
    if (i >= tree.size() || tree[i] == EMPTY) return;

    postorderArray(tree, 2 * i);
    postorderArray(tree, 2 * i + 1);
    cout << tree[i] << " ";
}

/*
    Cây đầy đủ lưu trữ tuần tự:

    index:  1 2 3 4 5 6 7
    value:  1 2 3 4 5 6 7
*/
vector<int> createFullTreeArray() {
    vector<int> tree(8, EMPTY);

    tree[1] = 1;
    tree[2] = 2;
    tree[3] = 3;
    tree[4] = 4;
    tree[5] = 5;
    tree[6] = 6;
    tree[7] = 7;

    return tree;
}

/*
    Cây lệch trái:

        1
       /
      2
     /
    3
   /
  4
 /
5

    Vị trí trong mảng: 1, 2, 4, 8, 16
*/
vector<int> createLeftSkewedTreeArray() {
    vector<int> tree(32, EMPTY);

    tree[1] = 1;
    tree[2] = 2;
    tree[4] = 3;
    tree[8] = 4;
    tree[16] = 5;

    return tree;
}

/*
    Cây lệch phải:

    1
     \
      2
       \
        3
         \
          4
           \
            5

    Vị trí trong mảng: 1, 3, 7, 15, 31
*/
vector<int> createRightSkewedTreeArray() {
    vector<int> tree(64, EMPTY);

    tree[1] = 1;
    tree[3] = 2;
    tree[7] = 3;
    tree[15] = 4;
    tree[31] = 5;

    return tree;
}

/*
    Cây zigzac:

        1
       /
      2
       \
        3
       /
      4
       \
        5

    Vị trí trong mảng:
    1
    left của 1: 2
    right của 2: 5
    left của 5: 10
    right của 10: 21
*/
vector<int> createZigZagTreeArray() {
    vector<int> tree(32, EMPTY);

    tree[1] = 1;
    tree[2] = 2;
    tree[5] = 3;
    tree[10] = 4;
    tree[21] = 5;

    return tree;
}

void printArrayTreeTraversal(const vector<int>& tree) {
    cout << "Duyet truoc  NLR: ";
    preorderArray(tree, 1);
    cout << endl;

    cout << "Duyet giua   LNR: ";
    inorderArray(tree, 1);
    cout << endl;

    cout << "Duyet sau    LRN: ";
    postorderArray(tree, 1);
    cout << endl;
}

/* =========================================================
   PHẦN 3: CÂY BIỂU THỨC THEO ẢNH TRÊN BẢNG
   ========================================================= */

struct ExprNode {
    string data;
    ExprNode* left;
    ExprNode* right;

    ExprNode(string value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

// Duyệt trước cây biểu thức
void preorderExpr(ExprNode* root) {
    if (root == nullptr) return;

    cout << root->data << " ";
    preorderExpr(root->left);
    preorderExpr(root->right);
}

// Duyệt giữa cây biểu thức
void inorderExpr(ExprNode* root) {
    if (root == nullptr) return;

    bool isOperatorNode = root->left != nullptr || root->right != nullptr;

    if (isOperatorNode) cout << "(";

    inorderExpr(root->left);
    cout << root->data;
    inorderExpr(root->right);

    if (isOperatorNode) cout << ")";
}

// Duyệt sau cây biểu thức
void postorderExpr(ExprNode* root) {
    if (root == nullptr) return;

    postorderExpr(root->left);
    postorderExpr(root->right);
    cout << root->data << " ";
}

/*
    Cây biểu thức trên bảng:

                    -
                  /   \
                 /     ^
                /     / \
               /     -   g
              /     / \
             /     *   h
            /     / \
           /     e   f
          /
         /
        /
       /
      /
     /
    /
   /
  /
 /
/
/
        /
       / \
      +   d
     / \
    *   c
   / \
  a   b

    Biểu thức trung tố:
    ((a * b + c) / d) - (((e * f) - h) ^ g)
*/

ExprNode* createExpressionTree() {
    ExprNode* root = new ExprNode("-");

    // Nhánh trái: ((a * b + c) / d)
    root->left = new ExprNode("/");

    root->left->left = new ExprNode("+");
    root->left->right = new ExprNode("d");

    root->left->left->left = new ExprNode("*");
    root->left->left->right = new ExprNode("c");

    root->left->left->left->left = new ExprNode("a");
    root->left->left->left->right = new ExprNode("b");

    // Nhánh phải: (((e * f) - h) ^ g)
    root->right = new ExprNode("^");

    root->right->left = new ExprNode("-");
    root->right->right = new ExprNode("g");

    root->right->left->left = new ExprNode("*");
    root->right->left->right = new ExprNode("h");

    root->right->left->left->left = new ExprNode("e");
    root->right->left->left->right = new ExprNode("f");

    return root;
}

void printExpressionTreeTraversal(ExprNode* root) {
    cout << "Duyet truoc, tien to : ";
    preorderExpr(root);
    cout << endl;

    cout << "Duyet giua, trung to : ";
    inorderExpr(root);
    cout << endl;

    cout << "Duyet sau, hau to    : ";
    postorderExpr(root);
    cout << endl;
}

/* =========================================================
   HÀM MAIN
   ========================================================= */

int main() {
    cout << "========================================" << endl;
    cout << "       BAI TAP CAY NHI PHAN C++" << endl;
    cout << "========================================" << endl;

    /* ==============================
       1. Lưu trữ móc nối
       ============================== */

    cout << "\n\n===== 1. CAY LUU TRU MOC NOI =====" << endl;

    cout << "\n--- Cay day du ---" << endl;
    Node* fullLinked = createFullTreeLinked();
    printLinkedTreeTraversal(fullLinked);

    cout << "\n--- Cay lech trai ---" << endl;
    Node* leftLinked = createLeftSkewedTreeLinked();
    printLinkedTreeTraversal(leftLinked);

    cout << "\n--- Cay lech phai ---" << endl;
    Node* rightLinked = createRightSkewedTreeLinked();
    printLinkedTreeTraversal(rightLinked);

    cout << "\n--- Cay zigzac ---" << endl;
    Node* zigzagLinked = createZigZagTreeLinked();
    printLinkedTreeTraversal(zigzagLinked);

    /* ==============================
       2. Lưu trữ tuần tự bằng mảng
       ============================== */

    cout << "\n\n===== 2. CAY LUU TRU TUAN TU BANG MANG =====" << endl;

    cout << "\n--- Cay day du ---" << endl;
    vector<int> fullArray = createFullTreeArray();
    printArrayTreeTraversal(fullArray);

    cout << "\n--- Cay lech trai ---" << endl;
    vector<int> leftArray = createLeftSkewedTreeArray();
    printArrayTreeTraversal(leftArray);

    cout << "\n--- Cay lech phai ---" << endl;
    vector<int> rightArray = createRightSkewedTreeArray();
    printArrayTreeTraversal(rightArray);

    cout << "\n--- Cay zigzac ---" << endl;
    vector<int> zigzagArray = createZigZagTreeArray();
    printArrayTreeTraversal(zigzagArray);

    /* ==============================
       3. Cây biểu thức
       ============================== */

    cout << "\n\n===== 3. CAY BIEU THUC =====" << endl;

    ExprNode* exprRoot = createExpressionTree();

    cout << "\nBieu thuc trung to day du: ";
    inorderExpr(exprRoot);
    cout << endl;

    cout << "\nKet qua cac phep duyet:" << endl;
    printExpressionTreeTraversal(exprRoot);

    return 0;
}