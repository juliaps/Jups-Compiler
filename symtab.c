/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Adaptado para C- por Júlia P.S.                  */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "util.h"
/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* the list of line numbers of the source
 * code in which a variable is referenced
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

/* The record in the bucket lists for
 * each variable, including name,
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec
   { char * name;
     LineList lines;
     int memloc ; /* memory location for variable */
     int params ;
     char * escopo;
     IDTypes tipoID;
     ExpType tipoDado;
     struct BucketListRec * next;
   } * BucketList;

/* the hash table */
static BucketList hashTable[SIZE];

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert( char * name, int lineno, int loc, char * escopo, IDTypes tipoID, ExpType tipoDado)
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && !((strcmp(name,l->name) == 0) && ((strcmp(escopo,l->escopo) == 0) || (strcmp(l->escopo, "global") == 0))))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->escopo = copyString(escopo);
    l->tipoID = tipoID;
    l->tipoDado = tipoDado;
    l->lines->next = NULL;
    l->next = hashTable[h];
    hashTable[h] = l; }
  else /* found in table, so just add line number */
  { LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
} /* st_insert */

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup ( char * name , char * escopo )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && !((strcmp(name,l->name) == 0) && ((strcmp(escopo,l->escopo) == 0) || (strcmp(l->escopo, "global") == 0))))
    l = l->next; //if(l != NULL)printf("variavel : %s ", l->name);
  if (l == NULL) return -1;
  else return l->memloc;
}

int checkType ( char * name , char * escopo )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && !((strcmp(name,l->name) == 0) && ((strcmp(escopo,l->escopo) == 0) || (strcmp(l->escopo, "global") == 0))))
    l = l->next;
  if (l == NULL) return -1;
  else return l->tipoDado;//0 void 1 int
}

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */

void update_function(char * name, int qtde){
  int h = hash(name);
  hashTable[h]->params = qtde;
}

int params(char * name){
  int h = hash(name);
  BucketList l =  hashTable[h];
  return l->params;
}
 char *qualESeuTipo(ExpType tipo){
   if(tipo == Integer)
     return "Integer";
    else return "Void";
}

char *qualESeuTipoID(IDTypes tipo){
  if(tipo == VAR)
    return "Var";
  else return "Fun";
}

int getQuantidadeParametros(TreeNode * functionNode) {
    int qtd = 0;
    TreeNode * temp = functionNode->child[0];
    if(temp != NULL) {
        ++qtd;
        while(temp->sibling != NULL) {
            temp = temp->sibling;
            ++qtd;
        }
    }
    return qtd;
}
TreeNode * getArgParametrosParametros(TreeNode * functionNode, int narg){
    TreeNode * temp = functionNode->child[0];
    for(int i = 0;  i< narg; i++){
      temp = temp->sibling;
    }
    return temp;
}
void printSymTab(FILE * listing)
{ int i;
  fprintf(listing,"Nome da Variavel  Localizaçao  Escopo          TipoID   Tipo Dado  Parametros  Numeros das linhas\n");
  fprintf(listing,"----------------- -----------  ------          ------   ---------- ----------- -------------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-17s ",l->name);
        fprintf(listing,"%-12d  ",l->memloc);
        fprintf(listing,"%-16s ",l->escopo);
        fprintf(listing,"%-8s ",qualESeuTipoID(l->tipoID));
        fprintf(listing,"%-10s ",qualESeuTipo(l->tipoDado));
        fprintf(listing, "%-10d ", l->params );
        while (t != NULL)
        { fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
} /* printSymTab */
