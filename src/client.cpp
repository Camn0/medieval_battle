#include <iostream>
#include <string>
#include <iomanip> 
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

// --- HACK: JSON PARSER MANUAL ---
string getJsonValue(const string& json, const string& key) {
    string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    if (pos == string::npos) return "";
    pos += searchKey.length();
    
    if (json[pos] == '\"') {
        size_t start = pos + 1;
        size_t end = json.find("\"", start);
        if (end == string::npos) return "";
        return json.substr(start, end - start);
    } else {
        size_t end = json.find_first_of(",}", pos);
        if (end == string::npos) return "";
        return json.substr(pos, end - pos);
    }
}

// --- CLIENT NETWORKING UTILS ---
void sendJson(SOCKET socket, int playerId, const string& action) {
    string request = "{\"player_id\":" + to_string(playerId) + ",\"action\":\"" + action + "\"}";
    send(socket, request.c_str(), request.length(), 0);
}

string receiveJson(SOCKET socket) {
    char buffer[1024] = {0};
    int valread = recv(socket, buffer, 1024, 0);
    if (valread <= 0) return "";
    return string(buffer);
}

// --- UI HELPERS ---
int getMaxHpByRole(const string& role) {
    if (role == "Assassin") return 80;
    if (role == "Paladin") return 120;
    return 100; // Knight
}

string safeSubstr(const string& str, size_t start, size_t len) {
    if (start >= str.length()) return "";
    return str.substr(start, len);
}

