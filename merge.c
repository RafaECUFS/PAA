#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct dados_Container {
    int peso, indice_Lista;
    char id_Container[12];
    char cnpj[19];
    int prioridade;
    float percentual_Excedido;// cpf = 0
} container;


int busca(char id_Container[12]  , container* cadastrados){
    int indice_Elemento=0;
    while(strcmp(cadastrados[indice_Elemento].id_Container,id_Container)!=0){
        indice_Elemento++;
    }
    return indice_Elemento;
}
int cnpj_Confere(char cnpj_Container[12] , container* container_Triagem, int indice_Elemento)
{

    int comparacao = strcmp(container_Triagem[indice_Elemento].cnpj,cnpj_Container);
    return (comparacao == 0) ? 0 : 2;
}

void Calcula_excedente(container* cadastrado , container* triagem){
    cadastrado->percentual_Excedido=0;
    triagem->percentual_Excedido = (((cadastrado->peso)*100)/cadastrado->peso)-100;
    if(triagem->percentual_Excedido>10 && triagem->prioridade!=2){triagem->prioridade=1;} //no intercalar, fazer logica para a maior diferença
}
void Copiar_subarray(container* saida , container* entrada, int restantes){
    while(restantes>0){
        memcpy(saida, entrada, sizeof(container));
        restantes--;
    }
}
void Intercalar(container* saida , container* entrada, int32_t lim_Inferior , int32_t pivo, int32_t lim_Superior) {
    //indices
    int32_t lim_Inf_Atual = lim_Inferior , pivo_Sup = pivo + 1 , indice = lim_Inferior;
    //comparar cnpj

    //enquanto houver elementos
    while ( lim_Inf_Atual <= pivo && pivo_Sup <= lim_Superior ) {
        //pula quem está nos conformes
        if(entrada[pivo_Sup].prioridade==0||entrada[lim_Inf_Atual].prioridade==0){
            if (entrada[lim_Inf_Atual].prioridade==0) lim_Inf_Atual++;
            if (entrada[pivo_Sup].prioridade==0) pivo_Sup++;
            }
        
        //comparacao cnpj
        if(entrada[pivo_Sup].prioridade ==2||entrada[lim_Inf_Atual].prioridade ==2)
            (entrada[lim_Inf_Atual].prioridade==2)?(saida[indice++] = entrada[lim_Inf_Atual++]):(saida[indice++] = entrada[pivo_Sup++]);
        
        //comparação peso
        else if(entrada[pivo_Sup].prioridade==1||entrada[lim_Inf_Atual].prioridade==1)
            (entrada[lim_Inf_Atual].percentual_Excedido >= entrada[pivo_Sup].percentual_Excedido)?(saida[indice++] = entrada[lim_Inf_Atual++]):(saida[indice++] = entrada[pivo_Sup++]);
    }
    if(lim_Inferior>pivo) Copiar_subarray(&saida[indice], &entrada[pivo_Sup], lim_Superior-pivo_Sup+1);
    else Copiar_subarray(&saida[indice], &entrada[lim_Inf_Atual], pivo-lim_Inf_Atual+1);
    Copiar_subarray(&entrada[lim_Inferior],&saida[lim_Inferior], lim_Superior-lim_Inferior+1);
    }

//mergesort recursivo
void mergesort (container*  saida , container*  entrada , int32_t lim_Inferior , int32_t lim_Superior) {
    if (lim_Inferior<lim_Superior){
        //indice medio do vetor
        int32_t pivo = lim_Inferior + ((lim_Superior-lim_Inferior)/2);

        //divisão do vetor em subvetores
        mergesort(saida,entrada,lim_Inferior,pivo);
        mergesort(saida,entrada,pivo+1,lim_Superior);
        //conquista
        Intercalar(saida , entrada , lim_Inferior , pivo, lim_Superior);
    }
}

int main(int argc, char* argv[]){

    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("ERRO!");
        return 0;
    }

    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        printf("ERRO!");
        fclose(input);
        return 0;
    }

    int tam_Array_cadastro=0, tam_Array_triagem=0, index_Array_cadastro=0, index_Triagem=0;
    fscanf(input, "%d\n", &tam_Array_cadastro);
    container* lista_Cadastro = (container*)malloc(sizeof(container)*tam_Array_cadastro);
    if (lista_Cadastro == NULL) {
        printf("ERRO!");
        return 1;
    }
    while(index_Array_cadastro<tam_Array_cadastro){
        fscanf(input, "%11[^ ] %18[^ ] %d\n",lista_Cadastro[index_Array_cadastro].id_Container, lista_Cadastro[index_Array_cadastro].cnpj, &lista_Cadastro[index_Array_cadastro].peso);
        index_Array_cadastro++;
    }
    fscanf(input, "%d\n", &tam_Array_triagem);
    container* lista_Triagem = (container*)malloc(sizeof(container)*tam_Array_triagem);
    container* lista_Saida = (container*)malloc(sizeof(container)*tam_Array_triagem);
    int lim_sup = tam_Array_triagem-1, lim_inf=0;
    if (lista_Triagem == NULL) {
        printf("ERRO!");
        return 1;
    }
    while(index_Triagem<tam_Array_triagem){
        fscanf(input, "%11[^ ] %18[^ ] %d\n",lista_Triagem[index_Triagem].id_Container, lista_Triagem[index_Triagem].cnpj, &lista_Triagem[index_Triagem].peso);
        int index_Coincide = busca(lista_Triagem[index_Triagem].id_Container,lista_Cadastro);
        lista_Triagem[index_Triagem].prioridade = cnpj_Confere(lista_Triagem[index_Triagem].cnpj,lista_Cadastro,index_Coincide);
        Calcula_excedente(&lista_Cadastro[index_Coincide],&lista_Triagem[index_Triagem]);
        index_Triagem++;
    }
    mergesort(lista_Saida,lista_Triagem,lim_inf,lim_sup);
    int ind = 0;
    while(lista_Triagem[ind].prioridade==2 ||lista_Triagem[ind].prioridade==1) fprintf(output,"%s\n",lista_Triagem[ind++].cnpj);
    fclose(input);
    fclose(output);
    return 0;
}
