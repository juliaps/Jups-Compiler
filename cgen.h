/****************************************************/
/* File: cgen.h                                     */
/* Global types and vars for TINY compiler          */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/*Adaptado para C- por Júlia P.S                    */
/****************************************************/

#include "globals.h"

#ifndef _CGEN_H_
#define _CGEN_H_

typedef enum {IntConst, String} OperandKind;

typedef enum instrucao {ADD, SUB, MULT, DIV,
    VEC, VEC_ADDR, VEC_ASG, EQL, NE, LTN, LET, GT, GET, ASG,
    FUNC, RTN, PARAM, CALL, JPF, GOTO, LBL, HALT} InstructionKind;

typedef struct operand {
    OperandKind kind;
    union {
        int val;
        struct {
            char * name;
            int t;
        } variable;
    } contents;
} * Operand;

/* Estrutura Quádrupla que armazena os dados do código
 * de três endereços
 */
typedef struct Quad {
    InstructionKind instruction;
    int linha;
    Operand op1;
    Operand op2;
    Operand op3;
    struct Quad * next;
} * Quadruple;

Operand createOperand(void);

Quadruple * insertQuad(Quadruple q);

Quadruple createQuad(InstructionKind instruction, Operand op1, Operand op2, Operand op3);

void printIntermediateCode();

Quadruple getCodigoIntermediario(void);

const char * toStringInstruction(enum instrucao i) ;

void printIntermediateCode();

static void cGen(TreeNode * tree);

static void genStmt(TreeNode * tree);

static void genExp(TreeNode * tree);

/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void codeGen(TreeNode * syntaxTree, char * codefile);

#endif
