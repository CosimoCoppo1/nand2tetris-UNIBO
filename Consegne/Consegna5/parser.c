#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "strmanage.h"

// lista dei comandi asm presi dall'input file
node* head=NULL;
node* insert_node=NULL;
unsigned int count;

// lista delle etichette
node* label_head=NULL;
node* label_insert=NULL;
unsigned int label_count;

// singola istruzione aasembler
char command[LENGTH+1];

// symbol table
char* symbol[SIZE+1];

bool parse(FILE* infile)
{
    char buffer[LENGTH+1];  // buffer che memorizza ogni singola riga dell'input file
    
    count = 0;

    while (fgets(buffer, LENGTH, infile) != NULL) {
        
        int len = 0; // lunghezza strina

        // ignora le stringhe che iniziano con commenti o sono vuote
        if ((buffer[0] == '/' && buffer[1] == '/') || (buffer[0] == '\r' || buffer[0] == '\n' || buffer[0] == '\0')) {
            count--;
        }
        // indentazione stringa
        else if (buffer[0] == ' ' || buffer[0] == '\t') {

            // ignora spazi bianchi
            int i = 0;
            while (buffer[i] == ' ') {
                i++;
            }
            // ignora commenti
            if (buffer[i] == '/' && buffer[i+1] == '/')
                count--;
            // carica l'istruzione del buffer in command
            else {
				//tiene lunghezza stringa ignorando spazi bianchi e commenti
                int c = 0;    
                while (buffer[i] != '\0' && buffer[i] != ' ' && buffer[i] != '/' && buffer[i] != '\r' && buffer[i] != '\n') {
                    len++;
                    command[c] = buffer[i]; 
                    i++;
                    c++;
                }
            }
        }
        else {        
            // tiene lunghezza stringa
            int i = 0;           
            int c = 0;
            while (buffer[i] != '\0' && buffer[i] != ' ' && buffer[i] != '/' && buffer[i] != '\r' && buffer[i] != '\n') {
                len++;
                command[c] = buffer[i];
                i++;
                c++;
            }
        }
        
        // carica comando assembler nella linked list
        if (len != 0) {
            
            // lista vuota
            if (head == NULL) {
				
                head = malloc(sizeof(node));
                if (head == NULL) {
                    printf("Couldn't allocate memory for head node.\n");
                    return false;
                }                
                strcpy(head->word, command);
                head->line = count;
                head->next = NULL;
            }
            else {
                // inserisce il nuovo nodo alla fine della lista
                node* cursor = head;
                while (cursor->next != NULL) {
                    cursor = cursor->next;
                }
                
                insert_node = malloc(sizeof(node));
                if (insert_node == NULL) {
                    printf("Couldn't allocate memory for insert node.\n");
                    return false;
                }
                strcpy(insert_node->word, command);
                insert_node->line = count;
                insert_node->next = NULL;
                cursor->next = insert_node;

            }
            // reset dei buffer
            int x = 0;
            while(buffer[x] != '\0')
            {
                buffer[x] = '\0';
                x++;
            }
            int y = 0;
            while(command[y] != '\0')
            {
                command[y] = '\0';
                y++;
            }            
        }
        count++;
    }    
    return true;
}


