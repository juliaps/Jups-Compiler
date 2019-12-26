/****************************************************/
/* File: cgen.c                                     */
/* Global types and vars for TINY compiler          */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Adaptado para C- por Júlia P.S                   */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "cgen.h"
#include "assembler.h"

/* Cabeça da Lista de representações quádruplas */
Quadruple head = NULL;

/* Numero para geração de nomes dos temporarios */
static int temporario = 0;

/* Numero para geração de labels */
static int label = 1;

/* Variavel para armazenar o ultimo operando a ser utilizado */
Operand operandoAtual;

/* Variavel para armazenar a ultima instrucao a ser gerada */
InstructionKind instrucaoAtual;

/* Contador de linhas de instrucao */
static int linha = 0;

/* temporary string to help printing text */
static char tempString[20];


const char * toStringInstruction(enum instrucao i) {
    const char * strings[] = {
        "addition", "subtraction", "multiplication", "division",
        "vector_value", "vector_address", "vector_assign","equal", "not_equal",
        "less_than", "less_than_equal_to","greater_than",
        "greater_than_equal_to", "assign", "function",
        "return", "param", "call", "jump_if_false",
        "goto", "label", "halt"
    };
    return strings[i];
}

Quadruple createQuad(InstructionKind instruction, Operand op1, Operand op2, Operand op3) {
    Quadruple q = (Quadruple) malloc(sizeof(struct Quad));
    q->instruction = instruction;
    q->op1 = op1;
    q->op2 = op2;
    q->op3 = op3;
    q->linha = ++linha;
    q->next = NULL;
    return q;
}

Quadruple * insertQuad(Quadruple q) {
    Quadruple * ptr = (Quadruple *) malloc(sizeof(struct Quad));
    if(head == NULL) {
        head = q;
        head->next = NULL;
        ptr = &head;
    } else {
        Quadruple temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = q;
        temp->next->next = NULL;
        ptr = &temp->next;
    }
    return ptr;
}

Quadruple getCodigoIntermediario(void) {
    return head;
}


void printIntermediateCode() {
    Quadruple q = head;
    char quad[100];

    while(q != NULL) {
        sprintf(quad, "%d: (", q->linha);
        strcat(quad, toStringInstruction(q->instruction));

        if(q->op1 == NULL) {
            strcat(quad, ", _");
        } else if(q->op1->kind == String) {
            strcat(quad, ", ");
            strcat(quad, q->op1->contents.variable.name);
        } else {
            sprintf(tempString, ", %d", q->op1->contents.val);
            strcat(quad,tempString);
        }

        if(q->op2 == NULL) {
            strcat(quad, ", _");
        } else if(q->op2->kind == String) {
            strcat(quad, ", ");
            strcat(quad, q->op2->contents.variable.name);
        } else {
            sprintf(tempString, ", %d", q->op2->contents.val);
            strcat(quad,tempString);
        }

        if(q->op3 == NULL) {
            strcat(quad, ", _)");
        } else if(q->op3->kind == String) {
            strcat(quad, ", ");
            strcat(quad, q->op3->contents.variable.name);
            strcat(quad, ")");
        } else {
            sprintf(tempString, ", %d", q->op3->contents.val);
            strcat(quad,tempString);
            strcat(quad, ")");
        }
        printf("%s\n", quad );
        q = q->next;
    }
}

static char * createLabelName() {
    char * temp = (char *) malloc(5);
    sprintf(temp, "L%d", label);
    ++label;
    return temp;
}

static char * createTemporaryOperandName() {
    char * temp = (char *) malloc(5);
    sprintf(temp, "t%d", temporario);
    ++temporario;
    return temp;
}

Operand createOperand(void) {
    Operand op = (Operand) malloc(sizeof(struct operand));
    return op;
}

static Operand createTemporaryOperand() {
    Operand temp = createOperand();
    temp->kind = String;
    temp->contents.variable.t = 1;
    temp->contents.variable.name = createTemporaryOperandName();
    return temp;
}

