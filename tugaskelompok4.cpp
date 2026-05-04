#include <iostream>
#include <string>
#include <cstdio>
#include <limits>
#include <cstring>
using namespace std;

const int MAX_STACK  = 64;
const int BARIS      = 4;
const int KOLOM      = 9;
const int TOTAL_SLOT = BARIS * KOLOM;

struct Node {
    string nama;
    int    jumlah;
    Node*  next;
    Node*  prev;
};

Node* head = NULL;
Node* tail = NULL;

bool registerUser() {
    char username[50], password[50];
    char u[50], p[50];

    cout << "\n=== REGISTER ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    FILE* file = fopen("users.txt", "r");

    if (file != NULL) {
        while (fscanf(file, "%s %s", u, p) != EOF) {
            if (strcmp(username, u) == 0) {
                cout << "Username sudah dipakai!\n";
                fclose(file);
                return false;
            }
        }
        fclose(file);
    }

    file = fopen("users.txt", "a");
    if (file == NULL) {
        cout << "Gagal membuka file!\n";
        return false;
    }

    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    cout << "Registrasi berhasil!\n";
    return true;
}

string loginUser() {
    char username[50], password[50];
    char u[50], p[50];

    cout << "\n=== LOGIN ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        cout << "Belum ada akun, silakan register dulu.\n";
        return "";
    }

    while (fscanf(file, "%s %s", u, p) != EOF) {
        if (strcmp(username, u) == 0 && strcmp(password, p) == 0) {
            fclose(file);
            return string(username); // ⬅️ return username

        }
    }

    fclose(file);
    return "";
}

void refresh() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

Node* buatNode(string nama, int jumlah) {
    Node* baru   = new Node;
    baru->nama   = nama;
    baru->jumlah = jumlah;
    baru->next   = NULL;
    baru->prev   = NULL;
    return baru;
}

void inisialisasiInventory() {
    for (int i = 0; i < TOTAL_SLOT; i++) {
        Node* baru = buatNode("Kosong", 0);
        if (head == NULL) {
            head = tail = baru;
        } else {
            tail->next = baru;
            baru->prev = tail;
            tail       = baru;
        }
    }
}

void bersihkanList() {
    Node* curr = head;
    while (curr != NULL) {
        Node* hapus = curr;
        curr = curr->next;
        delete hapus;  
    }
    head = tail = NULL;
}

void tambahItem(string nama, int jumlah) {
    if (jumlah <= 0) return;

    Node* curr = head;
    while (curr != NULL && jumlah > 0) {
        if (curr->nama == nama && curr->jumlah < MAX_STACK) {
            int sisa = MAX_STACK - curr->jumlah;
            if (jumlah <= sisa) { 
				curr->jumlah += jumlah; return; }
            else { 
				curr->jumlah = MAX_STACK; jumlah -= sisa; }
        }
        curr = curr->next;
    }

    curr = head;
    while (curr != NULL && jumlah > 0) {
        if (curr->nama == "Kosong") {
            int isi = (jumlah > MAX_STACK) ? MAX_STACK : jumlah;
            curr->nama   = nama;
            curr->jumlah = isi;
            jumlah -= isi;
        }
        curr = curr->next;
    }

    if (jumlah > 0) cout << "Inventory penuh!\n";
}

void hapusItem(string nama, int jumlah) {
    int totalStok = 0;
    Node* curr    = head;
    while (curr != NULL) {
        if (curr->nama == nama) totalStok += curr->jumlah;
        curr = curr->next;
    }

    if (totalStok == 0) {
        cout << "ERROR, Item '" << nama << "' ga ada mungkin udah dihapus hammam!\n";
        return;
    }
    if (jumlah > totalStok) {
        cout << "ERROR eyy, Stoknya ga cukup (" << jumlah << ") melebihi stok yang ada (" << totalStok << ")!\n";
        return;
    }

    curr = tail;
    while (curr != NULL && jumlah > 0) {
        if (curr->nama == nama) {
            if (curr->jumlah > jumlah) {
                curr->jumlah -= jumlah;
                jumlah = 0;
            } else {
                jumlah      -= curr->jumlah;
                curr->nama   = "Kosong";
                curr->jumlah = 0;
            }
        }
        curr = curr->prev;
    }
    cout << "Berhasil Menghapus Item: " << nama << "\n";
}

