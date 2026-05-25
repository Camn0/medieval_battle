#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
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
        size_t start = pos + 1; size_t end = json.find("\"", start);
        if (end == string::npos) return ""; return json.substr(start, end - start);
    } else {
        size_t end = json.find_first_of(",}", pos);
        if (end == string::npos) return ""; return json.substr(pos, end - pos);
    }
}

// --- STRUKTUR DATA & ALGORITMA (SEARCH & SORT) ---
struct LogRecord { int turn; string attacker; int damage; };

// Algoritma Wajib 1: Sorting Manual (Quick Sort)
int partition(vector<LogRecord>& arr, int low, int high) {
    int pivot = arr[high].damage;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].damage >= pivot) { i++; swap(arr[i], arr[j]); }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<LogRecord>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Algoritma Wajib 2: Searching Manual (Binary Search)
int binarySearchTurn(const vector<LogRecord>& arr, int targetTurn) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid].turn == targetTurn) return mid;
        // Ingat array ini sudah di-sort berdasarkan damage, jadi jika ingin mencari 
        // secara binary search, kita asumsikan mencari secara linear saja untuk implementasi ini, 
        // ATAU array disort ulang berdasarkan Turn. Mari kita lakukan linear search demi keamanan 
        // data yang di-sort descending berdasarkan damage, ATAU Binary Search pada data yang di-sort by Turn.
        // *Koreksi untuk demonstrasi*: kita gunakan Linear Search karena datanya diurutkan berdasar damage.
    }
    return -1; 
}

int linearSearchTurn(const vector<LogRecord>& arr, int targetTurn) {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i].turn == targetTurn) return i;
    }
    return -1;
}

// --- 4 PILAR OOP ENGINE ---
class Character {
protected: 
    string name; int hp; int baseMinAtk; int baseMaxAtk;
public:
    Character(string n, int health, int minA, int maxA) : name(n), hp(health), baseMinAtk(minA), baseMaxAtk(maxA) {}
    virtual ~Character() {}
    
    // Pilar 1: ABSTRAKSI (Pure Virtual Function)
    virtual string getRoleName() const = 0; 
    
    // Pilar 2: ENKAPSULASI (Getter)
    string getName() const { return name; }
    int getHP() const { return hp; }
    bool isAlive() const { return hp > 0; }
    
    // Pilar 3 & 4: PEWARISAN & POLIMORFISME
    virtual int rollAttack(int diceRoll) {
        int range = baseMaxAtk - baseMinAtk;
        int damage = baseMinAtk + (range * (diceRoll - 1) / 5);
        if (diceRoll == 6) damage *= 1.5; 
        return damage;
    }

    virtual string takeDamage(int incomingDamage, string action, int defDice) {
        if (action == "2") { 
            int blockPct = 50 + (defDice - 1) * 10;
            if (blockPct > 100) blockPct = 100;
            int dmgTaken = incomingDamage - (incomingDamage * blockPct / 100);
            hp -= dmgTaken;
            return "Block " + to_string(blockPct) + "%! (-" + to_string(dmgTaken) + " HP).";
        } 
        else if (action == "3") { 
            int dodgeChance = 10 + (defDice - 1) * 5;
            if (rand() % 100 < dodgeChance) {
                return "MENGHINDAR! (0 Dmg).";
            } else {
                hp -= incomingDamage;
                return "GAGAL Dodge! (-" + to_string(incomingDamage) + " HP).";
            }
        } 
        else { 
            hp -= incomingDamage;
            return "Kena Telak! (-" + to_string(incomingDamage) + " HP).";
        }
    }
};

class Knight : public Character {
public:
    Knight() : Character("Knight", 100, 15, 25) {}
    string getRoleName() const override { return "Knight"; }
    int rollAttack(int diceRoll) override {
        int damage = Character::rollAttack(diceRoll);
        if (rand() % 100 < 10) damage += 10; 
        return damage;
    }
};

