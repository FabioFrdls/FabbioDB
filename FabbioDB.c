#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INT_TYPE 0
#define SYM_TYPE 1
#define STR_TYPE 2

typedef struct entity {
	int type;
	union {
		int dbInteger;

		struct dbSymbol {
			char *sym;
			int len;
		} dbSymbol;

		struct dbString {
			char *str;
			int len;
		} dbString;
	};
} entity;


typedef struct parser {
	char *ptr;
	char *symbols[];
} parser;

typedef struct dataParser {
	parser *p;
	entity **tokens;
} dataParser;

typedef struct exeContext{

}exeContext; 

entity *createEntity(char *input, int type){
    entity *e = malloc(sizeof(entity));
    e->type = type;
    switch(type){
        case INT_TYPE:
            e->dbInteger = atoi(input);
            break;
        case STR_TYPE:
            e->dbString.str = input;
            break;
        case SYM_TYPE:
            e->dbSymbol.sym = input;
            break;
    }   
}

parser *createCmdParser(char *line){
    parser *p = malloc(sizeof(parser));
    p->ptr = line;
    p->symbols = {"USE", "SELECT", "FROM", ";"};
    return p;
}

dataParser *createDataParser(char *fptr){
    dataParser *p = malloc(sizeof(dataParser));
    p->p->ptr = fptr;
    p->p->symbols = {"<", ">", "/","NAME", "TABLES_DEF", "TABLES_DATA"};
    return p;
}

int fetchSymbols(char *string, char *symbols[]){
	int i = 0;
	while(symbols) {
   	int j = 0;
   	while(string) {
    	if(string[j] != symbols[i] [j]) 
        	break;
     	if(string[j] == 0 & & symbols[i] [j] == 0)
        	return 1;
     	i++;
  	} 
  	j++;
} 
    
    return 0;
}

entity **compile(char *line){
    parser *p = createCmdParser(line);
    p->ptr = strtok(p->ptr, " ");
    int i = 0;
    entity **tokens = malloc(sizeof(entity*));
    int type;
    while(p->ptr){
        int bo = fetchSymbols(p->ptr, p->symbols);
        if(bo){
            type = SYM_TYPE;
        }
        else{
            if((int)(*p->ptr) > 47 && (int)(*p->ptr) < 59)
                type = INT_TYPE;
            else
                type = STR_TYPE;
        }
        entity *e = createEntity(p->ptr, type);
        tokens[i] = e;
        i++;
        p->ptr = strtok(NULL, " ");
    }
    return tokens;
}

void exec(entity **program);

int main(char **argv, int argc) {
    exec(compile(*argv));
	return 0;
}