void hapusIndex(int idx, int jumlah) {
    Node* curr = head;

    // cari node sesuai index
    for (int i = 0; i < idx && curr != NULL; i++) {
        curr = curr->next;
    }

    // validasi index
    if (curr == NULL) {
        cout << "Index tidak valid!\n";
        return;
    }

    // kalau kosong
    if (curr->nama == "Kosong") {
        cout << "Slot ini udah kosong.\n";
        return;
    }

    // validasi jumlah
    if (jumlah <= 0) {
        cout << "Jumlah harus lebih dari 0!\n";
        return;
    }

    if (jumlah > curr->jumlah) {
        cout << "ERROR, jumlah (" << jumlah 
             << ") melebihi isi slot (" << curr->jumlah << ")!\n";
        return;
    }

    // proses hapus (mirip hapusItem tapi 1 slot)
    if (jumlah == curr->jumlah) {
        curr->nama   = "Kosong";
        curr->jumlah = 0;
    } else {
        curr->jumlah -= jumlah;
    }

    cout << "Berhasil menghapus " << jumlah 
         << " item di index " << idx << endl;
}
void hapusSemua() {
    Node* curr = head;
    while (curr != NULL) {
        curr->nama   = "Kosong";
        curr->jumlah = 0;
        curr         = curr->next;
    }
    cout << "Semua item berhasil dibuang ke samping Gedung Pattimura" << endl;
}

void tampilFull() {
    cout << "\n=== INVENTORY ===\n";
    Node* curr = head;
    for (int i = 0; i < BARIS; i++) {
        for (int j = 0; j < KOLOM; j++) {
            if (curr != NULL && curr->nama == "Kosong") cout << "[ - ] ";
			else if (curr != NULL) cout << "[" << curr->nama << ":" << curr->jumlah << "] ";
            curr = curr->next;
        }
        cout << "\n";
    }
}

void tampilHotbar() {
    cout << "\n=== HOTBAR ===\n";
    Node* curr = head;
    for (int i = 0; i < (BARIS - 1) * KOLOM && curr != NULL; i++)
        curr = curr->next;

    for (int j = 0; j < KOLOM && curr != NULL; j++) {
        if (curr->nama == "Kosong") cout << "[ - ] ";
        else cout << "[" << curr->nama << ":" << curr->jumlah << "] ";
        curr = curr->next;
    }
    cout << "\n";
}

void tampilView() {
    cout << "\n=== VIEW LIST ===\n";
    Node* curr = head;
    int idx    = 0;
    while (curr != NULL) {
        cout << "Slot " << idx << " : ";
        if (curr->nama == "Kosong") {
        cout << "Kosong\n"; 
        } else {
        cout << curr->nama << " (" << curr->jumlah << ")\n";}
        curr = curr->next;
        idx++;
    }
}

void cariItem(string nama) {
    Node* curr  = head;
    int idx     = 0;
    bool ketemu = false;
    while (curr != NULL) {
        if (curr->nama == nama) {
            cout << "Index " << idx << " jumlah: " << curr->jumlah << "\n";
            ketemu = true;
        }
        curr = curr->next;
        idx++;
    }
    if (!ketemu)
        cout << "Item '" << nama << "' udah cari emang ga ada ditas" << endl;
}

void cariJumlah(int jumlah) {
    Node* curr  = head;
    int idx     = 0;
    bool ketemu = false;
    while (curr != NULL) {
        if (curr->jumlah == jumlah && curr->nama != "Kosong") {
            cout << "Index " << idx << " -> " << curr->nama
                 << " (" << curr->jumlah << ")\n";
            ketemu = true;
        }
        curr = curr->next;
        idx++;
    }
    if (!ketemu) {
        cout << "Ga ada item yang jumlahnya " << jumlah << " babe" << endl;}
}

void editItem(int idx) {
    Node* curr = head;
    for (int i = 0; i < idx; i++) curr = curr->next;

    string namaBaru;
    int    jumlahBaru;

    refresh();
    cout << "Nama baru  : ";
    getline(cin, namaBaru);
    if (namaBaru.empty()) {
        cout << "Nama item jangan dikosongin dong";
        return;
    }

    cout << "Jumlah baru (1-" << MAX_STACK << "): ";
    if (!(cin >> jumlahBaru)) {
        refresh();
        cout << "Masukkan angka ya Mi Amor jangan huruf" << endl;
        return;
    }
    if (jumlahBaru < 1 || jumlahBaru > MAX_STACK) {
        cout << "Jumlah harus antara 1 - " << MAX_STACK << ", jangan serakah" << endl;
        return;
    }

    curr->nama   = namaBaru;
    curr->jumlah = jumlahBaru;
    cout << "Item di slot " << idx << " berhasil diupdate" << endl;
}

