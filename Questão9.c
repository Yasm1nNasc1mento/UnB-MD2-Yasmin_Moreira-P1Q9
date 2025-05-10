#include <stdio.h>
#include <stdlib.h>

int mdc(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int ehPrimo(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int inversoMultiplicativo(int G, int n) {
    int a = G, b = n;
    int x0 = 1, x1 = 0, y0 = 0, y1 = 1;

    while (b != 0) {
        int q = a / b;
        int temp = b;
        b = a % b;
        a = temp;

        int temp_x = x1;
        x1 = x0 - q * x1;
        x0 = temp_x;

        int temp_y = y1;
        y1 = y0 - q * y1;
        y0 = temp_y;
    }

    if (a != 1) return 0; 
    return (x0 % n + n) % n;
}

int phi(int n) {
    int resultado = n;
    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            resultado -= resultado / p;
        }
    }
    if (n > 1)
        resultado -= resultado / n;
    return resultado;
}

int exponenciacaoModular(int a, int x, int n) {
    int resultado = 1;
    a = a % n;
    while (x > 0) {
        if (x % 2 == 1)
            resultado = (resultado * a) % n;
        x = x >> 1;
        a = (a * a) % n;
    }
    return resultado;
}

int main() {
    int H, G, n, x, n1;

    printf("Digite os valores para H, G, n, x e n1, separados por espaço:\n");
    printf("Formato: H G n x n1\n");
    scanf("%d %d %d %d %d", &H, &G, &n, &x, &n1);

    printf("\nIniciando o cálculo de a^x mod n1:\n\n");

    printf("Passo 1: Verificando se G e n são coprimos (mdc(%d, %d) == 1).\n", G, n);
    if (mdc(G, n) != 1) {
        printf("   G (%d) e n (%d) não são coprimos. A divisão modular não é possível.\n", G, n);
        return 1;
    }
    printf("   mdc(%d, %d) = 1. G e n são coprimos.\n", G, n);

    printf("\nPasso 2: Calculando o inverso de G (%d) em Z_%d.\n", G, n);
    int g_inverso = inversoMultiplicativo(G, n);
    if (g_inverso == 0) {
        printf("   G não tem inverso em Z_%d.\n", n);
        return 1;
    }
    printf("   O inverso de G em Z_%d é: %d\n", n, g_inverso);

   
    printf("\nPasso 3: Calculando a = H / G mod n.\n");
    int a = (H * g_inverso) % n;
    printf("   a = (%d * %d) mod %d = %d\n", H, g_inverso, n, a);

    
    printf("\nPasso 4: Verificando se a (%d) e n1 (%d) são coprimos.\n", a, n1);
    if (mdc(a, n1) != 1) {
        printf("   %d e %d não são coprimos.\n", a, n1);
        return 1;
    }
    printf("   %d e %d são coprimos.\n", a, n1);

    
    printf("\nPasso 5: Verificando se n1 (%d) é primo.\n", n1);
    int x1;
    if (ehPrimo(n1)) {
        printf("   n1 é primo.\n");
        printf("\nPasso 6: Aplicando o Pequeno Teorema de Fermat.\n");
        x1 = n1 - 1;
        printf("   x1 = n1 - 1 = %d\n", x1);
    } else {
        printf("   n1 não é primo.\n");
        printf("\nPasso 7: Aplicando o Teorema de Euler.\n");
        x1 = phi(n1);
        printf("   x1 = phi(n1) = %d\n", x1);
    }


    printf("\nPasso 8: Decompondo o expoente x (%d) = x1 (%d) * q + r.\n", x, x1);
    int q = x / x1;
    int r = x % x1;
    printf("   q = %d, r = %d\n", q, r);
    printf("   %d = %d * %d + %d\n", x, x1, q, r);


    printf("\nPasso 9: Reescrevendo a expressão %d^%d mod %d ...\n", a, x, n1);
    printf("   %d^%d = (((%d^%d)^%d mod %d) * (%d^%d mod %d)) mod %d\n", a, x, a, x1, q, n1, a, r, n1, n1);

    printf("\nPasso 10: Calculando os valores intermediários:\n");
    int x2 = exponenciacaoModular(a, x1, n1);
    printf("   x2 = a^x1 mod n1 =>  %d^%d mod %d = %d\n", a, x1, n1, x2);

    int x2_q = exponenciacaoModular(x2, q, n1);
    printf("   (a^x1)^q mod n1 =>  %d^%d mod %d = %d\n", x2, q, n1, x2_q);

    int a_r = exponenciacaoModular(a, r, n1);
    printf("   a^r mod n1 =>  %d^%d mod %d = %d\n", a, r, n1, a_r);

    printf("\nPasso 11: Combinando os resultados:\n");
    int resultado_final = (x2_q * a_r) % n1;
    printf("   ((%d^%d)^%d mod %d) * (%d^%d mod %d) mod %d = (%d * %d) mod %d = %d\n", a, x1, q, n1, a, r, n1, n1, x2_q, a_r, n1, resultado_final);

    printf("\nResultado final: %d^%d mod %d = %d\n", a, x, n1, resultado_final);

    return 0;
}