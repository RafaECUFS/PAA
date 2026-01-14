#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct{ //define struct de cada ponto do labirinto
    char espaco;
    int ocupado;
}ponto;

typedef struct coordenadas//define struct de coordenadas de ponto de partida
{
    int linha, coluna;
    struct coordenadas* proximo;
}ponto_partida;
typedef struct{//define pilha para rollbacks
    ponto_partida *ultimo;
}Pilha_pontos;
void cria_pilha(Pilha_pontos* pilha)//estrutura para rollbacks
{
    pilha->ultimo=NULL;
}
void Empilhar_Pt_Partida(Pilha_pontos *pilha_pontos, int linha_input, int coluna_input)//registra pontos de partidas numa pilha
{
    ponto_partida *partida = malloc(sizeof(ponto_partida));
    partida->coluna=coluna_input;
    partida->linha=linha_input;
    partida->proximo=pilha_pontos->ultimo;
    pilha_pontos->ultimo=partida;
}
void Desempilhar_Pt_Partida(Pilha_pontos *pilha_pts, FILE* output)
{   
    //elimina pontos de partidas que não tenham levem à saidas
    ponto_partida *sem_saida = pilha_pts->ultimo;
    pilha_pts->ultimo=pilha_pts->ultimo->proximo;
    fprintf(output,"BT@%d,%d<-%d,%d\n", pilha_pts->ultimo->linha,pilha_pts->ultimo->coluna, sem_saida->linha,sem_saida->coluna);
    free(sem_saida);
    sem_saida=NULL;
}

ponto_partida* Testar_Direita(ponto** labirinto, int indice_linha,int indice_coluna, FILE* output){//testa vizinho da direita
    //lembre de passar &indice_linha como parametro para atualizar na referencia e noa só fazer uma cópia
    ponto_partida* retorno = (ponto_partida*)malloc(sizeof(ponto_partida));
    if(labirinto[indice_linha][indice_coluna+1].espaco=='0' && labirinto[indice_linha][indice_coluna+1].ocupado!=1)
    {   
        retorno->coluna=indice_coluna+1;
        retorno->linha=indice_linha;
        fprintf(output,"D@%d,%d->%d,%d\n", indice_linha, indice_coluna, retorno->linha, retorno->coluna);
        labirinto[indice_linha][indice_coluna+1].ocupado=1;
        return retorno;
    }
    return NULL;
}

ponto_partida* Testar_Cima(ponto** labirinto, int indice_linha,int indice_coluna, FILE* output){//testa vizinho da direita
    //lembre de passar &indice_linha como parametro para atualizar na referencia e noa só fazer uma cópia
    ponto_partida* retorno = (ponto_partida*)malloc(sizeof(ponto_partida));
    if(labirinto[indice_linha-1][indice_coluna].espaco=='0' && labirinto[indice_linha-1][indice_coluna].ocupado!=1)
    {   
        retorno->coluna=indice_coluna;
        retorno->linha=indice_linha-1;
        fprintf(output,"F@%d,%d->%d,%d\n", indice_linha, indice_coluna, retorno->linha, retorno->coluna);
        labirinto[indice_linha-1][indice_coluna].ocupado=1;
        return retorno;
    }
    return NULL;
}

ponto_partida* Testar_Esquerda(ponto** labirinto, int indice_linha,int indice_coluna, FILE* output){//testa vizinho da direita
    //lembre de passar &indice_linha como parametro para atualizar na referencia e noa só fazer uma cópia
    ponto_partida* retorno = (ponto_partida*)malloc(sizeof(ponto_partida));
    if(labirinto[indice_linha][indice_coluna-1].espaco=='0' && labirinto[indice_linha][indice_coluna-1].ocupado!=1)
    {   
        retorno->coluna=indice_coluna-1;
        retorno->linha=indice_linha;
        fprintf(output,"D@%d,%d->%d,%d\n", indice_linha, indice_coluna, retorno->linha, retorno->coluna);
        labirinto[indice_linha][indice_coluna-1].ocupado=1;
        
        return retorno;
    }
    return NULL;
}

ponto_partida* Testar_Tras(ponto** labirinto, int indice_linha,int indice_coluna, FILE* output){//testa vizinho da direita
    //lembre de passar &indice_linha como parametro para atualizar na referencia e noa só fazer uma cópia
    ponto_partida* retorno = (ponto_partida*)malloc(sizeof(ponto_partida));
    if(labirinto[indice_linha+1][indice_coluna].espaco=='0' && labirinto[indice_linha+1][indice_coluna].ocupado!=1)
    {   

        retorno->coluna=indice_coluna;
        retorno->linha=indice_linha+1;
        fprintf(output,"T@%d,%d->%d,%d\n", indice_linha, indice_coluna, retorno->linha, retorno->coluna);
        labirinto[indice_linha+1][indice_coluna].ocupado=1;
        
        return retorno;
    }
    return NULL;
}


ponto_partida* Testar_Caminho(int linhas, int colunas, ponto labirinto[linhas][colunas], int indice_linha, int indice_coluna, FILE *output) {
    ponto_partida* retorno = NULL; // Inicializa a variável de retorno

    // Testa os vizinhos na ordem: direita, cima, esquerda, tras
    if (indice_coluna < colunas - 1 ) {
        retorno = Testar_Direita(labirinto, indice_linha, indice_coluna,output);
    }
    else if (!retorno && indice_linha > 0) {
        retorno = Testar_Cima(labirinto, indice_linha, indice_coluna,output);
    }
    else if (!retorno && indice_coluna > 0) {
        retorno = Testar_Esquerda(labirinto, indice_linha, indice_coluna,output);
    }
    else if (!retorno && indice_linha < linhas - 1) {
        retorno = Testar_Tras(labirinto, indice_linha, indice_coluna,output);
    }

    return retorno;
}


