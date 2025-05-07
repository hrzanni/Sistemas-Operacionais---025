#include <stdio.h>
#include <stdlib.h>

#define ITERATIONS 10000000000LL

int main() {
    __uint128_t sum = 0;    

    printf("Iniciando cálculos...\n");
    for (__uint128_t i = 1; i <= ITERATIONS; i++) {
        sum += i;
    }
    
    unsigned long long high = (unsigned long long)(sum >> 64);  // Parte mais alta
    unsigned long long low = (unsigned long long)(sum);  // Parte mais baixa

    printf("%llu%llu\n", high, low);
    
    return 0;
}
