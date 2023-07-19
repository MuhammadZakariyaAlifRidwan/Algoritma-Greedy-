#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
using namespace std;

// Struktur data untuk merepresentasikan graf
struct Graph {
    unordered_map<string, int> cityToIndex; // Pemetaan nama kota ke indeks
    vector<string> indexToCity; // Pemetaan indeks ke nama kota
    vector<vector<pair<int, int>>> adjList; // Daftar adjacency (vertex, jarak)

    // Konstruktor
    Graph() {}

    // Menambahkan kota ke dalam graf
    void addCity(const string& city) {
        cityToIndex[city] = indexToCity.size();
        indexToCity.push_back(city);
        adjList.emplace_back();
    }

    // Menambahkan edge antara dua kota beserta jaraknya
    void addEdge(const string& city1, const string& city2, int distance) {
        int index1 = cityToIndex[city1];
        int index2 = cityToIndex[city2];
        adjList[index1].push_back(make_pair(index2, distance));
        adjList[index2].push_back(make_pair(index1, distance));
    }

    // Menggunakan algoritma Greedy untuk mencari jalur terpendek
    vector<string> greedyShortestPath(const string& startCity, const string& endCity) {
        int start = cityToIndex[startCity];
        int end = cityToIndex[endCity];

        // Array jarak terpendek
        vector<int> distance(indexToCity.size(), INT_MAX);
        // Array penanda apakah vertex sudah dikunjungi
        vector<bool> visited(indexToCity.size(), false);
        // Array untuk menyimpan jalur terpendek
        vector<int> path(indexToCity.size(), -1);

        // Prioritas queue untuk vertex dan jaraknya
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        // Inisialisasi jarak vertex awal sebagai 0
        distance[start] = 0;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            visited[u] = true;

            // Cek apakah sudah mencapai vertex tujuan
            if (u == end)
                break;

            // Iterasi melalui semua tetangga vertex saat ini
            for (pair<int, int>& neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                // Jika vertex belum dikunjungi dan jaraknya lebih pendek, perbarui jaraknya
                if (!visited[v] && distance[v] > distance[u] + weight) {
                    distance[v] = distance[u] + weight;
                    path[v] = u;
                    pq.push(make_pair(distance[v], v));
                }
            }

            // Tampilkan proses pencarian
            cout << "Proses pencarian - Vertex: " << indexToCity[u] << ", Jarak: " << distance[u] << endl;
        }

        // Membangun jalur terpendek dari array path
        vector<string> shortestPath;
        int current = end;
        while (current != -1) {
            shortestPath.push_back(indexToCity[current]);
            current = path[current];
        }
        reverse(shortestPath.begin(), shortestPath.end());

        return shortestPath;
    }
};

int main() {
    bool repeat = true;

    while (repeat) {
        Graph graph;
        system("cls");
        int cityCount;
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| \t\t PROGRAM IMPLEMENTASI PENCARIAN RUTE TERPENDEK MENGGUNAKAN ALGORITMA GREEDY \t\t    |" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| Masukkan jumlah kota: ";
        cin >> cityCount;

        for (int i = 0; i < cityCount; ++i) {
            string cityName;
            cout << "| Masukkan nama kota ke-" << i + 1 << ": ";
            cin >> cityName;
            graph.addCity(cityName);
        }

        int edgeCount;
        cout << "| Masukkan jumlah edge (jarak antara kota): ";
        cin >> edgeCount;

        for (int i = 0; i < edgeCount; ++i) {
            string city1, city2;
            int distance;
            cout << "| Masukkan nama kota pertama, nama kota kedua, dan jarak antara keduanya (dipisahkan dengan spasi): ";
            cin >> city1 >> city2 >> distance;
            graph.addEdge(city1, city2, distance);
        }

        string startCity, endCity;

        // Memasukkan kota awal
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| Masukkan kota awal: ";
        cin >> startCity;

        // Memasukkan kota tujuan
        cout << "| Masukkan kota tujuan: ";
        cin >> endCity;
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| \t\t\t\t\tMencari rute terpendek..." << endl;
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        // Mengecek apakah kota awal dan tujuan ada dalam graf
        while (!graph.cityToIndex.count(startCity) || !graph.cityToIndex.count(endCity) || startCity == endCity) {
            cout << "| Rute terpendek tidak valid. Silakan masukkan kota awal dan tujuan yang berbeda." << endl;
            cout << "| Masukkan kota awal: ";
            cin >> startCity;
            cout << "| Masukkan kota tujuan: ";
            cin >> endCity;
        }

        // Mencari jalur terpendek dan jaraknya
        vector<string> shortestPath = graph.greedyShortestPath(startCity, endCity);

        int shortestDistance = 0;
        for (int i = 0; i < shortestPath.size() - 1; ++i) {
            int index1 = graph.cityToIndex[shortestPath[i]];
            int index2 = graph.cityToIndex[shortestPath[i + 1]];
            int distance = -1;
            for (const auto& neighbor : graph.adjList[index1]) {
                if (neighbor.first == index2) {
                    distance = neighbor.second;
                    break;
                }
            }
            cout << "| Rute - " << shortestPath[i] << " -> " << shortestPath[i + 1] << ", Jarak: " << distance << endl;
            shortestDistance += distance;
        }

        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| \t\t\t\t\tHasil Pencarian" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| Jalur terpendek dari " << startCity << " ke " << endCity << ":" << endl;
        for (int i = 0; i < shortestPath.size() - 1; ++i) {
            cout << "| " << shortestPath[i] << " -> ";
        }
        cout << shortestPath.back() << endl;
        cout << "| Jarak terpendek: " << shortestDistance << endl;
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;

        // Mengulangi penggunaan program
        char repeatChoice;
        cout << "| Apakah Anda ingin menggunakan program ini kembali? (y/n): "<< endl;
        cin >> repeatChoice;
        repeat = (repeatChoice == 'y' || repeatChoice == 'Y');

    }
    cout << "| \t\t\tTerima kasih telah menggunakan program ini"<< endl;
    cout << "-------------------------------------------------------------------------------------------------------------" << endl;

    return 0;
}
