#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "strmanage.h"
#include "parser.h"

//nome del file senza estensione
char fname_noext[MAX_FNAME_CHARS+1];
//nome della funzione che viene processata
char current_fun[MAX_FNAME_CHARS+1];

int main(int argc, const char *argv[])
{
	//tiene ogni riga letta che viene processata in quel momento
    char line[MAX_LINE_LEN + 1];
    char tmp_line[MAX_LINE_LEN + 1];
    
    //Indica la linea di file processata in quel momento     
    int line_num = 0;
    
    //Tiene il codice assembler generato fino alla linea che in quel momento viene processata    
    char asm_output[MAX_ASM_OUT + 1];
    
    //contiene le sotto-stringhe di ogni riga del codice virtual machine ES: push local 0 --> {"push","local","0"}     
    char *sub_strgs[MAX_SUB_STRGS + 1] = {NULL};
    
    //numero di sotto-stringhe della riga corrente
    int n_sub_strgs;


	if (argc != 3) {
        printf("Usage: VMtranslator input.vm output.asm \n");
        return 1;
    }

    // controllo tipo dei file
    const char* infile = argv[1];     // ".vm"
    const char* outfile = argv[2];    // ".asm"

    if (findchr(infile, '.') == NULL) {
        printf("Invalid file type: Use files with extension \".vm\".\n");
        return 1;
    }

    // controllo estensione file input
    char* fileExt = malloc(strlen(infile) * sizeof(char));
    strcpy(fileExt, findchr(infile, '.'));
    if (strcmp(fileExt, ".vm") != 0)
    {
        printf("Invalid file extension: Use file extension \".vm\" for the input file.\n");
        free(fileExt);
        return 1;
    }
    free(fileExt);

    // controllo estensione file output
    fileExt = malloc(strlen(outfile) * sizeof(char));
    strcpy(fileExt, findchr(outfile, '.'));
    if (strcmp(fileExt, ".asm") != 0)
    {
        printf("Invalid file extension: Use file extension \".asm\" for the output file.\n");
        free(fileExt);
        return 1;
    }
    free(fileExt);

    // apertura file input (.vm)
    FILE* fp_input = fopen(infile, "r");
    if ( fp_input== NULL) {
        printf("Couldn't open file \"%s\"\n", infile);
        fclose(fp_input);
        return 1;
    }

    // creazione file output (.asm)
    FILE* fp_output = fopen(outfile, "w");
    if (fp_output == NULL) {
        printf("Couldn't create file \"%s\"\n", outfile);
        fclose(fp_output);
        return 1;
    }
	
	//inizializza la variabile "current_fun"
    strcpy(current_fun, "OutOfFunction");
	
	//si inizializza il codice assembler settando SP
    initializing_code(asm_output);
	fprintf(fp_output, asm_output);
	
	//si salva il nome del file privato dell'estensione .vm nella variabile "fname_noext"
	strcpy(fname_noext,argv[1]);
	remove_ext(fname_noext);
	
	while (fgets(line, sizeof(line), fp_input)) {
		line_num++;

		strip_comments(line);

		if (s_is_empty(line)) {
			continue; // se linea vuota passa alla linea successiva
		}
		
		strcpy(tmp_line, line);
		//suddivisione comando vm in sotto-stringhe
		n_sub_strgs = s_splitting(tmp_line, sub_strgs, MAX_SUB_STRGS+1, " ");
		// deve essere presente almeno una stottostringa (linee vuote saltate).
		//Ogni comando vm è identificato da un ID. In questo caso si indica a che comando corrisponde la riga corrente
		char* cmdid = str_to_cmdid(sub_strgs[0]);
		//in base al tipo di ID, "parser_fn" chiama la giusta funzione parser per sostituire tale comando vm con il corrispondente
		//codice assembler predefinito
		bool valid = parser_fn(cmdid, n_sub_strgs, (const char **) sub_strgs, asm_output);


		if (!valid) {
			printf("ERROR parsing line %d.\n", line_num);
			return 1;
		}


		fprintf(fp_output, asm_output);

	}

	printf(" Conversion success! \n\n");

	fclose(fp_input);
    fclose(fp_output);


    return 0;
}