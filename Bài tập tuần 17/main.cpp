#include <iostream>
#include <string>
#include <queue>
#include <iomanip>
using namespace std;

const int Sotinh_max = 11;
const int INF = 1e9;    

const string DSTinh[Sotinh_max] = { "HN", "TN", "BN", "BG", "UB", "HP", "HD", "HY", "PL", "HB", "ST" }; 

struct Matran {   // khai báo
    int Sotinhxet;
    int matrix[Sotinh_max][Sotinh_max];
};

void Khoitaomatran(Matran &g, int n) { // tạo mtr trống (0)
    g.Sotinhxet = n;
    for (int i = 0; i < n; i++) {     // mtr trống
        for (int j = 0; j < n; j++)  g.matrix[i][j] = 0; }
}

void ThemDuongMaTran(Matran &g, int u, int v, int matuyen) {
    g.matrix[u][v] = matuyen;  
    g.matrix[v][u] = matuyen; }

struct CanhChon {  
     int u, v;   };
     void ThuatToanPrim(Matran &g) {
    int n = g.Sotinhxet;
    bool Daduyet[Sotinh_max] = {false};  // Mang danh dau tinh da duyet ( nam trong cay)
    Daduyet[9] = true;     // Duyet tu Hoa Binh (10), danh so tu 0 -> HB(9)

    CanhChon dsCanh[Sotinh_max];
    int soCanh = 0;
    cout << "\nThuat toan PRIM (Bat dau tu dinh Hoa Binh):\n";
    for (int i = 0; i < n - 1; i++) { // n đinh, duyet n-1 lân
        int trso_min = INF;
        int u_min = -1, v_min = -1;  // bien danh dau

    for (int i = 0; i < n; i++) {
        if (Daduyet[i]) {   // Duyet cac dinh da nam trong cay khung
            for (int j = 0; j < n; j++) {  
            if (!Daduyet[j] && g.matrix[i][j] > 0 && g.matrix[i][j] < trso_min) { 
            // cac dinh co duong di, chua nam trong cay và co duong di ngan hơn duong min
                trso_min = g.matrix[i][j];    // cap nhat lai trong so nho nhat
                u_min = i;
                v_min = j; }  // danh dau cap dinh ngan nhat
             } }
        }
     
    if (u_min != -1 && v_min != -1) { // nếu tim duoc duong di moi
        cout << "Chon canh " << DSTinh[u_min] << " -> " << DSTinh[v_min] << " = " << trso_min << endl;
        Daduyet[v_min] = true;     // Kết nạp đỉnh mới vào cây
        dsCanh[soCanh++] = {u_min, v_min};   }   // lưu vào mảng để in ra kq
    else  break;    } 

    cout << "\nCay khung nho nhat: ";
    for (int i = 0; i < soCanh; i++) {
        cout << DSTinh[dsCanh[i].u] << " -> " << DSTinh[dsCanh[i].v]; // in ra cac canh vua chon
        if (i < soCanh - 1) {
            cout << ", "; // them , và tao khoang cach cho dep
        }
    }
    cout << endl;
}

struct Canh {   int u, v, ts;  };  // khai bao thong tin 1 canh

int Timgoc(int cha[], int i) {
      if (cha[i] == -1)
        return i;
    else
        return Timgoc(cha, cha[i]);
}
void TTKruskal(Matran &g) {
    int n = g.Sotinhxet, socanh = 0;
    Canh ds[100]; 

for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) { // duyet tim nua tren ma trạn doi xung
    if (g.matrix[i][j] > 0) {   // co duong
        ds[socanh] = {i, j, g.matrix[i][j]}; // luu
        socanh++;    } }
}

for (int i = 0; i < socanh - 1; i++) {   // sap xep cac canh theo thu tu trong so
    for (int j = i + 1; j < socanh; j++) {
        if (ds[i].ts > ds[j].ts) swap(ds[i], ds[j]);  }
}

cout << "\nThuat toan Kruskal - Sap xep cac canh:\n"; // in ra
for (int i = 0; i < socanh; i++) {
    cout << DSTinh[ds[i].u] << " -> " << DSTinh[ds[i].v] << " = " << ds[i].ts << ((i < socanh - 1) ? ", " : "\n\n");} 
 
int cha[Sotinh_max];
    for (int i = 0; i < n; i++) cha[i] = -1; // Ban dau, cac tinh nam doc lap

    Canh caykhung[Sotinh_max]; // Mang luu cac canh duoc chọn
    int socanhcay = 0;

    for (int i = 0; i < socanh; i++) {
        int goc_u = Timgoc(cha, ds[i].u);
        int goc_v = Timgoc(cha, ds[i].v);

    if (goc_u != goc_v) {  // gốc khac -> kh tao thanh chu trinh
        cout << "Them " << DSTinh[ds[i].u] << " -> " << DSTinh[ds[i].v] << " = " << ds[i].ts << endl; 
        caykhung[socanhcay++] = ds[i]; // luu vao mang
        cha[goc_u] = goc_v;            // gop 2 cum
    } }

    cout << "\nCay khung nho nhat: ";  // in ra ket luan
    for (int i = 0; i < socanhcay; i++) {
        cout << DSTinh[caykhung[i].u] << " -> " << DSTinh[caykhung[i].v];
        if (i < socanhcay - 1) cout << ", "; }
    cout << endl;
}

// ham in ma tran
void Inmatran(int P[Sotinh_max][Sotinh_max], int n) {
    cout << setw(6) << " ";
    for (int j = 0; j < n; j++) { cout << setw(5) << DSTinh[j]; }
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << setw(6) << DSTinh[i];
        for (int j = 0; j < n; j++)    cout << setw(5) << P[i][j];
        cout << endl;
    }  }

int main() {
    int n = Sotinh_max;
    Matran g; 
    Khoitaomatran(g, n);
    // Nhap du lieu tu do thi cac tinh trong slide, quy doi mã đường đi sang khoảng cách
int CacTuyenDuong[][3] = { {0, 6, 1}, {6, 7, 2}, {8, 7, 3}, {0, 8, 4}, {0, 9, 5},   
    {0, 10, 6},{0, 1, 7}, {0, 2, 8}, {2, 3, 9}, {3, 4, 10}, {2, 4, 11},  {4, 5, 12}, {6, 5, 13}   };
    int soTuyen = sizeof(CacTuyenDuong) / sizeof(CacTuyenDuong[0]);
    
    for(int i = 0; i < soTuyen; i++) { 
    ThemDuongMaTran(g, CacTuyenDuong[i][0], CacTuyenDuong[i][1], CacTuyenDuong[i][2]);}
    cout << "             Ma tran ban dau" << endl ; 
    Inmatran(g.matrix, n);

    ThuatToanPrim(g);
    TTKruskal(g);
    return 0;
}