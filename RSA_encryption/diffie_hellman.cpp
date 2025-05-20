#include <stdio.h>
#include <math.h>

int main() {
    int p = 23;      // publicly known prime number
    int g = 5;       // publicly known primitive root
    int x = 4;       // only Alice knows this secret
    int y = 3;       // only Bob knows this secret

    double aliceSends = fmod(pow(g, x), p);
    double bobSends = fmod(pow(g, y), p);

    double bobComputes = fmod(pow(aliceSends, y), p);
    double aliceComputes = fmod(pow(bobSends, x), p);

    double sharedSecret = fmod(pow(g, x * y), p);

    printf("Simulation of Diffie-Hellman key exchange algorithm\n");
    printf("Alice Sends: %.0f\n", aliceSends);
    printf("Bob Computes: %.0f\n", bobComputes);
    printf("Bob Sends: %.0f\n", bobSends);
    printf("Alice Computes: %.0f\n", aliceComputes);
    printf("Shared Secret: %.0f\n", sharedSecret);

    if (aliceComputes == sharedSecret && aliceComputes == bobComputes)
        printf("Success: Shared Secrets Match! %.0f\n", sharedSecret);
    else
        printf("Error: Shared Secrets do not Match\n");

    return 0;
}
