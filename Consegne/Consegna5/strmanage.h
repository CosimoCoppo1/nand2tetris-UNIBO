#ifndef STRMANAGE_H
#define STRMANAGE_H

//funzione che cerca un carattere in una stringa: se lo trova restituisce il puntatore a quel carattere, altrimenti NULL
char* findchr(const char* str, char c);

//funzione che concatena tre stringhe ad una quarta
void strconcat(char *dest, const char *s1, const char *s2, const char *s3);

//funzione che converte un numero decimale in una stringa binaria da 15-bit (realizzata tramite pattern)
void dectobinary(int num, char* string);




#endif // STRMANAGE_H