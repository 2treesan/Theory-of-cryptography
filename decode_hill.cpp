#include <iostream>
#include <cstring>

using namespace std;

// Ma trận nghịch đảo của ma trận mã hóa trong Hill Cipher
const int inverseKey[3][3] = {
    {8, 5, 10},
    {21, 8, 21},
    {21, 12, 8}
};

int mod(int x, int m) {
    return ((x % m) + m) % m;
}

int main() {
    char cipher[4];
    int c[3], p[3];

    cout << "Enter cipher text (3 letters, UPPERCASE A-Z only): ";
    cin >> cipher;

    // Chuyển cipher text thành số
    for (int i = 0; i < 3; i++) {
        c[i] = cipher[i] - 'A';
    }

    // Giải mã: P = A^(-1) * C mod 26
    for (int i = 0; i < 3; i++) {
        p[i] = 0;
        for (int j = 0; j < 3; j++) {
            p[i] += inverseKey[i][j] * c[j];
        }
        p[i] = mod(p[i], 26);
    }

    // In kết quả
    cout << "Decrypted Plain Text: ";
    for (int i = 0; i < 3; i++) {
        cout << char(p[i] + 'A');
    }

    cout << endl;
    return 0;
}

