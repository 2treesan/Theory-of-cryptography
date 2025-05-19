#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

// Hàm mã hóa
void encipher() {
    char input[100], key[100];
    int i, j;

    cout << "\nEnter Plain Text: ";
    cin >> input;

    cout << "Enter Key Value: ";
    cin >> key;

    cout << "\nCipher Text: ";
    for (i = 0, j = 0; i < strlen(input); i++, j++) {
        if (j == strlen(key)) j = 0;

        char p = toupper(input[i]);
        char k = toupper(key[j]);

        char c = ((p + k - 2 * 'A') % 26) + 'A';
        cout << c;
    }
    cout << endl;
}

// Hàm giải mã
void decipher() {
    char input[100], key[100];
    int i, j;

    cout << "\nEnter Cipher Text: ";
    cin >> input;

    cout << "Enter Key Value: ";
    cin >> key;

    cout << "\nDecrypted Plain Text: ";
    for (i = 0, j = 0; i < strlen(input); i++, j++) {
        if (j == strlen(key)) j = 0;

        char c = toupper(input[i]);
        char k = toupper(key[j]);

        char p = ((c - k + 26) % 26) + 'A';
        cout << p;
    }
    cout << endl;
}

int main() {
    int choice;

    while (true) {
        cout << "\n====== VIGENÈRE CIPHER ======\n";
        cout << "1. Encrypt Text\n";
        cout << "2. Decrypt Text\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
            encipher();
        else if (choice == 2)
            decipher();
        else if (choice == 3)
            break;
        else
            cout << "Invalid choice. Please try again.\n";
    }

    return 0;
}
