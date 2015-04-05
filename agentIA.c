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
#define DIM 4

// estrutura para abstrair a agenda do algoritmo A*

struct lista {
    int linha, coluna, custo_total;
    struct lista* prox;
};
typedef struct lista Lista;

Lista* Lista_A_estrela = NULL;


// Protótipos:
void geraMatriz();
void imprimeVagas();
int contaVagas();
void buscaVagaProfundidade();
void list_A_estrela();
//void inicia(node *LISTA);
int calc_custo_total(int linha, int coluna);
//void insereListaOrdenada(node LISTA, int linha, int coluna, int custo_total);
void insereListaOrdenada(int linha, int coluna, int custo_total);
//Lista* inicializa(void);
void imprime_rec();
void busca_vaga_estrela(Lista* l);
void loopProfundidade();
void loopAEstrela();


// matriz do estacionamento
int matriz[DIM][DIM];

/*
 * 
 */
int main(int argc, char** argv) {
    int continuar = 1;

    // "Planta" o random
    srand(time(NULL));
    geraMatriz();
    system("clear"); //clears the screen
    imprimeVagas();

    do {
        printf("\n\tEstacionamento agentIA:\n\n");
        printf("1. Buscar vaga em profundidade\n");
        printf("2. Buscar vaga A*\n");
        printf("3. Reiniciar Estacionamento\n");
        printf("0. Sair\n");

        scanf("%d", &continuar);

        system("clear"); //clears the screen

        switch (continuar) {
            case 1:
                loopProfundidade();
                break;

            case 2:
                loopAEstrela();
                break;

            case 3:
                geraMatriz();
                imprimeVagas();
                break;
            case 0:
                break;

            default:
                printf("Digite uma opcao valida\n");
        }
    } while (continuar);

    return (EXIT_SUCCESS);
}

void loopProfundidade() {
    int vagas;
    while (vagas > 0) {
        printf("Carro na entrada:\n");
        buscaVagaProfundidade();
        sleep(2);
        vagas = contaVagas();
    }
}

void loopAEstrela() {
    int vagas;
    while (vagas > 0) {
        printf("Carro na entrada:\n");
        sleep(2);
        list_A_estrela();
        busca_vaga_estrela(Lista_A_estrela);
        vagas = contaVagas();
    }
}

void geraMatriz() {
    int linha, coluna;

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

void list_A_estrela() {
    int linha, coluna, custo_total;
    //lendo valores da Matriz
    for (linha = 0; linha < DIM; linha++) {
        for (coluna = 0; coluna < DIM; coluna++) {
            custo_total = calc_custo_total(linha, coluna);
            insereListaOrdenada(linha, coluna, custo_total);
            imprime_rec(Lista_A_estrela);
        }
    }
}

int calc_custo_total(int linha, int coluna) {
    int result;


    // para coluna 0 a distancia sera igual a deslocamento pois o portão é na coluna 0
    if (coluna == 0) {
        return result = linha;
    }
    // custo total =  deslocamento + distancia em linha reta
    return result = (linha + coluna) + (coluna);
}

void insereListaOrdenada(int linha, int coluna, int custo_total) {
    Lista* ant = NULL;
    Lista* p = Lista_A_estrela;

    Lista* novo = (Lista*) malloc(sizeof (Lista));
    novo->coluna = coluna;
    novo->custo_total = custo_total;
    novo->linha = linha;
    novo->prox = NULL;

    // iserção em lista vazia
    if (Lista_A_estrela == NULL) {
        Lista_A_estrela = novo;
        novo->prox = NULL;
        imprime_rec(Lista_A_estrela);
        return;
    }
    // procura posição de inserção 
    while (p != NULL && p->custo_total < custo_total) {
        ant = p;
        p = p->prox;
    }
    //insere elemento 
    if (ant == NULL) { // insere elemento no início 
        novo->prox = Lista_A_estrela;
        Lista_A_estrela = novo;
        novo->prox = NULL;
    } else { // insere elemento no meio da lista 
        novo->prox = ant->prox;
        ant->prox = novo;
    }

}

void imprime_rec(Lista* l) {
    if (l == NULL)
        return;
    //fprintf(stderr, "\nlinha:%d|coluna:%d|custoTotal:%d", l->linha, l->coluna, l->custo_total);
    //printf(stderr, "\nprox: %p", l->prox);
    // sleep(2);
    imprime_rec(l->prox);
}

void busca_vaga_estrela(Lista* l) {
    int linha, coluna;

    if (l == NULL)
        return;

    linha = l->linha;
    coluna = l->coluna;
    
    printf("\nBUSCA A*\n");
    
    if (matriz[linha][coluna] == 0) {
        printf("\nVaga encontrada: %c%d\n", coluna + 65, linha);
        sleep(2);
        matriz[linha][coluna] = 1; //estaciona o carro na vaga
        printf("Carro estacionado na vaga: %c%d\n", coluna + 65, linha);
        imprimeVagas();
        sleep(2);
        return;
    } else {
        busca_vaga_estrela(l->prox);
    }
}
