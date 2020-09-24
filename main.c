#include "parser/lexer.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{	
	if(argc < 2) 
	{
		printf("Error: No file specified.\n");
		return 1;
	}

	FILE *fptr;
	long numbytes;
	char* buffer;
	
	if ((fptr = fopen(argv[1],"r")) == NULL)
	{
		printf("Error: Couldn't read from file.\n");
		return 1;
	}
	
	fseek(fptr, 0, SEEK_END);
	
	numbytes = ftell(fptr);
	
	fseek(fptr, 0, SEEK_SET);
	
	buffer = (char*)calloc(numbytes, sizeof(char));	
	
	if(buffer == NULL)
	{	
		printf("Eror: Couldn't allocate filestream.\n");
		return 1;
	}
	
	fread(buffer, sizeof(char), numbytes, fptr);
		
	fclose(fptr);
	
	printf("Compiling %s...\n", argv[1]);
	
	lex_state* state = make_lex_state(buffer);
	
	tokenize(state);
	
	free_lex_state(state);
	
	return 0;
}