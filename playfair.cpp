#include <iostream>
#include <cstring>
#include <cctype>

#define MX 5

using namespace std;

void playfair(char ch1, char ch2, char key[MX][MX])
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
        // cùng hàng
        x = (x + 1) % MX;
        z = (z + 1) % MX;
        cout << key[w][x] << key[y][z];
    }
    else if (x == z)
    {
        // cùng cột
        w = (w + 1) % MX;
        y = (y + 1) % MX;
        cout << key[w][x] << key[y][z];
    }
    else
    {
        // hình chữ nhật
        cout << key[w][z] << key[y][x];
    }
}

int main()
{
    int i, j, k = 0, m = 0, n;
    char key[MX][MX], keyminus[25], keystr[100], str[100];
    char alpa[26] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
        'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    cout << "Enter key: ";
    cin >> keystr;
    cout << "Enter the plain text: ";
    cin >> str;

    // chuyển key sang chữ hoa, thay J = I
    n = strlen(keystr);
    for (i = 0; i < n; i++)
    {
        if (keystr[i] == 'j' || keystr[i] == 'J')
            keystr[i] = 'I';
        keystr[i] = toupper(keystr[i]);
    }

    // chuyển plaintext sang chữ hoa, thay J = I
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == 'j' || str[i] == 'J')
            str[i] = 'I';
        str[i] = toupper(str[i]);
    }

    // tạo keyminus: những chữ cái không nằm trong key
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

    // xây ma trận key
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

    cout << "\nEntered text: " << str << "\nCipher Text: ";
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i + 1] == '\0')
        {
            playfair(str[i], 'X', key);
        }
        else
        {
            if (str[i] == str[i + 1])
            {
                playfair(str[i], 'X', key);
            }
            else
            {
                playfair(str[i], str[i + 1], key);
                i++; // skip next character
            }
        }
    }

    cout << endl;
    return 0;
}
