#include <iostream>
using namespace std;

struct Node {
    int key;
    int count;      // số lần xuất hiện, dùng cho dữ liệu bị trùng
    Node* left;
    Node* right;

    Node(int value) {
        key = value;
        count = 1;
        left = right = nullptr;
    }
};

// Thêm một phần tử vào cây nhị phân tìm kiếm
Node* insert(Node* root, int value) {
    if (root == nullptr) {
        return new Node(value);
    }

    if (value < root->key) {
        root->left = insert(root->left, value);
    } 
    else if (value > root->key) {
        root->right = insert(root->right, value);
    } 
    else {
        root->count++; // nếu giá trị đã tồn tại thì tăng số lần xuất hiện
    }

    return root;
}

// Tìm kiếm giá trị trên cây nhị phân tìm kiếm
Node* search(Node* root, int value) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root->key == value) {
        return root;
    }

    if (value < root->key) {
        return search(root->left, value);
    } 
    else {
        return search(root->right, value);
    }
}

// Tìm kiếm và in đường đi tìm kiếm
Node* searchWithPath(Node* root, int value) {
    Node* current = root;

    cout << "Duong di tim kiem: ";

    while (current != nullptr) {
        cout << current->key;

        if (current->key == value) {
            cout << endl;
            return current;
        }

        cout << " -> ";

        if (value < current->key) {
            current = current->left;
        } 
        else {
            current = current->right;
        }
    }

    cout << "NULL" << endl;
    return nullptr;
}

// Duyệt giữa cây BST sẽ cho ra dãy tăng dần
void inorder(Node* root) {
    if (root == nullptr) {
        return;
    }

    inorder(root->left);

    for (int i = 0; i < root->count; i++) {
        cout << root->key << " ";
    }

    inorder(root->right);
}

// Giải phóng bộ nhớ
void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    int a[] = {2001, 2002, 2006, 2007, 2008, 2004, 2005, 2001, 1999, 2004};
    int n = sizeof(a) / sizeof(a[0]);

    Node* root = nullptr;

    for (int i = 0; i < n; i++) {
        root = insert(root, a[i]);
    }

    cout << "Cay nhi phan tim kiem duyet giua: ";
    inorder(root);
    cout << endl;

    int x;
    cout << "Nhap nam can tim: ";
    cin >> x;

    Node* result = searchWithPath(root, x);

    if (result != nullptr) {
        cout << "Tim thay nam " << x << endl;
        cout << "So lan xuat hien: " << result->count << endl;
    } 
    else {
        cout << "Khong tim thay nam " << x << endl;
    }

    deleteTree(root);

    return 0;
}