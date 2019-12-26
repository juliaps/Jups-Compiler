/****************************************************/
/* File: tiny.y                                     */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Adaptado para C- por Júlia P.S.                  */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static TreeNode * editTree; /* arvore para gerar o codigo intermediario*/
static int yylex(void);
int yyerror(char * message);

%}

%token IF ELSE WHILE RETURN VOID INT
%token ID NUM
%token ASSIGN ACOL FCOL EQ LT PLUS MINUS TIMES OVER LPAREN RPAREN SEMI VIR ACH FCH LR MENEQ MAIEQ DIF
%token ERROR

%% /* Grammar for C- */

programa     : decl_list{
                  savedTree = $1;
                  editTree = $1;
             }
            ;

decl_list : decl_list decl
          {
            YYSTYPE t = $1;
              if (t != NULL){
                while (t->sibling != NULL)
                   t = t->sibling;
                t->sibling = $2;
                $$ = $1;
              }
              else $$ = $2;

          }
        | decl { $$ = $1; }
        ;

decl      : var_decl { $$ = $1 ; }
          | fun_decl { $$ = $1 ; }
          ;

var_decl  : INT identificador SEMI
            { $$ = newExpNode(TypeK);
              $$->attr.name = "INT";
              $$->size = 1;
              $$->child[0] = $2;
              $2->kind.exp = VarDeclK;
              $2->type = Integer;
              $2->size = $$->size;

            }
            | INT identificador ACOL numero FCOL SEMI
              { $$ = newExpNode(TypeK);
                $$->attr.name = "INT";
                $$->size = $4->attr.val;
                $$->child[0] = $2;
                $2->kind.exp = VarDeclK;
                $2->type = Integer; //criar um tipo pro vetor
                $2->size = $$->size;
              }
            | VOID identificador SEMI
              { $$ = newExpNode(TypeK);
                $$->attr.name = "VOID";
                $$->size = 1;
                $$->child[0] = $2;
                $2->kind.exp = VarDeclK;
                $2->type = Void;
               }
            | VOID identificador ACOL numero FCOL SEMI
              { $$ = newExpNode(TypeK);
                $$->attr.name = "VOID";
                $$->size = $4->attr.val;
                $$->child[0] = $2;
                $$->kind.exp = VarDeclK;
                $2->type = Void;
               }
            ;

tipo_espec  : INT
              {
                $$ = newExpNode(TypeK);
                $$->attr.name = "INT";
                $$->type = Integer;
                $$->size = 1;
              }
            | VOID
              {
                $$ = newExpNode(TypeK);
                $$->attr.name = "VOID";
                $$->type = Void;
                $$->size = 1;
              }
;

fun_decl : INT identificador LPAREN params RPAREN composto_decl
            {
              $$ = newExpNode(TypeK);
              $$->attr.name = "INT";
              $$->child[0] = $2;
              $2->kind.exp = FunDeclK;
              $2->lineno = $$->lineno;
              $2->type = Integer;
              $2->child[0] = $4;
              $2->child[1] = $6;
            }
        | VOID identificador LPAREN params RPAREN composto_decl
           {
              $$ = newExpNode(TypeK);
              $$->attr.name = "VOID";
              $$->child[0] = $2;
              $2->type = Void;
              $2->kind.exp = FunDeclK;
              $2->lineno = $$->lineno;
              $2->child[0] = $4;
              $2->child[1] = $6;

            }
        ;


params : param_list
          { $$ = newStmtNode(ParamK);
            $$->kind.stmt = ParamK;
            $$ = $1;
          }
       | VOID
          {
            $$ = newExpNode(TypeK);
            $$->attr.name = "VOID";
            $$->size = 1;
            $$->child[0] = NULL;
          }
       ;

param_list : param_list VIR param
            { YYSTYPE t = $1;
                if (t != NULL){
                  while (t->sibling != NULL)
                       t = t->sibling;
                  t->sibling = $3;
                  $$ = $1;
                }
                else $$ = $3;
            }

            | param {$$ = $1 ;}

            ;

param : tipo_espec identificador
        {
          $$ = $1;
          $$->child[0] = $2;
          $2->nodekind = StmtK;
          //$2->kind.exp = VarDeclK;
          $2->kind.stmt = ParamK;
          $2->type = Integer;
        }
      | tipo_espec identificador ACOL FCOL
        {
          $$ = $1;
          $$->size = 0;
          $$->child[0] = $2;
          $2->nodekind = StmtK;
          //$2->kind.exp = VarDeclK;
          $2->kind.stmt = ParamK;
          $2->type = Integer;
        }
      ;

composto_decl : ACH local_decl stmt_list FCH
              {
                YYSTYPE t = $2;
                  if (t != NULL){
                    while (t->sibling != NULL)
                       t = t->sibling;
                    t->sibling = $3;
                    $$ = $2;
                  }
                  else $$ = $3;
              }
             | ACH FCH {}
             | ACH  local_decl FCH { $$ = $2; }
             | ACH  stmt_list FCH { $$ = $2; }
             ;