/* Potencial estratégia
    1º passo: receber labirintos
        1.1: receber numero de labrintos
            1.1.1: Para cada iteração de labirinto: receber tamanho das matrizes
                1.1.1.1: Para cada iteração de linha e coluna, atribuir valor de char (0,1 ou X)
                1.1.1.2: Se char == 'X', guardar indices de ponto_partida_fixo na estrutura de labirinto
                ---------------------------------------------------------------------------------------------V
    2º passo: guardar labirinto completo (opcional)----------X
    
    3º passo: Em ordem de inserção, resolver labirinto:
        3.1: Lendo ponto de partida, avaliar espaços vizinhos:
            3.1.1: Se (linha+/-1 e coluna == partida ou coluna+/-1 e linha==partida) == '0': espaço livre 
                3.1.1.1: Se indice linha/coluna  == 0 ou length.array && (linha!= partida || coluna!=partida): Saída encontrada
                3.1.1.2: Senão, Novo ponto partida
                3.1.1.3: Nova iteração
            3.1.2: Se (linha+/-1 e coluna == partida ou coluna+/-1 e linha==partida) == '1' e iteração no ponto partida <=4: espaço ocupado
                3.1.2.1: Se iteração <=4, testar outro espaço vizinho a ponto de partida
                3.1.2.2: Senão, eliminar ponto de partida atual
                3.1.2.3: Nova iteração
            3.1.3: Se (linha+/-1 e coluna == partida ou coluna+/-1 e linha==partida) == '1' e iteração no ponto partida >4: não há solução

    Possivel implementação: Novo ponto de partida sempre inserido em uma pilha
                            Se todos os vizinhos forem testados sem sucesso: Elimina o ponto de partida da pilha
Esquerda: coluna-1 / Direita: coluna +1
Frente: Linha-1 / Trás: linha+1

ordem: D>F>E>T
Se ja passou, não conta. 
*/

int main(int argc, char* argv[])//loop principal
{   ponto_partida* atual = (ponto_partida*)malloc(sizeof(ponto_partida));
    int num_labirintos,linhas_lab,colunas_lab;
    int linha_atual=0,coluna_atual=0;

    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Erro ao abrir o arquivo de entrada!\n");
        perror("Detalhes do erro");
        return 0;
    }

    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        printf("Erro ao abrir o arquivo de saída!\n");
        perror("Detalhes do erro");
        fclose(input);
        return 0;
    }
    fscanf(input,"%d",&num_labirintos);

    for(int indice_labirintos=0;indice_labirintos<num_labirintos;indice_labirintos++)
    {
        fprintf(output,"L%d:\n",indice_labirintos);
        Pilha_pontos backtrack_labirinto;
        cria_pilha(&backtrack_labirinto);
        //recebe dien soes do labirinto e aloca espaço
        fscanf(input,"%d %d",&colunas_lab,&linhas_lab);
        ponto labirinto[linhas_lab][colunas_lab];
        //ponto **labirinto = (ponto**)calloc(linhas_lab,sizeof(ponto*));

        

        //aloca espaço para colunas e atrubui valores às células
        //preenche a tabela
        for(int indice_linha=0;indice_linha<linhas_lab;indice_linha++)
        {
            //labirinto[indice_linha] =(ponto*) calloc(colunas_lab,sizeof(ponto));

            /*if (labirinto[indice_linha] == NULL) 
                {
        // Tratar falha na alocação de memória
            fprintf(stderr, "Falha na alocação de memória para linha.\n");
            exit(EXIT_FAILURE);*/
                //}
            for(int indice_coluna=0;indice_coluna<colunas_lab;indice_coluna++)
            {           
                char c = fgetc(input);
                // Ignora os espaços em branco
                while (c == ' ' || c == '\n' || c == '\r') {
                    c = fgetc(input);
                    }

            //fgets(&labirinto[indice_linha][indice_coluna].espaco,colunas_lab,input);//nao ta pegando o caracter
            if(labirinto[indice_linha][indice_coluna].espaco=='X')//checa se é o ponto de partida
            {
                Empilhar_Pt_Partida(&backtrack_labirinto,indice_linha,indice_coluna);
                fprintf(output,"INICIO@%d,%d:\n",indice_linha,indice_coluna);

            }
        }    
        }
        coluna_atual=backtrack_labirinto.ultimo->coluna;
        linha_atual=backtrack_labirinto.ultimo->linha;
        
        //enquanto não achar saida ou pilha backtrack tiver membro
        while (((linha_atual > 0 && linha_atual < linhas_lab-1) &&
        (coluna_atual > 0 && coluna_atual < colunas_lab-1)) || labirinto[linha_atual][coluna_atual].espaco=='X')
        {
            //testar em ordem d>f>e>t
            atual = Testar_Caminho(linhas_lab,colunas_lab,labirinto[linhas_lab][colunas_lab],backtrack_labirinto.ultimo->linha,backtrack_labirinto.ultimo->coluna, output);
            if(atual!=NULL){Empilhar_Pt_Partida(&backtrack_labirinto,atual->linha,atual->coluna);}
            else{
                Desempilhar_Pt_Partida(&backtrack_labirinto,output);
                if(backtrack_labirinto.ultimo == NULL) break;}

        }
        if(backtrack_labirinto.ultimo==NULL)fprintf(output,"SEM_SAIDA\n");
        else fprintf(output,"SAIDA@%d,%d\n",backtrack_labirinto.ultimo->linha,backtrack_labirinto.ultimo->coluna);;
        
        return 0;
        }
}