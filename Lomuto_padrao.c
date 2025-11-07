// === QuickSort Lomuto (versão revisada e estável) ===

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura para armazenar o tipo de sort e total de trocas
typedef struct array_sort {
    char tipo_sort[3];
    int32_t total_trocas;
} tipo;

// ---- Funções utilitárias ----

void Trocar(int32_t* a, int32_t* b, int32_t* num_Trocas) {
    int32_t temp = *a;
    *a = *b;
    *b = temp;
    (*num_Trocas)++;
}

int32_t Lom_STD(int32_t* array, int32_t ind_Menor, int32_t ind_Maior, int32_t *num_Trocas) {
    (*num_Trocas)++; // conta a chamada de partição

    int32_t pivo = array[ind_Maior];
    int32_t i = ind_Menor - 1;

    for (int32_t j = ind_Menor; j < ind_Maior; j++) {
        if (array[j] <= pivo) {
            i++;
            Trocar(&array[i], &array[j], num_Trocas);
        }
    }

    Trocar(&array[i + 1], &array[ind_Maior], num_Trocas);
    
    return i + 1;
}


void quicksortLomuto(int32_t* array, int32_t ind_Menor, int32_t ind_Maior, int32_t* num_Trocas) {
    
    if (ind_Menor < ind_Maior)
    {
        (*num_Trocas)++;
        int32_t p = Lom_STD(array, ind_Menor, ind_Maior, num_Trocas);
        quicksortLomuto(array, ind_Menor, p - 1, num_Trocas);
        quicksortLomuto(array, p + 1, ind_Maior, num_Trocas);
    }
}


// ---- Conversão de string em vetor ----
void converte_string_int(const char *string, int32_t *array_nums, int32_t tam_array) {
    int32_t index_array_num = 0;
    char num_preconvert[12];
    int j = 0;

    for (int i = 0; string[i] != '\0'; i++) {
        char c = string[i];
        if ((c >= '0' && c <= '9') || c == '-' || c == '+') {
            if (j < (int)(sizeof(num_preconvert) - 1))
                num_preconvert[j++] = c;
        } else if (isspace((unsigned char)c)) {
            if (j > 0) {
                num_preconvert[j] = '\0';
                if (index_array_num < tam_array)
                    array_nums[index_array_num++] = atoi(num_preconvert);
                j = 0;
            }
        }
    }

    if (j > 0 && index_array_num < tam_array) {
        num_preconvert[j] = '\0';
        array_nums[index_array_num++] = atoi(num_preconvert);
    }
}

// ---- Processamento do vetor ----
void processarVetor(int32_t* vetor, int32_t tamanho, FILE* output) {
    tipo variante;
    int32_t* copia = malloc(sizeof(int32_t) * tamanho);

    if (!copia) {
        perror("Erro ao alocar memória para copia");
        exit(EXIT_FAILURE);
    }

    memcpy(copia, vetor, sizeof(int32_t) * tamanho);

    int32_t trocas = 1;
    quicksortLomuto(copia, 0, tamanho - 1, &trocas);

    strcpy(variante.tipo_sort, "LP");
    variante.total_trocas = trocas;

    fprintf(output, " %s(%d)\n", variante.tipo_sort, variante.total_trocas);

    free(copia);
}

// ---- Função principal ----
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s <entrada> <saida>\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (!input) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    FILE* output = fopen(argv[2], "w");
    if (!output) {
        perror("Erro ao abrir arquivo de saída");
        fclose(input);
        return 1;
    }

    uint32_t num_linhas;
    char buffer[256];

    if (!fgets(buffer, sizeof(buffer), input)) {
        fprintf(stderr, "Erro ao ler número de linhas.\n");
        return 1;
    }

    num_linhas = atoi(buffer);

    for (uint32_t linhaAtual = 0; linhaAtual < num_linhas; linhaAtual++) {
        if (!fgets(buffer, sizeof(buffer), input)) break;
        int32_t tam_array = atoi(buffer);

        int32_t* numeros = calloc(tam_array, sizeof(int32_t));
        if (!numeros) {
            perror("Erro ao alocar vetor numeros");
            exit(EXIT_FAILURE);
        }

        if (!fgets(buffer, sizeof(buffer), input)) break;
        buffer[strcspn(buffer, "\r\n")] = '\0';

        converte_string_int(buffer, numeros, tam_array);

        // Debug opcional: verificar leitura correta
        // printf("Linha %d (tam=%d): ", linhaAtual + 1, tam_array);
        // for (int i = 0; i < tam_array; i++) printf("%d ", numeros[i]);
        // printf("\n");

        processarVetor(numeros, tam_array, output);
        free(numeros);
    }

    fclose(input);
    fclose(output);
    return 0;
}
