#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct estrutura{
    int hash_valor, valor;
}tupla;
int Hashify(int chave, int tamanho){
    int hash;
    (chave<(tamanho*2))?(hash=chave):(hash=chave%(tamanho*2));
    return hash;
}
int Trata_Colisao(int hash, int tam, tupla* inteiros){
    int retorno, hash_extra=hash;
    while((inteiros[hash].valor!=0||inteiros[hash].valor!=0)&& ((hash<(tam*2))||(hash_extra>0))){
        if(hash<tam*2)hash++;
        if(inteiros[hash].valor==0 && hash<=(tam*2)) retorno = hash;
        if(hash_extra>0) hash_extra--;
        if(inteiros[hash_extra].valor==0 && hash_extra>=0) retorno = hash_extra;}
        return retorno;
}

int Busca(int hash, int valor_ent, int tam, tupla* inteiros){
    int retorno, hash_extra=hash;
    while((inteiros[hash].valor!=valor_ent && inteiros[hash_extra].valor!=valor_ent)&& ((hash<(tam*2))||(hash_extra>0))){
        if(hash<tam*2)hash++;
        if(hash_extra>0) hash_extra--;}
    (inteiros[hash].valor==valor_ent)?(retorno=hash):(retorno = hash_extra);
    return retorno;
}
int main(){
    int indice=0, tam_array, chave,hash,tam_max;
    tupla* array_chaves;
    printf("Insira o tamanho do vetor:\n");
    scanf("%d",&tam_array);
    tam_max=tam_array;
    array_chaves = (tupla*)calloc(tam_array*2,sizeof(int));
    for(;indice<tam_array;indice++){
        printf("Insira o valor:\n");
        scanf("%d", &chave);
        hash = Hashify(chave,tam_array);
        if(array_chaves[hash].valor!=0) hash = Trata_Colisao(hash,tam_array,array_chaves);
        array_chaves[hash].valor=chave;
    }
    printf("Insira o tamanho do vetor:\n");
    do
    {
        scanf("%d",&tam_array);
        if(tam_array>tam_max) {
            printf("Valor maior do que %d nao e permitido\n",tam_max);
            printf("Insira o tamanho do vetor:\n");}
        
    } while (tam_array>tam_max);
    
    
    tupla* array_chaves2 = (tupla*)calloc(tam_array,sizeof(int));
    for(indice=0;indice<tam_array;indice++){
        printf("Insira o valor:\n");
        scanf("%d", &chave);
        array_chaves2[indice].valor=chave;
        hash = Hashify(chave,tam_array);
        if(array_chaves[hash].valor!=0) hash = Busca(hash, chave,tam_array,array_chaves);
        printf("%d == %d\n",array_chaves[hash].valor,array_chaves2[indice].valor);
    }
    free(array_chaves);
    free(array_chaves2);
    return 0;
    }
