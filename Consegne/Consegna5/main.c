#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "strmanage.h"

int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        printf("Usage: assembler input.asm output.hack \n");
        return 1;
    }
    
    // controllo tipo dei file
    const char* infile = argv[1];     // ".asm"
    const char* outfile = argv[2];    // ".hack"

    if (findchr(infile, '.') == NULL) {
        printf("Invalid file type: Use files with extension \".asm\".\n");
        return 1;
    }

    // controllo estensione file input
    char* fileExt = malloc(strlen(infile) * sizeof(char));
    strcpy(fileExt, findchr(infile, '.'));
    if (strcmp(fileExt, ".asm") != 0)
    {
        printf("Invalid file extension: Use file extension \".asm\" for the input file.\n");
        free(fileExt);
        return 1;
    }
    free(fileExt);

    // controllo estensione file output
    fileExt = malloc(strlen(outfile) * sizeof(char));
    strcpy(fileExt, findchr(outfile, '.'));
    if (strcmp(fileExt, ".hack") != 0)
    {
        printf("Invalid file extension: Use file extension \".hack\" for the output file.\n");
        free(fileExt);
        return 1;
    }
    free(fileExt);

    // apertura file input (.asm)
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL) {
        printf("Couldn't open file \"%s\"\n", infile);
        fclose(inptr);
        return 1;
    }

    // creazione file output (.hack)
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL) {
        printf("Couldn't create file \"%s\"\n", outfile);
        fclose(outptr);
        return 1;
    }

    // spacchetta, "pulisce" e organizza le istruzioni assembler dell'input file  
    bool isParsed = parse(inptr);
    if (!isParsed) {
        printf("ERROR parsing input file.\n");
        return 1;
    }

    // converte e decodifica le istruzioni assembler in codice binario
    bool isConverted = convert(outptr);
    if (!isConverted) {
        printf("ERROR converting input file.\n");
        return 1;
    }

    // rilascio della memoria 
    bool isReleased = release();
    if (!isReleased) {
        printf("ERROR releasing memory.\n");
        return 1;
    }

    printf(" Conversion success! \n\n");

    
    fclose(inptr);
    fclose(outptr);
    return 0;
}