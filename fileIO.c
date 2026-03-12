/*
thi is a dedicated api for file operations
it has no logic inside, since the logic is provided by the db functions
the functions of this file do only unconditional search/read/write
*/

/*
each table name is paired with an, wich is the index of the line at which starts the table definition
in this way, when a table is needed, it is sufficient to jump at the line offset to find the table
*/
typedef struct pair{
  int id;
  char *name;
}pair;

pair createPair(int id, char *name){
  pair p = {id; name;};
  return p;
}

/*
for this is preferrable to create a dedicated file wich stores only the pairs
this file will be not accessible to the user
*/
pair tables[];
size_t numTables = 0;

pair getTableId(char *name){
  if(numTables == 0)
    return null;  // empty db message
  if(numTables == 1)
    return tables[0];
  for(size_t i = 0; i < numTables; i++){
    int firstChar = name[0] - 65;
    if(firstChar < numTables)
      int offset = numTables / firstChar;
    else if(firstChar == numTables)
      int offset = firstChar;
    else
      int offset = firstChar / numTables;
  }
}
/*
this function is wrapped by the select function, and return to it the list of
lines which correspond to the table
*/
char **dataRead(char *name){
  
  return null;
}

dataWrite();

defRead();
defWrite();
