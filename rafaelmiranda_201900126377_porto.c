#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct dados_Container {
    int peso, indice_Lista, valor, ocupado;
    char id_Container[12];
    char cnpj[19];
    int prioridade, percentual_Excedido, diferenca;
} container;

// DJB2 hash
unsigned int Hashify(char* str, int tamanho) {
    unsigned long hash = 5381;
    for (int i = 0; i < 12; i++) {
        hash = ((hash << 5) + hash) + (unsigned char)str[i];
    }
    return hash % tamanho;
}

// Quadratic probing
int Trata_Colisao(int hash, int tam, container* cadastrados) {
    int i = 0, pos = hash;
    while (cadastrados[pos].ocupado != 0) {
        i++;
        pos = (hash + i * i) % tam;
    }
    return pos;
}

// Busca segura
int Busca(int hash, char valor_ent[12], int tam, container* cadastrados) {
    int i = 0, pos = hash;
    while (i < tam) {
        if (cadastrados[pos].ocupado != 0 &&
            strncmp(cadastrados[pos].id_Container, valor_ent, 12) == 0)
            return pos;
        i++;
        pos = (hash + i * i) % tam;
    }
    return -1;
}

int cnpj_Confere(char cnpj_Container[19], container* container_Triagem, int indice_Elemento) {
    return (strcmp(container_Triagem[indice_Elemento].cnpj, cnpj_Container) == 0) ? 0 : 2;
}

void Calcula_excedente(container* cadastrado, container* triagem) {
    int diferenca_peso = triagem->peso - cadastrado->peso;
    triagem->diferenca = diferenca_peso;
    float excedente = ((float)diferenca_peso / cadastrado->peso) * 100.0;
    int arredondado = (int) round(excedente);
    if (arredondado >= 0)
        triagem->percentual_Excedido = arredondado;
    if (triagem->percentual_Excedido > 10 && triagem->prioridade != 2)
        triagem->prioridade = 1;
}

void Trocar(container* saida, container* entrada) {
    saida->indice_Lista = entrada->indice_Lista;
    saida->peso = entrada->peso;
    saida->percentual_Excedido = entrada->percentual_Excedido;
    saida->prioridade = entrada->prioridade;
    saida->diferenca = entrada->diferenca;
    saida->valor = entrada->valor;

    // cópias seguras de strings
    snprintf(saida->id_Container, sizeof(saida->id_Container), "%s", entrada->id_Container);
    snprintf(saida->cnpj, sizeof(saida->cnpj), "%s", entrada->cnpj);
}

void Intercalar(container* saida, container* entrada, int32_t lim_Inferior, int32_t pivo, int32_t lim_Superior) {
    int32_t i = lim_Inferior, j = pivo + 1, k = lim_Inferior;

    while (i <= pivo && j <= lim_Superior) {
        container *left = &entrada[i];
        container *right = &entrada[j];
        int cond = 0;

        if (left->prioridade == 2 && right->prioridade != 2) cond = 1;
        else if (left->prioridade != 2 && right->prioridade == 2) cond = 0;
        else if (left->prioridade == 2 && right->prioridade == 2)
            cond = (left->indice_Lista <= right->indice_Lista);
        else if (left->prioridade == 1 && right->prioridade == 1)
            cond = (left->percentual_Excedido >= right->percentual_Excedido);
        else cond = 1;

        if (cond) Trocar(&saida[k++], left), i++;
        else Trocar(&saida[k++], right), j++;
    }

    while (i <= pivo) Trocar(&saida[k++], &entrada[i++]);
    while (j <= lim_Superior) Trocar(&saida[k++], &entrada[j++]);

    memcpy(&entrada[lim_Inferior], &saida[lim_Inferior],
           sizeof(container) * (lim_Superior - lim_Inferior + 1));
}

