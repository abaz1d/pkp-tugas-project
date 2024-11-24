#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <ctype.h>


#define MAX_PATIENTS 100

// Struktur alamat
typedef struct {
    char line[100];       // Alamat lengkap
    char city[50];        // Kota
    char postalCode[10];  // Kode pos
    char country[20];     // Negara
    char province[10];    // Kode Provinsi
    char district[10];    // Kode Kecamatan
    char village[10];     // Kode Desa/Kelurahan
    char rt[5];           // RT
    char rw[5];           // RW
} Address;

// Struktur pasien
typedef struct {
    char identifier[20];        // NIK
    char name[100];             // Nama lengkap
    char birthdate[11];         // Tanggal lahir (format DD-MM-YYYY)
    char birthplace[50];        // Tempat lahir
    char gender[3];             // Jenis kelamin (L/P)
    Address address;            // Alamat
    char telecom[50];           // Email atau nomor telepon
    char maritalStatus[20];     // Status pernikahan
    char citizenshipStatus[10]; // WNI atau WNA
    char foreignCountry[50];    // Nama negara jika WNA
} Patient;

Patient patients[MAX_PATIENTS];
int patientCount = 0;

// Fungsi validasi huruf saja
int is_alpha_only(const char *str) {
    if (str == NULL || str[0] == '\0' || str[0] == ' ') {
        return 0;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

// Fungsi validasi hanya angka
int is_numeric_only(const char *str) {
    if (str == NULL || str[0] == '\0' || str[0] == ' ') {
        return 0;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

// Deklarasi prototipe fungsi is_leap_year
int is_leap_year(int year);

// Fungsi untuk memeriksa apakah tahun kabisat
int is_leap_year(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// Fungsi untuk memvalidasi tanggal (YYYY-MM-DD)
int is_valid_date(const char *date) {
    // Memastikan format YYYY-MM-DD (panjang 10 karakter dan tanda "-" ada di posisi yang benar)
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') {
        return 0;
    }

    // Memeriksa apakah semua karakter selain tanda "-" adalah digit
    for (int i = 0; i < 10; i++) {
        if ((i != 4 && i != 7) && !isdigit(date[i])) {
            return 0;
        }
    }

    // Memisahkan tahun, bulan, dan hari
    char year[5], month[3], day[3];
    strncpy(year, date, 4);
    year[4] = '\0';
    strncpy(month, date + 5, 2);
    month[2] = '\0';
    strncpy(day, date + 8, 2);
    day[2] = '\0';

    int y = atoi(year);
    int m = atoi(month);
    int d = atoi(day);

    // Memeriksa apakah tahun valid
    if (y < 1900 || y > 2021) {
        return 0;
    }

    // Memeriksa apakah bulan valid
    if (m < 1 || m > 12) {
        return 0;
    }

    // Memeriksa jumlah hari dalam bulan
    int days_in_month[] = {31, (is_leap_year(y) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (d < 1 || d > days_in_month[m - 1]) {
        return 0;
    }

    return 1; // Jika semua valid, kembalikan 1
}

// Fungsi validasi email atau nomor telepon
int is_valid_telecom(const char *telecom) {
    int is_email = strchr(telecom, '@') != NULL;
    if (is_email) {
        return strchr(telecom, '.') != NULL;
    } else {
        for (int i = 0; telecom[i] != '\0'; i++) {
            if (!isdigit(telecom[i])) {
                return 0;
            }
        }
        return strlen(telecom) >= 5;
    }
}
// Fungsi format alamat
const char* format_address(Patient* p) {
    static char formatted_address[512];
    snprintf(formatted_address, sizeof(formatted_address),
        "Alamat: %s, RT: %s, RW: %s, Kelurahan: %s, Kecamatan: %s, Kota: %s, Provinsi: %s, Kode Pos: %s, Negara: %s",
        p->address.line,
        p->address.rt,
        p->address.rw,
        p->address.village,
        p->address.district,
        p->address.city,
        p->address.province,
        p->address.postalCode,
        strcmp(p->citizenshipStatus, "WNI") == 0 ? "Indonesia" : p->address.country);
    return formatted_address;
}
// Fungsi menampilkan garis pemisah
void print_line() {
    printf("+----+------------------+----------------------+------------+---------------+---------------+----------------+----------------------------+------------------+------------------+\n");
}

//fungsi menampilkan data pasien
void print_patient_table(Patient *filteredPatients, int count) {
    if (count == 0) {
        printf("\nTidak ada data pasien.\n");
        return;
    }

    printf("\n=== Data Semua Pasien ===\n");
    print_line();
    printf("| No | NIK/ ID NUMBER   | Nama                 | Tgl Lahir  | Tempat Lahir  | Jenis Kelamin | Status Menikah | Email/Telepon              | Kewarganegaraan  | Alamat Lengkap (Detail, RT, RW, Kelurahan, Kecamatan, Kota, Provinsi, Kode Pos, Negara) \n");
    print_line();

    for (int i = 0; i < count; i++) {
        Patient *p = &filteredPatients[i];
        printf("| %-2d | %-16s | %-20s | %-10s | %-13s | %-13s | %-14s | %-26s | %-16s | %-s \n",
            i + 1,
            p->identifier[0] != '\0' ? p->identifier : "-",
            p->name,
            p->birthdate,
            p->birthplace,
            strcmp(p->gender, "L") == 0 ? "Laki - Laki" : "Perempuan",
            strcmp(p->maritalStatus, "sudah") == 0 ? "Sudah Menikah" : "Belum Menikah",
            p->telecom,
            strcmp(p->citizenshipStatus, "WNI") == 0 ? "WNI" : p->foreignCountry,
            format_address(p));
    }

    print_line();
}

// Fungsi untuk mencari pasien
void search_patient() {
    if (patientCount == 0) {
        printf("\nTidak ada data pasien.\n");
        return;
    }

    char search[100];
    printf("\nMasukkan Nama Pasien yang dicari: ");
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

    // Trim leading and trailing spaces
    char *trimmed = strtok(search, "\n\t ");
    if (trimmed == NULL) {
        printf("Input pencarian kosong!\n");
        return;
    }

    // Convert search string to lowercase for case-insensitive search
    char search_lower[100];
    for (int i = 0; trimmed[i]; i++) {
        search_lower[i] = tolower(trimmed[i]);
        search_lower[i + 1] = '\0';
    }

    Patient filteredPatients[MAX_PATIENTS];
    int found = 0;

    for (int i = 0; i < patientCount; i++) {
        // Convert patient name to lowercase for case-insensitive comparison
        char name_lower[100];
        for (int j = 0; patients[i].name[j]; j++) {
            name_lower[j] = tolower(patients[i].name[j]);
            name_lower[j + 1] = '\0';
        }

        if (strstr(name_lower, search_lower) != NULL) {
            filteredPatients[found++] = patients[i];
        }
    }

    if (found > 0) {
        printf("\n=== Hasil Pencarian ===\n");
        print_patient_table(filteredPatients, found);
    } else {
        printf("\nPasien dengan nama '%s' tidak ditemukan.\n", search);
    }
}

// Fungsi untuk menambah pasien
void add_patient() {
    if (patientCount < MAX_PATIENTS) {
        Patient *p = &patients[patientCount];
        int valid = 0;
        printf("\n===== Formulir Tambah Data Pasien =====\n");
        // Validasi status kewarganegaraan
        valid = 0;
        while (!valid) {
            printf("Masukkan Status Kewarganegaraan (WNI/WNA): ");
            fgets(p->citizenshipStatus, sizeof(p->citizenshipStatus), stdin);
            p->citizenshipStatus[strcspn(p->citizenshipStatus, "\n")] = '\0';
            if (strcmp(p->citizenshipStatus, "WNI") == 0 || strcmp(p->citizenshipStatus, "WNA") == 0) {
                valid = 1;
            } else {
                printf("Kewarganegaraan hanya boleh WNI atau WNA. Harap masukkan ulang.\n");
            }
        }

        // Input NIK atau ID berdasarkan kewarganegaraan
        if (strcmp(p->citizenshipStatus, "WNI") == 0) {
            valid = 0;
            while (!valid) {
                printf("Masukkan NIK (maksimal 16 digit): ");
                fgets(p->identifier, sizeof(p->identifier), stdin);
                p->identifier[strcspn(p->identifier, "\n")] = '\0';
                valid = strlen(p->identifier) <= 16;
                for (int i = 0; i < strlen(p->identifier) && valid; i++) {
                    if (!isdigit(p->identifier[i])) {
                        valid = 0;
                    }
                }
                if (!valid) {
                    printf("NIK harus berupa angka maksimal 16 digit. Harap masukkan ulang.\n");
                }
            }
        } else {
            printf("Masukkan Nama Negara: ");
            fgets(p->foreignCountry, sizeof(p->foreignCountry), stdin);
            p->foreignCountry[strcspn(p->foreignCountry, "\n")] = '\0';

            valid = 0;
            while (!valid) {
                printf("Masukkan ID Number: ");
                fgets(p->identifier, sizeof(p->identifier), stdin);
                p->identifier[strcspn(p->identifier, "\n")] = '\0';
                valid = 1;
                for (int i = 0; i < strlen(p->identifier); i++) {
                    if (!isdigit(p->identifier[i])) {
                        valid = 0;
                        break;
                    }
                }
                if (!valid) {
                    printf("ID Negara harus berupa angka. Harap masukkan ulang.\n");
                }
            }
        }

        // Validasi nama
        valid = 0;
        while (!valid) {
            printf("Masukkan Nama Lengkap: ");
            fgets(p->name, sizeof(p->name), stdin);
            p->name[strcspn(p->name, "\n")] = '\0';
            valid = is_alpha_only(p->name);
            if (!valid) {
                printf("Nama hanya boleh berisi huruf. Harap masukkan ulang.\n");
            }
        }

            // Validasi jenis kelamin
        valid = 0;
        while (!valid) {
            printf("Masukkan Jenis Kelamin (L/P): ");
            fgets(p->gender, sizeof(p->gender), stdin);
            p->gender[strcspn(p->gender, "\n")] = '\0'; // Menghapus newline

            // Trim leading and trailing whitespace
            char *start = p->gender;
            while (*start == ' ') start++;
            char *end = start + strlen(start) - 1;
            while (end > start && *end == ' ') end--;
            *(end + 1) = '\0';

            if (strcmp(start, "L") == 0 || strcmp(start, "P") == 0) {
                valid = 1;
            } else {
                printf("Jenis kelamin hanya boleh L atau P. Harap masukkan ulang.\n");
            }
        }

        // Validasi tempat lahir
        valid = 0;
        while (!valid) {
            printf("Masukkan Tempat Lahir: ");
            fgets(p->birthplace, sizeof(p->birthplace), stdin);
            p->birthplace[strcspn(p->birthplace, "\n")] = '\0';
            valid = is_alpha_only(p->birthplace);
            if (!valid) {
                printf("Tempat lahir hanya boleh berisi huruf. Harap masukkan ulang.\n");
            }
        }

        // Validasi tanggal lahir
        valid = 0;
        while (!valid) {
            printf("Masukkan Tanggal Lahir (YYYY-MM-DD) contoh (2000-01-01): ");
            fgets(p->birthdate, sizeof(p->birthdate), stdin);
            p->birthdate[strcspn(p->birthdate, "\n")] = '\0';  // Menghapus newline
            valid = is_valid_date(p->birthdate);
            if (!valid) {
                printf("Format tanggal salah. Harap masukkan ulang.\n");
            }
        }

        // Validasi status pernikahan
        valid = 0;
        while (!valid) {
            getchar();  // Membaca newline yang mungkin tertinggal
            printf("Masukkan Status Pernikahan (sudah/belum): ");
            fgets(p->maritalStatus, sizeof(p->maritalStatus), stdin);
            p->maritalStatus[strcspn(p->maritalStatus, "\n")] = '\0';
            valid = strcmp(p->maritalStatus, "sudah") == 0 || strcmp(p->maritalStatus, "belum") == 0;
            if (!valid) {
                printf("Status pernikahan hanya boleh sudah atau belum. Harap masukkan ulang.\n");
                printf("Masukkan Status Pernikahan (sudah/belum): ");
            }
        }
        
        //  Validasi provinsi (kode)
        valid = 0;
        while (!valid) {
            printf("Masukkan Provinsi (kode): ");
            fgets(p->address.province, sizeof(p->address.province), stdin);
            p->address.province[strcspn(p->address.province, "\n")] = '\0';  // Menghapus newline
            valid = is_numeric_only(p->address.province);
            if (!valid) {
                printf("Provinsi hanya boleh berisi angka. Harap masukkan ulang.\n");
            }
        }
        
        // Validasi kota
        valid = 0;
        while (!valid) {
            printf("Masukkan Kota (kode): ");
            fgets(p->address.city, sizeof(p->address.city), stdin);
            p->address.city[strcspn(p->address.city, "\n")] = '\0';  // Menghapus newline
            valid = is_numeric_only(p->address.city);
            if (!valid) {
                printf("Kota hanya boleh berisi angka. Harap masukkan ulang.\n");
                printf("Masukkan Kota: ");
            }
        }

        // Validasi kecamatan (kode)
        valid = 0;
        while (!valid) {
            printf("Masukkan Kecamatan (kode): ");
            fgets(p->address.district, sizeof(p->address.district), stdin);
            p->address.district[strcspn(p->address.district, "\n")] = '\0';  // Menghapus newline
            valid = is_numeric_only(p->address.district);
            if (!valid) {
                printf("Kecamatan hanya boleh berisi angka. Harap masukkan ulang.\n");
            }
        }

        // Validasi desa/kelurahan (kode)
        valid = 0;
        while (!valid) {
            printf("Masukkan Desa/Kelurahan (kode): ");
            fgets(p->address.village, sizeof(p->address.village), stdin);
            p->address.village[strcspn(p->address.village, "\n")] = '\0';  // Menghapus newline
            valid = is_numeric_only(p->address.village);
            if (!valid) {
                printf("Desa/Kelurahan hanya boleh berisi angka. Harap masukkan ulang.\n");
            }
        }


        // Validasi RT (kode)
        valid = 0;
        while (!valid) {
            printf("Masukkan RT: ");
            fgets(p->address.rt, sizeof(p->address.rt), stdin);
            p->address.rt[strcspn(p->address.rt, "\n")] = '\0';  // Menghapus newline
            valid = is_numeric_only(p->address.rt);
            if (!valid) {
                printf("RT hanya boleh berisi angka. Harap masukkan ulang.\n");
            }
        }

        // Validasi RW (kode)
        valid = 0;
        while (!valid) {
            printf("Masukkan RW: ");
            fgets(p->address.rw, sizeof(p->address.rw), stdin);
            p->address.rw[strcspn(p->address.rw, "\n")] = '\0';  // Menghapus newline
            valid = is_numeric_only(p->address.rw);
            if (!valid) {
                printf("RW hanya boleh berisi angka. Harap masukkan ulang.\n");
            }
        }

        // Validasi alamat lengkap (boleh berisi angka dan huruf)
        valid = 0;
        while (!valid) {
            printf("Masukkan Alamat Lengkap: ");
            fgets(p->address.line, sizeof(p->address.line), stdin);
            p->address.line[strcspn(p->address.line, "\n")] = '\0';  // Menghapus newline
            valid = 1;  // Alamat lengkap bisa berisi angka dan huruf
            if (strlen(p->address.line) == 0) {
                valid = 0;
                printf("Alamat lengkap tidak boleh kosong. Harap masukkan ulang.\n");
            }
        }
        // Validasi kode pos
        valid = 0;
        while (!valid) {
            printf("Masukkan Kode Pos (5 digit angka): ");
            fgets(p->address.postalCode, sizeof(p->address.postalCode), stdin);
            p->address.postalCode[strcspn(p->address.postalCode, "\n")] = '\0';
            valid = strlen(p->address.postalCode) == 5;
            for (int i = 0; i < strlen(p->address.postalCode) && valid; i++) {
                if (!isdigit(p->address.postalCode[i])) {
                    valid = 0;
                }
            }
            if (!valid) {
                printf("Kode pos harus 5 digit angka. Harap masukkan ulang.\n");
            }
        }

        // Validasi email atau nomor telepon
        valid = 0;
        while (!valid) {
            printf("Masukkan Email/Nomor Telepon: ");
            fgets(p->telecom, sizeof(p->telecom), stdin);
            p->telecom[strcspn(p->telecom, "\n")] = '\0';
            valid = is_valid_telecom(p->telecom);
            if (!valid) {
                printf("Email/Nomor Telepon tidak valid. Harap masukkan ulang.\n");
            }
        }

        patientCount++;
        printf("==================================================\n");
        printf("Data pasien berhasil ditambahkan!\n");
    } else {
        printf("Mohon maaf, kapasitas data pasien penuh!\n");
    }
}

// Fungsi untuk menghapus pasien
void delete_patient() {
    if (patientCount == 0) {
        printf("\nTidak ada data pasien untuk dihapus.\n");
        return;
    }

    char identifier[20];
    printf("\n===== Hapus Data Pasien =====\n");
    printf("Masukkan NIK/ID Number pasien yang ingin dihapus: ");
    fgets(identifier, sizeof(identifier), stdin);
    identifier[strcspn(identifier, "\n")] = '\0'; // Menghapus newline

    int index = -1;
    for (int i = 0; i < patientCount; i++) {
        if (strcmp(patients[i].identifier, identifier) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Pasien dengan NIK/ID Number '%s' tidak ditemukan.\n", identifier);
        return;
    }

    // Geser data pasien untuk menghapus elemen di indeks `index`
    for (int i = index; i < patientCount - 1; i++) {
        patients[i] = patients[i + 1];
    }

    patientCount--; // Kurangi jumlah pasien
    printf("Data pasien dengan NIK/ID Number '%s' berhasil dihapus.\n", identifier);
}


int main() {
    int choice;
    do {
        printf("\n===== Sistem Data Pasien =====\n");
        printf("1. Tambah Data Pasien\n");
        printf("2. Lihat Data Semua Pasien\n");
        printf("3. Cari Data Pasien\n");
        printf("4. Hapus Data Pasien\n");
        printf("5. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &choice);
        getchar(); // Hapus newline dari buffer

        switch (choice) {
        case 1:
            add_patient();
            break;

        case 2:
            print_patient_table(patients, patientCount);
            break;

        
        case 3:
            search_patient();
            break;

        case 4:
            print_patient_table(patients, patientCount);
            delete_patient();
            break;
        case 5:
            printf("Terima kasih telah menggunakan program ini, sampai jumpa lagi! :)\n");
            break;

        default:
            printf("Pilihan tidak valid!\n");
        }
    } while (choice != 5);

    return 0;
}
