#include <iostream>
#include <filesystem>
#include <string>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <limits>

using namespace std;
namespace fs = std::filesystem;

struct FileInfo {
    string name;
    string fullPath;
    uintmax_t sizeBytes;
    fs::file_time_type modifiedTime;
};

struct Node {
    FileInfo data;
    Node* next;

    Node(const FileInfo& f) : data(f), next(nullptr) {}
};

string formatSize(uintmax_t bytes) {
    const double KB = 1024.0;
    const double MB = KB * 1024.0;
    const double GB = MB * 1024.0;

    ostringstream oss;
    oss << fixed << setprecision(2);

    if (bytes >= (uintmax_t)GB) {
        oss << (bytes / GB) << " GB";
    } else if (bytes >= (uintmax_t)MB) {
        oss << (bytes / MB) << " MB";
    } else if (bytes >= (uintmax_t)KB) {
        oss << (bytes / KB) << " KB";
    } else {
        oss << bytes << " B";
    }
    return oss.str();
}

string timeToString(const fs::file_time_type& ftp) {
    using namespace std::chrono;

    auto sctp = time_point_cast<system_clock::duration>(
        ftp - fs::file_time_type::clock::now() + system_clock::now()
    );

    time_t tt = system_clock::to_time_t(sctp);
    tm localTm{};

#ifdef _WIN32
    localtime_s(&localTm, &tt);
#else
    localtime_r(&tt, &localTm);
#endif

    ostringstream oss;
    oss << put_time(&localTm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

FileInfo readFileInfo(const fs::path& p) {
    FileInfo f;
    f.name = p.filename().string();
    f.fullPath = fs::absolute(p).lexically_normal().string();
    f.sizeBytes = fs::file_size(p);
    f.modifiedTime = fs::last_write_time(p);
    return f;
}

void clearList(Node*& head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

void printList(Node* head) {
    if (head == nullptr) {
        cout << "Danh sach rong.\n";
        return;
    }

    int stt = 1;
    Node* cur = head;
    while (cur != nullptr) {
        cout << stt++ << ". "
             << cur->data.name
             << " | Size: " << formatSize(cur->data.sizeBytes)
             << " | Time: " << timeToString(cur->data.modifiedTime)
             << "\n   Path: " << cur->data.fullPath << "\n";
        cur = cur->next;
    }
}

void insertSortedByTime(Node*& head, const FileInfo& f) {
    Node* newNode = new Node(f);

    if (head == nullptr || f.modifiedTime < head->data.modifiedTime) {
        newNode->next = head;
        head = newNode;
        return;
    }

    Node* cur = head;
    while (cur->next != nullptr &&
           cur->next->data.modifiedTime <= f.modifiedTime) {
        cur = cur->next;
    }

    newNode->next = cur->next;
    cur->next = newNode;
}

bool removeByPath(Node*& head, const string& targetPath) {
    if (head == nullptr) return false;

    if (head->data.fullPath == targetPath) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    Node* cur = head;
    while (cur->next != nullptr) {
        if (cur->next->data.fullPath == targetPath) {
            Node* temp = cur->next;
            cur->next = temp->next;
            delete temp;
            return true;
        }
        cur = cur->next;
    }
    return false;
}

void loadDirectory(Node*& head, const fs::path& folder) {
    clearList(head);

    if (!fs::exists(folder) || !fs::is_directory(folder)) {
        throw runtime_error("Thu muc khong ton tai: " + folder.string());
    }

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            FileInfo f = readFileInfo(entry.path());
            insertSortedByTime(head, f);
        }
    }
}

bool copyFileAndInsert(const fs::path& source, const fs::path& destFolder, Node*& head) {
    if (!fs::exists(source) || !fs::is_regular_file(source)) {
        cout << "File nguon khong hop le.\n";
        return false;
    }

    if (!fs::exists(destFolder) || !fs::is_directory(destFolder)) {
        cout << "Thu muc dich khong hop le.\n";
        return false;
    }

    fs::path dest = destFolder / source.filename();

    try {
        fs::copy_file(source, dest, fs::copy_options::overwrite_existing);

        // Giữ lại thời gian của file nguồn để đúng ý "sắp xếp theo thời gian"
        auto srcTime = fs::last_write_time(source);
        fs::last_write_time(dest, srcTime);

        string absDest = fs::absolute(dest).lexically_normal().string();

        // Nếu file cùng tên đã tồn tại trong danh sách, xóa node cũ trước
        removeByPath(head, absDest);

        FileInfo f = readFileInfo(dest);
        insertSortedByTime(head, f);

        cout << "Copy thanh cong: " << source << " -> " << dest << "\n";
        return true;
    } catch (const exception& e) {
        cout << "Loi copy file: " << e.what() << "\n";
        return false;
    }
}

uintmax_t totalSize(Node* head) {
    uintmax_t sum = 0;
    Node* cur = head;
    while (cur != nullptr) {
        sum += cur->data.sizeBytes;
        cur = cur->next;
    }
    return sum;
}

bool removeSmallest(Node*& head, FileInfo& removed) {
    if (head == nullptr) return false;

    Node* minNode = head;
    Node* minPrev = nullptr;

    Node* prev = head;
    Node* cur = head->next;

    while (cur != nullptr) {
        if (cur->data.sizeBytes < minNode->data.sizeBytes) {
            minNode = cur;
            minPrev = prev;
        }
        prev = cur;
        cur = cur->next;
    }

    removed = minNode->data;

    if (minPrev == nullptr) {
        head = head->next;
    } else {
        minPrev->next = minNode->next;
    }

    delete minNode;
    return true;
}

Node* cloneList(Node* head) {
    if (head == nullptr) return nullptr;

    Node* newHead = nullptr;
    Node* tail = nullptr;

    Node* cur = head;
    while (cur != nullptr) {
        Node* node = new Node(cur->data);
        if (newHead == nullptr) {
            newHead = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        cur = cur->next;
    }

    return newHead;
}

void fitToUsbByRemovingSmallest(Node*& head, uintmax_t capacity) {
    uintmax_t sum = totalSize(head);

    cout << "Tong dung luong hien tai: " << formatSize(sum) << "\n";
    cout << "Dung luong USB: " << formatSize(capacity) << "\n";

    if (sum <= capacity) {
        cout << "Khong can loai file. Du dieu kien backup.\n";
        return;
    }

    cout << "Can loai bo file nho nhat cho den khi tong <= 32GB.\n";

    while (head != nullptr && sum > capacity) {
        FileInfo removed;
        if (removeSmallest(head, removed)) {
            sum -= removed.sizeBytes;
            cout << "Da loai: " << removed.name
                 << " | " << formatSize(removed.sizeBytes)
                 << " | Tong con lai: " << formatSize(sum) << "\n";
        }
    }

    if (sum <= capacity) {
        cout << "Danh sach da phu hop de backup.\n";
    } else {
        cout << "Khong con file nao de backup.\n";
    }
}

void copyListToFolder(Node* head, const fs::path& destFolder) {
    if (head == nullptr) {
        cout << "Danh sach backup rong.\n";
        return;
    }

    try {
        fs::create_directories(destFolder);

        Node* cur = head;
        while (cur != nullptr) {
            fs::path src = cur->data.fullPath;
            fs::path dst = destFolder / cur->data.name;

            fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
            fs::last_write_time(dst, fs::last_write_time(src));

            cur = cur->next;
        }

        cout << "Da sao luu cac file con lai vao: " << destFolder << "\n";
    } catch (const exception& e) {
        cout << "Loi sao luu: " << e.what() << "\n";
    }
}

void showMenu() {
    cout << "\n========== MENU ==========\n";
    cout << "1. Nap danh sach tu D:\\Document\n";
    cout << "2. Hien thi danh sach file\n";
    cout << "3. Copy file tu noi khac vao D:\\Document va chen theo thoi gian\n";
    cout << "4. Tinh tong kich thuoc file\n";
    cout << "5. Lap danh sach backup cho USB 32GB (loai file nho nhat neu can)\n";
    cout << "0. Thoat\n";
    cout << "Chon: ";
}

int main() {
    const fs::path DOCUMENT_FOLDER = R"(D:\Document)";
    const uintmax_t USB_32GB = 32ULL * 1024ULL * 1024ULL * 1024ULL;

    Node* head = nullptr;
    int choice;

    do {
        showMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            switch (choice) {
                case 1: {
                    loadDirectory(head, DOCUMENT_FOLDER);
                    cout << "Da nap danh sach file tu " << DOCUMENT_FOLDER << "\n";
                    break;
                }

                case 2: {
                    printList(head);
                    break;
                }

                case 3: {
                    string srcPath;
                    cout << "Nhap duong dan file nguon: ";
                    getline(cin, srcPath);

                    copyFileAndInsert(srcPath, DOCUMENT_FOLDER, head);
                    break;
                }

                case 4: {
                    uintmax_t sum = totalSize(head);
                    cout << "Tong kich thuoc cac file: " << formatSize(sum) << "\n";
                    break;
                }

                case 5: {
                    Node* backupList = cloneList(head);

                    fitToUsbByRemovingSmallest(backupList, USB_32GB);

                    cout << "\nDanh sach file duoc backup:\n";
                    printList(backupList);

                    char ans;
                    cout << "\nBan co muon copy danh sach backup sang thu muc USB gia lap khong? (y/n): ";
                    cin >> ans;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (ans == 'y' || ans == 'Y') {
                        string usbPath;
                        cout << "Nhap duong dan thu muc USB gia lap: ";
                        getline(cin, usbPath);
                        copyListToFolder(backupList, usbPath);
                    }

                    clearList(backupList);
                    break;
                }

                case 0:
                    cout << "Ket thuc chuong trinh.\n";
                    break;

                default:
                    cout << "Lua chon khong hop le.\n";
            }
        } catch (const exception& e) {
            cout << "Loi: " << e.what() << "\n";
        }

    } while (choice != 0);

    clearList(head);
    return 0;
}