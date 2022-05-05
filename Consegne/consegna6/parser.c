#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "asm.h"
#include "strmanage.h"
#include "parser.h"

int eq_label_counter = 0;
int gt_label_counter = 0;
int lt_label_counter = 0;
int return_label_counter = 0;

//nella riornagnizzazione del codice nei vari file (per rendere il programma più ordinato), per necessità (e semplicità) è stata introdotta
//la parola chiave "extern" per queste due variabili
extern char fname_noext[MAX_FNAME_CHARS+1];
extern char current_fun[MAX_FNAME_CHARS+1];

char* str_to_cmdid(const char *s)
{
    char* id ="CMD_INVALID";

    if (s == NULL) {
    } else if (!strcmp(s, "push")) {
        id = "CMD_PUSH";
    } else if (!strcmp(s, "pop")) {
        id ="CMD_POP";
    } else if (!strcmp(s, "add")) {
        id ="CMD_ADD";
    } else if (!strcmp(s, "sub")) {
        id ="CMD_SUB";
    } else if (!strcmp(s, "neg")) {
        id ="CMD_NEG";
    } else if (!strcmp(s, "and")) {
        id ="CMD_AND";
    } else if (!strcmp(s, "or")) {
        id ="CMD_OR";
    } else if (!strcmp(s, "not")) {
        id ="CMD_NOT";
    } else if (!strcmp(s, "eq")) {
        id ="CMD_EQ";
    } else if (!strcmp(s, "gt")) {
        id ="CMD_GT";
    } else if (!strcmp(s, "lt")) {
        id ="CMD_LT";
    } else if (!strcmp(s, "label")) {
        id ="CMD_LABEL";
    } else if (!strcmp(s, "goto")) {
        id ="CMD_GOTO";
    } else if (!strcmp(s, "if-goto")) {
        id ="CMD_IFGOTO";
    } else if (!strcmp(s, "function")) {
        id ="CMD_FUNCTION";
    } else if (!strcmp(s, "return")) {
        id ="CMD_RETURN";
    } else if (!strcmp(s, "call")) {
        id ="CMD_CALL";
    }

    return id;
}


char *strip_comments(char *s)
{
    if (s == NULL) {
        return NULL;
    } else if (*s=='\0') {
        return s;
    }

    char *last_char = s;

    for (char *s2 = s; *s2!='\0'; s2++) {
        if (*s2 == '/' && *(s2+1) == '/') {
            *s2 = '\0';
            break;
        } else if (!isspace(*s2)) {
            last_char = s2;
        }
    }

    if (*last_char) {
        *(last_char + 1) = '\0';
    }

    return s;
}


bool parser_push(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 3) {
        return false;
    }

    int i = atoi(args[2]);

    if ( !args[2] || i < 0) {
        return false;
    }

    if (!strcmp(args[1], "constant")) {
        sprintf(output, ASM_PUSH_CONST, i);
    } else if (!strcmp(args[1], "static")) {
        sprintf(output, ASM_PUSH_STATIC, fname_noext, i);
    } else if (!strcmp(args[1], "local")) {
        sprintf(output, ASM_PUSH_LA, i, "LCL");
    } else if (!strcmp(args[1], "argument")) {
        sprintf(output, ASM_PUSH_LA, i, "ARG");
    } else {
        return false;
    }

    return true;
}

bool parser_pop(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 3) {
        return false;
    }

    int i = atoi(args[2]);

    if (!args[2] || i < 0) {
        return false; 
    }

    if (!strcmp(args[1], "static")) {
        sprintf(output, ASM_POP_STATIC, fname_noext, i);
    } else if (!strcmp(args[1], "local")) {
        sprintf(output, ASM_POP_LA, "LCL", i);
    } else if (!strcmp(args[1], "argument")) {
        sprintf(output, ASM_POP_LA, "ARG", i);
    } else {
        return false;
    }

    return true;
}

bool parser_add(int nargs, char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_ADD);
    return true;
}

bool parser_sub(int nargs, char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_SUB);
    return true;
}

bool parser_neg(int nargs, char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_NEG);
    return true;
}

bool parser_and(int nargs, char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_AND);
    return true;
}

bool parser_or(int nargs, char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_OR);
    return true;
}

