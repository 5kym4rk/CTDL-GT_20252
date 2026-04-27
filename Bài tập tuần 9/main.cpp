#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <chrono>
#include <cassert>
using namespace std;

// =========================
// PHAN 1: DANH SACH SINH VIEN
// =========================
struct Ngay {
    int ngay, thang, nam;
};

struct SinhVien {
    char maSV[8];
    char hoTen[50];
    int gioiTinh; // 1: Nam, 0: Nu
    Ngay ngaySinh;
    char diaChi[100];
    char lop[12];
    char khoa[7];
};

struct Node {
    SinhVien data;
    Node *link;
};

struct List {
    Node *first;
    Node *last;
};

void initList(List &l) {
    l.first = nullptr;
    l.last = nullptr;
}

void safeCopy(char *dest, const char *src, size_t size) {
    if (size == 0) return;
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

SinhVien makeSV(const char *ma, const char *ten, int gt, Ngay ns,
                const char *diaChi, const char *lop, const char *khoa) {
    SinhVien sv{};
    safeCopy(sv.maSV, ma, sizeof(sv.maSV));
    safeCopy(sv.hoTen, ten, sizeof(sv.hoTen));
    sv.gioiTinh = gt;
    sv.ngaySinh = ns;
    safeCopy(sv.diaChi, diaChi, sizeof(sv.diaChi));
    safeCopy(sv.lop, lop, sizeof(sv.lop));
    safeCopy(sv.khoa, khoa, sizeof(sv.khoa));
    return sv;
}

Node* createNode(const SinhVien &sv) {
    Node *p = new Node;
    p->data = sv;
    p->link = nullptr;
    return p;
}

void addLast(List &l, const SinhVien &sv) {
    Node *p = createNode(sv);
    if (l.first == nullptr) {
        l.first = l.last = p;
    } else {
        l.last->link = p;
        l.last = p;
    }
}

int compareMaSV(const SinhVien &a, const SinhVien &b) {
    return strcmp(a.maSV, b.maSV);
}

void insertSortedByMaSV(List &l, const SinhVien &sv) {
    Node *p = createNode(sv);

    if (l.first == nullptr) {
        l.first = l.last = p;
        return;
    }

    // Chen dau danh sach
    if (compareMaSV(sv, l.first->data) < 0) {
        p->link = l.first;
        l.first = p;
        return;
    }

    // Tim vi tri chen
    Node *prev = l.first;
    Node *cur = l.first->link;
    while (cur != nullptr && compareMaSV(cur->data, sv) < 0) {
        prev = cur;
        cur = cur->link;
    }

    prev->link = p;
    p->link = cur;
    if (cur == nullptr) {
        l.last = p;
    }
}

void sortByMaSV(List &l) {
    List sorted;
    initList(sorted);

    Node *cur = l.first;
    while (cur != nullptr) {
        insertSortedByMaSV(sorted, cur->data);
        Node *old = cur;
        cur = cur->link;
        delete old;
    }

    l = sorted;
}

bool sameDate(const Ngay &a, const Ngay &b) {
    return a.ngay == b.ngay && a.thang == b.thang && a.nam == b.nam;
}

bool existsDate(const vector<Ngay> &dates, const Ngay &d) {
    for (const Ngay &x : dates) {
        if (sameDate(x, d)) return true;
    }
    return false;
}

int countBirthday(const List &l, const Ngay &d) {
    int count = 0;
    for (Node *p = l.first; p != nullptr; p = p->link) {
        if (sameDate(p->data.ngaySinh, d)) count++;
    }
    return count;
}

vector<Ngay> getDuplicateBirthdays(const List &l) {
    vector<Ngay> duplicateDates;
    for (Node *p = l.first; p != nullptr; p = p->link) {
        Ngay d = p->data.ngaySinh;
        if (countBirthday(l, d) >= 2 && !existsDate(duplicateDates, d)) {
            duplicateDates.push_back(d);
        }
    }
    return duplicateDates;
}

bool isDuplicateBirthday(const vector<Ngay> &duplicateDates, const Ngay &d) {
    return existsDate(duplicateDates, d);
}

void printDate(const Ngay &d) {
    cout << right << setw(2) << setfill('0') << d.ngay << "/"
         << setw(2) << setfill('0') << d.thang << "/"
         << setw(4) << setfill('0') << d.nam << setfill(' ') << left;
}

void printOneSV(const SinhVien &sv) {
    cout << left << setw(8) << sv.maSV
         << setw(18) << sv.hoTen
         << setw(6) << (sv.gioiTinh == 1 ? "Nam" : "Nu");
    printDate(sv.ngaySinh);
    cout << "  " << left << setw(16) << sv.diaChi
         << setw(10) << sv.lop
         << setw(8) << sv.khoa << '\n';
}

void printHeaderSV() {
    cout << left << setw(8) << "MaSV"
         << setw(18) << "Ho ten"
         << setw(6) << "GT"
         << setw(12) << "Ngay sinh"
         << setw(16) << "Dia chi"
         << setw(10) << "Lop"
         << setw(8) << "Khoa" << '\n';
    cout << string(78, '-') << '\n';
}

void printListSV(const List &l) {
    printHeaderSV();
    for (Node *p = l.first; p != nullptr; p = p->link) {
        printOneSV(p->data);
    }
}

void printSameBirthdayStudents(const List &l) {
    vector<Ngay> duplicateDates = getDuplicateBirthdays(l);
    if (duplicateDates.empty()) {
        cout << "Khong tim thay sinh vien cung ngay sinh.\n";
        return;
    }

    printHeaderSV();
    for (Node *p = l.first; p != nullptr; p = p->link) {
        if (isDuplicateBirthday(duplicateDates, p->data.ngaySinh)) {
            printOneSV(p->data);
        }
    }
}

void removeSameBirthdayStudents(List &l) {
    vector<Ngay> duplicateDates = getDuplicateBirthdays(l);
    if (duplicateDates.empty()) return;

    Node *prev = nullptr;
    Node *cur = l.first;
    while (cur != nullptr) {
        bool removeThis = isDuplicateBirthday(duplicateDates, cur->data.ngaySinh);
        if (removeThis) {
            Node *del = cur;
            cur = cur->link;

            if (prev == nullptr) {
                l.first = cur;
            } else {
                prev->link = cur;
            }
            if (del == l.last) {
                l.last = prev;
            }
            delete del;
        } else {
            prev = cur;
            cur = cur->link;
        }
    }
}

void clearListSV(List &l) {
    Node *cur = l.first;
    while (cur != nullptr) {
        Node *old = cur;
        cur = cur->link;
        delete old;
    }
    l.first = l.last = nullptr;
}

// Ham nhap thu cong neu can dung khi nop bai
void inputListSV(List &l) {
    int n;
    cout << "Nhap so luong sinh vien: ";
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; ++i) {
        SinhVien sv{};
        cout << "\nNhap sinh vien thu " << i + 1 << ":\n";
        cout << "Ma SV: "; cin.getline(sv.maSV, sizeof(sv.maSV));
        cout << "Ho ten: "; cin.getline(sv.hoTen, sizeof(sv.hoTen));
        cout << "Gioi tinh, 1 Nam, 0 Nu: "; cin >> sv.gioiTinh;
        cout << "Ngay sinh, ngay thang nam: ";
        cin >> sv.ngaySinh.ngay >> sv.ngaySinh.thang >> sv.ngaySinh.nam;
        cin.ignore();
        cout << "Dia chi: "; cin.getline(sv.diaChi, sizeof(sv.diaChi));
        cout << "Lop: "; cin.getline(sv.lop, sizeof(sv.lop));
        cout << "Khoa: "; cin.getline(sv.khoa, sizeof(sv.khoa));
        addLast(l, sv);
    }
}

