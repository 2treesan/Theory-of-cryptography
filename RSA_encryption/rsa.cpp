#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

long int p, q, n, t, flag, e[100], d[100], temp[100], j, m[100], en[100], i;
char msg[100];

int is_prime(long int);
void calculate_e_d();
long int compute_d(long int);
void encrypt();
void decrypt();

int main() {
    printf("NHAP SO NGUYEN TO THU NHAT (p): ");
    scanf("%ld", &p);
    flag = is_prime(p);
    if (flag == 0) {
        printf("KHONG PHAI SO NGUYEN TO. THOAT!\n");
        return 1;
    }

    printf("NHAP SO NGUYEN TO THU HAI (q): ");
    scanf("%ld", &q);
    flag = is_prime(q);
    if (flag == 0 || p == q) {
        printf("KHONG PHAI SO NGUYEN TO HOAC TRUNG VOI p. THOAT!\n");
        return 1;
    }

    printf("NHAP THONG DIEP (chu thuong, khong dau, khong cach): ");
    scanf("%s", msg);

    for (i = 0; msg[i] != '\0'; i++)
        m[i] = msg[i];

    n = p * q;
    t = (p - 1) * (q - 1);
    calculate_e_d();

    printf("\nCAC CAP e, d CO THE SU DUNG:\n");
    for (i = 0; i < j - 1; i++)
        printf("e = %ld,\td = %ld\n", e[i], d[i]);

    encrypt();
    decrypt();

    return 0;
}

int is_prime(long int num) {
    long int i;
    long int sqrt_num = sqrt(num);
    for (i = 2; i <= sqrt_num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

void calculate_e_d() {
    long int k = 0;
    for (i = 2; i < t; i++) {
        if (t % i == 0)
            continue;
        flag = is_prime(i);
        if (flag == 1 && i != p && i != q) {
            e[k] = i;
            long int d_candidate = compute_d(e[k]);
            if (d_candidate > 0) {
                d[k] = d_candidate;
                k++;
            }
            if (k == 99)
                break;
        }
    }
    j = k;
}

long int compute_d(long int x) {
    long int k = 1;
    while (1) {
        k = k + t;
        if (k % x == 0)
            return (k / x);
    }
}

void encrypt() {
    long int pt, ct, key = e[0], k, len;
    i = 0;
    len = strlen(msg);
    printf("\n--- MA HOA ---\n");
    printf("BAN MA: ");
    while (i != len) {
        pt = m[i] - 96; // quy A=1, B=2,...
        k = 1;
        for (j = 0; j < key; j++) {
            k = k * pt;
            k = k % n;
        }
        temp[i] = k;
        ct = k + 96;
        en[i] = ct;
        printf("%c", en[i]);
        i++;
    }
    en[i] = -1;
    printf("\n");
}

void decrypt() {
    long int pt, ct, key = d[0], k;
    i = 0;
    printf("\n--- GIAI MA ---\n");
    printf("BAN GOC: ");
    while (en[i] != -1) {
        ct = temp[i];
        k = 1;
        for (j = 0; j < key; j++) {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        m[i] = pt;
        printf("%c", m[i]);
        i++;
    }
    m[i] = -1;
    printf("\n");
}
