#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct dados_Container {
    int peso, indice_Lista, valor, ocupado;
    char id_Container[12];
    char cnpj[19];
    int prioridade, percentual_Excedido, diferenca;// cpf = 0
} container;
int Hashify(char* to_hash, int tamanho){
    int hash,chave=0, tam_str=12;
    for(int posicao=0;posicao<tam_str;posicao++){
        chave += ((int)(to_hash[posicao]));
    }
    if(chave%2==0)chave+=chave-1;
    (chave<(tamanho*2))?(hash=chave):(hash=chave%(tamanho*2));
    return hash;
}
int Trata_Colisao(int hash, int tam_cadastrados, container* cadastrados){
    int retorno = hash;
    int hash_extra = hash;
    
    while (cadastrados[retorno].ocupado != 0 && (retorno < tam_cadastrados * 2 || hash_extra >= 0)) {
        if (retorno < tam_cadastrados * 2) {
            retorno++;
            if (cadastrados[retorno].ocupado == 0) break;
        }
        if (hash_extra >= 0) {
            hash_extra--;
            if (cadastrados[hash_extra].ocupado == 0) {
                retorno = hash_extra;
                break;
            }
        }
    }
    return retorno;
}


int Busca(int hash, char valor_ent[12], int tam_cadastrados, container* cadastrados){
    int retorno, hash_extra=hash;
    while((strcmp(cadastrados[hash].id_Container,valor_ent)!=0 && strcmp(cadastrados[hash_extra].id_Container,valor_ent)!=0)&& ((hash<(tam_cadastrados*2))||(hash_extra>0))){
        if(hash<tam_cadastrados*2)hash++;
        if(hash_extra>0) hash_extra--;}
    (strcmp(cadastrados[hash].id_Container,valor_ent)==0)?(retorno=hash):(retorno = hash_extra);
    return retorno;
}


/*int busca(char id_Container[12]  , container* cadastrados){
    int indice_Elemento=0;
    while(strcmp(cadastrados[indice_Elemento].id_Container,id_Container)!=0){
        indice_Elemento++;
    }
    return indice_Elemento;
}*/
int cnpj_Confere(char cnpj_Container[12] , container* container_Triagem, int indice_Elemento)
{
    int comparacao = strcmp(container_Triagem[indice_Elemento].cnpj,cnpj_Container);
    return (comparacao == 0) ? 0 : 2;
}

void Calcula_excedente(container* cadastrado , container* triagem){
    int diferenca_peso = (triagem->peso)-(cadastrado->peso);
    triagem->diferenca = diferenca_peso;
    float excedente = ((float)diferenca_peso/cadastrado->peso)*100.0;
    int arredondado = (int)round(excedente);
    if(arredondado>=0) (triagem->percentual_Excedido = arredondado);
    if(triagem->percentual_Excedido>10 && triagem->prioridade!=2){triagem->prioridade=1;} //no intercalar, fazer logica para a maior diferença
}
void Trocar(container* saida, container* entrada) {
    saida->indice_Lista = entrada->indice_Lista;
    saida->peso = entrada->peso;
    saida->percentual_Excedido = entrada->percentual_Excedido;
    saida->prioridade = entrada->prioridade;
    saida->diferenca=entrada->diferenca;
    saida->valor = entrada->valor;

    // Copiar id_Container e garantir terminação nula
    strcpy(saida->id_Container, entrada->id_Container);

    // Copiar cnpj e garantir terminação nula
    strcpy(saida->cnpj, entrada->cnpj);
}

