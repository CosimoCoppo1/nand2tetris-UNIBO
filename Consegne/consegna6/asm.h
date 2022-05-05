#ifndef ASM_H_
#define ASM_H_
//ogni comando vm corrisponde a una serie di comandi assembler che saranno sempre gli stessi a meno di alcuni identificatori (che verranno sostituiti in fase di esecuzione)
//Di conseguenza si (pre)definisce lo schema di istruzioni asm che corrisponde a ogni comando vm. 
//Quando quest'ultimo viene identificato, nel file output viene semplicemente incollato tale schema predefinito con i pochi valori sostituiti.  
const char* ASM_INITIAL="@256\n" "D=A\n" "@SP\n" "M=D\n";

const char* ASM_PUSH_CONST="@%d\n" "D=A\n" "@SP\n" "AM=M+1\n" "A=A-1\n" "M=D\n";

const char* ASM_PUSH_STATIC="@%s.%d\n" "D=M\n" "@SP\n" "AM=M+1\n" "A=A-1\n" "M=D\n";

// local, argument
const char* ASM_PUSH_LA="@%d\n" "D=A\n" "@%s\n" "A=M\n" "A=D+A\n" "D=M\n" "@SP\n" "AM=M+1\n" "A=A-1\n" "M=D\n";

const char* ASM_POP_STATIC="@SP\n" "AM=M-1\n" "D=M\n" "@%s.%d\n" "M=D\n";

const char* ASM_POP_LA="@%s\n" "D=M\n" "@%d\n" "D=D+A\n" "@R13\n" "M=D\n" "@SP\n" "AM=M-1\n" "D=M\n" "@R13\n" "A=M\n" "M=D\n";

const char* ASM_ADD="@SP\n" "AM=M-1\n" "D=M\n" "A=A-1\n" "M=D+M\n";

const char* ASM_SUB="@SP\n" "AM=M-1\n" "D=M\n" "A=A-1\n" "M=M-D\n";

const char* ASM_NEG="@SP\n" "A=M-1\n" "M=-M\n";

const char* ASM_AND="@SP\n" "AM=M-1\n" "D=M\n" "A=A-1\n" "M=D&M\n";

const char* ASM_OR="@SP\n" "AM=M-1\n" "D=M\n" "A=A-1\n" "M=D|M\n";

const char* ASM_NOT="@SP\n" "A=M-1\n" "M=!M\n";

const char* ASM_EQ="@SP\n" "AM=M-1\n" "D=M\n" "A=A-1\n" "D=M-D\n" "M=-1\n" "@EQ_LBL_%d\n" "D;JEQ\n" "@SP\n" "A=M-1\n" "M=0\n" "(EQ_LBL_%d)\n";

const char* ASM_GT="@SP\n" "AM=M-1\n" "D=M\n" "A=A-1\n" "D=M-D\n" "M=-1\n" "@GT_LBL_%d\n" "D;JGT\n" "@SP\n" "A=M-1\n" "M=0\n" "(GT_LBL_%d)\n";

const char* ASM_LT="@SP\n" "AM=M-1\n" "D=M\n" "A=A-1\n" "D=D-M\n" "M=-1\n" "@END\n" "@LT_LBL_%d\n" "D;JGT\n" "@SP\n" "A=M-1\n" "M=0\n" "(LT_LBL_%d)\n";

const char* ASM_LABEL="(%s$%s)\n";

const char* ASM_GOTO="@%s$%s\n" "0;JMP\n";

const char* ASM_IFGOTO="@SP\n" "AM=M-1\n" "D=M\n" "@%s$%s\n" "D=D;JNE\n";

const char* ASM_RETURN="@LCL\n" "D=M\n" "@R13\n" "M=D\n" "@5\n" "D=D-A\n" "A=D\n" "D=M\n" "@R14\n" "M=D\n" "@SP\n" "A=M-1\n" "D=M\n" "@ARG\n" "A=M\n" "M=D\n" "@ARG\n" \
                       "D=M+1\n" "@SP\n" "M=D\n" "@R13\n" "AM=M-1\n" "D=M\n" "@LBL_B\n" "M=D\n" "@R13\n" "AM=M-1\n" "D=M\n" "@LBL_A\n" "M=D\n" "@R13\n" "AM=M-1\n" \
					   "D=M\n" "@ARG\n" "M=D\n" "@R13\n" "AM=M-1\n" "D=M\n" "@LCL\n" "M=D\n" "@R14\n" "A=M\n" "0;JMP\n";

const char* ASM_CALL="@SP\n" "D=M\n" "@R13\n" "M=D\n" "@RETURN_LABEL$%d\n" "D=A\n" "@SP\n" "AM=M+1\n" "A=A-1\n" "M=D\n" "@LCL\n" "D=M\n" "@SP\n" "AM=M+1\n" "A=A-1\n" \
                     "M=D\n" "@ARG\n" "D=M\n" "@SP\n" "AM=M+1\n" "A=A-1\n" "M=D\n" "@LBL_A\n" "D=M\n" "@SP\n" "AM=M+1\n" "A=A-1\n" "M=D\n" "@LBL_B\n" "D=M\n" "@SP\n" \
					 "AM=M+1\n" "A=A-1\n" "M=D\n" "@R13\n" "D=M\n" "@%d\n" "D=D-A\n" "@ARG\n" "M=D\n" "@SP\n" "D=M\n" "@LCL\n" "M=D\n" "@%s\n" "0;JMP\n" "(RETURN_LABEL$%d)\n";



#endif /* ASM_H_ */
