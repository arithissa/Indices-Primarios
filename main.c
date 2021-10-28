#include "indicesprimarios.h"

int main(){
    long int inicio; int flag = 0;
    char str[max], aux[max], delim[2] = ",", *token;
    FILE *arquivo = fopen("registros.dat", "w+");	//cria arquivo para leitura e escrita
    FILE *index = fopen("indice.dat", "w+");

    while(1){
        fgets(str, max, stdin);
        strcpy(aux, str);
        token = strtok(aux, " ");

        if(strcmp(token, "insert") == 0){
            token = strtok(NULL, delim);
            if(busca(index, token) != -1)
                printf("O Registro ja existe!\n");
            else{
            long int pos = -1;
            escreveArquivo(arquivo, index, token, str, flag);
            
            inicio = (flag * tamanhoRegistros(arquivo));

            armazenaIndice(index, token, inicio, pos);
            flag++;
            }
        }
        else if(strcmp(token, "search") == 0){
            token = strtok(NULL, delim);
            if(busca(index, token) == -1)
                printf("Registro nao encontrado!\n");
            else{
                long int endereco = busca(index, token);
                imprimeRegistros(arquivo, endereco, (endereco + tamanhoRegistros(arquivo)));
            }
        }
        else if(strcmp(token, "delete") == 0){
            token = strtok(NULL, delim);
            deletarRegistro(arquivo, index, token);
        } 
        else
            break;
    }

    fclose(index);
    fclose(arquivo);

    return 0;
}