// =========================
// PHAN 2: JOSEPHUS BANG DANH SACH LIEN KET VONG
// =========================
struct JNode {
    int value;
    JNode *next;
};

JNode* createJNode(int value) {
    JNode *p = new JNode;
    p->value = value;
    p->next = nullptr;
    return p;
}

pair<int, vector<int>> josephusLinkedList(int n, int m, bool saveOrder = true) {
    vector<int> eliminated;
    if (n <= 0 || m < 0) return {-1, eliminated};

    JNode *head = createJNode(1);
    JNode *tail = head;
    for (int i = 2; i <= n; ++i) {
        tail->next = createJNode(i);
        tail = tail->next;
    }
    tail->next = head; // Tao danh sach lien ket vong

    JNode *prev = tail;
    JNode *cur = head;
    int remaining = n;

    while (remaining > 1) {
        // M = 0: loai ngay nguoi dang giu bong.
        // M = 1: truyen sang nguoi ke tiep roi loai nguoi do.
        for (int step = 0; step < m; ++step) {
            prev = cur;
            cur = cur->next;
        }

        if (saveOrder) eliminated.push_back(cur->value);

        prev->next = cur->next;
        JNode *del = cur;
        cur = cur->next; // Nguoi ke tiep bat dau luot moi
        delete del;
        --remaining;
    }

    int winner = cur->value;
    delete cur;
    return {winner, eliminated};
}