void sortNama(bool asc) {
    bool swapTerjadi;
    do {
        swapTerjadi = false;
        Node* curr  = head;
        while (curr != NULL && curr->next != NULL) {
            bool kondisi = asc ? (curr->nama > curr->next->nama)
                               : (curr->nama < curr->next->nama);
            if (kondisi) {
                swap(curr->nama,   curr->next->nama);
                swap(curr->jumlah, curr->next->jumlah);
                swapTerjadi = true;
            }
            curr = curr->next;
        }
    } while (swapTerjadi);
}

Node* partition(Node* low, Node* high, bool asc) {
    int pivot = high->jumlah;
    Node* i     = low->prev;
    for (Node* j = low; j != high; j = j->next) {
        bool kondisi = asc ? (j->jumlah <= pivot) : (j->jumlah >= pivot);
        if (kondisi) {
            i = (i == NULL) ? low : i->next;
            swap(i->nama,   j->nama);
            swap(i->jumlah, j->jumlah);
        }
    }
    i = (i == NULL) ? low : i->next;
    swap(i->nama,   high->nama);
    swap(i->jumlah, high->jumlah);
    return i;
}

void quickSort(Node* low, Node* high, bool asc) {
    if (high != NULL && low != high && low != high->next) {
        Node* pivot = partition(low, high, asc);
        quickSort(low, pivot->prev, asc);
        quickSort(pivot->next, high, asc);
    }
}

void sortJumlah(bool asc) { quickSort(head, tail, asc); }

void tukarPosisi(int a, int b) {
    Node* x = head;
    Node* y = head;
    for (int i = 0; i < a; i++) x = x->next;
    for (int i = 0; i < b; i++) y = y->next;

    swap(x->nama,   y->nama);
    swap(x->jumlah, y->jumlah);
    cout << "Berhasil! Item index " << a << " udah tukaran posisi sama " << b << endl;
}

void simpanFile(string user) {
    string filename = "inventory_" + user + ".txt";
    FILE* fp = fopen(filename.c_str(), "w");
    if (fp == NULL) { 
		cout << "Gagal membuka file, filenya lagi ngambek" << endl; 
		return; }
    Node* curr = head;
    while (curr != NULL) {
        fprintf(fp, "%s#%d\n", curr->nama.c_str(), curr->jumlah);
        curr = curr->next;
    }
    fclose(fp);
    cout << "Data inventory aman didalam file" << endl;
}

void loadFile(string user) {
	string filename = "inventory_" + user + ".txt";
    FILE* file = fopen(filename.c_str(), "r");    
    if (file == nullptr) return;

    Node* curr = head;
    char buffer[100];
    int jml;

    while (curr != NULL && fscanf(file, "%99[^#]#%d\n", buffer, &jml) == 2) {
        curr->nama   = string(buffer);
        curr->jumlah = jml;

        if (curr->jumlah <= 0 || curr->nama.empty()) {
            curr->nama   = "Kosong";
            curr->jumlah = 0;
        }
        curr = curr->next;
    }

    fclose(file);
    cout << "Data di-load!\n";
}

