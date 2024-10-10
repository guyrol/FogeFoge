#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "mapa.h"
#include <time.h>
#include "ui.h"

//GLOBAIS
MAPA m;
POSICAO player;
int tempilula = 0;

//FUNÇÃO PARA EXPlOSÃO DA BOMBA
void explodepilula(){

    if (!tempilula) return;

    explodepilula2(player.x, player.y, 0, 1, 3);
    explodepilula2(player.x, player.y, 0, -1, 3);
    explodepilula2(player.x, player.y, 1, 0, 3);
    explodepilula2(player.x, player.y, -1, 0, 3);

    tempilula = 0;
}

//EXECUTA EXPLOSÃO
void explodepilula2(int x, int y,int somax,
     int somay, int qtd) {
    
    if(qtd == 0) return; 

    int novox = x+somax;
    int novoy = y+somay;

    if(!ehvalida(&m, novox, novoy)) return;
    if(ehparede(&m, novox, novoy)) return;

    m.matriz[novox][novoy] = VAZIO;
        
    explodepilula2(novox, novoy, somax,
         somay, qtd-1);
    
}

//INDICA O FIM DO JOGO
int acabou() {

    POSICAO pos;

    int perdeu = !encontramapa(&m, &pos, PLAYER);
    int ganhou = !encontramapa(&m, &pos, FANTASMA);

    return ganhou || perdeu;
    
}

//Movimento do fantasma
int praondefantasmavai(int xatual, int yatual,
    int* xdestino, int* ydestino) {
    
    int opcoes [4][2] = {
        { xatual, yatual+1 },
        { xatual+1, yatual },
        { xatual, yatual-1 },
        { xatual-1, yatual }
    };


    srand(time(0));
    for (int i = 0; i < 10; i++) {
        int posicao = rand () % 4;

        if (podeandar(&m, FANTASMA, opcoes[posicao][0], 
            opcoes[posicao][1])) {
            *xdestino = opcoes[posicao][0];
            *ydestino = opcoes[posicao][1];

            return 1;
        }
        

    }

    return 0;
    
}

//DETERMINA FANTASMA NO MAPA
void fantasma() {
    MAPA copia;

    copiamapa (&copia, &m);

    for (int i = 0; i < copia.linhas; i++) {
        for (int j = 0; j < copia.colunas; j++) {
            if (copia.matriz[i][j] == FANTASMA)  {
                int xdestino;
                int ydestino;

                int encontrou = praondefantasmavai(i, j,
                     &xdestino, &ydestino);

                if (encontrou) {
                    andanomapa(&m, i, j, xdestino, ydestino);
                }
            }
            
        }
        
    }
    liberamapa(&copia);

}

//VERIFICA SE DIREÇÃO É VALIDA
int ehdirecao(char direcao) {
    return
        direcao == ESQUERDA ||
        direcao == DIREITA ||
        direcao == CIMA ||
        direcao == BAIXO;
}


//MOVIMENTA PERSONAGEM
void move(char direcao) {

    //verificação de entrada valida
    if(!ehdirecao(direcao)){ 
        return;
    }

    //guarda posição de jogador
    int proximox = player.x;
    int proximoy = player.y;

    //realiza movimentação do jogador
    switch (direcao) 
    {
        case ESQUERDA:
            proximoy--;
            break;
        case CIMA:
            proximox--;
            break;
        case BAIXO:
            proximox++;
            break;
        case DIREITA:
            proximoy++;
            break;
    }

    //verifica se é direção de comando é valido
    if (!podeandar(&m, PLAYER, proximox, proximoy)) {
        return;
    }

    if (ehpersonagem(&m, PILULA, proximox, proximoy)) {
        tempilula = 1;
    }
    

    //movimenta personagem
    andanomapa(&m, player.x, player.y,
         proximox, proximoy);
    player.x = proximox;
    player.y = proximoy;

}

//MAIN
int main () {
    //leitura do mapa
   lemapa(&m);

    //localização de jogador
   encontramapa(&m, &player, PLAYER);

    do {

        printf("Tem pilula: %s\n", (tempilula ? "SIM" : "NAO"));

        //impressão do mapa
        imprimemapa(&m);

        //solicita comando de movimento do jogador
        char comando;
        scanf(" %c", &comando);

        //realiza movimento do jogador
        move(comando);

        //explode a bomba
        if (comando == BOMBA){
            explodepilula();
        }
        //executa fantasmas
        fantasma();

    } while(!acabou());

    //liberação de memoria do mapa
   liberamapa (&m);
}