void Intercalar(container* saida, container* entrada, int32_t lim_Inferior, int32_t pivo, int32_t lim_Superior) {
    // índices
    int32_t lim_Inf_Atual = lim_Inferior, pivo_Sup = pivo + 1, indice = lim_Inferior;

    // enquanto houver elementos
    while (lim_Inf_Atual <= pivo && pivo_Sup <= lim_Superior) {
        // pular quem está nos conformes
        for (; lim_Inf_Atual < pivo - 1; lim_Inf_Atual++)
            if (entrada[lim_Inf_Atual].prioridade != 0)
                break;
        for (; pivo_Sup < lim_Superior; pivo_Sup++)
            if (entrada[pivo_Sup].prioridade != 0)
                break;

        // comparação maior prioridade
        if (entrada[pivo_Sup].prioridade < entrada[lim_Inf_Atual].prioridade)
            Trocar(&saida[indice++], &entrada[lim_Inf_Atual++]);
        else if (entrada[pivo_Sup].prioridade > entrada[lim_Inf_Atual].prioridade)
            Trocar(&saida[indice++], &entrada[pivo_Sup++]);
        else if (entrada[pivo_Sup].prioridade == entrada[lim_Inf_Atual].prioridade) {
            // prioridade igual
            if (entrada[lim_Inf_Atual].prioridade == 2 && entrada[pivo_Sup].prioridade == 2) {
                // desempate pelo índice da lista
                if (entrada[lim_Inf_Atual].indice_Lista < entrada[pivo_Sup].indice_Lista)
                    Trocar(&saida[indice++], &entrada[lim_Inf_Atual++]);
                else
                    Trocar(&saida[indice++], &entrada[pivo_Sup++]);
            } else if (entrada[lim_Inf_Atual].prioridade == 1 && entrada[pivo_Sup].prioridade == 1){
                // prioridade igual e não é 2
                if (entrada[lim_Inf_Atual].percentual_Excedido > entrada[pivo_Sup].percentual_Excedido)
                    Trocar(&saida[indice++], &entrada[lim_Inf_Atual++]);
                else if (entrada[lim_Inf_Atual].percentual_Excedido < entrada[pivo_Sup].percentual_Excedido)
                    Trocar(&saida[indice++], &entrada[pivo_Sup++]);
                else if (entrada[lim_Inf_Atual].indice_Lista > entrada[pivo_Sup].indice_Lista) 
                    Trocar(&saida[indice++], &entrada[pivo_Sup++]);    
                else
                    Trocar(&saida[indice++], &entrada[lim_Inf_Atual++]);
                
            }
        }
    }

    // copiar o restante
    if (lim_Inferior > pivo) {
        int restantes = lim_Superior - pivo_Sup + 1;
        while ((restantes--) > 0)
            Trocar(&saida[indice++], &entrada[pivo_Sup++]);
    } else if(pivo_Sup>lim_Superior) {
        int restantes = pivo - lim_Inf_Atual + 1;
        while ((restantes--) > 0)
            Trocar(&saida[indice++], &entrada[lim_Inf_Atual++]);
    }

    // copiar de volta para entrada
    memcpy(&entrada[lim_Inferior], &saida[lim_Inferior], sizeof(container) * (lim_Superior - lim_Inferior + 1));
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
    container* lista_Cadastro = (container*)calloc(tam_Array_cadastro*2,sizeof(container));
    if (lista_Cadastro == NULL) {
        printf("ERRO!");
        return 1;
    }
    while(index_Array_cadastro<tam_Array_cadastro){
        int chave_Hash;
        chave_Hash = Hashify(lista_Cadastro[index_Array_cadastro].id_Container,tam_Array_cadastro);
        if(lista_Cadastro[chave_Hash].ocupado!=0) chave_Hash = Trata_Colisao(chave_Hash,tam_Array_cadastro,lista_Cadastro);
        fscanf(input, "%s %s %d\n",lista_Cadastro[index_Array_cadastro].id_Container, lista_Cadastro[index_Array_cadastro].cnpj, &(lista_Cadastro[index_Array_cadastro].peso));
        lista_Cadastro[chave_Hash].valor=chave_Hash; 
        lista_Cadastro[chave_Hash].ocupado=1;
        index_Array_cadastro++;
        lista_Cadastro[chave_Hash].indice_Lista = index_Array_cadastro;
        
        
    }
    fscanf(input, "%d\n", &tam_Array_triagem);
    container* lista_Triagem = (container*)malloc(sizeof(container)*tam_Array_triagem);
    container* lista_Saida = (container*)malloc(sizeof(container)*tam_Array_triagem);
    int lim_sup = tam_Array_triagem-1, lim_inf=0;
    if (lista_Triagem == NULL) {
        printf("ERRO!");
        return 1;
    }
   while (index_Triagem < tam_Array_triagem) {
    int chave_hash;
    fscanf(input, "%s %s %d\n", lista_Triagem[index_Triagem].id_Container, lista_Triagem[index_Triagem].cnpj, &(lista_Triagem[index_Triagem].peso));
    chave_hash = Hashify(lista_Triagem[index_Triagem].id_Container, tam_Array_triagem);
    int index_cad = Busca(chave_hash, lista_Triagem[index_Triagem].id_Container, tam_Array_cadastro, lista_Cadastro);
    lista_Triagem[index_Triagem].valor = index_cad;
    lista_Triagem[index_Triagem].indice_Lista = lista_Cadastro[index_cad].indice_Lista;
    lista_Triagem[index_Triagem].prioridade = cnpj_Confere(lista_Triagem[index_Triagem].cnpj, lista_Cadastro, index_cad);
    Calcula_excedente(&lista_Cadastro[index_cad], &lista_Triagem[index_Triagem]);

    index_Triagem++;
}

    mergesort(lista_Saida,lista_Triagem,lim_inf,lim_sup);
    int ind = 0;
    
    while(lista_Triagem[ind].prioridade!=0){ (lista_Triagem[ind].prioridade==2)?fprintf(output,"%s: %s<->%s\n",lista_Triagem[ind].id_Container, lista_Cadastro[lista_Triagem[ind].valor].cnpj, lista_Triagem[ind].cnpj):fprintf(output,"%s: %dkg (%d%%)\n",lista_Triagem[ind].id_Container, lista_Triagem[ind].diferenca, lista_Triagem[ind].percentual_Excedido);

    ind++;}
    
    fclose(input);
    fclose(output);
    return 0;
}
