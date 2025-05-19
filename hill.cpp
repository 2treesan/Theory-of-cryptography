#include <stdio.h>
#include <string.h>

int main() {
    // Ma trận khóa 3x3
    unsigned int a[3][3] = {
        {6, 24, 1},
        {13, 16, 10},
        {20, 17, 15}
    };

    // Ma trận nghịch đảo modulo 26 của a (để giải mã)
    unsigned int b[3][3] = {
        {8, 5, 10},
        {21, 8, 21},
        {21, 12, 8}
    };

    int i, j, t = 0;
    unsigned int c[20], d[20];
    char msg[20];

    printf("Enter plain text (3 letters, UPPERCASE A-Z only): ");
    scanf("%s", msg);

    // Bước 1: chuyển ký tự thành số (A=0,...,Z=25)
    for (i = 0; i < strlen(msg); i++) {
        c[i] = msg[i] - 65;
        printf("%d ", c[i]); // hiển thị số tương ứng
    }

    // Bước 2: Mã hóa: C = A * P (mod 26)
    for (i = 0; i < 3; i++) {
        t = 0;
        for (j = 0; j < 3; j++) {
            t += a[i][j] * c[j];
        }
        d[i] = t % 26;
    }

    printf("\nEncrypted Cipher Text: ");
    for (i = 0; i < 3; i++) {
        printf("%c", d[i] + 65);
    }

    // Bước 3: Giải mã: P = A⁻¹ * C (mod 26)
    for (i = 0; i < 3; i++) {
        t = 0;
        for (j = 0; j < 3; j++) {
            t += b[i][j] * d[j];
        }
        c[i] = t % 26;
    }

    printf("\nDecrypted Plain Text: ");
    for (i = 0; i < 3; i++) {
        printf("%c", c[i] + 65);
    }

    printf("\n");
    return 0;
}

