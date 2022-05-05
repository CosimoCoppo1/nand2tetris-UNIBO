#nome dell'eseguibile: "assembler"
#l'assemblatore è fatto linkando i seguenti file oggetto
assembler	:	main.o	parser.o	strmanage.o
				gcc -o assembler main.o parser.o strmanage.o

#il file oggetto main.o viene ricompilato se cambia parser.c oppure strmanage.c			   
main.o	:	parser.c	parser.h	strmanage.c	strmanage.h
			gcc -c main.c

#il file oggetto parser.o viene ricompilato se cambia parser.c oppure strmanage.c			
parser.o	:	parser.c	parser.h	strmanage.c	strmanage.h
				gcc	-c	parser.c
		  
#il file oggetto strmanage.o viene ricompilato se cambia strmanage.c			
strmanage.o	:	strmanage.c	strmanage.h
				gcc	-c	strmanage.c
		  
#rimozione dei file oggetto e dell'eseguibile compilato		  
clean	:	
		rm -f *.o assembler