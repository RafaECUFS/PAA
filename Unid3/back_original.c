#include <stdio.h>
#include <stdlib.h>
int  linhas_lab, colunas_lab;
typedef struct coordenadas {
    int linha, coluna;
    struct coordenadas* proximo;
} ponto_partida;

typedef struct {
    ponto_partida *ultimo;
} Pilha_pontos;

void cria_pilha(Pilha_pontos* pilha) {
    pilha->ultimo = NULL;
}

void Empilhar_Pt_Partida(Pilha_pontos *pilha_pontos, int linha_input, int coluna_input) {
    ponto_partida *partida = malloc(sizeof(ponto_partida));
    partida->coluna = coluna_input;
    partida->linha = linha_input;
    partida->proximo = pilha_pontos->ultimo;
    pilha_pontos->ultimo = partida;
}

void Desempilhar_Pt_Partida(Pilha_pontos *pilha_pts, FILE* output) {
    ponto_partida *sem_saida = pilha_pts->ultimo;
    pilha_pts->ultimo = pilha_pts->ultimo->proximo;
    fprintf(output, "BT@%d,%d<-%d,%d\n", pilha_pts->ultimo->linha, pilha_pts->ultimo->coluna, sem_saida->linha, sem_saida->coluna);
    free(sem_saida);
}

ponto_partida* Testar_Vizinho(char** labirinto, int indice_linha, int indice_coluna, int desloc_linha, int desloc_coluna, FILE* output) {
    ponto_partida* retorno = NULL;
    int nova_linha = indice_linha + desloc_linha;
    int nova_coluna = indice_coluna + desloc_coluna;
    
    // Verifica se a nova posição está dentro dos limites do labirinto
    if (nova_linha > 0 && nova_linha < linhas_lab && nova_coluna > 0 && nova_coluna < colunas_lab) {
        if (labirinto[nova_linha][nova_coluna] == '0') {
            labirinto[nova_linha][nova_coluna] = '1';
            retorno = (ponto_partida*)malloc(sizeof(ponto_partida));
            retorno->coluna = nova_coluna;
            retorno->linha = nova_linha;
            fprintf(output, "%c@%d,%d->%d,%d\n", (desloc_coluna == 1 ? 'D' : (desloc_coluna == -1 ? 'E' : (desloc_linha == -1 ? 'F' : 'T'))), indice_linha, indice_coluna, retorno->linha, retorno->coluna);
        }
    }
    labirinto[indice_linha][indice_coluna] = '1';
    return retorno;
}


ponto_partida* Testar_Caminho(char** labirinto, Pilha_pontos* backtrack, FILE *output) {
    int movimentos[4][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
    ponto_partida* retorno = NULL;
    int indice_linha = backtrack->ultimo->linha; 
    int indice_coluna = backtrack->ultimo->coluna;

    for (int i = 0; i < 4; i++) {
        retorno = Testar_Vizinho(labirinto, indice_linha, indice_coluna, movimentos[i][0], movimentos[i][1], output);
        if (retorno != NULL) {
            //labirinto[retorno->linha][retorno->coluna] = '1'; // Marca o ponto como "ocupado"
            Empilhar_Pt_Partida(backtrack, retorno->linha, retorno->coluna);
            return retorno;
        }
        
    }
    
    return retorno;
}

int main(int argc, char* argv[]) {
    ponto_partida* atual = NULL;
    int num_labirintos;

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

    fscanf(input, "%d", &num_labirintos);

    for (int indice_labirintos = 0; indice_labirintos < num_labirintos; indice_labirintos++) {
        fprintf(output, "L%d:\n", indice_labirintos);
        Pilha_pontos backtrack_labirinto;
        cria_pilha(&backtrack_labirinto);
        
        fscanf(input, "%d %d", &colunas_lab, &linhas_lab);
        char **labirinto = (char**)malloc(linhas_lab * sizeof(char*));

        if (labirinto == NULL) {
            fprintf(stderr, "Falha na alocação de memória para linha.\n");
            exit(EXIT_FAILURE);
        }

        for (int indice_linha = 0; indice_linha < linhas_lab; indice_linha++) {
            labirinto[indice_linha] = (char*)malloc(colunas_lab * sizeof(char));

            if (labirinto[indice_linha] == NULL) {
                fprintf(stderr, "Falha na alocação de memória para linha.\n");
                exit(EXIT_FAILURE);
            }

            for (int indice_coluna = 0; indice_coluna < colunas_lab; indice_coluna++) {
                char espaco = fgetc(input);
                while (espaco == ' ' || espaco == '\n' || espaco == '\0') {
                    espaco = fgetc(input);
                }
                labirinto[indice_linha][indice_coluna] = espaco;
                if (labirinto[indice_linha][indice_coluna] == 'X') {
                    Empilhar_Pt_Partida(&backtrack_labirinto, indice_linha, indice_coluna);
                    fprintf(output, "INICIO@%d,%d\n", indice_linha, indice_coluna);
                }
            }    
        }

        while (backtrack_labirinto.ultimo != NULL) {
            atual = Testar_Caminho(labirinto, &backtrack_labirinto,output);
            if (atual != NULL) {
                if (atual->linha == 0 || atual->linha == linhas_lab - 1 || atual->coluna == 0 || atual->coluna == colunas_lab - 1) {
                    fprintf(output, "SAIDA@%d,%d\n", atual->linha, atual->coluna);
                    break;
                } else {
                    Empilhar_Pt_Partida(&backtrack_labirinto, atual->linha, atual->coluna);
                }
            } else {
                Desempilhar_Pt_Partida(&backtrack_labirinto, output);
            }
        }
        if (backtrack_labirinto.ultimo == NULL) {
            fprintf(output, "SEM_SAIDA\n");
        }
        
    }

    fclose(input);
    fclose(output);

    return 0;
}