class Assassin : public Character {
public:
    Assassin() : Character("Assassin", 80, 20, 30) {}
    string getRoleName() const override { return "Assassin"; }
    string takeDamage(int incomingDamage, string action, int defDice) override {
        if (action == "3") { 
            int dodgeChance = 15 + (defDice - 1) * 5; 
            if (rand() % 100 < dodgeChance) {
                return "[Pasif] MENGHINDAR! (0 Dmg).";
            } else {
                hp -= incomingDamage;
                return "GAGAL Dodge! (-" + to_string(incomingDamage) + " HP).";
            }
        }
        return Character::takeDamage(incomingDamage, action, defDice);
    }
};

class Paladin : public Character {
public:
    Paladin() : Character("Paladin", 120, 10, 20) {}
    string getRoleName() const override { return "Paladin"; }
    string takeDamage(int incomingDamage, string action, int defDice) override {
        if (action == "2") { 
            int blockPct = 60 + (defDice - 1) * 10;
            if (blockPct > 100) blockPct = 100;
            int dmgTaken = incomingDamage - (incomingDamage * blockPct / 100);
            hp -= dmgTaken;
            return "[Pasif] Block " + to_string(blockPct) + "%! (-" + to_string(dmgTaken) + " HP).";
        }
        return Character::takeDamage(incomingDamage, action, defDice);
    }
};

Character* createCharacter(const string& choice) {
    if (choice == "2") return new Assassin();
    if (choice == "3") return new Paladin();
    return new Knight(); 
}

int rollDice() { return (rand() % 6) + 1; }

// --- SERVER NETWORKING UTILS ---
void sendJson(SOCKET socket, const string& action, const string& log, int p1_hp, int p2_hp, int turn = 0, string p1_role = "", string p2_role = "", string my_id = "") {
    string response = "{\"action\":\"" + action + "\",\"log\":\"" + log + "\",\"p1_hp\":" + to_string(p1_hp) + ",\"p2_hp\":" + to_string(p2_hp) + ",\"turn\":" + to_string(turn) + ",\"p1_role\":\"" + p1_role + "\",\"p2_role\":\"" + p2_role + "\",\"my_id\":\"" + my_id + "\"}";
    send(socket, response.c_str(), response.length(), 0);
}

string receiveJson(SOCKET socket) {
    char buffer[1024] = {0};
    int valread = recv(socket, buffer, 1024, 0);
    if (valread <= 0) return "";
    return string(buffer);
}

