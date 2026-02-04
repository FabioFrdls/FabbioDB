//======================================fShell==================================================//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

char cmdSymbols[] = "USE SELECT FROM , ;";


int main(int argc, char **argv){
    char buff[1024];
	
	char **cmdSym = malloc(sizeof(char*)*10);
	char *copy = strdup(cmdSymbols);
	cmdSym = strtoarr(cmdSym, copy);
	
    while (1) {
        printf(CYAN "fsql>> " RESET);
        fflush(stdout);
		char *line = fgets(buff, sizeof buff, stdin);
		line[strcspn(line, "\n")] = '\0';
			
		if (strcmp(line, "exit") == 0){
			printf(CYAN "Bye\n" RESET);
			free(cmdSym);
			return 0;
		}
		entity *prog = compile(line, cmdSym);
    	exec(prog);
    }
	free(cmdSym);
    return 0;
}