static void genExp(TreeNode * tree)
{
    Quadruple * q;
    Operand op1, op2, op3;
    TreeNode * p1, * p2, * p3;
    switch (tree->kind.exp)
    {
        case OpK:

            p1 = tree->child[0];
            p2 = tree->child[1];

            cGen(p1);
            op1 = operandoAtual;
            cGen(p2);
            op2 = operandoAtual;
            switch (tree->attr.op)
            {
              case PLUS:
                instrucaoAtual = ADD;
              break;

              case MINUS:
                instrucaoAtual = SUB;
              break;

              case TIMES:
                instrucaoAtual = MULT;
              break;

              case OVER:
                instrucaoAtual = DIV;
              break;

              case EQ:
                instrucaoAtual = EQL;
              break;

              case LT:
                instrucaoAtual = LTN;
              break;

              case LR:
                instrucaoAtual = GT;
              break;

              case MENEQ:
                instrucaoAtual = LET;
              break;

              case MAIEQ:
                instrucaoAtual = GET;
              break;

              case DIF:
                instrucaoAtual = NE;
              break;
            }
            op3 = createTemporaryOperand();

            insertQuad(createQuad(instrucaoAtual,op3, op1, op2));
            operandoAtual = op3;
        break;

        case ConstK:
            op1 = createOperand();
            op1->contents.val = tree->attr.val;
            op1->kind = IntConst;
            operandoAtual = op1;
        break;

        case IdK:
            p1 = tree;
            if(p1->type == Vector){
              p2 = p1->child[0];
              cGen(p2);
              op1 = operandoAtual;
              instrucaoAtual = VEC_ADDR;
              op2 = createTemporaryOperand();
              op3 = createOperand();
              op3->contents.variable.name = p1->attr.name;
              op3->kind = String;
              op3->contents.variable.t = 0;
              operandoAtual = op2;
              insertQuad(createQuad(instrucaoAtual,op2,op1,op3));
              instrucaoAtual = VEC;
              op1 = createTemporaryOperand();
              operandoAtual = op1;
              insertQuad(createQuad(instrucaoAtual, op1, op2,NULL));
            }else{
              op1 = createOperand();
              op1->contents.variable.name = tree->attr.name;
              op1->kind = String;
              op1->contents.variable.t = 0;
              operandoAtual = op1;
            }

        break;

        case AtivK:

            op1 = createOperand();
            op1->contents.variable.name = tree->attr.name;
            op1->kind = String;
            op1->contents.variable.t = 0;
            p1 = tree->child[0];
            //if(p1->type != Void){
              while(p1!=NULL){
                genExp(p1);
                op2 = operandoAtual;
                instrucaoAtual = PARAM;
                insertQuad(createQuad(instrucaoAtual, op2, op1, NULL));
                p1 = p1->sibling;
              }
            //}
            op2 = createOperand();
            op2->contents.val = getQuantidadeParametros(tree);
            update_function(op1->contents.variable.name , op2->contents.val);
            op2->kind = IntConst;
            instrucaoAtual = CALL;
            op3 = createTemporaryOperand();
            operandoAtual = op3;
            insertQuad(createQuad(instrucaoAtual, op1, op2, op3));
        break;

        case VarDeclK:

            p1 = tree->child[0];
            cGen(p1);
        break;


        case FunDeclK:

            p1 = tree->child[0];
            p2 = tree->child[1];
            p3 = tree->child[2];
            /* Nome da funcao*/
            cGen(p1);
            op1 = createOperand();
            op1->contents.variable.name = tree->attr.name;
            op1->kind = String;
            op1->contents.variable.t = 0;
            operandoAtual = op1;
            instrucaoAtual = FUNC;
            insertQuad(createQuad(instrucaoAtual, op1, NULL, NULL));
            /* Parâmetros*/
            cGen(p2);
            /*Corpo da funcao*/
            cGen(p3);
        break;

        case TypeK:

            p1 = tree->child[0];
            cGen(p1);
        break;
    }
}


