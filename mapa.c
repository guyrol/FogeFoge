#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"
#include <string.h>

//realiza copia do mapa para verificar posição de personagens
void copiamapa(MAPA* destino, MAPA* origem) {
    destino->linhas = origem->linhas;
    destino->colunas = origem->colunas;

    alocamapa(destino);

    for (int i = 0; i < origem->linhas; i++)
    {
        strcpy(destino->matriz[i], origem->matriz[i]);
    }
    
}

//verifica limite de perimetro do mapa
int ehparede(MAPA* m, int x, int y){
    return m->matriz[x][y] == PAREDE_VERTICAL ||
        m->matriz[x][y] == PAREDE_HORIZONTAL;
}

//verificar posição do personagem
int ehpersonagem(MAPA* m, char personagem, int x, int y) {

    return m->matriz[x][y] == personagem;

}

//verifica celula valida para movimento
int podeandar(MAPA* m,char personagem, int x, int y){
    return
        ehvalida(m, x, y) &&
        !ehparede(m, x, y) &&
        !ehpersonagem(m, personagem, x, y);
}

//REALIZA MOVIMENTO NO MAPA
void andanomapa (MAPA* m, int xorigem,
     int yorigem, int xdestino, int ydestino) {
    
    char personagem = m->matriz[xorigem][yorigem];
    m->matriz[xdestino][ydestino] = personagem;
    m->matriz[xorigem][yorigem] = VAZIO;
}

//VERIFICA SE MOVIMENTO ESTÁ DENTRO DO LIMITE
int ehvalida (MAPA* m, int x, int y){
    if(x >= m->linhas){
        return 0;
    }
    if (y >= m->colunas)
    {
        return 0;
    }
    return 1;
}


//VERIRIFCA SE CELULA DO MAPA ESTÁ VAZIA
int ehvazia (MAPA* m, int x, int y) {
    return m->matriz[x][y] == VAZIO;
}

//***LOCALIZA O JOGADOR NO MAPA***
int encontramapa (MAPA* m, POSICAO* p, char c) {

    for(int  i = 0; i < m->linhas; i++) {
        for (int j = 0; j < m->colunas; j++) {
            if (m->matriz[i][j] == c) {
                p->x = i;
                p->y = j;
                return 1;
            }
        }
    }  
    return 0; 
}


//***LEITURA DO ARQUIVO DE MAPA***
void lemapa(MAPA* m) {

    //abertura de arquivo
    FILE* f;
    f = fopen("mapa.txt", "r");
    if(f == 0) {
        printf("Erro na leitura do mapa");
        exit(1);
    }

    //verifica tamanho do mapa
    fscanf(f, "%d %d", &m->linhas, &m->colunas);

    //envoca alocação de mapa
    alocamapa(m);
    
    //guarda informação do mapa
    for(int i = 0; i < 5; i++) {
        fscanf(f, "%s", m->matriz[i]);

    }
    
    //fecha arquivo
    fclose(f);
}

//ALOCAÇÃO DINAMICA DE MOEMORIA PARA O MAPA
void alocamapa(MAPA* m) {

    m->matriz = malloc(sizeof(char*) * m->linhas);

    for (int i = 0; i < m->linhas; i++)
    {
        m->matriz[i] = malloc(sizeof(char*) * (m->colunas+1));  
    }
}

//LIBERAÇÃO DE MEMORIA ALOCADA PARA MAPA.
void liberamapa(MAPA* m) {

     for (int i = 0; i < m->linhas; i++)
    {
        free(m->matriz[i]);
    }
    free(m->matriz);
}


