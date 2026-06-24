#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>

using namespace std;

const int N = 11;

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

const string cityName[N] = {
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

// Danh sách kề của đồ thị
vector<vector<int>> graph(N);

// Thêm một cạnh vô hướng
void addEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

// Khởi tạo đồ thị theo hình vẽ
void createGraph() {
    addEdge(HA_NOI, HAI_DUONG);       // D1
    addEdge(HAI_DUONG, HUNG_YEN);     // D2
    addEdge(PHU_LY, HUNG_YEN);        // D3
    addEdge(HA_NOI, PHU_LY);          // D4
    addEdge(HA_NOI, HOA_BINH);        // D5
    addEdge(HA_NOI, SON_TAY);         // D6
    addEdge(HA_NOI, THAI_NGUYEN);     // D7
    addEdge(HA_NOI, BAC_NINH);        // D8
    addEdge(BAC_NINH, BAC_GIANG);     // D9
    addEdge(BAC_GIANG, UONG_BI);      // D10
    addEdge(BAC_NINH, UONG_BI);       // D11
    addEdge(UONG_BI, HAI_PHONG);      // D12
    addEdge(HAI_DUONG, HAI_PHONG);    // D13
}

// Tạo đường đi từ mảng parent
vector<int> createPath(int start, int destination,
                       const vector<int>& parent) {
    vector<int> path;

    if (start == destination) {
        path.push_back(start);
        return path;
    }

    // Không tìm thấy đường đi
    if (parent[destination] == -1) {
        return path;
    }

    int current = destination;

    while (current != -1) {
        path.push_back(current);

        if (current == start) {
            break;
        }

        current = parent[current];
    }

    reverse(path.begin(), path.end());

    return path;
}

// DFS đệ quy
bool dfs(int current, int destination,
         vector<bool>& visited,
         vector<int>& parent) {
    visited[current] = true;

    if (current == destination) {
        return true;
    }

    for (int next : graph[current]) {
        if (!visited[next]) {
            parent[next] = current;

            if (dfs(next, destination, visited, parent)) {
                return true;
            }
        }
    }

    return false;
}

// Tìm một đường đi bất kỳ bằng DFS
vector<int> findPathDFS(int start, int destination) {
    vector<bool> visited(N, false);
    vector<int> parent(N, -1);

    bool found = dfs(start, destination, visited, parent);

    if (!found) {
        return {};
    }

    return createPath(start, destination, parent);
}

// Tìm đường đi ngắn nhất bằng BFS
vector<int> findShortestPathBFS(int start, int destination) {
    vector<bool> visited(N, false);
    vector<int> parent(N, -1);

    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == destination) {
            break;
        }

        for (int next : graph[current]) {
            if (!visited[next]) {
                visited[next] = true;
                parent[next] = current;
                q.push(next);
            }
        }
    }

    if (!visited[destination]) {
        return {};
    }

    return createPath(start, destination, parent);
}

// In một đường đi
void printPath(const vector<int>& path) {
    if (path.empty()) {
        cout << "Khong ton tai duong di.\n";
        return;
    }

    for (int i = 0; i < static_cast<int>(path.size()); i++) {
        cout << cityName[path[i]];

        if (i < static_cast<int>(path.size()) - 1) {
            cout << " -> ";
        }
    }

    cout << '\n';
}

// In danh sách các thành phố
void printCityList() {
    cout << "Danh sach cac tinh/thanh pho:\n";

    for (int i = 0; i < N; i++) {
        cout << i << ". " << cityName[i] << '\n';
    }
}

int main() {
    createGraph();

    printCityList();

    int start;
    int destination;

    cout << "\nNhap dinh bat dau: ";
    cin >> start;

    cout << "Nhap dinh ket thuc: ";
    cin >> destination;

    if (start < 0 || start >= N ||
        destination < 0 || destination >= N) {
        cout << "Dinh nhap vao khong hop le.\n";
        return 1;
    }

    vector<int> dfsPath = findPathDFS(start, destination);

    cout << "\nMot duong di tim bang DFS:\n";
    printPath(dfsPath);

    vector<int> shortestPath =
        findShortestPathBFS(start, destination);

    cout << "\nDuong di ngan nhat tim bang BFS:\n";
    printPath(shortestPath);

    if (!shortestPath.empty()) {
        cout << "So tuyen duong di qua: "
             << shortestPath.size() - 1 << '\n';
    }

    return 0;
}