bool convert(FILE* outfile)
{
    // inizializza la symbol table con i simboli predefiniti
    symbol[0] = "SP";
    symbol[1] = "LCL";
    symbol[2] = "ARG";
    symbol[3] = "THIS";
    symbol[4] = "THAT";
    symbol[16384] = "SCREEN";
    symbol[24576] = "KBD";

    // reset cursore lista comandi
    node* cursor = head;
    
    // definisco cursore che visiti la lista delle etichette
    node* label_cursor;
    
    // numero etichette
    int num = 0;

    // per le variabili da caricare oltre i 16 simboli predefiniti nella symbol table
    int pos = 16;

    label_count = 0;

    // ricerca le etichette
    while (cursor->word != NULL) {

        if (findchr(cursor->word, '(') != NULL) {
          
            int length = strlen(cursor->word)-2;
            char temp[length];

            temp[length] = '\0';
            strncpy(temp, findchr(cursor->word, '(')+1, strlen(cursor->word)-2);

            // lista etichette vuota
            if (label_head == NULL) {

                label_head = malloc(sizeof(node));
                if (label_head == NULL) {
                    printf("Couldn't allocate memory for label_head node.\n");
                    return false;
                }                
                strcpy(label_head->word, temp);
                label_head->line = cursor->line - num;
                label_head->next = NULL;
            }
            else {
                // si inserisce il nuovo nodo alla fine della lista
                label_cursor = label_head;
                
                while (label_cursor->next != NULL) {
                    label_cursor = label_cursor->next;
                }
                
                label_insert = malloc(sizeof(node));
                if (label_insert == NULL) {
                    printf("Couldn't allocate memory for label insert node.\n");
                    return false;
                }
                
                strcpy(label_insert->word, temp);

                label_insert->line = cursor->line - num;
                label_insert->next = NULL;
                label_cursor->next = label_insert;
            }            
            num++;
            label_count++;
        }
        
        cursor = cursor->next;
    }

    // reset cursore
    cursor = head;

    // converte le istruzioni da assembler a binario
    while (cursor->word != NULL) {
        
        // A-Instruction inizia con il prefisso "0" 
        char a_instr[16+1] = "0";
        
        // C-Instruction inizia con il prefisso "111" 
        char c_instr[16+1] = "111";
        
        // campi della C-Instruction (dest = comp;jump)
        char* dest = "000";     // d1, d2, d3
        char* comp = "0000000"; // a, c1, c2, c3, c4, c5, c6
        char* jump = "000";     // j1, j2, j3

        bool is_a = false;      // è una A-instruction?
        bool a_label = false;   // è una etichetta?
        bool a_addr = false;    // è un indirizzo?
        // ricerca le istruzioni di tipo '@value', che possano essere: A-Instruction, Label o Symbol
        if (findchr(cursor->word, '@') != NULL) {
            
            // carattere '@' seguito da una cifra o un simbolo
            is_a = true;
            char* c = findchr(cursor->word, '@')+1;

            int decimal;
            char* address = malloc((INSTRUCTION-1) * sizeof(char));
            if (address == NULL) {
                printf("Failed to allocate memory for address.\n");
                return false;
            }

            // spacchetta comando, solo per i NUMERI
            if (isdigit(*c) != 0) {
                strcpy(command, c);
                decimal = atoi(command);
                a_addr = true;
            }
            // spacchetta comando, solo per LABEL o SYMBOL
            else if (a_addr != true) {
                
                // se è una @LABEL allora cerca il match nella lista delle etichette                
                // reset cursore lista etichette per ricerca
                label_cursor = label_head;

                while (label_cursor != NULL) {

                    if (strcmp(label_cursor->word, c) == 0) {
                        decimal = label_cursor->line;
                        a_label = true;
                    }
                    
                    label_cursor = label_cursor->next;
                }

                // altrimenti gestire i simboli (di 2 tipi): predefiniti e definiti dall'utente
                if (a_label == false) {
                    //predefiniti
                    if ((strcmp(c, "SP") == 0) || (strcmp(c, "R0") == 0))
                        decimal = 0;
                    else if ((strcmp(c, "LCL") == 0) || (strcmp(c, "R1") == 0))
                        decimal = 1;
                    else if ((strcmp(c, "ARG") == 0) || (strcmp(c, "R2") == 0))
                        decimal = 2;
                    else if ((strcmp(c, "THIS") == 0) || (strcmp(c, "R3") == 0))
                        decimal = 3;
                    else if ((strcmp(c, "THAT") == 0) || (strcmp(c, "R4") == 0))
                        decimal = 4;
                    else if (strcmp(c, "R5") == 0)
                        decimal = 5;
                    else if (strcmp(c, "R6") == 0)
                        decimal = 6;
                    else if (strcmp(c, "R7") == 0)
                        decimal = 7;
                    else if (strcmp(c, "R8") == 0)
                        decimal = 8;
                    else if (strcmp(c, "R9") == 0)
                        decimal = 9;
                    else if (strcmp(c, "R10") == 0)
                        decimal = 10;
                    else if (strcmp(c, "R11") == 0)
                        decimal = 11;
                    else if (strcmp(c, "R12") == 0)
                        decimal = 12;
                    else if (strcmp(c, "R13") == 0)
                        decimal = 13;
                    else if (strcmp(c, "R14") == 0)
                        decimal = 14;
                    else if (strcmp(c, "R15") == 0)
                        decimal = 15;
                    else if (strcmp(c, "SCREEN") == 0)
                        decimal = 16384;
                    else if (strcmp(c, "KBD") == 0)
                        decimal = 24576;
                    else {
                        // ricerca i simboli definiti dall'utente nella symbol table
                        bool exists = false;
                        
                        for (int i = 16; i < SIZE+1; i++) {

                            // già nella symbol table
                            if ((symbol[i] != NULL) && (strcmp(symbol[i], c) == 0)) {  
                                decimal = i;
                                exists = true;
                            }
                        }
                        // carica simbolo
                        if (exists == false) {
                            symbol[pos] = c;
                            decimal = pos;
                            pos++;
                        }
                    }
                }
            }
            // converte il decimale in un indirizzo binario da 15-bit
            dectobinary(decimal, address);

            // aggiunge il valore binario al prefisso dell'istruzione
            strcat(a_instr, address);

            // si inserisce l'istruzione nell'output file            
			fprintf(outfile, "%s\n", a_instr);
			            
            free(address);
        }
        // spacchetta il campo dest della C-instruction
        else if (strncmp(cursor->word, "M=", 2) == 0)
            dest = "001";
        else if (strncmp(cursor->word, "D=", 2) == 0)
            dest = "010";
        else if (strncmp(cursor->word, "MD=", 3) == 0)
            dest = "011";
        else if (strncmp(cursor->word, "A=", 2) == 0)        
            dest = "100";
        else if (strncmp(cursor->word, "AM=", 3) == 0)
            dest = "101";
        else if (strncmp(cursor->word, "AD=", 3) == 0)
            dest = "110";
        else if (strncmp(cursor->word, "AMD=", 4) == 0)    
            dest = "111";
        
        // se è una C-instruction allora spacchetta il campo 'comp'
        if ((is_a == false && findchr(cursor->word, '(') == NULL) && (strncmp(cursor->word, "0;", 2) != 0)) {
            
            // compara campo 'comp' della C-instruction
            if (findchr(cursor->word, '=') != NULL) {

                // = 0
                if (findchr(findchr(cursor->word, '=')+1, '0') != NULL)
                    comp = "0101010";
                // = 1
                else if (strncmp(findchr(cursor->word, '=')+1, "1", 1) == 0)
                    comp = "0111111";
                // = -1
                else if (strncmp(findchr(cursor->word, '=')+1, "-1", 2) == 0)
                    comp = "0111010";
                // = D
                else if (strncmp(findchr(cursor->word, '=')+1, "D", 2) == 0)
                    comp = "0001100";
                // a = 0
                // = A
                else if (strncmp(findchr(cursor->word, '=')+1, "A", 2) == 0)
                    comp = "0110000";
                // = !D
                else if (strncmp(findchr(cursor->word, '=')+1, "!D", 2) == 0)
                    comp = "0001101";
                // = !A
                else if (strncmp(findchr(cursor->word, '=')+1, "!A", 2) == 0)
                    comp = "0110001";
                // = -D
                else if (strncmp(findchr(cursor->word, '=')+1, "-D", 2) == 0)
                    comp = "0001111";    
                // = -A
                else if (strncmp(findchr(cursor->word, '=')+1, "-A", 2) == 0)
                    comp = "0110011";
                // = D+1
                else if (strncmp(findchr(cursor->word, '=')+1, "D+1", 3) == 0)
                    comp = "0011111";
                // = A+1
                else if (strncmp(findchr(cursor->word, '=')+1, "A+1", 3) == 0)
                    comp = "0110111";
                // = D-1
                else if (strncmp(findchr(cursor->word, '=')+1, "D-1", 3) == 0)
                    comp = "0001110";
                // = A-1
                else if (strncmp(findchr(cursor->word, '=')+1, "A-1", 3) == 0)
                    comp = "0110010";
                // = D+A
                else if (strncmp(findchr(cursor->word, '=')+1, "D+A", 3) == 0)
                    comp = "0000010";
                // = D-A
                else if (strncmp(findchr(cursor->word, '=')+1, "D-A", 3) == 0)
                    comp = "0010011";
                // = A-D
                else if (strncmp(findchr(cursor->word, '=')+1, "A-D", 3) == 0)
                    comp = "0000111";
                // = D&A
                else if (strncmp(findchr(cursor->word, '=')+1, "D&A", 3) == 0)
                    comp = "0000000";
                // = D|A
                else if (strncmp(findchr(cursor->word, '=')+1, "D|A", 3) == 0)
                    comp = "0010101";
                // a = 1 (A diventa M)
                // = M
                else if (strncmp(findchr(cursor->word, '=')+1, "M", 2) == 0)
                    comp = "1110000";
                // = !M
                else if (strncmp(findchr(cursor->word, '=')+1, "!M", 2) == 0)
                    comp = "1110001";
                // = -M
                else if (strncmp(findchr(cursor->word, '=')+1, "-M", 2) == 0)
                    comp = "1110011";
                // = M+1
                else if (strncmp(findchr(cursor->word, '=')+1, "M+1", 3) == 0)
                    comp = "1110111";
                // = M-1
                else if (strncmp(findchr(cursor->word, '=')+1, "M-1", 3) == 0)
                    comp = "1110010";
                // = D+M
                else if ((strncmp(findchr(cursor->word, '=')+1, "D+M", 3) == 0) || 
                         (strncmp(findchr(cursor->word, '=')+1, "M+D", 3) == 0))
                    comp = "1000010";
                // = D-M
                else if (strncmp(findchr(cursor->word, '=')+1, "D-M", 3) == 0)
                    comp = "1010011";
                // = M-D
                else if (strncmp(findchr(cursor->word, '=')+1, "M-D", 3) == 0)
                    comp = "1000111";
                // = D&M
                else if (strncmp(findchr(cursor->word, '=')+1, "D&M", 3) == 0)
                    comp = "1000000";
                // = D|M
                else if (strncmp(findchr(cursor->word, '=')+1, "D|M", 3) == 0)
                    comp = "1010101";
            }
            // valutare D;JGT, D;JLT, ecc.
            else if (strncmp(cursor->word, "D;", 2) == 0) {
                // dest = D
                comp = "0001100";
            }

            // spacchetta il campo 'jump' (null, JGT, JEQ, JGE, JLT, JNE, JLE, JMP)
            if (findchr(cursor->word, ';') != NULL) {
                
                if (strncmp(findchr(cursor->word, ';')+1, "JGT", 3) == 0)
                    jump = "001";
                else if (strncmp(findchr(cursor->word, ';')+1, "JEQ", 3) == 0)
                    jump = "010";
                else if (strncmp(findchr(cursor->word, ';')+1, "JGE", 3) == 0)
                    jump = "011";
                else if (strncmp(findchr(cursor->word, ';')+1, "JLT", 3) == 0)
                    jump = "100";
                else if (strncmp(findchr(cursor->word, ';')+1, "JNE", 3) == 0)
                    jump = "101";
                else if (strncmp(findchr(cursor->word, ';')+1, "JLE", 3) == 0)
                    jump = "110";
                else if (strncmp(findchr(cursor->word, ';')+1, "JMP", 3) == 0)
                    jump = "111";
            }
            
            // concatenare i campi dest, comp e jump             
			strconcat(c_instr, comp, dest, jump);
            
            // si scrive la C-instruction sull'output file
            fprintf(outfile, "%s\n", c_instr);            
        }
        // si valuta il comando del loop infinito
        else if ((strcmp(dest, "000") == 0) && (strcmp(cursor->word, "0;JMP") == 0)) {
            
			strcat(c_instr, "0101010000111");

            // si scrive la C-instruction sull'output file
			fprintf(outfile, "%s\n", c_instr);
			
            
        }
        cursor = cursor->next;
    }    
    return true;
}


bool release() {

    node* cursor = head;
    node* label_cursor = label_head;

    unsigned int compare = 0;
    unsigned int label_compare = 0;

    // libera la lista istruzioni
    while (cursor != NULL) {
        node* temp = cursor;
        cursor = cursor->next;
        free(temp);
        compare++;
    }
    
    // libera la lista etichette
    while (label_cursor != NULL) {
        node* temp = label_cursor;
        label_cursor = label_cursor->next;
        free(temp);
        label_compare++;
    }

    if (count == compare && label_count == label_compare)
        return true;
    else
        return false;
}
