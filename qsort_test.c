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
void quicksortHoare(int32_t* array, int32_t ind_Menor, int32_t ind_Maior, int32_t num_Trocas) {
    if (ind_Menor < ind_Maior) {
        int32_t partitionIndex = Hoa_STD(array, ind_Menor, ind_Maior, num_Trocas);

        quicksortHoare(array, ind_Menor, partitionIndex, num_Trocas);
        quicksortHoare(array, partitionIndex + 1, ind_Maior, num_Trocas);
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
/*void converte_string_int(char* string, int32_t array_nums[]){
    int32_t index_array_num=0;
    char* num_preconvert = (char*)malloc(sizeof(char)*10);
    for(int tamanhostring=0;tamanhostring<100;tamanhostring++){
        if(strcmp(string[tamanhostring]," ")!=0){
            strcat(num_preconvert,string[tamanhostring]);
        }
        else{
            array_nums[index_array_num++]= atoi(num_preconvert);
        }
    }
}*/

//prioridade 1: fazer o quicksort funcionar para os inteiros(WIP)
//prioridade 2: testar o sort na lista de resultados de variantes do qsort(TBD)
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
    for(int linha=0;linha<num_linhas;linha++){
        indice_array_num=0;
        fscanf(input,"%d\n",&tam_array);
        fprintf(output,"%d: N(%d)", linha, tam_array);
        for(;tam_array>0;tam_array--){
            fscanf(input,"%d",&numeros[indice_array_num++]);
        }
        fgetc(input);
        quicksortHoare(numeros,0,tam_array,num_trocas);
        fprintf(output," HP(%d)\n", num_trocas);
    }
    
   
}