int main() {
	int pilihlog;
	
	do{
		cout << "+---------------------------+\n" ;
		cout << "|           LOGIN           |\n";
		cout << "+---------------------------+\n" ;
		cout << "| 1.Register                |\n";
		cout << "| 2.Login                   |\n";
		cout << "| 3.Keluar                  |\n";
		cout << "+---------------------------+\n" ;
		cout << "Pilih menu : ";
		if (!(cin >> pilihlog)) {
			cout << "Salah eyy yang bener donggg\n\n";
			refresh();
			continue;
		}
		if (pilihlog == 1){
			registerUser();
			} else if (pilihlog == 2){
				string currentUser = loginUser();
				if(currentUser != ""){
					cout << "login berhasil\n";
	bersihkanList();
	inisialisasiInventory();
    loadFile(currentUser);
    int pilih;
    string nama;
    int jumlah, idx;

    do {
        cout << "\n+-------------------------------------------+\n";
        cout << "|         MINECRAFT INVENTORY SYSTEM        |\n";
        cout << "+----------------------+--------------------+\n";
        cout << "|  1. Tambah Item      |  5. Tampil View    |\n";
        cout << "|  2. Hapus Item       |  6. Cari Nama      |\n";
        cout << "|  3. Tampil Full      |  7. Cari Jumlah    |\n";
        cout << "|  4. Tampil Hotbar    |  8. Edit Item      |\n";
        cout << "+----------------------+--------------------+\n";
        cout << "|  9. Sort Nama        | 12. Load File      |\n";
        cout << "|  10. Sort Jumlah     | 13. Tukar Posisi   |\n";
        cout << "|  11. Simpan File     | 14. Keluar         |\n";
        cout << "+----------------------+--------------------+\n";
        cout << "Pilih Nomor: ";

        if (!(cin >> pilih)) {
            refresh();
            cout << "Harus input angka mba/mas, jangan ngadi-ngadi <>_<>" << endl;
            continue;
        }

        switch (pilih) {
            case 1: {
                refresh();
                cout << "Nama Item : ";
                if (!getline(cin, nama) || nama.empty()) {
                    cout << "Nama item jangan dikosongin dong" << endl;
                    break;
                }
                cout << "Jumlah    : ";
                if (!(cin >> jumlah)) {
                    refresh();
                    cout << "Input jumlah pakai angka ya Mi Amor" << endl;
                    break;
                }
                if (jumlah <= 0) {
                    cout << "Mana ada item jumlahnya 0 atau kurang dari 0, Halu ya?" << endl;
                    break;
                }
                tambahItem(nama, jumlah);
                cout << "Berhasil Menambahkan Item: " << nama << "\n";
                break;
            }

            case 2: {
                int sub;
                cout << "\n=== MENU HAPUS ===\n";
                cout << "1. Hapus berdasarkan Nama\n";
                cout << "2. Hapus berdasarkan Index\n";
                cout << "3. Hapus Semua\n";
                cout << "Pilih (1-3): ";

                if (!(cin >> sub)) {
                    refresh();
                    cout << "Masukkan angka (1-3) ya babe" << endl;
                    break;
                }
                if (sub < 1 || sub > 3) {
                    cout << "Pilihan cuma 1-3, jangan nyari yang ga pasti" << endl;
                    break;
                }

                switch (sub) {
                    case 1: {
                        refresh();
                        cout << "Nama Item : ";
                        if (!getline(cin, nama) || nama.empty()) {
                            cout << "Nama itemnya mana" << endl;
                            break;
                        }
                        cout << "Jumlah    : ";
                        if (!(cin >> jumlah)) {
                            refresh();
                            cout << "Pake angka dong eyyyyy" << endl;
                            break;
                        }
                        if (jumlah <= 0) {
                            cout << "Mana bisa hapus barang yang ga ada" << endl;
                            break;
                        }
                        hapusItem(nama, jumlah);
                        break;
                    }
                    case 2: {
                        cout << "Index (0-" << TOTAL_SLOT - 1 << "): ";
                        if (!(cin >> idx)) {
                            refresh();
                            cout << "Input pakai angka dong Mi Amor" << endl;
                            break;
                        }
                        if (idx < 0 || idx >= TOTAL_SLOT) {
                            cout << "Index-nya kejauhan Mi Amor, slot kita cuma 0 sampe 35"<< endl;
                            break;
                        }
						cout << "Jumlah yang mau dihapus: ";
						if (!(cin >> jumlah)) {
							refresh();
							cout << "Jumlah harus angka!" << endl;
							break;
						}
						hapusIndex(idx,jumlah);
                        break;
                    }
                    case 3: {
                        char konfirmasi;
                        cout << "Beneran mau hapus semua? (y/n): ";
                        cin  >> konfirmasi;
                        if (konfirmasi == 'y' || konfirmasi == 'Y') {
                            hapusSemua();
                        } else {
                            cout << "Batal menghapus luka (.)_(.)" << endl;
                        }
                        break;
                    }
                }
                break;
            }

            case 3:
                tampilFull();
                cout << "Berhasil menampilkan Full Inventory.\n";
                break;

            case 4:
                tampilHotbar();
                cout << "Berhasil menampilkan Hotbar Inventory.\n";
                break;

            case 5:
                tampilView();
                break;

            case 6: {
                refresh();
                cout << "Nama Item : ";
                if (!getline(cin, nama) || nama.empty()) {
                    cout << "Ketik namanya dulu dong" << endl;
                    break;
                }
                cariItem(nama);
                break;
            }

            case 7: {
                cout << "Jumlah Item : ";
                if (!(cin >> jumlah)) {
                    refresh();
                    cout << "Input harus berupa angka" << endl;
                    break;
                }
                if (jumlah < 0) {
                    cout << "Item apa yang jumlahnya negatif? Goblok banget sih" << endl;
                    break;
                }
                if (jumlah > MAX_STACK) {
                    cout << "Jumlah melebihi kapasitas maksimum ("
                         << MAX_STACK << "). Mana ada item sampe segitu jumlahnya? Nge-cheat ya" << endl;
                    break;
                }
                cariJumlah(jumlah);
                break;
            }

            case 8: {
                cout << "Edit Item Index ke (0-" << TOTAL_SLOT - 1 << "): ";
                if (!(cin >> idx)) {
                    refresh();
                    cout << "Input pakai angka kok ngeyel banget" << endl;
                    break;
                }
                if (idx < 0 || idx >= TOTAL_SLOT) {
                    cout << "Index ga valid, Harus antara 0 - " << TOTAL_SLOT - 1 << ".\n";
                    break;
                }
                editItem(idx);
                break;
            }

            case 9: {
                int sub;
                cout << "\n=== SORT NAMA ===\n";
                cout << "1. Ascending  (A-Z)\n";
                cout << "2. Descending (Z-A)\n";
                cout << "Pilih (1-2): ";
                if (!(cin >> sub)) {
                    refresh();
                    cout << "Masukkan angka 1 atau 2 babe<>" << endl;
                    break;
                }
                if (sub == 1) {
                    sortNama(true);
                    cout << "Berhasil Sorting Nama (Ascending)" << endl;
                } else if (sub == 2) {
                    sortNama(false);
                    cout << "Berhasil Sorting Nama (Descending)" << endl;
                } else {
                    cout << "Udah ada pilihan di depan mata kok milih yang lain" << endl;
                }
                break;
            }

            case 10: {
                int sub;
                cout << "\n=== SORT JUMLAH ===\n";
                cout << "1. Ascending  (Kecil -> Besar)\n";
                cout << "2. Descending (Besar -> Kecil)\n";
                cout << "Pilih (1-2): ";
                if (!(cin >> sub)) {
                    refresh();
                    cout << "Masukkan angka 1 atau 2" << endl;
                    break;
                }
                if (sub == 1) {
                    sortJumlah(true);
                    cout << "Berhasil Sorting Jumlah (Ascending).\n";
                } else if (sub == 2) {
                    sortJumlah(false);
                    cout << "Berhasil Sorting Jumlah (Descending).\n";
                } else {
                    cout << "Udah ada pilihan di depan mata kok milih yang lain" << endl;
                }
                break;
            }

            case 11:
                simpanFile(currentUser);
                break;
                
            case 12:
                bersihkanList();
				inisialisasiInventory();
                loadFile(currentUser);
                break;

            case 13: {
                int a, b;
                cout << "\n=== TUKAR POSISI ITEM ===\n";
                cout << "Index asal  (0-" << TOTAL_SLOT - 1 << "): ";
                if (!(cin >> a)) {
                    refresh();
                    cout << "Index asal harus angka eyyyy" << endl;
                    break;
                }
                if (a < 0 || a >= TOTAL_SLOT) {
                    cout << "Index asal tidak valid eyyyyy! Harus antara 0 - "
                         << TOTAL_SLOT - 1 << ".\n";
                    break;
                }

                cout << "Tukar index (0-" << TOTAL_SLOT - 1 << "): ";
                if (!(cin >> b)) {
                    refresh();
                    cout << "Index tujuan harus angka eyyyy" << endl;
                    break;
                }
                if (b < 0 || b >= TOTAL_SLOT) {
                    cout << "Index tujuan tidak valid eyyyy! Harus antara 0 - " << TOTAL_SLOT - 1 << ".\n";
                    break;
                }
                if (a == b) {
                    cout << "ERROR, Index asal dan tujuan sama. Tidak ada perubahan diantara kita.\n";
                    break;
                }
                tukarPosisi(a, b);
                break;
            }

            case 14:
                simpanFile(currentUser);
                cout << "\n=================================================\n";
                cout << "Makasih sudah mencoba System Inventory Kelompok 4!\n";
                cout << "\tBerhasil keluar dari sistem.\n";
                cout << "=================================================\n";
                break;

            default:
                cout << "Pilihan menu tidak tersedia (1-14)! Silakan coba lagi" << endl;
                break;
        }
					}while(pilih != 14);
				} else {
					cout << "loginn gagal\n";
					}
				}else if (pilihlog < 1 || pilihlog > 3){
				cout << "Salah eyy yang bener dong\n\n";
			}
			}while(pilihlog != 3);
			cout << "Jangan lupa kembali lagi\n";
			return 0;
		}
