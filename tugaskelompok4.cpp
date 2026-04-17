#include <iostream>
#include <fstream>
using namespace std;

const int MAX_STACK = 64;
const int BARIS = 4;
const int KOLOM = 9;

struct Item {
    string nama;
    int jumlah;
};

Item inventory[BARIS][KOLOM];
int startBaris = 0;

void inisialisasiInventory() {
    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++) {
            inventory[i][j].nama = "Kosong";
            inventory[i][j].jumlah = 0;
        }
}

void tambahItem(string nama, int jumlah) {
    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++)
            if (inventory[i][j].nama == nama && inventory[i][j].jumlah < MAX_STACK) {
                int sisa = MAX_STACK - inventory[i][j].jumlah;
                if (jumlah <= sisa) {
                    inventory[i][j].jumlah += jumlah;
                    return;
                } else {
                    inventory[i][j].jumlah = MAX_STACK;
                    jumlah -= sisa;
                }  
            }

    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++)
            if (inventory[i][j].nama == "Kosong") {
                inventory[i][j].nama = nama;
                if (jumlah > MAX_STACK) {
                    inventory[i][j].jumlah = MAX_STACK;
                    jumlah -= MAX_STACK;
                } else {
                    inventory[i][j].jumlah = jumlah;
                    return;
                }
              
                
                
            }
    if (jumlah > 0)
        cout << "Inventory penuh!\n";
}

void hapusItem(string nama, int jumlah) {
    for (int i = BARIS - 1; i >= 0; i--)
        for (int j = KOLOM - 1; j >= 0; j--)
            if (inventory[i][j].nama == nama) {
                if (inventory[i][j].jumlah > jumlah) {
                    inventory[i][j].jumlah -= jumlah;
                    return;
                } else {
                    jumlah -= inventory[i][j].jumlah;
                    inventory[i][j].nama = "Kosong";
                    inventory[i][j].jumlah = 0;
                }
            }
}

void editItem() {
    int i, j;
    cout << "Posisi [baris][kolom]: ";
    cin >> i >> j;

    if (i < 0 || i >= BARIS || j < 0 || j >= KOLOM) {
        cout << "Tidak valid!\n";
        return;
    }

    cout << "1.Ubah 2.Kosongkan\nPilih: ";
    int p; cin >> p;

    if (p == 1) {
        cout << "Nama: ";
        cin >> inventory[i][j].nama;
        cout << "Jumlah: ";
        cin >> inventory[i][j].jumlah;

        if (inventory[i][j].jumlah > MAX_STACK)
            inventory[i][j].jumlah = MAX_STACK;
    }
    else if (p == 2) {
        inventory[i][j].nama = "Kosong";
        inventory[i][j].jumlah = 0;
    }
}

void pindahItem() {
    int i1, j1, i2, j2;

    cout << "Dari [baris][kolom]: ";
    cin >> i1 >> j1;

    cout << "Ke [baris][kolom]: ";
    cin >> i2 >> j2;

    if (i1 < 0 || i1 >= BARIS || j1 < 0 || j1 >= KOLOM ||
        i2 < 0 || i2 >= BARIS || j2 < 0 || j2 >= KOLOM) {
        cout << "Tidak valid!\n";
        return;
    }

    Item temp = inventory[i1][j1];
    inventory[i1][j1] = inventory[i2][j2];
    inventory[i2][j2] = temp;

    cout << "Item berhasil dipindahkan!\n";
}

void tampilFull() {
    cout << "\n=== INVENTORY ===\n";
    for (int i = 0; i < BARIS; i++) {
        for (int j = 0; j < KOLOM; j++) {
            if (inventory[i][j].nama == "Kosong")
                cout << "[ - ] ";
            else
                cout << "[" << inventory[i][j].nama << ":" << inventory[i][j].jumlah << "] ";
        }
        cout << endl;
    }
}

void tampilHotbar() {
    cout << "\n=== HOTBAR ===\n";
    for (int j = 0; j < KOLOM; j++) {
        if (inventory[BARIS - 1][j].nama == "Kosong")
            cout << "[ - ] ";
        else
            cout << "[" << inventory[BARIS - 1][j].nama << ":" 
                 << inventory[BARIS - 1][j].jumlah << "] ";
    }
    cout << endl;
}

void tampilView() {
    cout << "\n=== VIEW BARIS " << startBaris << " ===\n";
    for (int j = 0; j < KOLOM; j++) {
        if (inventory[startBaris][j].nama == "Kosong")
            cout << "[ - ] ";
        else
            cout << "[" << inventory[startBaris][j].nama << ":" 
                 << inventory[startBaris][j].jumlah << "] ";
    }
    cout << endl;
}

void scrollBawah() {
    startBaris++;
    if (startBaris >= BARIS) startBaris = 0;
    tampilView();
}

