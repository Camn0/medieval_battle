# Medieval Battle: TCP/IP Multiplayer RPG

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Winsock2](https://img.shields.io/badge/Winsock2-Network_API-blue?style=for-the-badge)
![UI](https://img.shields.io/badge/Universitas_Indonesia-Tugas_Akhir-yellow?style=for-the-badge)

**Medieval Battle** adalah aplikasi permainan *Role-Playing Game* (RPG) taktis multipemain berbasis terminal (CLI). Proyek ini dibangun sepenuhnya menggunakan **C++ murni** sebagai implementasi komprehensif dari *Socket Programming* dan 4 Pilar Pemrograman Berorientasi Objek (OOP).

Sistem ini dirancang dengan arsitektur **Authoritative Client-Server (TCP/IPv4)**, di mana server bertindak sebagai pusat kebenaran logika absolut tanpa menggunakan pustaka eksternal pihak ketiga untuk pengolahan data maupun struktur data algoritma.

## Fitur Utama (Engineering Highlights)

- **Custom JSON Parser:** Implementasi modul pembaca token JSON secara manual murni menggunakan metode `std::string` bawaan C++ (`find`, `substr`) untuk memastikan portabilitas tanpa membebani kompilator dengan *library* eksternal.
- **Network Synchronization:** Sinkronisasi jaringan *real-time* berbasis `Winsock2` dengan jaminan keandalan transmisi *Three-way Handshake* TCP[cite: 68, 125, 126]. [cite_start]Server memanfaatkan *blocking I/O* untuk sinkronisasi dua klien secara mulus.
- **4 Pillars of OOP Architecture:** - **Abstraksi & Enkapsulasi:** Kelas dasar `Character` berstatus *pure abstract* dengan atribut memori vital (`hp`, `baseMinAtk`) yang terkunci rapat.
  - **Pewarisan & Polimorfisme:** Penurunan *class* ke 3 faksi unik (`Knight`, `Assassin`, `Paladin`) menggunakan *Dynamic Binding* (`virtual override`) untuk kalkulasi kalkulasi pasif unit.
- **Manual Data Structures & Algorithms:**
  - Penyimpanan memori riwayat dinamis menggunakan `std::vector`.
  - Mesin *ranking* pertempuran yang dibangun dari nol menggunakan algoritma pemilahan rekursif **Quick Sort**.
  - Fitur pelacakan riwayat spesifik menggunakan algoritma iterasi berurutan **Linear Search**.
- **Precision CLI UI:** Rendering terminal antarmuka pengguna diatur dengan ketat menggunakan *stream modifiers* (`std::setw`, `std::right`, `std::setfill`) untuk menciptakan *grid* visual asimetris yang tangguh dari cacat *rendering*.

## Persyaratan Sistem

- **OS:** Windows 10/11 (membutuhkan integrasi kernel `ws2_32.lib`).
- **Compiler:** GCC/G++ Compiler (MinGW).

## Panduan Kompilasi & Eksekusi

Kompilasi program menuntut pengikatan (*linkage parsing*) terhadap modul tautan antarmuka jaringan `ws2_32`. Buka terminal dan jalankan perintah kompilasi berikut:

**1. Kompilasi Server:**
```bash
g++ src/server.cpp -o server.exe -lws2_32
```

**2. Kompilasi Client:**

```bash
g++ src/client.cpp -o client.exe -lws2_32
```

**Cara Menjalankan:**

1. Eksekusi `server.exe` terlebih dahulu untuk mendirikan pangkalan pendengar di *Port* `8080`.


2. Buka dua terminal terpisah dan jalankan `client.exe` pada masing-masing terminal.


3. (Opsional) Jika bermain beda perangkat di LAN yang sama, ubah variabel `ipAddress` di `client.cpp` ke IP lokal IPv4 Server sebelum dikompilasi.



## Analisis Kompleksitas Kinerja

| Algoritma | Skenario Terbaik | Skenario Rata-rata | Skenario Terburuk |
| --- | --- | --- | --- |
| **Quick Sort Manual** | $\mathcal{O}(n \log n)$ | $\mathcal{O}(n \log n)$ | $\mathcal{O}(n^2)$ |
| **Linear Search Manual** | $\mathcal{O}(1)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ |

> 
> Catatan: Quick Sort dialokasikan untuk pemeringkatan riwayat kerusakan (*damage history*), sementara Linear Search difungsikan untuk pencarian log berdasarkan putaran (*turn*) spesifik.
> 
> 

## Pengembang (Kelompok Cihuy)

Tugas Akhir Algoritma dan Pemrograman - Departemen Teknik Elektro, Fakultas Teknik Universitas Indonesia.

* **Maheztha Sulthan Syadi** (NPM: 2506609574) 

* **Mohamad Rafly Nursidiq** (NPM: 2506608691) 

* **Falsya Hizri Anbiya** (NPM: 2506603135) 
