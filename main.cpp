#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
using namespace std;

/*
    HEAP SORT - SẮP XẾP VUN ĐỐNG

    Dùng Max Heap để sắp xếp tăng dần.

    Quy ước lưu trữ cây bằng mảng 1-based:
    - Gốc ở vị trí i
    - Con trái  = 2 * i
    - Con phải  = 2 * i + 1
    - Cha       = i / 2
*/

void printArray(const vector<int>& a, int heapSize, string message) {
    cout << message << endl;

    cout << "Mang hien tai: ";
    for (int i = 1; i < a.size(); i++) {
        cout << setw(4) << a[i];
    }
    cout << endl;

    cout << "Vung heap    : ";
    for (int i = 1; i <= heapSize; i++) {
        cout << setw(4) << a[i];
    }

    if (heapSize < (int)a.size() - 1) {
        cout << "   | Da sap xep:";
        for (int i = heapSize + 1; i < a.size(); i++) {
            cout << setw(4) << a[i];
        }
    }

    cout << "\n" << endl;
}

void printTreeInfo(const vector<int>& a) {
    cout << "Bieu dien cay bang mang tuan tu:" << endl;

    cout << "Vi tri : ";
    for (int i = 1; i < a.size(); i++) {
        cout << setw(4) << i;
    }
    cout << endl;

    cout << "Gia tri: ";
    for (int i = 1; i < a.size(); i++) {
        cout << setw(4) << a[i];
    }
    cout << "\n" << endl;
}

/*
    Hàm vun đống tại vị trí i.

    Mục tiêu:
    Đưa phần tử lớn nhất trong 3 node:
    - node i
    - con trái của i
    - con phải của i
    lên vị trí i.

    Sau khi đổi chỗ, tiếp tục vun đống xuống dưới.
*/
void heapify(vector<int>& a, int heapSize, int i, int& step) {
    int largest = i;
    int left = 2 * i;
    int right = 2 * i + 1;

    if (left <= heapSize && a[left] > a[largest]) {
        largest = left;
    }

    if (right <= heapSize && a[right] > a[largest]) {
        largest = right;
    }

    if (largest != i) {
        cout << "Buoc " << step++ << ": Doi a[" << i << "] = " << a[i]
             << " voi a[" << largest << "] = " << a[largest] << endl;

        swap(a[i], a[largest]);

        printArray(a, heapSize, "Sau khi doi trong qua trinh vun dong:");

        heapify(a, heapSize, largest, step);
    }
}

/*
    Xây dựng Max Heap từ mảng ban đầu.

    Ta vun đống từ node không phải lá cuối cùng:
        i = n / 2
    về đến gốc:
        i = 1
*/
void buildMaxHeap(vector<int>& a, int n, int& step) {
    cout << "===== BAT DAU XAY DUNG MAX HEAP =====" << endl;

    for (int i = n / 2; i >= 1; i--) {
        cout << "Vun dong tai vi tri i = " << i
             << ", gia tri = " << a[i] << endl;

        heapify(a, n, i, step);

        printArray(a, n, "Trang thai sau khi vun dong tai vi tri i = " + to_string(i) + ":");
    }

    cout << "===== KET THUC XAY DUNG MAX HEAP =====\n" << endl;
}

/*
    Heap Sort:
    Bước 1: Xây dựng Max Heap.
    Bước 2: Đổi phần tử lớn nhất ở gốc với phần tử cuối heap.
    Bước 3: Giảm kích thước heap.
    Bước 4: Vun đống lại tại gốc.
*/
void heapSort(vector<int> a, string name) {
    cout << "\n==================================================" << endl;
    cout << "SAP XEP VUN DONG CHO: " << name << endl;
    cout << "==================================================\n" << endl;

    int n = a.size() - 1;
    int step = 1;

    printTreeInfo(a);
    printArray(a, n, "Trang thai ban dau:");

    buildMaxHeap(a, n, step);

    cout << "===== BAT DAU SAP XEP HEAP SORT =====" << endl;

    for (int heapSize = n; heapSize >= 2; heapSize--) {
        cout << "Buoc " << step++ << ": Doi goc heap a[1] = " << a[1]
             << " voi a[" << heapSize << "] = " << a[heapSize] << endl;

        swap(a[1], a[heapSize]);

        printArray(a, heapSize - 1, "Sau khi dua phan tu lon nhat ve cuoi:");

        cout << "Vun dong lai tai goc voi heapSize = " << heapSize - 1 << endl;

        heapify(a, heapSize - 1, 1, step);

        printArray(a, heapSize - 1, "Trang thai sau khi vun dong lai:");
    }

    cout << "===== KET QUA CUOI CUNG =====" << endl;

    cout << "Mang sau khi sap xep tang dan: ";
    for (int i = 1; i <= n; i++) {
        cout << setw(4) << a[i];
    }
    cout << "\n" << endl;
}

int main() {
    /*
        Cây trên bảng:

                    11
                  /    \
                54      32
               /  \    /  \
             10   58  78   20
            / \   / \  /
          16  84 17 29 15

        Lưu trữ tuần tự:
        vị trí:  1   2   3   4   5   6   7   8   9   10  11  12
        giá trị: 11  54  32  10  58  78  20  16  84  17  29  15
    */

    vector<int> cayTrenBang = {
        0,
        11, 54, 32, 10, 58, 78, 20, 16, 84, 17, 29, 15
    };

    /*
        Nếu trong slide có thêm cây khác,
        chỉ cần thêm mảng theo thứ tự lưu trữ tuần tự.

        Ví dụ:
        vector<int> caySlide1 = {0, 50, 30, 40, 10, 20, 35, 37};
        heapSort(caySlide1, "Cay trong slide 1");
    */

    heapSort(cayTrenBang, "Cay tren bang");

    return 0;
}