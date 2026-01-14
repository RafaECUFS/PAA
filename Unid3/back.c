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
/*
void Desempilhar_Pt_Partida(Pilha_pontos* pilha_pts, FILE* output)
{   
    //elimina pontos de partidas que não tenham levem à saidas
    ponto_partida *sem_saida = pilha_pts->ultimo;
    pilha_pts->ultimo=pilha_pts->ultimo->proximo;
    fprintf(output,"BT@%d,%d<-%d,%d\n", pilha_pts->ultimo->linha,pilha_pts->ultimo->coluna, sem_saida->linha,sem_saida->coluna);
    free(sem_saida);
    sem_saida=NULL;
}
*/

int Desempilhar_Pt_Partida(Pilha_pontos* backtrack_pts, FILE* output) {  
    //elimina pontos de partidas que não tenham levem à saidas
    
    if (backtrack_pts->ultimo->proximo == NULL) return 1; 

    ponto_partida *sem_saida = backtrack_pts->ultimo;
    backtrack_pts->ultimo=backtrack_pts->ultimo->proximo;

    fprintf(output,"BT@%d,%d<-%d,%d\n", backtrack_pts->ultimo->linha,backtrack_pts->ultimo->coluna, sem_saida->linha,sem_saida->coluna);
        
    free(sem_saida);
    sem_saida=NULL;
    return 0;
    
}

ponto_partida* Testar_Direita(ponto** labirinto, int indice_linha,int indice_coluna, FILE* output){//testa vizinho da direita
    //lembre de passar &indice_linha como parametro para atualizar na referencia e noa só fazer uma cópia
    ponto_partida* retorno = NULL;

    if(labirinto[indice_linha][indice_coluna+1].espaco=='0')
    {   retorno = (ponto_partida*)malloc(sizeof(ponto_partida));
        retorno->coluna=indice_coluna+1;
        retorno->linha=indice_linha;
        fprintf(output,"D@%d,%d->%d,%d\n", indice_linha, indice_coluna, retorno->linha, retorno->coluna);
        labirinto[retorno->linha][retorno->coluna].ocupado=1;
        
        
    }
    return retorno;
}

ponto_partida* Testar_Cima(ponto** labirinto, int indice_linha,int indice_coluna, FILE* output){//testa vizinho da direita
    //lembre de passar &indice_linha como parametro para atualizar na referencia e noa só fazer uma cópia
    ponto_partida* retorno= NULL;
    if(labirinto[indice_linha-1][indice_coluna].espaco=='0')
    {   retorno = (ponto_partida*)malloc(sizeof(ponto_partida));
        retorno->coluna=indice_coluna;
        retorno->linha=indice_linha-1;
        fprintf(output,"F@%d,%d->%d,%d\n", indice_linha, indice_coluna, retorno->linha, retorno->coluna);
        labirinto[retorno->linha][retorno->coluna].ocupado=1;
        
    }
    return retorno;
}

ponto_partida* Testar_Esquerda(ponto** labirinto, int indice_linha,int indice_coluna, FILE* output){//testa vizinho da direita
    //lembre de passar &indice_linha como parametro para atualizar na referencia e noa só fazer uma cópia
    ponto_partida* retorno = NULL;
    if(labirinto[indice_linha][indice_coluna-1].espaco=='0' )
    {   retorno = (ponto_partida*)malloc(sizeof(ponto_partida));
        retorno->coluna=indice_coluna-1;
        retorno->linha=indice_linha;
        fprintf(output,"D@%d,%d->%d,%d\n", indice_linha, indice_coluna, retorno->linha, retorno->coluna);
        labirinto[retorno->linha][retorno->coluna].ocupado=1;
        
    }
    return retorno;
}

ponto_partida* Testar_Tras(ponto** labirinto, int indice_linha,int indice_coluna, FILE* output){//testa vizinho da direita
    //lembre de passar &indice_linha como parametro para atualizar na referencia e noa só fazer uma cópia
    ponto_partida* retorno = NULL; 
    if(labirinto[indice_linha+1][indice_coluna].espaco=='0')
    {   
        retorno = (ponto_partida*)malloc(sizeof(ponto_partida));
        retorno->coluna=indice_coluna;
        retorno->linha=indice_linha+1;
        fprintf(output,"T@%d,%d->%d,%d\n", indice_linha, indice_coluna, retorno->linha, retorno->coluna);
        labirinto[retorno->linha][retorno->coluna].ocupado=1;
        
    }
    return retorno;
}

/*
ponto_partida* Testar_Caminho(ponto** labirinto, int indice_linha, int indice_coluna, int linhas, int colunas, FILE *output) {
    ponto_partida* retorno = NULL; // Inicializa a variável de retorno

    // Testa os vizinhos na ordem: direita, cima, esquerda, tras
    if (indice_coluna < colunas - 1  && labirinto[indice_linha][indice_coluna+1].ocupado!=1) {
        retorno = Testar_Direita(labirinto, indice_linha, indice_coluna,output);
        if (retorno!=NULL) return retorno;
    }
    if (retorno==NULL && indice_linha > 0 && labirinto[indice_linha-1][indice_coluna].ocupado!=1) {
        retorno = Testar_Cima(labirinto, indice_linha, indice_coluna,output);
        if (retorno!=NULL) return retorno;
    }
    if (retorno==NULL && indice_coluna > 0 && labirinto[indice_linha][indice_coluna-1].ocupado!=1) {
        retorno = Testar_Esquerda(labirinto, indice_linha, indice_coluna,output);
        if (retorno!=NULL) return retorno;
    }
    if (retorno==NULL && indice_linha < linhas - 1  && labirinto[indice_linha+1][indice_coluna].ocupado!=1) {
        retorno = Testar_Tras(labirinto, indice_linha, indice_coluna,output);
    }

    return retorno;
}
*/




