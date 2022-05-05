#ifndef STRMANAGE_H_
#define STRMANAGE_H_

#include <stdbool.h>

 //controlla se la stringa data vuota (cioè riga oriva di codice)
bool s_is_empty(const char *s);

 
 //suddivide in comando vm, fornito sotto forma di stringa, in sottostringhe.
 //Ogni sotto-stringa è inserita nell'array "sub_str". In questo caso viene considerato lo spazio vuoto " " come separatoree delle nostre future sotto-stringhe.
 //Ritorna il numero di sotto-stringhe
int s_splitting(char *s, char *sub_str[], int max_s, const char *borders);

/*
 * Remove the extension from the given filename / path.
 * Expect a mutable c-string as input.
 */
//rimuove l'estensione del file. In questo caso .vm
char *remove_ext(char *s);

//funzione che cerca un carattere in una stringa: se lo trova restituisce il puntatore a quel carattere, altrimenti NULL
char* findchr(const char* str, char c);

#endif /* STRMANAGE_H_ */