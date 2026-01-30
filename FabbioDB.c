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

entity *compile1(char *line, char **symbols){
    entity *program = createEntity("", LIST_TYPE);
    parser *p = createParser(line);
    p->token = strtok(p->prog, " ");
    char type;
    int i = 0;
    entity *func = NULL;
    entity *next = NULL;		// for colon and semicolon
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
    if(func != NULL)
    	addFirstl(program, func);
    return program;
}

entity *compile2(entity *program){
	
	
	return program;
}

void exec1(entity *program){
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
}

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
		FILE *db = fopen(name->dbString.str, "r+");
		if(db == NULL)
			printf("No existing schema with this name\n");
	}
	//printf("Open db name\n");
	return NULL;
}

entity *dbSelect(entity *e){
	printf("Select function\n");
	return e;
}
