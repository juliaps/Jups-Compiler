/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Adaptado para C- por Júlia P.S.                  */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"
/* counter for variable memory locations */
char *escopo;
int funmain = 0;

void startEscopo(){
    escopo = copyString("global");
}

static void typeError(TreeNode * t, int msg);

/* Procedure traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

/* nullProc is a do-nothing procedure to
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}

/* Procedure insertNode inserts
 * identifiers stored in t into
 * the symbol table
 */
static void insertNode( TreeNode * t)
{ switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { case AssignK:
        if(t->child[1]->kind.exp != OpK && t->child[1]->kind.exp != ConstK)
        {
          if(t->child[1]->kind.exp == AtivK){
            if ((checkType(t->child[0]->attr.name, escopo) != checkType(t->child[1]->attr.name,"global")) && (checkType(t->child[0]->attr.name, escopo)) != -1 && (checkType(t->child[1]->attr.name, "global")) != -1){
               typeError(t, 5);
             }
          }else{
            if ((checkType(t->child[0]->attr.name, escopo) != checkType(t->child[1]->attr.name,escopo)) && (checkType(t->child[0]->attr.name, escopo)) != -1 && (checkType(t->child[1]->attr.name, escopo)) != -1){
               typeError(t, 5);
             }
          }
        }
        break;

        case ParamK: // adiciona os parametros na tabela 
          if(st_lookup(t->attr.name , escopo) == -1){
            //printf("atual escopo: %s\n", escopo);
            st_insert(t->attr.name,t->lineno,location++, escopo, VAR, t->type);
          }
        break;

        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { case VarDeclK:
          if(t->type == Void){
            typeError(t, 6);
          }
          if(strcmp(t->attr.name, "input") == 0 || strcmp(t->attr.name, "output")== 0){
            typeError(t,7);
          }else{
            if (st_lookup(t->attr.name , escopo) == -1){
              st_insert(t->attr.name,t->lineno,location++, escopo, VAR, t->type);
              if(t->size!=1){
                location = (location + t->size) -1;
              }
            }
            else{
              //erro : variavel ja declarada ou declarado como função
              typeError(t, 3);
            }
          }

          break;

        case FunDeclK:
          if(strcmp(t->attr.name, "input") == 0 || strcmp(t->attr.name, "output")== 0){
            typeError(t,7);
          }else{
            if (st_lookup(t->attr.name , "global") == -1){
              st_insert(t->attr.name,t->lineno,location++,"global", FUN, t->type);
              strcpy(escopo,t->attr.name);
              if(!strcmp("main",t->attr.name))
                  funmain = 1;
            }else{
              //erro : funcao ja declarada
              typeError(t, 4);
            }
          }

          break;
        case IdK:
          if (st_lookup(t->attr.name , escopo) == -1){
            //variavel nao declarada
            typeError(t, 1);
          }
          else{
            st_insert(t->attr.name,t->lineno,location, escopo, VAR, t->type);
          }
          break;
        case AtivK:
          if(strcmp(t->attr.name,"output") == 0 || 
             strcmp(t->attr.name,"input") == 0 || 
             strcmp(t->attr.name, "setpc") == 0 ||
             strcmp(t->attr.name, "getpc") == 0 ||
             strcmp(t->attr.name, "cpyrbtomem") == 0 ||
             strcmp(t->attr.name, "getprocstatus") == 0 ||
             strcmp(t->attr.name, "cpymemtorb") == 0) { //ADICIONAR AS NOVAS FUNCOES
                if (st_lookup(t->attr.name , escopo) == -1){
                  //primeira vez que a funcao foi chamada
                  st_insert(t->attr.name, t->lineno, location, escopo, FUN, Void);
                  location++;
                }else{//funcao ja foi chamada entao nao precisa incrementar localizacao na memoria
                  st_insert(t->attr.name, t->lineno, location, escopo, FUN, Void);
                }
          }else{
            if (st_lookup(t->attr.name , escopo) == -1){
              //Erro : função nao declaradas
              typeError(t, 2);
            }else{
                st_insert(t->attr.name,t->lineno,location, escopo, FUN, t->type);
            }
          }

          break;
          /*case TypeK:
            if (st_lookup(t->child[0]->attr.name , escopo) == -1){
              st_insert(t->child[0]->attr.name,t->child[0]->lineno,location++, escopo, VAR, t->type);
              if(t->size!=1){
                location = (location + t->size) -1;
              }
            }
            break;*/
        default:
          break;
      }
      break;
    default:
      break;
  }
}

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */

void checkMain(){
    if(!funmain){
        fprintf(listing,"Função main não declarada! Que absurdo! :0\n\n");
        Error = TRUE;
    }
}

void buildSymtab(TreeNode * syntaxTree)
{ startEscopo();
  traverse(syntaxTree,insertNode,nullProc);
  if (TraceAnalyze)
  { checkMain();
    //fprintf(listing,"\nTabela de Simbolos:\n\n");
    printSymTab(listing);
  }
}

static void typeError(TreeNode * t, int msg)
{
  switch (msg) {
    case 1:
      fprintf(listing, "Erro Semantico na linha %d : Variavel %s não declarada!\n", t->lineno, t->attr.name);
    break;
    case 2:
      fprintf(listing, "Erro Semantico na linha %d : Função %s não declarada!\n", t->lineno, t->attr.name);
    break;
    case 3:
      fprintf(listing, "Erro Semantico na linha %d : Variavel %s já foi declarada ou foi declarado como função!\n", t->lineno, t->attr.name);
    break;
    case 4 :
      fprintf(listing, "Erro Semantico na linha %d : Função %s já foi declarada ou foi declarada como variavél!\n", t->lineno, t->attr.name);
    break;
    case 5 :
      fprintf(listing, "Erro Semantico na linha %d : Atribuição inválida!\n", t->lineno );
    break;
    case 6 :
      fprintf(listing, "Erro Semantico na linha %d : Declaração inválida a varivável %s não pode ser declarada como tipo VOID !!!\n", t->lineno, t->attr.name );
    break;
    case 7 :
      fprintf(listing, "Erro na linha %d : não pode ser declaradas funções/variveis com o nome input ou output elas ja existem\n", t->lineno );

  }
  Error = TRUE;
}
