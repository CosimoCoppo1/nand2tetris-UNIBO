#ifndef PARSER_H_
#define PARSER_H_

#include <stdbool.h>

#define MAX_LINE_LEN 200
//massimo numero di caratteri generati dall'utput assembler
#define MAX_ASM_OUT  2000
//massimo numero di sotto-stringhe che compone un'istruzione asm
#define MAX_SUB_STRGS 3
#define MAX_FNAME_CHARS 150

//identifica che tipo di comando vm stiamo trattando nella riga corrente. Con una piccola sistemazione del codice questa funzione può essere evitata. (diciamo che è un po' ridondante)
char* str_to_cmdid(const char *s);

//rimuove i commenti dalla riga ed i withespaces
char *strip_comments(char *s);

//funzioni che corrispondono ad ogni comando vm. "Incollano" il corrispondente codice asm.  
bool parser_push(int nargs, const char *args[nargs], char *output);

bool parser_pop(int nargs, const char *args[nargs], char *output);

bool parser_add(int nargs, char *output);

bool parser_sub(int nargs, char *output);

bool parser_neg(int nargs, char *output);

bool parser_and(int nargs, char *output);

bool parser_or(int nargs, char *output);

bool parser_eq(int nargs, char *output);

bool parser_gt(int nargs, char *output);

bool parser_lt(int nargs, char *output);

bool parser_label(int nargs, const char *args[nargs], char *output);

bool parser_goto(int nargs, const char *args[nargs], char *output);

bool parser_ifgoto(int nargs, const char *args[nargs], char *output);

bool parser_function(int nargs, const char *args[nargs], char *output);

bool parser_call(int nargs, const char *args[nargs], char *output);

bool parser_return(int nargs, char *output);

//funzione che in base a "id" riconosce il tipo di comando vm corrente e chiama la giusta funzione tra quelle precedenti  
bool parser_fn(char* id, int n, const char *arg[n], char *out);

//inizializzazione puntatore SP
void initializing_code(char *output);

#endif /* PARSER_H_ */