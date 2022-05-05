#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

// lunghezza massima della riga che contiene l'istruzione assembler dell'input file
#define LENGTH 1024

// lunghezza massima del comando assembler "puro" 
#define COMMAND 64

// lunghezza istruzione
#define INSTRUCTION 16

// numero di locazioni di memoria
#define SIZE 24576

// nodo della struttura che mantiene le istruzioni
typedef struct node {
    char word[COMMAND+1];
    struct node* next;
    int line;
} node;

// spartiziona ogni comando assembler del file input
bool parse(FILE* infile);

// converte da istruzioni assembler a binarie
bool convert(FILE* outfile);

// rilascia la lista dei comandi dalla memoria
bool release();

#endif // PARSER_H