local_decl : local_decl var_decl
            {
              YYSTYPE t = $1;
                if (t != NULL){
                  while (t->sibling != NULL)
                     t = t->sibling;
                  t->sibling = $2;
                  $$ = $1;
                }
                else $$ = $2;
            }
          | var_decl { $$ = $1; }
          ;

stmt_list : stmt_list stmt
            {
              YYSTYPE t = $1;
              if (t != NULL){
                while (t->sibling != NULL)
                t = t->sibling;
                t->sibling = $2;
                $$ = $1;
              }
              else $$ = $2;
            }
          | stmt { $$ = $1; }
          ;


stmt : exp_decl { $$ = $1; }
     | composto_decl { $$ = $1; }
     | selecao_decl { $$ = $1; }
     | iteracao_decl { $$ = $1; }
     | retorno_decl { $$ = $1; }
     ;

exp_decl : exp SEMI { $$ = $1; }
        |  SEMI {}
        ;


selecao_decl : IF LPAREN exp RPAREN stmt
               { $$ = newStmtNode(IfK);
                 $$->child[0] = $3;
                 $$->child[1] = $5;
               }
               | IF LPAREN exp RPAREN stmt ELSE stmt
               { $$ = newStmtNode(IfK);
                 $$->child[0] = $3;
                 $$->child[1] = $5;
                 $$->child[2] = $7;
               }
               ;
iteracao_decl : WHILE LPAREN exp RPAREN stmt
                { $$ = newStmtNode(WhileK);
                  $$->child[0] = $3;
                  $$->child[1] = $5;
                }
                ;

retorno_decl : RETURN SEMI {$$ = newStmtNode(ReturnK);}
               | RETURN exp SEMI
                 { $$ = newStmtNode(ReturnK);
                   $$->child[0] = $2;
                 }
               ;

exp : var ASSIGN exp
      {
        $$ = newStmtNode(AssignK);
        $$->child[0] = $1;
        $$->child[1] = $3;
      }
    | simples_exp { $$ = $1; }
    ;

var : identificador {$$ = $1;}
      | identificador ACOL exp FCOL
        { $$ = $1;
          $$->type = Vector;
          $$->child[0] = $3;
        }
      ;

simples_exp : soma_exp relacional soma_exp
              { $$ = $2;
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
              | soma_exp {$$ = $1; }
              ;

relacional : EQ
              { $$ = newExpNode(OpK);
                $$->attr.op = EQ;
              }
              | LT
              { $$ = newExpNode(OpK);
                $$->attr.op = LT;
              }
              | LR
              { $$ = newExpNode(OpK);
                $$->attr.op = LR;
              }
              | MAIEQ
              { $$ = newExpNode(OpK);
                $$->attr.op = MAIEQ;
              }
              | MENEQ
              { $$ = newExpNode(OpK);
                $$->attr.op = MENEQ;
              }
              | DIF
              { $$ = newExpNode(OpK);
                $$->attr.op = DIF;
              }
              ;

soma_exp  : soma_exp soma termo
            { $$ = $2;
              $$->child[0] = $1;
              $$->child[1] = $3;
            }
            | termo {$$ = $1;}
            ;

soma      : PLUS
            { $$ = newExpNode(OpK);
              $$->attr.op = PLUS;
            }
            | MINUS
              { $$ = newExpNode(OpK);
                $$->attr.op = MINUS;
              }
            ;

termo     : termo mult fator
            { $$ = $2;
              $$->child[0] = $1;
              $$->child[1] = $3;

            }
            | fator { $$ = $1;}
            ;

mult      : TIMES
            { $$ = newExpNode(OpK);
              $$->attr.op = TIMES;
            }
            | OVER
              { $$ = newExpNode(OpK);
                $$->attr.op = OVER;
              }
            ;

fator     : LPAREN exp RPAREN { $$ = $2;}
          | var { $$ = $1;}
          | ativacao { $$ = $1; }
          | numero { $$ = $1; }
          ;

ativacao  : identificador LPAREN arg_list RPAREN
            { $$ = newExpNode(AtivK);
              $$->attr.name = $1->attr.name;
              $$->child[0] = $3;
            }
            | identificador LPAREN RPAREN
              { $$ = newExpNode(AtivK);
                $$->attr.name = $1->attr.name;
              }
            ;

arg_list : arg_list VIR exp
            {
              YYSTYPE t = $1;
              if (t != NULL){
                while (t->sibling != NULL)
                t = t->sibling;
                t->sibling = $3;
                $$ = $1;
              }
              else $$ = $3;
            }
         | exp
          { $$ = $1;}
         ;

identificador : ID
                { $$ = newExpNode(IdK);
                  $$->attr.name = copyString(tokenString);

                }
              ;
numero : NUM
          { $$ = newExpNode(ConstK);
            $$->type = Integer;
            $$->attr.val = atoi(tokenString);
          }


%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}
