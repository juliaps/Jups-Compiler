/****************************************************/
/* File: assembler.c                                */
/* Conversor de código intermediário para assembly. */
/* Feito para a linguagem C- por Júlia P.S          */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "cgen.h"
#include "assembler.h"

///CODIGO ASSEMBLY VAI FICAR COM ORDEM : RESULTADO E OPERADORES
int p_reg = 1;
int p_mem;
int linha = 0;
int n = 16;
char funcao_atual[50];


void tipo_r(Quadruple q, char *inst){
  Operand op;
  op = q->op1;
  int vet[3];
  int i;
  for(i = 0; i< 3; i++){
    vet[i] = -1;
  }
  i = 0;
  int flag = -1;
  while(op != NULL){
    if(op->kind == String){
      if(op->contents.variable.t == 1){
        if(flag == 1){
          printf("%s ", op->contents.variable.name );
        }
      }else{
        p_mem = st_lookup(op->contents.variable.name, funcao_atual);
        if(p_mem!=-1){
          if(flag == -1){
            vet[i] = p_reg;
            printf("   load r%d r0 %d \n", vet[i], p_mem); // carrega da memoria pro registrador
            linha++;
            p_reg = (p_reg + 1)%n;
          }
          else if (flag == 1) {
            printf("r%d ", vet[i]);
          }
        }else if(flag == 1){
          printf("%s\n", op->contents.variable.name);
        }
      }
    }else if(op->kind == IntConst){
      if(flag == -1){
        vet[i] = p_reg;
        printf("   loadi r%d %d\n",  vet[i], op->contents.val); // carrega o valor imediato pro registrador
        linha++;
      }else if(flag == 1){
        printf("r%d ", vet[i]);
      }
    }
    i++;
    if(i == 1){
      op = q->op2;
    }else if(i == 2){
      op = q->op3;
    }
    if((op == NULL || i == 3)&& flag == -1){
      flag = 0;
    }
    if(flag == 0){
      flag = 1;
      printf("   %s ",  inst);
      linha++;
      if(strcmp(inst, "store") == 0){
        op = q->op2;
        i = 1;
        p_mem = st_lookup(q->op1->contents.variable.name, funcao_atual);
        printf("m%d ", p_mem); //rever isso aqui 
      }else{
        op = q->op1;
        i = 0;
      }

    }
    if(flag == 1 && i == 3){
      op = NULL;
    }
  }
  if(strcmp(inst,"store")==0){
    printf("r0 ");
  }
  printf("\n");
}

void salto_cond(Quadruple q, char *inst){
  Operand op;
  op = q->op2;
  int vet[3];
  int i;
  for(i = 0; i< 3; i++){
    vet[i] = -1;
  }
  i = 1;
  int flag = -1;
  while(op != NULL){
    if(op->kind == String){
      if(op->contents.variable.t == 1){
        if(flag == 1){
          printf("%s ", op->contents.variable.name );
        }
      }else{
        p_mem = st_lookup(op->contents.variable.name, funcao_atual);
        if(p_mem!=-1){
          if(flag == -1){
            vet[i] = p_reg;
            printf("    load r%d r0 %d\n", vet[i], p_mem); // carrega da memoria para o registrador
            linha++;
            p_reg = (p_reg + 1)%n;
          }
          else if (flag == 1) {
            printf("r%d ", vet[i]);
          }
        }else if(flag == 1){
          printf("%s\n", op->contents.variable.name);
        }
      }
    }else if(op->kind == IntConst){
      if(flag == -1){
        vet[i] = p_reg;
        printf("    loadi r%d %d\n",  vet[i], op->contents.val ); // carrega o imediato direto pro registrador 
        linha++;
      }else if(flag == 1){
        printf("v%d ", vet[i]); //rever isso aqui tbm 
      }
    }
    i++;
    if(i == 2){
      op = q->op3;
    }
    if((op == NULL || i == 3)&& flag == -1){
      flag = 0;
    }
    if(flag == 0){
      flag = 1;
      op = q->op2;
      i = 1;
      printf("    %s ", inst);
      linha++;
    }
    if(flag == 1 && i == 3){
      op = NULL;
    }
  }
  q = q->next;
  printf("%s\n", q->op2->contents.variable.name);
}