ponto_partida* Testar_Caminho(ponto** labirinto, Pilha_pontos* backtrack,  int linhas, int colunas, FILE *output) {
  ponto_partida* retorno = NULL; // Inicializa a variável de retorno
int indice_linha=backtrack->ultimo->linha;
 int indice_coluna=backtrack->ultimo->coluna;
  // Testa os vizinhos na ordem: direita, cima, esquerda, tras
  if (indice_coluna < colunas - 1 && labirinto[indice_linha][indice_coluna+1].ocupado!=1 && labirinto[indice_linha][indice_coluna+1].espaco!='1') {
    retorno = Testar_Direita(labirinto, indice_linha, indice_coluna,output);
    if (retorno!=NULL) return retorno;
  }

  if (retorno==NULL && indice_linha > 0 && labirinto[indice_linha-1][indice_coluna].ocupado!=1 && labirinto[indice_linha-1][indice_coluna].espaco!='1') {
    retorno = Testar_Cima(labirinto, indice_linha, indice_coluna,output);
    if (retorno!=NULL) return retorno;
  }

  if (retorno==NULL && indice_coluna > 0 && labirinto[indice_linha][indice_coluna-1].ocupado!=1 && labirinto[indice_linha][indice_coluna-1].espaco!='1') {
    retorno = Testar_Esquerda(labirinto, indice_linha, indice_coluna,output);
    if (retorno!=NULL) return retorno;
  }

  if (retorno==NULL && indice_linha < linhas - 1 && labirinto[indice_linha+1][indice_coluna].ocupado!=1 && labirinto[indice_linha+1][indice_coluna].espaco!='1') {
    retorno = Testar_Tras(labirinto, indice_linha, indice_coluna,output);
    if (retorno!=NULL) return retorno;
  }

  /* Se todas as opções forem inválidas (beco sem saída), realiza backtracking
  if (retorno == NULL) {
    
    if (backtrack->ultimo->proximo != NULL) {
      Desempilhar_Pt_Partida(backtrack,output); // Volta para o ponto anterior
    indice_linha = backtrack->ultimo->linha;
    indice_coluna = backtrack->ultimo->coluna;
    }
    
    // Retorna NULL para indicar que o backtracking foi realizado
    return NULL;
  }*/

  return retorno;
}



int main(int argc, char* argv[])//loop principal
{   ponto_partida* atual = (ponto_partida*)malloc(sizeof(ponto_partida));
    int num_labirintos,linhas_lab,colunas_lab;
    int linha_atual=0,coluna_atual=0;
    int end = 0;
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
        //recebe dimensoes do labirinto e aloca espaço
        fscanf(input,"%d %d",&colunas_lab,&linhas_lab);
        ponto **labirinto = (ponto**)malloc(linhas_lab*sizeof(ponto*));

        if (labirinto == NULL) 
            {
        // Tratar falha na alocação de memória
            fprintf(stderr, "Falha na alocação de memória para linha.\n");
            exit(EXIT_FAILURE);
             }

        //aloca espaço para colunas e atrubui valores às células
        //preenche a tabela
        for(int indice_linha=0;indice_linha<linhas_lab;indice_linha++)
        {
            labirinto[indice_linha] =(ponto*) malloc(colunas_lab*sizeof(ponto));

            if (labirinto[indice_linha] == NULL) 
                {
        // Tratar falha na alocação de memória
            fprintf(stderr, "Falha na alocação de memória para linha.\n");
            exit(EXIT_FAILURE);
                }
            for(int indice_coluna=0;indice_coluna<colunas_lab;indice_coluna++)
            
{           // Lê o próximo caractere do arquivo
                char espaco = fgetc(input);
                
                // Ignora os espaços em branco
                while (espaco == ' ' || espaco == '\n' || espaco == '\0') {
                    espaco = fgetc(input);
                }
                
                // Armazena o caractere na matriz
                labirinto[indice_linha][indice_coluna].espaco = espaco;
            if(labirinto[indice_linha][indice_coluna].espaco=='X')//checa se é o ponto de partida
            {
                Empilhar_Pt_Partida(&backtrack_labirinto,indice_linha,indice_coluna);
                fprintf(output,"INICIO@%d,%d\n",indice_linha,indice_coluna);

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
            atual = Testar_Caminho(labirinto,&backtrack_labirinto,linhas_lab,colunas_lab, output);
            if(atual!=NULL)
            {
                Empilhar_Pt_Partida(&backtrack_labirinto,atual->linha,atual->coluna);
            }
            else{
                
                end = Desempilhar_Pt_Partida(&backtrack_labirinto,output);
                if(end==1){fprintf(output,"SEM_SAIDA\n");break;} 
                    }
            
                
            coluna_atual = backtrack_labirinto.ultimo->coluna;
            linha_atual = backtrack_labirinto.ultimo->linha;

        }
        if(backtrack_labirinto.ultimo->proximo!=NULL) fprintf(output,"SAIDA@%d,%d\n",backtrack_labirinto.ultimo->linha,backtrack_labirinto.ultimo->coluna);
        for (int indice_linha = 0; indice_linha < linhas_lab; indice_linha++) {
            free(labirinto[indice_linha]);  // Libera a memória alocada para cada linha
            }
            free(labirinto);
        
        
        }
        return 0;
}