bool parser_not(int nargs, char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_NOT);
    return true;
}

bool parser_eq(int nargs, char *output)
{
    if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_EQ, eq_label_counter, eq_label_counter);
    eq_label_counter++;
    return true;
}

bool parser_gt(int nargs, char *output)
{
	if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_GT, gt_label_counter, gt_label_counter);
    gt_label_counter++;
    return true;
}

bool parser_lt(int nargs, char *output)
{
    if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_LT, lt_label_counter, lt_label_counter);
    lt_label_counter++;
    return true;
}

bool parser_label(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 2) {
        return false;
    }
    sprintf(output, ASM_LABEL, current_fun, args[1]);
    return true;
}

bool parser_goto(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 2) {
        return false;
    }
    sprintf(output, ASM_GOTO, current_fun, args[1]);
    return true;
}

bool parser_ifgoto(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 2) {
        return false;
    }
    sprintf(output, ASM_IFGOTO, current_fun, args[1]);
    return true;
}

bool parser_function(int nargs, const char *args[nargs], char *output)
{
	const char *added_line[]={ "push", "constant", "0" };

    if (nargs != 3) {
        return false;
    }

    int nvars = atoi(args[2]);
    char tmp_output[MAX_ASM_OUT+1];

    if ( !args[2] || nvars < 0) {
        return false; 
    }

    strcpy(current_fun, args[1]);

    strcpy(output, "(");
    strcat(output, args[1]);
    strcat(output, ")\n");

    for (int i = 0; i < nvars; i++) {
        parser_push(3, added_line, tmp_output);
        strncat(output, tmp_output, MAX_ASM_OUT);
        output[MAX_ASM_OUT] = '\0';
    }

    return true;
}

bool parser_call(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 3) {
        return false;
    }

    int i = atoi(args[2]);

    if (!args[2] || i < 0) {
        return false; 
    }

    sprintf(output, ASM_CALL, return_label_counter, i, args[1], return_label_counter);
    return_label_counter++;

    return true;
}

bool parser_return(int nargs, char *output)
{
	if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_RETURN);
    return true;
}

bool parser_fn(char* id, int n, const char *arg[n], char *out)
{

	bool valid=false;

    if (!strcmp(id, "CMD_PUSH")) {
		valid=parser_push(n, (const char **) arg, out);

    } else if (!strcmp(id, "CMD_POP")) {
        valid=parser_pop(n, (const char **) arg, out);

    } else if (!strcmp(id, "CMD_ADD")) {
        valid=parser_add(n, out);

    } else if (!strcmp(id, "CMD_SUB")) {
        valid=parser_sub(n, out);

    } else if (!strcmp(id, "CMD_NEG")) {
        valid=parser_neg(n, out);

    } else if (!strcmp(id, "CMD_AND")) {
        valid=parser_and(n, out);

    } else if (!strcmp(id, "CMD_OR")) {
        valid=parser_or(n, out);

    } else if (!strcmp(id, "CMD_NOT")) {
        valid=parser_not(n, out);

    } else if (!strcmp(id, "CMD_EQ")) {
        valid=parser_eq(n, out);

    } else if (!strcmp(id, "CMD_GT")) {
        valid=parser_gt(n, out);

    } else if (!strcmp(id, "CMD_LT")) {
        valid=parser_lt(n, out);

    } else if (!strcmp(id, "CMD_LABEL")) {
        valid=parser_label(n, (const char **) arg, out);

    } else if (!strcmp(id, "CMD_GOTO")) {
        valid=parser_goto(n, (const char **) arg, out);

    } else if (!strcmp(id, "CMD_IFGOTO")) {
        valid=parser_ifgoto(n, (const char **) arg, out);

    } else if (!strcmp(id, "CMD_FUNCTION")) {
        valid=parser_function(n, (const char **) arg, out);

    } else if (!strcmp(id, "CMD_RETURN")) {
        valid=parser_return(n, out);

    } else if (!strcmp(id, "CMD_CALL")) {
        valid=parser_call(n, (const char **) arg, out);
    }

	return valid;
}

void initializing_code(char *output)
{
    sprintf(output, ASM_INITIAL);
    output[MAX_ASM_OUT] = '\0';
}