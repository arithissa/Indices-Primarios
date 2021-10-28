#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 100

typedef struct dados{
    int nUSP;
    char nome[15];
    char sobrenome[35];
    char curso[50];
    float nota;
} dados;

typedef struct RRN{
    char chave[20];
    long int endereco;
} RRN;

long int busca(FILE *, char *);
int deletarRegistro(FILE *, FILE *, char *);
int armazenaIndice(FILE *, char *, long int, long int);
int escreveArquivo(FILE *, FILE *, char *, char *, int);
RRN *leituraIndex(FILE *);
dados *leituraArquivo(FILE *);
void imprimeRegistros(FILE *, long int, long int);
long int tamanhoArquivo(char *);
long int tamanhoIndex(FILE *);
long int tamanhoRegistros(FILE *);
