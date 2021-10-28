#include "indicesprimarios.h"


long int busca(FILE *index, char *chave){   //busca o registro por meio da chave
    int cChave = atoi(chave);
    RRN *p = NULL;

    fseek(index, 0, SEEK_SET);
    while((p = leituraIndex(index)) != NULL){   //faz a busca no arquivo de índice
        int pChave = atoi(p->chave);
        if(pChave == cChave)
            return p->endereco;
    }

    return -1; //não encontrado
}

int deletarRegistro(FILE *arquivo, FILE *index, char *chave){   //faz a exclusão lógica do registro
    RRN *p = NULL;
    int cChave = atoi(chave);

    fseek(index, 0, SEEK_SET);
    while((p = leituraIndex(index)) != NULL){   //busca o registro no arquivo de índice
        long int posicao = (ftell(index) - tamanhoIndex(index));
        int pChave = atoi(p->chave);
        if(pChave == cChave){
            armazenaIndice(index, "*|", p->endereco, posicao);  //marca o registro se encontrado
        } else{
            armazenaIndice(index, p->chave, p->endereco,posicao);
        }
    }

    return 0; //sucesso
}

int armazenaIndice(FILE *arquivo, char *indice, long int endereco, long int posicao){   //armazena os dados no arquivo de índice
    RRN *p = (RRN *) malloc(sizeof(RRN));
    if(p == NULL) 
        return 1;  //erro ao alocar
    
    if(posicao != -1)
        fseek(arquivo, posicao, SEEK_CUR);
    
    char *chave = (char *) malloc(sizeof(char));

    strcpy(chave, indice);
    strcpy(p->chave, chave);
    p->endereco = endereco;

    if((fwrite(&p->chave, (sizeof(char) * 20), 1, arquivo)) != 1 || fwrite(&p->endereco, sizeof(long int), 1, arquivo) != 1){
        if(p != NULL)
            free(p);
        return 1;
    }
    fflush(arquivo);

    return 0;
}

int escreveArquivo(FILE *arquivo, FILE *arqIndice, char *n, char *str, int flag){	//armazena os dados no arquivo 
    int nUSP, nome, sobrenome, curso, nota;
    char delim[2] = ",", aux[max], *token;

    dados *p = (dados *) malloc(sizeof(dados));
    if(p == NULL) 
        return 1;  //erro ao alocar

    //lê os registros e escreve cada campo no arquivo
    strcpy(aux, str);
    token = strtok(aux, delim);

    p->nUSP = atoi(n);
    nUSP = fwrite(&p->nUSP, sizeof(int), 1, arquivo);
    token = strtok(NULL, delim);

    strcpy(p->nome, token);
    nome = fwrite(&p->nome, (sizeof(char) * 15), 1, arquivo);
    token = strtok(NULL, delim);

    strcpy(p->sobrenome, token);
    sobrenome = fwrite(&p->sobrenome, (sizeof(char) * 35), 1, arquivo);
    token = strtok(NULL, delim);

    strcpy(p->curso, token);
    curso = fwrite(&p->curso, (sizeof(char) * 50), 1, arquivo);
    token = strtok(NULL, delim);

    p->nota = atof(token);
    nota = fwrite(&p->nota, sizeof(float), 1, arquivo);
    aux[0] = '\0';
    fflush(arquivo);

    //desaloca p em caso de algum erro de leitura
    if(nUSP != 1 || nome != 1 || sobrenome != 1 || curso != 1 || nota != 1){
        if(p != NULL)
            free(p);
        return 1;
    }

    return 0;
}

RRN *leituraIndex(FILE *index){ //faz a leitura dos registros do arquivo de índice
    int chave, endereco;
    RRN *p = (RRN *) malloc(sizeof(RRN));
    if(p == NULL) 
        return NULL;  //erro ao alocar

    //lê os campos do registro 
    chave = fread(&p->chave, (sizeof(char) * 20), 1, index);
    endereco = fread(&p->endereco, sizeof(long int), 1, index);

    //desaloca p em caso de algum erro de leitura
    if(chave != 1 || endereco != 1){
        if(p != NULL)
            free(p);
        return NULL;
    }

    return p;
}

dados *leituraArquivo(FILE *arquivo){ //faz a leitura dos registros do arquivo
    int nUSP, nome, sobrenome, curso, nota;
    dados *p;

    p = (dados *) malloc(sizeof(dados));
    if(p == NULL) 
        return NULL;  //erro ao alocar

    //lê os campos do registro 
    nUSP = fread(&p->nUSP, sizeof(int), 1, arquivo);
    nome = fread(&p->nome, (sizeof(char) * 15), 1, arquivo);
    sobrenome = fread(&p->sobrenome, (sizeof(char) * 35), 1, arquivo);
    curso = fread(&p->curso, (sizeof(char) * 50), 1, arquivo);
    nota = fread(&p->nota, sizeof(float), 1, arquivo);

    //desaloca p em caso de algum erro de leitura
    if(nUSP != 1 || nome != 1 || sobrenome != 1 || curso != 1 || nota != 1){
        if(p != NULL)
            free(p);
        return NULL;
    }

    return p;
}

void imprimeRegistros(FILE *arquivo, long int inicio, long int fim){    //impressão dos registros
    fseek(arquivo, inicio, SEEK_SET); //parte da posição de inicio

    long int i = ftell(arquivo);
    int primeiroReg = 1;  //marcador para imprimir a quebra de linha

    dados *p = NULL;
    while(i < fim && (p = leituraArquivo(arquivo)) != NULL){
            if(!primeiroReg)
                printf("\n"); //quebra de linha
            else
                primeiroReg = 0;
            printf("-------------------------------\n");
            printf("USP number: %d\n", p->nUSP);
            printf("Name: %s\n", p->nome);
            printf("Surname: %s\n", p->sobrenome);
            printf("Course: %s\n", p->curso);
            printf("Test grade: %.2f\n", p->nota);
            printf("-------------------------------\n");
            if(p != NULL)
                free(p);  //desalocação da memória
        i = ftell(arquivo);
    }

    return;
}

long int tamanhoArquivo(char *nomeArquivo){ //lê e retorna o tamanho do arquivo
	FILE *arquivo = fopen(nomeArquivo, "r");
	fseek(arquivo, 0, SEEK_END);  //vai até o final do arquivo
	long int tamanho = (long int) ftell(arquivo);
	fclose(arquivo);
	fseek(arquivo, 0, SEEK_SET);  //volta para a posição inicial
	
	return tamanho;
}

long int tamanhoIndex(FILE *index){ //retorna o tamanho dos registros de índice em bytes
    long int tamanho = (sizeof(char) * 20) + sizeof(long int);

    return tamanho;
}

long int tamanhoRegistros(FILE *arquivo){ //retorna o tamanho dos registros em bytes
    long int tamanho = (sizeof(int) + (sizeof(char) * 100) + sizeof(float));

    return tamanho;
}