static void genStmt(TreeNode * tree)
{
    Quadruple * q;
    Operand op1, op2, op3;
    TreeNode * p1, * p2, * p3;

    switch (tree->kind.stmt)
    {
      case IfK:

        p1 = tree->child[0];
        p2 = tree->child[1];
        p3 = tree->child[2];

        op2 = createOperand();
        op2->contents.variable.name  = createLabelName();
        op2->kind = String;
        op2->contents.variable.t = 0;
        /* expressao da condição */
        cGen(p1);
        op1 = operandoAtual;
        instrucaoAtual = JPF;
        insertQuad(createQuad(instrucaoAtual,op1,op2,NULL));
        /* corpo do if */
        cGen(p2);
        if(p3 != NULL){
          op3 = createOperand();
          op3->contents.variable.name  = createLabelName();
          op3->kind = String;
          op3->contents.variable.t = 0;
          instrucaoAtual = GOTO;
          insertQuad(createQuad(instrucaoAtual, op3, NULL,NULL));
        }
        instrucaoAtual = LBL;
        insertQuad(createQuad(instrucaoAtual,op2,NULL,NULL));
        if(p3 != NULL){
          /* corpo do else */
          cGen(p3);
          instrucaoAtual = LBL;
          insertQuad(createQuad(instrucaoAtual,op3,NULL, NULL));
        }
        break;

      case WhileK:

          p1 = tree->child[0];
          p2 = tree->child[1];
          op1 = createOperand();
          op1->contents.variable.name = createLabelName();
          op1->kind = String;
          op1->contents.variable.t = 0;
          instrucaoAtual = LBL;
          insertQuad(createQuad(instrucaoAtual, op1, NULL, NULL));
          /* expressao da condição */
          cGen(p1);
          op2 = operandoAtual;
          instrucaoAtual = JPF;
          op3 = createOperand();
          op3->contents.variable.name = createLabelName();
          op3->kind = String;
          op3->contents.variable.t = 0;
          insertQuad(createQuad(instrucaoAtual,op2,op3, NULL));
          /* corpo do while */
          cGen(p2);
          instrucaoAtual = GOTO;
          insertQuad(createQuad(instrucaoAtual, op1, NULL, NULL));
          instrucaoAtual = LBL;
          insertQuad(createQuad(instrucaoAtual, op3, NULL, NULL));
      break;

      case AssignK:

          p1 = tree->child[0];
          Operand op4 , op5;
          if(p1->type == Vector){
            p3 = p1->child[0];
            cGen(p3);
            op1 = operandoAtual;
            op3 = createOperand();
            op3->contents.variable.name = p1->attr.name;
            op3->kind = String;
            op4 = createTemporaryOperand();
            instrucaoAtual = VEC_ADDR;
            insertQuad(createQuad(instrucaoAtual, op4, op1, op3));
          }else{
            cGen(p1);
            op1 = operandoAtual;
          }
          p2 = tree->child[1];
          cGen(p2);
          op2 = operandoAtual;
          if(p1->type == Vector){
            instrucaoAtual = VEC_ASG;
            insertQuad(createQuad(instrucaoAtual, op4, op2, NULL));
          }else{
            instrucaoAtual = ASG;
            insertQuad((createQuad(instrucaoAtual,op1, op2, NULL)));
          }

      break;

      case ParamK:

          p1 = tree->child[0];
          cGen(p1);
      break;

      case ReturnK:

          p1 = tree->child[0];
          if(p1!= NULL){
              cGen(p1);
              op1 = operandoAtual;
              instrucaoAtual = RTN;
              insertQuad(createQuad(instrucaoAtual,op1,NULL, NULL));
          }else{
            instrucaoAtual = RTN;
            insertQuad(createQuad(instrucaoAtual,NULL,NULL, NULL));
          }
      break;
    }
}

static void cGen(TreeNode * tree) {
    if (tree != NULL) {
        switch (tree->nodekind) {
            case StmtK:
                genStmt(tree);
            break;
            case ExpK:
                genExp(tree);
            break;
            default:
            break;
        }
        cGen(tree->sibling);
    }
}

void codeGen(TreeNode * syntaxTree, char * codefile)
{
      cGen(syntaxTree);
      /* Para finalizar o algoritmo */
      instrucaoAtual = HALT;
      insertQuad(createQuad(instrucaoAtual,NULL,NULL,NULL));
    //  printSymTab(listing);
      printf("\n**************************** Código Intermediário ****************************\n\n");
      printIntermediateCode();
      printf("\n*************************Código Assembly************************\n\n");
      printf("  mov r0 r31\n");
      printf("  j main\n");
      cAssembly(getCodigoIntermediario());
}
