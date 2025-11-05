#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
typedef struct array_sort{ //lista contendo os tipos de sort e a quanidade de trcas
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

int32_t Lom_STD(int32_t* array, int32_t ind_Menor, int32_t ind_Maior, int32_t num_Trocas){
    int32_t pivo = array[ind_Maior], ind_Half1 = ind_Menor-1, ind_Half2 = ind_Menor;

    for(;ind_Half2<ind_Maior;ind_Half2++) 
        if(array[ind_Half2]<=pivo) {
            Trocar(&array[++ind_Half1],&array[++ind_Half2]);
            num_Trocas++;
            }
    Trocar(&array[++ind_Half1],&array[ind_Maior]);
    num_Trocas++;
    return ind_Half1;
}
int32_t Lom_MED3(int32_t* array, int32_t ind_Menor, int32_t ind_Maior,int32_t num_Trocas){
    int32_t num1=(ind_Maior+1
    )/4,num2=(ind_Maior+1)/2,num3=(ind_Maior+1)*(3/4);
    int32_t mediana;
    if ((array[num1] >= array[num2] && array[num1] <= array[num3]) || (array[num1] >= array[num3] && array[num1] <= array[num2]))
        mediana = num1;
    else if ((array[num2] >= array[num1] && array[num2] <= array[num3]) || (array[num2] >= array[num3] && array[num2] <= array[num1]))
        mediana = num2;
    else
        mediana = num3;
    Trocar(&array[ind_Maior],&array[mediana]);
    return Lom_STD(array,ind_Menor,ind_Maior,num_Trocas);
}
int32_t Lom_RDM(int32_t* array, int32_t ind_Menor, int32_t ind_Maior,int32_t num_Trocas){
    int32_t random_index= ind_Menor + (array[ind_Menor]%ind_Maior);
    Trocar(&array[ind_Maior],&array[random_index]);
    return Lom_STD(array,ind_Menor,ind_Maior,num_Trocas);
}
int32_t Hoa_STD(int32_t* array, int32_t ind_Menor, int32_t ind_Maior,int32_t num_Trocas){
    int32_t pivo = array[ind_Menor], lim_inf = ind_Menor-1, lim_sup = ind_Maior+1;
    while(1){
        while(array[--lim_sup]>pivo);
        while(array[++lim_inf]<pivo);
        if(lim_inf<lim_sup){
            Trocar(&array[lim_inf],&array[lim_sup]);
            num_Trocas++;
        } 
        else return lim_sup;
    }
}
int32_t Hoa_MED3(int32_t* array, int32_t ind_Menor, int32_t ind_Maior,int32_t num_Trocas){
    int32_t num1=(ind_Maior+1
    )/4,num2=(ind_Maior+1)/2,num3=(ind_Maior+1)*(3/4);
    int32_t mediana;
    if ((array[num1] >= array[num2] && array[num1] <= array[num3]) || (array[num1] >= array[num3] && array[num1] <= array[num2]))
        mediana = num1;
    else if ((array[num2] >= array[num1] && array[num2] <= array[num3]) || (array[num2] >= array[num3] && array[num2] <= array[num1]))
        mediana = num2;
    else
        mediana = num3;
    Trocar(&array[ind_Menor],&array[mediana]);
    return Hoa_STD(array, ind_Menor, ind_Maior,num_Trocas);
}
int32_t Hoa_RDM(int32_t* array, int32_t ind_Menor, int32_t ind_Maior,int32_t num_Trocas){
    int32_t random_index= ind_Menor + (array[ind_Menor]%ind_Maior);
    Trocar(&array[ind_Menor],&array[random_index]);
    return Hoa_STD(array, ind_Menor, ind_Maior,num_Trocas);
}
void quicksortLomuto(int32_t* array, int32_t ind_Menor, int32_t ind_Maior, int32_t* num_Trocas) {
    if (ind_Menor < ind_Maior) {
        int32_t partitionIndex = Lom_STD(array, ind_Menor, ind_Maior, num_Trocas);

        quicksortLomuto(array, ind_Menor, partitionIndex, num_Trocas);
        quicksortLomuto(array, partitionIndex + 1, ind_Maior, num_Trocas);
    }
}
void quicksortLomutoMED3(int32_t* array, int32_t ind_Menor, int32_t ind_Maior, int32_t* num_Trocas) {
    if (ind_Menor < ind_Maior) {
        int32_t partitionIndex = Lom_MED3(array, ind_Menor, ind_Maior, num_Trocas);

        quicksortLomutoMED3(array, ind_Menor, partitionIndex, num_Trocas);
        quicksortLomutoMED3(array, partitionIndex + 1, ind_Maior, num_Trocas);
    }
}
void quicksortLomutoRDM(int32_t* array, int32_t ind_Menor, int32_t ind_Maior, int32_t* num_Trocas) {
    if (ind_Menor < ind_Maior) {
        int32_t partitionIndex = Lom_RDM(array, ind_Menor, ind_Maior, num_Trocas);

        quicksortLomutoRDM(array, ind_Menor, partitionIndex, num_Trocas);
        quicksortLomutoRDM(array, partitionIndex + 1, ind_Maior, num_Trocas);
    }
}
void quicksortHoare(tipo* array, int32_t ind_Menor, int32_t ind_Maior, int32_t* num_Trocas) {
    if (ind_Menor < ind_Maior) {
        int32_t partitionIndex = Hoa_STD(array, ind_Menor, ind_Maior, num_Trocas);

        quicksortHoare(array, ind_Menor, partitionIndex, num_Trocas);
        quicksortHoare(array, partitionIndex + 1, ind_Maior, num_Trocas);
    }
}
void quicksortHoareMED3(tipo* array, int32_t ind_Menor, int32_t ind_Maior, int32_t* num_Trocas) {
    if (ind_Menor < ind_Maior) {
        int32_t partitionIndex = Hoa_STD(array, ind_Menor, ind_Maior, num_Trocas);

        quicksortHoareMED3(array, ind_Menor, partitionIndex, num_Trocas);
        quicksortHoareMED3(array, partitionIndex + 1, ind_Maior, num_Trocas);
    }
}
void quicksortHoareRDM(tipo* array, int32_t ind_Menor, int32_t ind_Maior, int32_t* num_Trocas) {
    if (ind_Menor < ind_Maior) {
        int32_t partitionIndex = Hoa_STD(array, ind_Menor, ind_Maior, num_Trocas);

        quicksortHoareRDM(array, ind_Menor, partitionIndex, num_Trocas);
        quicksortHoareRDM(array, partitionIndex + 1, ind_Maior, num_Trocas);
    }
}
size_t Hoa_STD_tipo(tipo* array, int32_t ind_Menor, int32_t ind_Maior){
    int32_t pivo = array[ind_Menor].total_trocas;
    size_t lim_inf = ind_Menor-1, lim_sup = ind_Maior+1;
    while(1){
        while(array[--lim_sup].total_trocas>pivo);
        while(array[++lim_inf].total_trocas<pivo);
        if(lim_inf<lim_sup){
            Trocar_tipo(array,lim_inf,lim_sup);
        } 
        else return lim_sup;
    }
}
void quicksortHoaretipo(tipo* array, size_t ind_Menor, size_t ind_Maior) {
    if (ind_Menor < ind_Maior) {
        size_t partitionIndex = Hoa_STD_tipo(array, ind_Menor, ind_Maior);

        quicksortHoaretipo(array, ind_Menor, partitionIndex);
        quicksortHoaretipo(array, partitionIndex + 1, ind_Maior);
    }
}

void processarVetor(int32_t* vetor, int32_t tamanho, FILE* output, int32_t linha_impressa) {
    tipo* lista_variantes = (tipo*)calloc(6,sizeof(tipo));
    // Cópias dinamicamente alocadas do vetor
    int32_t* copia1 = malloc(sizeof(int32_t) * tamanho);
    int32_t* copia2 = malloc(sizeof(int32_t) * tamanho);
    int32_t* copia3 = malloc(sizeof(int32_t) * tamanho);
    int32_t* copia4 = malloc(sizeof(int32_t) * tamanho);
    int32_t* copia5 = malloc(sizeof(int32_t) * tamanho);
    int32_t* copia6 = malloc(sizeof(int32_t) * tamanho);

    if (copia1 == NULL || copia2 == NULL || copia3 == NULL || copia4 == NULL || copia5 == NULL || copia6 == NULL) {
        // Lidar com erro de alocação de memória, se necessário
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(EXIT_FAILURE);
    }

    // Copiar conteúdo do vetor original para as cópias
    memcpy(copia1, vetor, sizeof(int32_t) * tamanho);
    memcpy(copia2, vetor, sizeof(int32_t) * tamanho);
    memcpy(copia3, vetor, sizeof(int32_t) * tamanho);
    memcpy(copia4, vetor, sizeof(int32_t) * tamanho);
    memcpy(copia5, vetor, sizeof(int32_t) * tamanho);
    memcpy(copia6, vetor, sizeof(int32_t) * tamanho);

    // Variáveis para armazenar o número de trocas de cada versão do Quicksort
    int32_t trocas1 = 0, trocas2 = 0, trocas3 = 0, trocas4 = 0, trocas5=0, trocas6=0;

    // Chamar cada função explicitamente
    quicksortLomuto(copia1, 0, tamanho - 1, &trocas1);
    strcpy(lista_variantes[0].tipo_sort,"LP");
    lista_variantes[0].total_trocas = trocas1;
    free(copia1);
    // lomuto padrão

    quicksortLomutoMED3(copia2, 0, tamanho - 1, &trocas2);
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
    // hoare aleatorio

    
    quicksortHoaretipo(lista_variantes,0,5);
    fprintf(output,"%d: N(%d)", linha_impressa++, tamanho);
    int index;
    for(index=0; index<tamanho-1;index++)
        fprintf(output," %s(%d)", lista_variantes[index].tipo_sort, lista_variantes[index].total_trocas);
    fprintf(output," %s(%d)\n", lista_variantes[index].tipo_sort, lista_variantes[index].total_trocas);

    // Liberar memória alocada dinamicamente

}
