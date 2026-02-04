//======================================fabbioDB==================================================//
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

//extern FILE *db;

//=========================================struct manipulation functions================================================//

entity *createEntity(char *input, char type){
    entity *e = malloc(sizeof(entity));
    e->type = type;
    switch(type){
        case INT_TYPE:
            e->dbInteger = atoi(input);
            break;
        case FUNC_TYPE:
            e->dbFunc = dbfunc[atoi(input)];
            break;
        case LIST_TYPE:
            e->dbList.size = 0;
            e->dbList.head = NULL;
            e->dbList.tail = NULL;
            break;    
        default:
            e->dbString.str = input;
            e->dbString.len = strlen(input);
    }
    e->next = NULL;
	return e;   
}

// list functions

// removes the element at the head of the list, and frees it
void removel(entity *l){
	if(l->dbList.head == NULL)
		return;
	entity *e = l->dbList.head;
	l->dbList.head = l->dbList.head->next;
	l->dbList.size--;
	if(l->dbList.size == 0)
		l->dbList.tail = NULL;	
	e->next = NULL;
	free(e);
}

// removes the element at the head of the list, and returns it
entity *peekl(entity *l){
	if(l->dbList.head == NULL)
		return NULL;
	entity *e = l->dbList.head;
	if(e->next != NULL)
		l->dbList.head = e->next;
	l->dbList.size--;
	if(l->dbList.size == 0)
		l->dbList.tail = NULL;	
	return e;
}
 

void deleteEntity(entity *e){
	e->next = NULL;
	if(e->type == LIST_TYPE){
		while(e->dbList.size > 0)
			removel(e);
	}
	free(e);
}


void addLastl(entity *l, entity *e){
	if(l->dbList.head == NULL)
		l->dbList.head = e;
	if(l->dbList.tail != NULL)
		l->dbList.tail->next = e;
	l->dbList.tail = e;
	l->dbList.size++;
	//printf("Added element of type: %d\n", e->type);
}


void addFirstl(entity *l, entity *e){
	if(l->dbList.head != NULL)
		e->next = l->dbList.head;
	l->dbList.head = e;
	if(l->dbList.tail == NULL)
		l->dbList.tail = e;
	l->dbList.size++;
	//printf("Added element of type: %d\n", e->type);
}


entity *getl(entity *l, size_t index){
	entity *e = l->dbList.head;
	if(index >= l->dbList.size)
		return NULL;
	for(int i = 0; i <= index; i++){
		if(i == index)
			return e;
		e = e->next;
	}
	return NULL;			
}

parser *createParser(char *line){
    parser *p = malloc(sizeof(parser));
    p->prog = line;
    p->token = line;
    return p;
}



//=========================================program exec================================================//