// mergesort recursivo
void mergesort(container* saida, container* entrada, int32_t lim_Inferior, int32_t lim_Superior) {
    if (lim_Inferior < lim_Superior) {
        int32_t pivo = lim_Inferior + ((lim_Superior - lim_Inferior) / 2);
        mergesort(saida, entrada, lim_Inferior, pivo);
        mergesort(saida, entrada, pivo + 1, lim_Superior);
        Intercalar(saida, entrada, lim_Inferior, pivo, lim_Superior);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) { printf("Uso: %s <input> <output>\n", argv[0]); return 1; }

    FILE* input = fopen(argv[1], "r");
    if (!input) { printf("ERRO!a\n"); return 0; }
    FILE* output = fopen(argv[2], "w");
    if (!output) { printf("ERRO!b\n"); fclose(input); return 0; }

    int tam_Array_cadastro = 0, tam_Array_triagem = 0;
    fscanf(input, "%d\n", &tam_Array_cadastro);

    int tam_pot2 = 2;
    while (tam_pot2 < tam_Array_cadastro) tam_pot2 *= 3;

    container* lista_Cadastro = (container*)calloc(tam_pot2, sizeof(container));
    if (!lista_Cadastro) { printf("ERRO!c\n"); return 1; }

    // leitura do cadastro
    for (int i = 0; i < tam_Array_cadastro; i++) {
        char id[12], cnpj[19];
        int peso;
        fscanf(input, "%s %s %d\n", id, cnpj, &peso);
        int chave_Hash = Hashify(id, tam_pot2);
        if (lista_Cadastro[chave_Hash].ocupado != 0)
            chave_Hash = Trata_Colisao(chave_Hash, tam_pot2, lista_Cadastro);

        snprintf(lista_Cadastro[chave_Hash].id_Container,
                 sizeof(lista_Cadastro[chave_Hash].id_Container), "%s", id);
        snprintf(lista_Cadastro[chave_Hash].cnpj,
                 sizeof(lista_Cadastro[chave_Hash].cnpj), "%s", cnpj);
        lista_Cadastro[chave_Hash].peso = peso;
        lista_Cadastro[chave_Hash].valor = chave_Hash;
        lista_Cadastro[chave_Hash].ocupado = 1;
        lista_Cadastro[chave_Hash].indice_Lista = i;
    }

    fscanf(input, "%d\n", &tam_Array_triagem);
    container* lista_Triagem = (container*)malloc(sizeof(container) * tam_Array_triagem);
    container* lista_Saida = (container*)malloc(sizeof(container) * tam_Array_triagem);
    if (!lista_Triagem || !lista_Saida) { printf("ERRO!d\n"); return 1; }

    // leitura da triagem
    for (int i = 0; i < tam_Array_triagem; i++) {
        char id[12], cnpj[19];
        int peso;
        fscanf(input, "%s %s %d\n", id, cnpj, &peso);

        snprintf(lista_Triagem[i].id_Container, sizeof(lista_Triagem[i].id_Container), "%s", id);
        snprintf(lista_Triagem[i].cnpj, sizeof(lista_Triagem[i].cnpj), "%s", cnpj);
        lista_Triagem[i].peso = peso;

        int chave_hash = Hashify(id, tam_pot2);
        int index_cad = Busca(chave_hash, id, tam_pot2, lista_Cadastro);

        if (index_cad != -1) { // existe no cadastro
            lista_Triagem[i].valor = index_cad;
            lista_Triagem[i].indice_Lista = lista_Cadastro[index_cad].indice_Lista;
            lista_Triagem[i].prioridade = cnpj_Confere(cnpj, lista_Cadastro, index_cad);
            lista_Triagem[i].ocupado = 1;
            Calcula_excedente(&lista_Cadastro[index_cad], &lista_Triagem[i]);
        } else { // não existe
            lista_Triagem[i].valor = -1;
            lista_Triagem[i].indice_Lista = -1;
            lista_Triagem[i].prioridade = 0;
            lista_Triagem[i].ocupado = 1;
            lista_Triagem[i].diferenca = 0;
            lista_Triagem[i].percentual_Excedido = 0;
        }
    }

    // ordenação
    mergesort(lista_Saida, lista_Triagem, 0, tam_Array_triagem - 1);

    // impressão somente divergências
    for (int i = 0; i < tam_Array_triagem; i++) {
        if (lista_Triagem[i].valor != -1) {
            if (lista_Triagem[i].prioridade == 2) {
                fprintf(output, "%s: %s<->%s\n",
                        lista_Triagem[i].id_Container,
                        lista_Cadastro[lista_Triagem[i].valor].cnpj,
                        lista_Triagem[i].cnpj);
            } else if (lista_Triagem[i].prioridade == 1) {
                fprintf(output, "%s: %dkg (%d%%)\n",
                        lista_Triagem[i].id_Container,
                        lista_Triagem[i].diferenca,
                        lista_Triagem[i].percentual_Excedido);
            }
        }
    }

    fclose(input);
    fclose(output);
    free(lista_Cadastro);
    free(lista_Triagem);
    free(lista_Saida);
    return 0;
}