void printVector(const vector<int> &a) {
    for (size_t i = 0; i < a.size(); ++i) {
        if (i > 0) cout << ", ";
        cout << a[i];
    }
}

// =========================
// TEST CASES
// =========================
void testStudentList() {
    cout << "\n================ TEST 1: QUAN LY SINH VIEN ================\n";
    List l;
    initList(l);

    // Tao danh sach ban dau chua sap xep
    addLast(l, makeSV("SV003", "Le Van C", 1, {12, 5, 2005}, "Ha Noi", "CTK45", "CNTT"));
    addLast(l, makeSV("SV001", "Nguyen An", 1, {1, 1, 2005}, "Da Nang", "CTK45", "CNTT"));
    addLast(l, makeSV("SV005", "Pham Thi E", 0, {12, 5, 2005}, "Hue", "CTK46", "CNTT"));
    addLast(l, makeSV("SV002", "Tran Binh", 1, {20, 9, 2004}, "TP HCM", "CTK45", "CNTT"));
    addLast(l, makeSV("SV004", "Do Thi D", 0, {20, 9, 2004}, "Can Tho", "CTK46", "CNTT"));

    cout << "Danh sach ban dau, chua sap xep:\n";
    printListSV(l);

    sortByMaSV(l);
    cout << "\nDanh sach sau khi sap xep tang dan theo maSV:\n";
    printListSV(l);

    SinhVien newSV = makeSV("SV000", "Hoang Moi", 1, {7, 7, 2005}, "Hai Phong", "CTK44", "CNTT");
    insertSortedByMaSV(l, newSV);
    cout << "\nSau khi chen SV000 vao danh sach da sap xep:\n";
    printListSV(l);

    cout << "\nCac sinh vien co cung ngay sinh:\n";
    printSameBirthdayStudents(l);

    removeSameBirthdayStudents(l);
    cout << "\nSau khi loai bo sinh vien co cung ngay sinh:\n";
    printListSV(l);

    cout << "\nKiem tra lai danh sach sau xoa:\n";
    printSameBirthdayStudents(l);

    clearListSV(l);
}

void testJosephus() {
    cout << "\n================ TEST 2: JOSEPHUS ================\n";

    auto t0 = josephusLinkedList(5, 0);
    cout << "N = 5, M = 0 => Thu tu loai: "; printVector(t0.second);
    cout << " | Nguoi thang: " << t0.first << '\n';
    assert(t0.first == 5);

    auto t1 = josephusLinkedList(5, 1);
    cout << "N = 5, M = 1 => Thu tu loai: "; printVector(t1.second);
    cout << " | Nguoi thang: " << t1.first << '\n';
    assert(t1.first == 3);

    for (int m = 1; m <= 3; ++m) {
        auto ans = josephusLinkedList(10, m);
        cout << "N = 10, M = " << m << " => Thu tu loai: ";
        printVector(ans.second);
        cout << " | Nguoi thang: " << ans.first << '\n';
    }

    const int bigN = 100001;
    auto start = chrono::high_resolution_clock::now();
    auto big = josephusLinkedList(bigN, 1, false);
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();

    cout << "\nTest hieu nang voi N = " << bigN << ", M = 1\n";
    cout << "Nguoi thang: " << big.first << '\n';
    cout << "Thoi gian chay: " << fixed << setprecision(3) << ms << " ms\n";
    cout << "Do phuc tap khi M = 1: O(N) thoi gian, O(N) bo nho.\n";
}

int main() {
    testStudentList();
    testJosephus();
    cout << "\nTat ca test case da chay xong.\n";
    return 0;
}