/* this funciton is used to transform the string of symbols in an array.
Note that the array has to be defined globally, since this function is 
called by fShell.c, but it's ret value is used here.
Also note that commas and semicolons do not have their specific position 
in the array since the array is create by splitting on the spaces,
so comma and semicolon remain binded to the previous string;
this issue is properly handled in the compile function.*/
char **strtoarr(char **arr, char *str){
	size_t size = 10;
	char *token = strtok(str, " ");
	size_t i  = 0;
	while(token != NULL){
		if(i >= size){
			size *= 2;
			char **tmp = realloc(arr, size*sizeof(char*));
			if(!tmp)
				return NULL;
			arr = tmp;
		}
		arr[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	return arr;
}


/* this function checks if a given string is a symbol or not.
If the string is a symbol, the function returns the index of the 
symbols array at which position is stored the string;
if not it returns -1
*/
int fetchSymbols(char *string, char **symbols){
	int i  = 0;
	char *ptr;
	while((ptr = symbols[i]) != NULL) {
   		size_t j = 0;
   		char c1;
   		char c2;
   		while(1){
			c1 = string[j], c2 = symbols[i][j];
			if(c1 == 0 && c2 == 0)
				return i;	
			
			if(c1 != c2)
				break;
     		j++;
  		}	
  		i++;
	}
    return -1;
}

/*this function takes the user input and transforms it in a list of entities each of which of the proper type*/
entity *compile(char *line, char **symbols){
    entity *program = createEntity("", LIST_TYPE);
    parser *p = createParser(line);
    p->token = strtok(p->prog, " ");
    char type;
    int i = 0;
    entity *func = NULL;
    /*since commas and semicolon come binded to the previous string,
	in case the string contains one of them, an extra node is needed
	in order to keep the two elements separated in the list*/
    entity *next = NULL;	
    while(p->token != NULL){
    	size_t len = strlen(p->token);
    	if(len > 1){
    		char *nexttok;
			if(p->token[len-1] == ',' || p->token[len-1] == ';'){
				if(p->token[len-1] == ',')
					nexttok = ",";
				else
					nexttok = ";";
			p->token[len-1] = 0;
			next = createEntity(nexttok, SYM_TYPE);
			}	
		}
    	/* once the comma/semicolon is trimmed out, we check for
    	the type of the node which will correspond to the actual string
    	*/
        int sym = fetchSymbols(p->token, symbols);
		if(sym > -1){
            type = SYM_TYPE;
            if(i == 0)
        		func = createEntity((char*)&sym, FUNC_TYPE);
        }
        else{
            if((int)(*(p->token)) > 47 && (int)(*(p->token)) < 59)
                type = INT_TYPE;
            else
                type = STR_TYPE;
        }
        entity *e = createEntity(p->token, type);
        addLastl(program, e);
        if(next != NULL)
        	addLastl(program, next);
        p->token = strtok(NULL, " ");
    }
    free(p);
    /* note that the function is added at the head of the list because
	in the exec function, it is the first element to check*/
    if(func != NULL)
    	addFirstl(program, func);
    return program;
}

/* this function takes the program list as input, and decides whether to call
the function or not*/
void exec(entity *program){
	if(program == NULL){
		printf("Nothing to execute\n");
		return;
	}
	//printf("%ld", program->dbList.size);
	entity *f = peekl(program);
	//printf("%ld", program->dbList.size);
	if(f->type != FUNC_TYPE){
		printf("No function to execute\n");
		return;
	}
	entity *sCol = getl(program, program->dbList.size-1);
	//printf("Semicolon: %c", sCol->dbString.str[0]);
	if(sCol->type == SYM_TYPE && sCol->dbString.str[0] == ';'){
		entity *result = f->dbFunc(program);
	}
		
	else
		printf("Complete statement...\n");
}
/*
void exec(entity *program){
	entity *e = program->dbList.head;
	for(size_t i = 0; i < program->dbList.size; i++){
		switch(e->type){
			case INT_TYPE:
				printf(YELLOW "INT: %d\n" RESET, e->dbInteger);
				break;
			case SYM_TYPE:
				printf(GREEN "SYM: %s\n" RESET, e->dbString.str);
				break;
			case STR_TYPE:
				printf("STR: %s\n", e->dbString.str);	
				break;
		}	
		e = e->next;
	}
}*/

//==============================================db functions==================================//

entity *dbUse(entity *prog){
	if(prog->dbList.size < 3){
		printf("Missing arguments\n");
		return NULL;
	}
	if(prog->dbList.size > 3){
		//printf("%ld", prog->dbList.size);
		printf("Too many arguments\n");
		return NULL;
	}
	entity *name = getl(prog, 1);
	if(name->type == STR_TYPE){
		DIR *dir = opendir(name->dbString.str);
    	if (dir == NULL) {
        		printf("No existing schema with this name\n");
        		return NULL;
    	}
		
		printf("Using database '%s'\n", name->dbString.str);

    	closedir(dir);
	}
	//printf("Open db name\n");
	return NULL;
}

// for later
entity *dbSelect(entity *prog){
	printf("Select function\n");
	
	if(prog->dbList.size < 4){
		printf("Missing arguments\n");
		return NULL;
	}
	entity *next = prog->dbList.head;
	char *tName = NULL;
	for(size_t i = 0; i < prog->dbList.size; i++){
		next = next->next;
	}
	if(head->next)
	
	return e;
}
