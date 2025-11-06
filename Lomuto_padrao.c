//WIP: Lomuto QS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
typedef struct array_sort{ //lista contendo os tipos de sort e a quanidade de trocas
    char tipo_sort[3];
    int32_t total_trocas;
} tipo;

void Trocar(int32_t* a, int32_t* b) {
    int32_t temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void Trocar_tipo(tipo* array, size_t indiceA, size_t indiceB) {
    // Aloca memória temporária para armazenar o conteúdo de 'array[indiceA]'
    tipo temp;
    temp = array[indiceA];

    // Copia o conteúdo de 'array[indiceB]' para 'array[indiceA]'
    array[indiceA] = array[indiceB];

    // Copia o conteúdo temporário para 'array[indiceB]'
    array[indiceB] = temp;
}

int32_t Lom_STD(int32_t* array, int32_t ind_Menor, int32_t ind_Maior, int32_t *num_Trocas) {
    int32_t pivo = array[ind_Maior], ind_Half1 = ind_Menor - 1, ind_Half2 = ind_Menor;

    for(; ind_Half2 < ind_Maior; ind_Half2++) {
        if(array[ind_Half2] <= pivo) {
            Trocar(&array[++ind_Half1], &array[ind_Half2]);
            (*num_Trocas)++; // <-- note o asterisco!
        }
    }

    Trocar(&array[++ind_Half1], &array[ind_Maior]);
    (*num_Trocas)++;

    return ind_Half1;
}
void quicksortLomuto(int32_t* array, int32_t ind_Menor, int32_t ind_Maior, int32_t* num_Trocas) {
    if (ind_Menor < ind_Maior) {
        int32_t partitionIndex = Lom_STD(array, ind_Menor, ind_Maior, num_Trocas);

        quicksortLomuto(array, ind_Menor, partitionIndex, num_Trocas);
        quicksortLomuto(array, partitionIndex + 1, ind_Maior, num_Trocas);
    }
}

void processarVetor(int32_t* vetor, int32_t tamanho, FILE* output, int32_t linha_impressa) {
    tipo* lista_variantes = (tipo*)calloc(6,sizeof(tipo));
    // Cópias dinamicamente alocadas do vetor
    int32_t* copia1 = malloc(sizeof(int32_t) * tamanho);
    /*int32_t* copia2 = malloc(sizeof(int32_t) * tamanho);
    int32_t* copia3 = malloc(sizeof(int32_t) * tamanho);
    int32_t* copia4 = malloc(sizeof(int32_t) * tamanho);
    int32_t* copia5 = malloc(sizeof(int32_t) * tamanho);
    int32_t* copia6 = malloc(sizeof(int32_t) * tamanho);*/

    if (copia1 == NULL)// || copia2 == NULL || copia3 == NULL || copia4 == NULL || copia5 == NULL || copia6 == NULL) {
    {   // Lidar com erro de alocação de memória, se necessário
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(EXIT_FAILURE);
    }

    // Copiar conteúdo do vetor original para as cópias
    memcpy(copia1, vetor, sizeof(int32_t) * tamanho);
    /*memcpy(copia2, vetor, sizeof(int32_t) * tamanho);
    memcpy(copia3, vetor, sizeof(int32_t) * tamanho);
    memcpy(copia4, vetor, sizeof(int32_t) * tamanho);
    memcpy(copia5, vetor, sizeof(int32_t) * tamanho);
    memcpy(copia6, vetor, sizeof(int32_t) * tamanho);*/

    // Variáveis para armazenar o número de trocas de cada versão do Quicksort
    int32_t trocas1 = 0; //, trocas2 = 0, trocas3 = 0, trocas4 = 0, trocas5=0, trocas6=0;

    // Chamar cada função explicitamente
    quicksortLomuto(copia1, 0, tamanho - 1, &trocas1);
    strcpy(lista_variantes[0].tipo_sort,"LP");
    lista_variantes[0].total_trocas = trocas1;
    free(copia1);
    // lomuto padrão

    /*quicksortLomutoMED3(copia2, 0, tamanho - 1, &trocas2);
    strcpy(lista_variantes[1].tipo_sort,"LM");
    lista_variantes[1].total_trocas = trocas2;
    free(copia2);
    // lomuto mediana

    quicksortLomutoRDM(copia3, 0, tamanho - 1, &trocas3);
    strcpy(lista_variantes[2].tipo_sort,"LA");
    lista_variantes[2].total_trocas = trocas3;
    free(copia3);
    // lomuto aleatório

    quicksortHoare(copia4, 0, tamanho - 1, &trocas4);
    strcpy(lista_variantes[3].tipo_sort,"HP");
    lista_variantes[3].total_trocas = trocas4;
    free(copia4);
    // hoare padrao
    quicksortHoareMED3(copia5, 0, tamanho - 1, &trocas5);
    strcpy(lista_variantes[4].tipo_sort,"HM");
    lista_variantes[4].total_trocas = trocas5;
    free(copia5);
    // hoare mediana
    quicksortHoare(copia6, 0, tamanho - 1, &trocas6);
    strcpy(lista_variantes[5].tipo_sort,"HL");
    lista_variantes[5].total_trocas = trocas6;
    free(copia6);
    // hoare aleatorio*/

    
    //quicksortHoaretipo(lista_variantes,0,5);
    //fprintf(output,"%d: N(%d)", linha_impressa++, tamanho);
    //int index;
    /*for(index=0; index<tamanho-1;index++)
        fprintf(output," %s(%d)", lista_variantes[index].tipo_sort, lista_variantes[index].total_trocas);
    */fprintf(output," %s(%d)\n", lista_variantes[0].tipo_sort, lista_variantes[0].total_trocas);

    // Liberar memória alocada dinamicamente

}

void converte_string_int(char* string, int32_t array_nums[]) {
    int32_t index_array_num = 0;
    char num_preconvert[16];  // buffer temporário para cada número
    int i = 0, j = 0;

    while (string[i] != '\0') {
        if (string[i] != ' ' && string[i] != '\n') {
            // adiciona caractere ao buffer
            num_preconvert[j++] = string[i];
        } else if (j > 0) {
            // finaliza número e converte
            num_preconvert[j] = '\0';
            array_nums[index_array_num++] = atoi(num_preconvert);
            j = 0; // reinicia o buffer
        }
        i++;
    }

    // último número (caso a string não termine com espaço)
    if (j > 0) {
        num_preconvert[j] = '\0';
        array_nums[index_array_num++] = atoi(num_preconvert);
    }
}

//Incompleto por agora
int main(int argc, char* argv[]) {
    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Erro ao abrir o arquivo de entrada!\n");
        return 0;
    }

    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        printf("Erro ao abrir o arquivo de saída!\n");
        fclose(input);
        return 0;
    }
    uint32_t num_linhas,tam_array,indice_array_num,num_trocas=0;
    int32_t* numeros = (int32_t*)calloc(tam_array,sizeof(int32_t));
    fscanf(input,"%d\n",&num_linhas);
