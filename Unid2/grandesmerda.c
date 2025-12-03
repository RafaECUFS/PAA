#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct transporte{
    double peso_lim,volume_lim;
    char placa[7];
} frota;
typedef struct carga{
    double peso,volume,valor;
    char codigo[13];
}pacote;
typedef struct resultado{
    double peso_usado, valor_max,vol_max;
}celula_matriz;
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
    int num_veiculo, num_pacote;
    fscanf(input,"%d",&num_veiculo);
    frota* disponiveis = (frota*)malloc(num_veiculo*(sizeof(frota)));
    for(int quantidade=0; quantidade<num_veiculo; quantidade++){
        fscanf(input, "%s %.2f %.2f",disponiveis[quantidade].placa, disponiveis[quantidade].peso_lim, disponiveis[quantidade].volume_lim);
    }
    fscanf(input, "%d",&num_pacote);
    pacote* a_levar  = (pacote*)malloc(num_pacote*(sizeof(pacote)));
    for(int quantidade=0; quantidade<num_veiculo; quantidade++){
        fscanf(input, "%s %.2f %.2f %.2f",a_levar[quantidade].codigo,a_levar[quantidade].valor, a_levar[quantidade].peso, a_levar[quantidade].volume);
    }
    int total_peso=num_veiculo,total_vol=num_veiculo,total_itens=num_pacote;
    celula_matriz*** tabela_3d = (celula_matriz***)malloc(sizeof(celula_matriz***)*total_itens);
    for(int celula=0;celula<total_itens;celula++){
        tabela_3d[celula]= (celula_matriz**)malloc(sizeof(celula_matriz*)*total_peso);
    }
     //tridimensional: totl itens, peso,volume
}
