/* 
 * File:   agentIA.c
 * Author: Igor Paim, Lucas Borges, Thiago Pacheco
 * 
 * Projeto de Agente Inteligente para localizar as vagas mais próximas da entrada
 * no estacionamento e indicá-las ao motorista que chega ao local.
 *
 * Created on 2 de Abril de 2015, 17:29
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define DIM 5

// Protótipos:
void imprimeVagas();
int contaVagas();
void buscaVagaProfundidade();


// matriz do estacionamento
int matriz[DIM][DIM];

/*
 * 
 */
int main(int argc, char** argv) {
    int linha, coluna, vagas = 0;

    // "Planta" o random
    srand(time(NULL));

    //escrevendo na Matriz
    for (linha = 0; linha < DIM; linha++) {
        for (coluna = 0; coluna < DIM; coluna++) {
            //printf("Elemento [%d][%d]: ", linha + 1, coluna + 1);
            //scanf("%d", &matriz[linha][coluna]);
            matriz[linha][coluna] = rand() % 2;
            // printf("\n");
            //printf("Um numero entre 0 e 1: %d\n", rand() % 2);
        }
    }

    system("clear"); //clears the screen
    imprimeVagas();
    vagas = contaVagas();

    // chegada de carros
    while (vagas > 0) {
        printf("Carro na entrada:\n");
        buscaVagaProfundidade();
        vagas = contaVagas();
    }

    return (EXIT_SUCCESS);
}

void imprimeVagas() {
    int linha, coluna;

    printf("Estacionamento:\n");
    for (linha = 0; linha < DIM; linha++) {
        printf("\t%c", linha + 65);
    }
    printf("\n");
    // imprimindo a matriz na tela
    for (linha = 0; linha < DIM; linha++) {
        for (coluna = 0; coluna < DIM; coluna++) {
            if (coluna == 0) {
                printf("%d", linha);
                printf("\t%d", matriz[linha][coluna]);
            } else {
                printf("\t%d", matriz[linha][coluna]);
            }
        }
        printf("\n"); //após cada linha ser impressa, um salto de linha
    }
}

int contaVagas() {
    int linha, coluna, count = 0;

    for (linha = 0; linha < DIM; linha++) {
        for (coluna = 0; coluna < DIM; coluna++) {
            if (matriz[linha][coluna] == 0)
                count++;
        }
    }
    if (count > 0) {
        if (count == 1)
            printf("\n%d vaga no estacionamento\n", count);
        else
            printf("\n%d vagas no estacionamento\n", count);
    } else
        printf("\nLamentamos, mas não há vagas no estacionamento!\n");

    return count;
}

void buscaVagaProfundidade() {
    int linha, coluna = 0, count = 0;

    printf("\nBUSCA EM PROFUNDIDADE\n");

    if (coluna == 0) {
        for (linha = 0; linha < DIM; linha++) {
            if (matriz[linha][coluna] == 0) {
                printf("\nVaga encontrada: %c%d\n", coluna + 65, linha);
                sleep(2);
                printf("Carro estacionado na vaga: %c%d\n", coluna + 65, linha);
                matriz[linha][coluna] = 1; // estaciona na primeira vaga encontrada
                count++;
                sleep(2);
                imprimeVagas();
                return;
            }
        }
    }

    coluna++;

    for (linha = DIM - 1; linha >= 0; coluna++) {
        // printf("dentro do for");
        if (matriz[linha][coluna] == 0) {
            printf("\nVaga encontrada: %c%d\n", coluna + 65, linha);
            sleep(2);
            printf("Carro estacionado na vaga: %c%d\n", coluna + 65, linha);
            matriz[linha][coluna] = 1; // estaciona na primeira vaga encontrada
            count++;
            sleep(2);
            imprimeVagas();
            return;
        }
        if (coluna == DIM - 1) {
            linha--;
            coluna = 1;
            if (matriz[linha][coluna] == 0) {
                printf("\nVaga encontrada: %c%d\n", coluna + 65, linha);
                sleep(2);
                printf("Carro estacionado na vaga: %c%d\n", coluna + 65, linha);
                matriz[linha][coluna] = 1; // estaciona na primeira vaga encontrada
                count++;
                sleep(2);
                imprimeVagas();
                return;
            }
        }
    }
}
