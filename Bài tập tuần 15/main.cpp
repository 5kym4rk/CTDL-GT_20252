#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

const int MAX_VERTEX = 11;

// Đánh số các đỉnh
enum City {
    HA_NOI,
    SON_TAY,
    HOA_BINH,
    THAI_NGUYEN,
    BAC_NINH,
    HAI_DUONG,
    PHU_LY,
    BAC_GIANG,
    UONG_BI,
    HAI_PHONG,
    HUNG_YEN
};

string cityName[MAX_VERTEX] = {
    "Ha Noi",
    "Son Tay",
    "Hoa Binh",
    "Thai Nguyen",
    "Bac Ninh",
    "Hai Duong",
    "Phu Ly",
    "Bac Giang",
    "Uong Bi",
    "Hai Phong",
    "Hung Yen"
};

// Một cạnh của đồ thị
struct Edge {
    int start;
    int end;
};

// Danh sách các tuyến đường trong hình
Edge edges[] = {
    {HA_NOI, HAI_DUONG},       // D1
    {HAI_DUONG, HUNG_YEN},     // D2
    {PHU_LY, HUNG_YEN},        // D3
    {HA_NOI, PHU_LY},          // D4
    {HA_NOI, HOA_BINH},        // D5
    {HA_NOI, SON_TAY},         // D6
    {HA_NOI, THAI_NGUYEN},     // D7
    {HA_NOI, BAC_NINH},        // D8
    {BAC_NINH, BAC_GIANG},     // D9
    {BAC_GIANG, UONG_BI},      // D10
    {BAC_NINH, UONG_BI},       // D11
    {UONG_BI, HAI_PHONG},      // D12
    {HAI_DUONG, HAI_PHONG}     // D13
};

const int EDGE_COUNT = sizeof(edges) / sizeof(edges[0]);

// ======================================================
// PHẦN 1: BIỂU DIỄN BẰNG MA TRẬN KỀ
// ======================================================

void createAdjacencyMatrix(int matrix[MAX_VERTEX][MAX_VERTEX]) {
    // Khởi tạo ma trận bằng 0
    for (int i = 0; i < MAX_VERTEX; i++) {
        for (int j = 0; j < MAX_VERTEX; j++) {
            matrix[i][j] = 0;
        }
    }

    // Thêm các cạnh
    for (int i = 0; i < EDGE_COUNT; i++) {
        int u = edges[i].start;
        int v = edges[i].end;

        matrix[u][v] = 1;
        matrix[v][u] = 1; // Đồ thị vô hướng
    }
}

void printAdjacencyMatrix(int matrix[MAX_VERTEX][MAX_VERTEX]) {
    cout << "Ma tran ke:\n\n";

    cout << "    ";
    for (int i = 0; i < MAX_VERTEX; i++) {
        cout << i << " ";
    }
    cout << "\n";

    for (int i = 0; i < MAX_VERTEX; i++) {
        cout << i << " : ";

        for (int j = 0; j < MAX_VERTEX; j++) {
            cout << matrix[i][j] << " ";
        }

        cout << "\n";
    }
}

// BFS bằng ma trận kề
vector<int> bfsMatrix(
    int matrix[MAX_VERTEX][MAX_VERTEX],
    int start
) {
    bool visited[MAX_VERTEX] = {false};
    queue<int> q;
    vector<int> result;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        result.push_back(current);

        // Xét tất cả đỉnh kề với current
        for (int i = 0; i < MAX_VERTEX; i++) {
            if (matrix[current][i] == 1 && !visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }

    return result;
}

// ======================================================
// PHẦN 2: BIỂU DIỄN BẰNG DANH SÁCH LIÊN KẾT
// ======================================================

struct Node {
    int vertex;
    Node* next;

    Node(int value) {
        vertex = value;
        next = nullptr;
    }
};

// Chèn đỉnh vào danh sách theo thứ tự tăng dần
void insertSorted(Node*& head, int vertex) {
    Node* newNode = new Node(vertex);

    if (head == nullptr || vertex < head->vertex) {
        newNode->next = head;
        head = newNode;
        return;
    }

    Node* current = head;

    while (current->next != nullptr &&
           current->next->vertex < vertex) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

void createAdjacencyList(Node* adjacencyList[MAX_VERTEX]) {
    for (int i = 0; i < MAX_VERTEX; i++) {
        adjacencyList[i] = nullptr;
    }

    for (int i = 0; i < EDGE_COUNT; i++) {
        int u = edges[i].start;
        int v = edges[i].end;

        // Đồ thị vô hướng nên thêm ở cả hai chiều
        insertSorted(adjacencyList[u], v);
        insertSorted(adjacencyList[v], u);
    }
}

void printAdjacencyList(Node* adjacencyList[MAX_VERTEX]) {
    cout << "\nDanh sach lien ket cac dinh ke:\n\n";

    for (int i = 0; i < MAX_VERTEX; i++) {
        cout << i << " - " << cityName[i] << ": ";

        Node* current = adjacencyList[i];

        while (current != nullptr) {
            cout << cityName[current->vertex];

            if (current->next != nullptr) {
                cout << " -> ";
            }

            current = current->next;
        }

        cout << "\n";
    }
}

// BFS bằng danh sách liên kết
vector<int> bfsAdjacencyList(
    Node* adjacencyList[MAX_VERTEX],
    int start
) {
    bool visited[MAX_VERTEX] = {false};
    queue<int> q;
    vector<int> result;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int currentVertex = q.front();
        q.pop();

        result.push_back(currentVertex);

        Node* current = adjacencyList[currentVertex];

        while (current != nullptr) {
            int adjacentVertex = current->vertex;

            if (!visited[adjacentVertex]) {
                visited[adjacentVertex] = true;
                q.push(adjacentVertex);
            }

            current = current->next;
        }
    }

    return result;
}

// Giải phóng bộ nhớ danh sách liên kết
void deleteAdjacencyList(Node* adjacencyList[MAX_VERTEX]) {
    for (int i = 0; i < MAX_VERTEX; i++) {
        Node* current = adjacencyList[i];

        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }

        adjacencyList[i] = nullptr;
    }
}

// In thứ tự duyệt BFS
void printBFSResult(const vector<int>& result) {
    for (int i = 0; i < static_cast<int>(result.size()); i++) {
        cout << cityName[result[i]];

        if (i < static_cast<int>(result.size()) - 1) {
            cout << " -> ";
        }
    }

    cout << "\n";
}

int main() {
    // ----------------------------------------
    // Cài đặt bằng ma trận kề
    // ----------------------------------------
    int adjacencyMatrix[MAX_VERTEX][MAX_VERTEX];

    createAdjacencyMatrix(adjacencyMatrix);
    printAdjacencyMatrix(adjacencyMatrix);

    vector<int> matrixResult =
        bfsMatrix(adjacencyMatrix, HA_NOI);

    cout << "\nBFS tu Ha Noi bang ma tran ke:\n";
    printBFSResult(matrixResult);

    // ----------------------------------------
    // Cài đặt bằng danh sách liên kết
    // ----------------------------------------
    Node* adjacencyList[MAX_VERTEX];

    createAdjacencyList(adjacencyList);
    printAdjacencyList(adjacencyList);

    vector<int> listResult =
        bfsAdjacencyList(adjacencyList, HA_NOI);

    cout << "\nBFS tu Ha Noi bang danh sach lien ket:\n";
    printBFSResult(listResult);

    deleteAdjacencyList(adjacencyList);

    return 0;
}