void cariItem(string nama) {
    bool ketemu = false;
    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++)
            if (inventory[i][j].nama == nama) {
                cout << "Ditemukan di [" << i << "][" << j 
                     << "] jumlah: " << inventory[i][j].jumlah << endl;
                ketemu = true;
            }

    if (!ketemu) cout << "Tidak ditemukan\n";
}

void sortNama() {
    Item temp[BARIS * KOLOM];
    int k = 0;

    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++)
            temp[k++] = inventory[i][j];

    for (int i = 0; i < k - 1; i++)
        for (int j = 0; j < k - 1; j++)
            if (temp[j].nama > temp[j + 1].nama)
                swap(temp[j], temp[j + 1]);

    k = 0;
    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++)
            inventory[i][j] = temp[k++];
}

void sortJumlah() {
    Item temp[BARIS * KOLOM];
    int k = 0;

    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++)
            temp[k++] = inventory[i][j];

    for (int i = 0; i < k - 1; i++)
        for (int j = 0; j < k - 1; j++)
            if (temp[j].jumlah < temp[j + 1].jumlah)
                swap(temp[j], temp[j + 1]);

    k = 0;
    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++)
            inventory[i][j] = temp[k++];
}

void simpanFile() {
    ofstream file("inventory.txt");
    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++)
            file << inventory[i][j].nama << " "
                 << inventory[i][j].jumlah << endl;

    file.close();
    cout << "Data disimpan!\n";
}

void loadFile() {
    ifstream file("inventory.txt");
    if (!file) return;

    for (int i = 0; i < BARIS; i++)
        for (int j = 0; j < KOLOM; j++)
            file >> inventory[i][j].nama >> inventory[i][j].jumlah;

    file.close();
    cout << "Data di-load!\n";
}

#include <iostream>
#include <iomanip>

using namespace std;

void tampilkanMenu() {
    cout << "\n+-------------------------------------------+" << endl;
    cout << "|        MINECRAFT INVENTORY SYSTEM         |" << endl;
    cout << "+----------------------+--------------------+" << endl;
    cout << "|  1. Tambah Item      |  5. Tampil View    |" << endl;
    cout << "|  2. Hapus Item       |  6. Scroll Baris   |" << endl;
    cout << "|  3. Tampil Full      |  7. Cari Item      |" << endl;
    cout << "|  4. Tampil Hotbar    |  8. Edit Item      |" << endl;
    cout << "+----------------------+--------------------+" << endl;
    cout << "|  9. Sort Nama        | 12. Load File      |" << endl;
    cout << "|  10. Sort Jumlah     | 13. Pindah Item    |" << endl;
    cout << "|  11. Simpan File     | 14. Keluar         |" << endl;
    cout << "+----------------------+--------------------+" << endl;
    cout << "Pilih Nomor: ";
}

int main() {
    inisialisasiInventory();
    loadFile();

    int pilih;
    string nama;
    int jumlah;

    do {
        tampilkanMenu();
        if (!(cin >> pilih)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (pilih) {
            case 1:
                cout << "\n[TAMBAH ITEM]" << endl;
                cout << "Nama: "; cin >> nama;
                cout << "Jumlah: "; cin >> jumlah;
                tambahItem(nama, jumlah);
                cout << "=> Data Berhasil Ditambah" << endl;
                break;
            case 2:
                cout << "\n[HAPUS ITEM]" << endl;
                cout << "Nama: "; cin >> nama;
                cout << "Jumlah: "; cin >> jumlah;
                hapusItem(nama, jumlah);
                cout << "=> Data Berhasil Dihapus" << endl;
                break;
            case 3: tampilFull(); break;
            case 4: tampilHotbar(); break;
            case 5: tampilView(); break;
            case 6: scrollBawah(); break;
            case 7:
                cout << "\n[CARI ITEM]" << endl;
                cout << "Nama: "; cin >> nama;
                cariItem(nama);
                break;
            case 8: 
                editItem(); 
                cout << "=> Data Berhasil Diedit" << endl;
                break;
            case 9: 
                sortNama(); 
                cout << "=> Inventory Diurutkan Berdasarkan Nama" << endl;
                break;
            case 10: 
                sortJumlah(); 
                cout << "=> Inventory Diurutkan Berdasarkan Jumlah" << endl;
                break;
            case 11: 
                simpanFile(); 
                cout << "=> Data Berhasil Disimpan" << endl;
                break;
            case 12: 
                loadFile(); 
                cout << "=> Data Berhasil Dimuat" << endl;
                break;
            case 13: 
                pindahItem(); 
                cout << "=> Data Berhasil Dipindah" << endl;
                break;
        }
    } while (pilih != 14);

    cout << "\n--- Terima Kasih Sudah Mencoba System Inventory Minecraft ---" << endl;
    return 0;
}