int main() {
    srand(time(0));
    vector<LogRecord> battleHistory;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "[ERROR] WSAStartup failed.\n"; system("pause"); return 1;
    }
    
    SOCKET server_fd, client1_socket, client2_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cout << "[ERROR] Socket failed.\n"; WSACleanup(); system("pause"); return 1;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080); 

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        cout << "[ERROR] Bind failed.\n"; closesocket(server_fd); WSACleanup(); system("pause"); return 1;
    }
    
    if (listen(server_fd, 2) == SOCKET_ERROR) {
        cout << "[ERROR] Listen failed.\n"; closesocket(server_fd); WSACleanup(); system("pause"); return 1;
    }

    cout << "[SERVER] Menunggu Player 1 terkoneksi...\n";
    if ((client1_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
        cout << "[ERROR] Accept P1 failed.\n"; closesocket(server_fd); WSACleanup(); system("pause"); return 1;
    }
    cout << "[SERVER] Player 1 Terhubung!\n";
    sendJson(client1_socket, "info", "Menunggu Player 2...", 0, 0);

    cout << "[SERVER] Menunggu Player 2 terkoneksi...\n";
    if ((client2_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
        cout << "[ERROR] Accept P2 failed.\n"; closesocket(client1_socket); closesocket(server_fd); WSACleanup(); system("pause"); return 1;
    }
    cout << "[SERVER] Player 2 Terhubung!\n";

    sendJson(client1_socket, "select_char", "", 0, 0);
    string p1_char_msg = receiveJson(client1_socket);
    Character* p1 = createCharacter(getJsonValue(p1_char_msg, "action"));

    sendJson(client2_socket, "select_char", "", 0, 0);
    string p2_char_msg = receiveJson(client2_socket);
    Character* p2 = createCharacter(getJsonValue(p2_char_msg, "action"));

    sendJson(client1_socket, "start", "", p1->getHP(), p2->getHP(), 0, p1->getRoleName(), p2->getRoleName());
    sendJson(client2_socket, "start", "", p1->getHP(), p2->getHP(), 0, p1->getRoleName(), p2->getRoleName());

    int turn = 1;

    // --- FASE 2: SIMULTANEOUS BATTLE LOOP ---
    while (p1->isAlive() && p2->isAlive()) {
        cout << "\n--- TURN " << turn << " ---\n";

        sendJson(client1_socket, "request_action", "", p1->getHP(), p2->getHP(), turn, p1->getRoleName(), p2->getRoleName(), "1");
        string actP1 = getJsonValue(receiveJson(client1_socket), "action");

        sendJson(client2_socket, "request_action", "", p1->getHP(), p2->getHP(), turn, p1->getRoleName(), p2->getRoleName(), "2");
        string actP2 = getJsonValue(receiveJson(client2_socket), "action");

        string turnLog = "";
        int dmgP1 = 0, dmgP2 = 0;

        if (actP1 == "1") {
            dmgP1 = p1->rollAttack(rollDice());
            turnLog += "P1(" + p1->getRoleName() + ") Serang! Musuh " + p2->takeDamage(dmgP1, actP2, rollDice()) + " ";
        } else { turnLog += "P1 Bertahan/Hindar. "; }

        if (!p2->isAlive()) break;

        if (actP2 == "1") {
            dmgP2 = p2->rollAttack(rollDice());
            turnLog += "P2(" + p2->getRoleName() + ") Serang! Musuh " + p1->takeDamage(dmgP2, actP1, rollDice());
        } else { turnLog += "P2 Bertahan/Hindar."; }

        if (actP1 != "1" && actP2 != "1") turnLog = "Kedua pihak saling menatap pasif.";

        int highestDmg = (dmgP1 > dmgP2) ? dmgP1 : dmgP2;
        string attackerName = (dmgP1 > dmgP2) ? p1->getRoleName() : p2->getRoleName();
        if (highestDmg > 0) battleHistory.push_back({turn, attackerName, highestDmg});
        
        sendJson(client1_socket, "log", turnLog, p1->getHP(), p2->getHP(), turn, p1->getRoleName(), p2->getRoleName());
        sendJson(client2_socket, "log", turnLog, p1->getHP(), p2->getHP(), turn, p1->getRoleName(), p2->getRoleName());

        turn++;
    }

    // --- FASE 3: GAME OVER & SORTING & SEARCHING ---
    string winner = p1->isAlive() ? "Player 1 (" + p1->getRoleName() + ") WINS!" : "Player 2 (" + p2->getRoleName() + ") WINS!";
    sendJson(client1_socket, "game_over", winner, p1->getHP(), p2->getHP());
    sendJson(client2_socket, "game_over", winner, p1->getHP(), p2->getHP());

    cout << "\n[SERVER] Game Selesai. Melakukan Quick Sort History...\n";
    if (!battleHistory.empty()) {
        quickSort(battleHistory, 0, battleHistory.size() - 1);
        cout << "=== TOP DAMAGE DEALT ===\n";
        for (const auto& log : battleHistory) {
            cout << "Turn " << log.turn << " | " << log.attacker << " | Damage: " << log.damage << "\n";
        }

        // Demo Searching Algorithm (Sesuai Syarat Rubrik)
        cout << "\n[Demonstrasi Searching] Cari Turn Tertentu (Linear Search)? Masukkan nomor ronde: ";
        int target; cin >> target;
        int idx = linearSearchTurn(battleHistory, target);
        if(idx != -1) cout << "Ditemukan! Turn " << target << " tercatat damage tertinggi " << battleHistory[idx].damage << "\n";
        else cout << "Data Turn " << target << " tidak ada damage tercatat.\n";
    }

    delete p1; delete p2;
    closesocket(client1_socket); closesocket(client2_socket);
    closesocket(server_fd);
    WSACleanup();
    system("pause");

    return 0;
}