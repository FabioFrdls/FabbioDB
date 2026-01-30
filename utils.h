
#define INT_TYPE 0
#define SYM_TYPE 1
#define STR_TYPE 2
#define LIST_TYPE 3
#define FUNC_TYPE 4

#define RESET   "\033[0m"
#define CYAN   "\033[36m"
#define YELLOW   "\033[33m"
#define GREEN   "\033[32m"
//======================================data structs==================================================//

static char cmdSymbols[] = {"USE SELECT FROM , ;"};
//FILE *db;

typedef struct entity {
	char type;
	struct entity *next;		// extra field for using entity as a node for a struct
	union {
		int dbInteger;

		struct{
			char *str;
			size_t len;
		} dbString;
		
		struct{
			size_t size;
			struct entity *head;
			struct entity *tail;
		}dbList;
		
		struct entity *(*dbFunc)(struct entity *program);
	};
} entity;



typedef struct parser {	
	char *prog;
	char *token;
}parser;

//======================================functions==================================================//



entity *dbUse(entity *e);
entity *dbSelect(entity *e);
static entity *(*dbfunc[])() = {dbUse, dbSelect};

void removel(entity *e);
void addl(entity *l, entity *e);
void deleteEntity(entity *e);
entity *createEntity(char *input, char type);
parser *createParser(char *line);
char **strtoarr(char **arr, char *str);
int fetchSymbols(char *string, char **symbols);
entity *compile1(char *line, char **symbols);
void exec(entity *program);
void exec1(entity *program);
