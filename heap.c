#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct pessoa {
    int idade, prioridade, ordem;
    char nome_pessoa[51];
    char nome_orgao[51];
} pessoa;

typedef struct orgao {
    int qtd_atend,alocado, last_member;//last_member = ultimo elemento com nós filhos
    char nome_orgao[51];
    struct pessoa* fila;
} orgao;

void trocar(pessoa* cliente, int pai, int filho) {
    pessoa cliente_temp;
    memcpy(&cliente_temp, &cliente[filho], sizeof(pessoa));
    memcpy(&cliente[filho], &cliente[pai], sizeof(pessoa));
    memcpy(&cliente[pai], &cliente_temp, sizeof(pessoa));
}
void heapify(pessoa* arr, int n, int i) {
    int pai = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && (arr[left].prioridade > arr[pai].prioridade || 
                    (arr[left].prioridade == arr[pai].prioridade && arr[left].ordem < arr[pai].ordem))) {
        pai = left;
    }

    if (right < n && (arr[right].prioridade > arr[pai].prioridade ||
                     (arr[right].prioridade == arr[pai].prioridade && arr[right].ordem < arr[pai].ordem))) {
        pai = right;
    }

    if (pai != i) {
        trocar(arr, i, pai);
        heapify(arr, n, pai);
    }
}
void remover(pessoa* person, FILE* output, int tam){//fazer loop para cada heap
    fprintf(output,"%s",person[0].nome_pessoa);
    memcpy(&person[0],&person[tam-1], sizeof(pessoa));//remover ultima posição
    //dentro de um loop fora da função: fprintf orgao + loop(remover) enquanto i<qtd_atend + fprintf \n
}

int main(int argc, char* argv[]) {
    pessoa* temp_pessoa = (pessoa*)malloc(sizeof(pessoa));
    int indice_orgaos = 0;
    int ind_fila;
    int ind_fil = 0;
    int ult_pai=0;

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

    int qtd_orgs, tam_fila;
    fscanf(input, "%d\n", &qtd_orgs);

    orgao* lista_orgaos = (orgao*)malloc(sizeof(orgao) * qtd_orgs);
    if (lista_orgaos == NULL) {
        printf("ERRO!");
        return 1;
    }

    for (int index = 0; index < qtd_orgs; index++) {
        fscanf(input, " %50s %d\n", lista_orgaos[index].nome_orgao, &lista_orgaos[index].qtd_atend);
        lista_orgaos[index].fila = NULL;
        lista_orgaos[index].alocado = 0;
    } //preenche o array de orgaos

    fscanf(input, "%d\n", &tam_fila);
    //pega o valor do tamanho maximo da fila

    for (int linha = 0; linha < tam_fila; linha++) {
        ind_fila = 0;
        indice_orgaos=0;
        fscanf(input, "%50[^|]|%50[^|]|%d\n", temp_pessoa->nome_orgao, temp_pessoa->nome_pessoa, &temp_pessoa->idade);
        
        while(strcmp(temp_pessoa->nome_orgao, lista_orgaos[indice_orgaos].nome_orgao )!= 0 && indice_orgaos<qtd_orgs){
            indice_orgaos++;
            //encontra o orgao igual ao da pessoa
        }

        if(indice_orgaos>qtd_orgs){
                printf("Orgao não encontrado\n");
                continue;
            }
            
        if (lista_orgaos[indice_orgaos].alocado==0) {
            lista_orgaos[indice_orgaos].fila = (pessoa*)calloc(tam_fila, sizeof(pessoa));
            if (lista_orgaos[indice_orgaos].fila == NULL) {
                printf("ERRO!");
                return 1;
                }
            lista_orgaos[indice_orgaos].alocado=1;
            }   
            //checa se a fila ja foi alocada. se não, aloca.
        
        while(lista_orgaos[indice_orgaos].fila[ind_fila].prioridade != 0 && ind_fila<tam_fila) {
            ind_fila++;}
        if(ind_fila>tam_fila){printf("ERROW!!!");
        continue;}    
            memcpy( &lista_orgaos[indice_orgaos].fila[ind_fila],temp_pessoa, sizeof(pessoa));

            

            if (temp_pessoa->idade < 60) {
                lista_orgaos[indice_orgaos].fila[ind_fila].prioridade = 1;
            } else {
                lista_orgaos[indice_orgaos].fila[ind_fila].prioridade = 2;
            }

            lista_orgaos[indice_orgaos].fila[ind_fila].ordem = ind_fila;
            
        }
        for(int ind_org=0; ind_org<qtd_orgs; ind_org++){//coletar tamanho da fila de cada orgao
            
            if(lista_orgaos[ind_org].alocado==1){
                ind_fil=0;
                while(lista_orgaos[ind_org].fila[ind_fil].prioridade==1 || lista_orgaos[ind_org].fila[ind_fil].prioridade==2){
                        ++ind_fil;
                    }
                lista_orgaos[ind_org].last_member=ind_fil;          
            }
            }
        
        for(int org_heap=0; org_heap<qtd_orgs;org_heap++){
            ult_pai=((lista_orgaos[org_heap].last_member - 1)-1)/2;
            
            for(;ult_pai>=0;ult_pai--){heapify(lista_orgaos[org_heap].fila,lista_orgaos[org_heap].last_member,ult_pai);}}
        while(tam_fila>0){
            for(int org_heap=0; org_heap<qtd_orgs;org_heap++){
            if( lista_orgaos[org_heap].last_member<1){continue;}
            ult_pai=((lista_orgaos[org_heap].last_member - 1)-1)/2;
            fprintf(output,"%s:",lista_orgaos[org_heap].nome_orgao);
            for(int chamadas=0; chamadas<lista_orgaos[org_heap].qtd_atend; chamadas++){
                if( lista_orgaos[org_heap].last_member>=1){remover(lista_orgaos[org_heap].fila,output,lista_orgaos[org_heap].last_member);}
                
                if( lista_orgaos[org_heap].last_member>1 && chamadas<(lista_orgaos[org_heap].qtd_atend-1) ){
                    fprintf(output,",");
                }
                lista_orgaos[org_heap].last_member--;
                tam_fila--;
                if(lista_orgaos[org_heap].last_member!=0){heapify(lista_orgaos[org_heap].fila,lista_orgaos[org_heap].last_member,0);}
                else{break;}
                
            }
            if(tam_fila==0){break;}
            fprintf(output,"\n");
            
        }}
        fclose(input);
        fclose(output);
        free(temp_pessoa);
        return 0;
    }

    

