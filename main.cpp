cat << 'EOF' > main.cpp
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <emscripten/emscripten.h>

using namespace std;

struct Katilimci {
    string ad;
    int bilet_sayisi;
};

vector<Katilimci> havuz;
int secilen_mod = 2;

extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void WebHavuzuTemizle() {
        havuz.clear();
    }

    EMSCRIPTEN_KEEPALIVE
    void WebModSec(int mod) {
        secilen_mod = mod;
    }

    EMSCRIPTEN_KEEPALIVE
    void WebKatilimciEkle(const char* isim, int bilet) {
        havuz.push_back({string(isim), bilet});
    }

    EMSCRIPTEN_KEEPALIVE
    int WebCekilisBaslat() {
        if (havuz.empty()) return -1;

        vector<int> torba;

        if (secilen_mod == 0) {
            for (size_t i = 0; i < havuz.size(); i++) {
                torba.push_back(i);
            }
        }
        else if (secilen_mod == 1) {
            for (size_t i = 0; i < havuz.size(); i++) {
                int bilet_hakki = i + 1; 
                for (int j = 0; j < bilet_hakki; j++) {
                    torba.push_back(i);
                }
            }
        }
        else if (secilen_mod == 2) {
            for (size_t i = 0; i < havuz.size(); i++) {
                for (int j = 0; j < havuz[i].bilet_sayisi; j++) {
                    torba.push_back(i);
                }
            }
        }

        if (torba.empty()) return -1;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, torba.size() - 1);

        int rastgele_indeks = dis(gen);
        return torba[rastgele_indeks];
    }

    EMSCRIPTEN_KEEPALIVE
    const char* WebKazananAdiniGetir(int indeks) {
        if (indeks >= 0 && indeks < (int)havuz.size()) {
            return havuz[indeks].ad.c_str();
        }
        return "";
    }

    int main() {
        return 0;
    }
}
EOF