void cAssembly(Quadruple  q){
  if (q != NULL){
    switch (q->instruction){
      case ADD:
        tipo_r(q,"add");
      break;
      case SUB:
        tipo_r(q, "sub");
      break;
      case MULT:
        tipo_r(q, "mult");
        break;
      case DIV:
        tipo_r(q, "div");
        break;
      case LBL:
        printf("label %s\n",q->op1->contents.variable.name);
      break;
      case GOTO:
        printf("   j %s\n",  q->op1->contents.variable.name);
        linha++;
      break;
      case CALL:
        if(strcmp("input", q->op1->contents.variable.name) == 0){
          printf("   in r31\n");
          linha++;
          printf("   out r31\n");
          linha++;
        }else if (strcmp("output", q->op1->contents.variable.name) == 0){
            //printf("   pop r%d\n", p_reg);
            //linha++;
            //printf("   subi r31 r31 1\n");
            //linha++;
            printf("   out r%d\n", p_reg);
            p_reg = (p_reg + 1)%n;
            linha++;
        } else if (strcmp("cpy_hd_to_bios", q->op1->contents.variable.name) == 0){
        } else if (strcmp("cpy_hd_to_mem", q->op1->contents.variable.name) == 0){
        } else if (strcmp("bios_to_mem", q->op1->contents.variable.name) == 0){
        } else if (strcmp("get_pc", q->op1->contents.variable.name) == 0){
        } else {
          printf("   jal %s %d\n",  q->op1->contents.variable.name, linha+1);
          linha++;
          printf("   mov %s r28\n" ,  q->op3->contents.variable.name);
        }
      break;
      case PARAM:

        if(q->op1->kind == String){
          if(q->op1->contents.variable.t == 0){
            p_mem = st_lookup(q->op1->contents.variable.name, funcao_atual);
            printf("   load r%d r0 %d\n", p_reg, p_mem);
            linha++;

          }else{
            printf("   mov r%d %s\n",  p_reg, q->op1->contents.variable.name);
            linha++;
          }

        }else{
          printf("   loadi r%d %d\n",  p_reg, q->op1->contents.val);
          linha++;
        }
        printf("   push r28\n");
        linha++;
        printf("   addi r31 r31 1\n");
        linha++;
      break;
      case RTN:
        if(q->op1->kind == String){
          if(q->op1->contents.variable.t == 0){
            p_mem = st_lookup(q->op1->contents.variable.name, funcao_atual);
            printf("   load r28 r0 %d\n", p_mem); // carrega o retorno armazenado na memoria para o registrador 28 
            linha++;
          }else{
            printf("   mov r28 %s\n",  q->op1->contents.variable.name);
            linha++;
          }
        }else{
          printf("   loadi r28 %d\n", q->op1->contents.val);
          linha++;
        }
        printf("   pop r28\n");
        linha++;
        printf("   jr r30\n");
        linha++;
      break;
      case HALT:
        printf("   halt\n");
      break;
      case EQL:
        salto_cond(q, "bneq");
      break;
      case NE:
        salto_cond(q, "beq");
      break;
      case GET:
        tipo_r(q, "sget");
      break;
      case GT:
        tipo_r(q, "sgt");
      break;
      case LTN:
        tipo_r(q, "slt");
      break;
      case LET:
        tipo_r(q, "slet");
      break;
      case JPF:
        printf("   beq %s r0 %s\n", q->op1->contents.variable.name, q->op2->contents.variable.name );
      break;
      case FUNC:
        strcpy(funcao_atual,"");
        strcpy(funcao_atual, q->op1->contents.variable.name);
        printf("%s\n", q->op1->contents.variable.name );
        int k = 0;
        int p = params(q->op1->contents.variable.name);
        while(k < p){
          printf("   pop r28\n");
          linha++;
          printf("   subi r31 r31 1\n");
          linha++;
          k++;
        }
        printf("   addi r31 r31 1\n");
        linha++;
        printf("   push r28\n");
        linha++;
      break;
      case ASG:
        if(q->op1->contents.variable.t == 1){
            tipo_r(q,"mov");
        }else{
          tipo_r(q, "store");
        }
      break;
      case VEC_ADDR:

        if(q->op2->kind == String){
          if(q->op2->contents.variable.t == 0){
            p_mem = st_lookup(q->op2->contents.variable.name, funcao_atual);
            printf("   load r27 r0 %d\n", p_mem);
            linha++;
          }else{
            printf("   mov %s r27\n",  q->op2->contents.variable.name);
            linha++;
          }
        }else{
          printf("   addi r27 r0 %d\n",  q->op2->contents.val);
          linha++;
        }
        p_mem = st_lookup(q->op3->contents.variable.name, funcao_atual);
        //VERIFICAR SE O VETOR VAI COMEÇAR POR ZERO E SE PRECISA SOMAR
        printf("   addi %s r27 %d  \n", q->op1->contents.variable.name,p_mem ); //verificar se todas as posições de operações estao de acordo com o processador
        linha++;
      break;

      case VEC_ASG: //store vai ser a posicao da memoria o registrador de onde vai pegar o valor e o registrador que contem o valor que vai ser somado com a posição da memoria
        printf("   store %s r0 %s\n",  q->op2->contents.variable.name, q->op1->contents.variable.name);
        linha++; // verificar quem é op1 e op2
      break;
      case VEC: //load vai ser a posicao da memoria o registrador que vai ser armazenado o valor e o registrador qeu contem o valor que vai ser somado com a posição da memoria
        printf("   load  %s r0 %s\n",q->op1->contents.variable.name, q->op2->contents.variable.name);
        linha++; // verificar quem é op1 e op2
      break;
      default:

      break;
    }
    cAssembly(q->next);
  }
}