void drawBattleUI(int turn, const string& myRole, int myHp, const string& enemyRole, int enemyHp, const string& logMsg) {
    int myMaxHp = getMaxHpByRole(myRole);
    int enemyMaxHp = getMaxHpByRole(enemyRole);
    
    string myHpStr = to_string(myHp) + "/" + to_string(myMaxHp);
    string enemyHpStr = to_string(enemyHp) + "/" + to_string(enemyMaxHp);

    cout << "\n" << string(70, '=') << "\n";
    cout << "| MEDIEVAL BATTLE                                         | RONDE " 
         << right << setw(2) << setfill('0') << turn << setfill(' ') << left << " |\n";
         
    cout << string(70, '=') << "\n";
    cout << "| [ STATUS ANDA ]                 | [ STATUS MUSUH ]                 |\n";
    cout << "| Role   : " << left << setw(22) << myRole << " | Role   : " << left << setw(23) << enemyRole << " |\n";
    cout << "| HP     : " << left << setw(22) << myHpStr << " | HP     : " << left << setw(23) << enemyHpStr << " |\n";
    cout << string(70, '-') << "\n";
    cout << "| LOG PERTANDINGAN:               | APA YANG AKAN ANDA LAKUKAN?      |\n";
    
    string log1 = safeSubstr(logMsg, 0, 32);
    string log2 = safeSubstr(logMsg, 32, 32);
    string log3 = safeSubstr(logMsg, 64, 32);

    // Lebar absolut: 33 karakter agar total grid persis 70 karakter
    string opt1 = "[1] Serang       [2] Bertahan    ";
    string opt2 = "[3] Menghindar                   ";
    string opt3 = "                                 ";

    // KOREKSI: Pemisah tengah menggunakan "| " agar posisi pipa tetap di indeks ke-34
    cout << "| " << left << setw(32) << log1 << "| " << opt1 << "|\n";
    cout << "| " << left << setw(32) << log2 << "| " << opt2 << "|\n";
    cout << "| " << left << setw(32) << log3 << "| " << opt3 << "|\n";
    cout << string(70, '=') << "\n";
    cout << "> Masukkan Tindakan Anda (1-3): ";
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "\n[ERROR] WSAStartup failed.\n"; system("pause"); return -1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); 

    // Ganti dengan IP Server saat demonstrasi
    string ipAddress = "127.0.0.1"; 
    serv_addr.sin_addr.s_addr = inet_addr(ipAddress.c_str());

    cout << "[CLIENT] Mencoba terhubung ke Server di " << ipAddress << ":8080...\n";
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "\n[ERROR] Connection Failed! Pastikan server.exe sudah jalan duluan.\n";
        closesocket(sock); WSACleanup(); system("pause"); return -1;
    }
    cout << "[CLIENT] Terhubung ke Server!\n";

    string myPlayerId = ""; 
    string lastBattleLog = "- Pertarungan dimulai -"; // Log default

    while (true) {
        string serverMsg = receiveJson(sock);
        if (serverMsg == "") {
            cout << "\n[DISCONNECT] Koneksi ke Server terputus.\n"; break;
        }

        string action = getJsonValue(serverMsg, "action");
        string log = getJsonValue(serverMsg, "log");
        int p1_hp = atoi(getJsonValue(serverMsg, "p1_hp").c_str());
        int p2_hp = atoi(getJsonValue(serverMsg, "p2_hp").c_str());
        int turn = atoi(getJsonValue(serverMsg, "turn").c_str());
        string p1_role = getJsonValue(serverMsg, "p1_role");
        string p2_role = getJsonValue(serverMsg, "p2_role");
        string my_id = getJsonValue(serverMsg, "my_id"); 

        if (action == "info") {
            cout << "\n" << log << "\n";
        } 
        else if (action == "start") {
            cout << "\n|===========================Game Start============================\n";
            cout << "|       ___        _      ____     _       _     ___     ____          |\n";
            cout << "|      (___)      | |    |__  |  _| |_   _| |_  |_  |   | [] |         |\n";
            cout << "|       | |       | |__   __| | |_   _| |_   _|   | |   |  __|         |\n";
            cout << "|      _|_|_      | [] | | [] |   | |     | |    _| |_  | |__          |\n";
            cout << "|  ___|     |___  |____| |____|   |_|     |_|   |_____| |____|         |\n";
            cout << "| |   |_____|   |                                                      |\n";
            cout << "| |  ___| |___  |  ____      _    ()     ____   _   _   ____   ___     |\n";
            cout << "| |  |  | |  |  | | [] |    | |  ___    | [] | | | | | |__  | |_  |    |\n";
            cout << "| |  |  | |  |  | |  __|  __| | |_  |   |  __| | | | |  __| |   | |    |\n";
            cout << "| |  |  \\ |  |  | | |__  | [] |  _| |_  | |__  | |_| | | [] |  _| |_   |\n";
            cout << "| |__|   \\|  |__| |____| |____| |_____| |____|  \\___/  |____| |_____|  |\n";
            cout << "| By: Kelompok Cihuy                                                   |\n";
            cout << "|======================================================================|\n";
        }
        else if (action == "select_char") {
            cout << "_______________________________________________________________________\n";
            cout << "|   _                          _                                      |\n";
            cout << "|  | | _   ____   ___   ____  | | _   _____   ____   ___              |\n";
            cout << "|  | |/ / | [] | |  _| | [] | | |/ / |_   _| | []_| |  _|             |\n";
            cout << "|  |_|\\_\\ |_||_| |_|   |_||_| |_|\\_\\   |_|   |____| |_|               |\n";
            cout << "|_____________________________________________________________________|\n";
            cout << "|1. Knight            |     2. Asassin           |    3. Paladin      |\n";
            cout << "|   HP(100)           |        HP(80)            |       HP(120)      |\n";
            cout << "|   Damage(15-25)     |        Damage(20-30)     |       Damage(10-20)|\n";
            cout << "|_____________________|__________________________|____________________|\n\n";
            cout << " Pilih Karakter: ";
            
            string playerInput; cin >> playerInput;
            while (playerInput != "1" && playerInput != "2" && playerInput != "3") {
                cout << "Input salah! Ketik 1, 2, atau 3: "; cin >> playerInput;
            }
            sendJson(sock, 0, playerInput);
            
            // FIX UX: Mencegah layar terkesan "hang"
            cout << "\n[Loading] Menunggu lawan mengunci pilihan karakter...\n";
        }
        else if (action == "request_action") {
            myPlayerId = my_id; 
            string myRole = (myPlayerId == "1") ? p1_role : p2_role;
            string enemyRole = (myPlayerId == "1") ? p2_role : p1_role;
            int myHp = (myPlayerId == "1") ? p1_hp : p2_hp;
            int enemyHp = (myPlayerId == "1") ? p2_hp : p1_hp;

            // Render UI Terstruktur
            drawBattleUI(turn, myRole, myHp, enemyRole, enemyHp, lastBattleLog);
            
            string playerInput; cin >> playerInput;
            while (playerInput != "1" && playerInput != "2" && playerInput != "3") {
                cout << "\n> Input salah! Ketik 1, 2, atau 3: "; cin >> playerInput;
            }
            sendJson(sock, 0, playerInput);
            
            // FIX UX: Feedback langsung saat menunggu lawan
            cout << "\n[Loading] Tunggu lawan mengambil langkah...\n";
        }
        else if (action == "log") {
            // Update log pertarungan untuk dirender di ronde berikutnya
            lastBattleLog = log; 
        }
        else if (action == "game_over") {
            // Jaga-jaga jika server mengirim log akhir bersamaan dengan game_over
            if (log != "") {
                lastBattleLog = log;
            }

            cout << "\n=======================================================================\n";
            cout << "| _______                                                             |\n";
            cout << "||  _____|   ____   _________   ____   ____   _   _   ____   ____     |\n";
            cout << "|| |   ___  |___ | |  _   _  | | [] | | __ | | | | | | [] | |  __|    |\n";
            cout << "|| | |_  |  ___| | | | | | | | |  __| ||  || | | | | |  __| | |       |\n";
            cout << "|| |___| | | []  | | | | | | | | |__  ||__|| | |_| | | |__  | |       |\n";
            cout << "||_______| |_____| |_| |_| |_| |____| |____|  \\___/  |____| |_|       |\n";
            cout << "|---------------------------------------------------------------------|\n";
            cout << "|                              __________                             |\n";
            cout << "|                             _\\        /_                            |\n";
            cout << "|                            |[_\\      /_]|                           |\n";
            cout << "|                               _|____|_                              |\n";
            cout << "|                              |________|                             |\n";
            cout << "|---------------------------------------------------------------------|\n";
            cout << "| [ HASIL AKHIR ]: " << left << setw(51) << lastBattleLog << "|\n";
            cout << "=======================================================================\n\n";
            
            break; // Keluar setelah menampilkan data dengan aman
        }
    }

    closesocket(sock); WSACleanup(); system("pause"); return 0;
}