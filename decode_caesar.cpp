#include <iostream>
#include <cstring>
#include <cctype>

#define MX 5

using namespace std;

// Hàm giải mã 1 cặp ký tự
void decrypt_pair(char ch1, char ch2, char key[MX][MX])
{
    int i, j, w, x, y, z;

    for (i = 0; i < MX; i++)
    {
        for (j = 0; j < MX; j++)
        {
            if (ch1 == key[i][j])
            {
                w = i;
                x = j;
            }
            else if (ch2 == key[i][j])
            {
                y = i;
                z = j;
            }
        }
    }

    if (w == y)
    {
        // Cùng hàng → lùi trái
        x = (x - 1 + MX) % MX;
        z = (z - 1 + MX) % MX;
        cout << key[w][x] << key[y][z];
    }
    else if (x == z)
    {
        // Cùng cột → lùi lên
        w = (w - 1 + MX) % MX;
        y = (y - 1 + MX) % MX;
        cout << key[w][x] << key[y][z];
    }
    else
    {
        // Hình chữ nhật
        cout << key[w][z] << key[y][x];
    }
}

int main()
{
    int i, j, k = 0, m = 0, n;
    char key[MX][MX], keyminus[25], keystr[100], cipher[100];
    char alpa[26] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
        'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    cout << "Enter key: ";
    cin >> keystr;
    cout << "Enter the cipher text: ";
    cin >> cipher;

    // Chuẩn hóa key
    n = strlen(keystr);
    for (i = 0; i < n; i++)
    {
        if (keystr[i] == 'j' || keystr[i] == 'J')
            keystr[i] = 'I';
        keystr[i] = toupper(keystr[i]);
    }

    // Chuẩn hóa cipher text
    for (i = 0; i < strlen(cipher); i++)
    {
        if (cipher[i] == 'j' || cipher[i] == 'J')
            cipher[i] = 'I';
        cipher[i] = toupper(cipher[i]);
    }

    // Tạo mảng keyminus gồm các ký tự không có trong key
    j = 0;
    for (i = 0; i < 26; i++)
    {
        for (k = 0; k < n; k++)
        {
            if (keystr[k] == alpa[i] || alpa[i] == 'J')
                break;
        }
        if (k == n)
        {
            keyminus[j++] = alpa[i];
        }
    }

    // Tạo ma trận key 5x5
    k = 0;
    for (i = 0; i < MX; i++)
    {
        for (j = 0; j < MX; j++)
        {
            if (k < n)
            {
                key[i][j] = keystr[k++];
            }
            else
            {
                key[i][j] = keyminus[m++];
            }
            cout << key[i][j] << " ";
        }
        cout << endl;
    }

    // Giải mã
    cout << "\nCipher text: " << cipher << "\nDecrypted Text: ";
    for (i = 0; i < strlen(cipher); i += 2)
    {
        char ch1 = cipher[i];
        char ch2 = (i + 1 < strlen(cipher)) ? cipher[i + 1] : 'X';
        decrypt_pair(ch1, ch2, key);
    }

    cout << endl;
    return 0;
}
