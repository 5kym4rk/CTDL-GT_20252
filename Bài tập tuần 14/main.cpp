#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    int height;

    Node(int x) {
        data = x;
        left = right = nullptr;
        height = 1;
    }
};

int getHeight(Node* root) {
    if (root == nullptr)
        return 0;
    return root->height;
}

int getBalance(Node* root) {
    if (root == nullptr)
        return 0;
    return getHeight(root->left) - getHeight(root->right);
}

// Xoay phải
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Xoay trái
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

Node* insertAVL(Node* root, int key) {
    // Bước 1: Chèn như cây nhị phân tìm kiếm
    if (root == nullptr)
        return new Node(key);

    if (key < root->data)
        root->left = insertAVL(root->left, key);
    else if (key > root->data)
        root->right = insertAVL(root->right, key);
    else
        return root; // Không thêm phần tử trùng

    // Bước 2: Cập nhật chiều cao
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // Bước 3: Tính hệ số cân bằng
    int balance = getBalance(root);

    // Trường hợp Left Left
    if (balance > 1 && key < root->left->data)
        return rotateRight(root);

    // Trường hợp Right Right
    if (balance < -1 && key > root->right->data)
        return rotateLeft(root);

    // Trường hợp Left Right
    if (balance > 1 && key > root->left->data) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Trường hợp Right Left
    if (balance < -1 && key < root->right->data) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// Duyệt trước: Root - Left - Right
void preorder(Node* root) {
    if (root != nullptr) {
        cout << root->data << " ";
        preorder(root->left);
        preorder(root->right);
    }
}

// Duyệt giữa: Left - Root - Right
void inorder(Node* root) {
    if (root != nullptr) {
        inorder(root->left);
        cout << root->data << " ";
        inorder(root->right);
    }
}

// Duyệt sau: Left - Right - Root
void postorder(Node* root) {
    if (root != nullptr) {
        postorder(root->left);
        postorder(root->right);
        cout << root->data << " ";
    }
}

int main() {
    Node* root = nullptr;

    int a[] = {32, 51, 27, 83, 96, 11, 45, 75, 66};
    int n = sizeof(a) / sizeof(a[0]);

    cout << "Them cac phan tu vao cay AVL:\n";
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
        root = insertAVL(root, a[i]);
    }

    cout << "\n\nDuyet truoc NLR: ";
    preorder(root);

    cout << "\nDuyet giua LNR: ";
    inorder(root);

    cout << "\nDuyet sau LRN: ";
    postorder(root);

    cout << endl;

    return 0;
}