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
int pc_salvo;
int current_program = 0;
char funcao_atual[50];
int count; 


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
            if(p_reg == 0) p_reg = 1;
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
    p_mem = st_lookup(q->op1->contents.variable.name, funcao_atual);
    printf("r0 %d ", p_mem); //rever isso aqui 
    
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
            if(p_reg == 0) p_reg = 1;
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
          if(select_compilation) { //program
            //syscall
            
           // printf("   loadi r16 2\n");
           // linha++;
            // uma instrucao para alterar a  flag e executar o so
            printf("   disablewriteproc\n"); 
            linha++;
            printf("   syscall 2\n"); // altera a flag e escreve no registrador do so
            linha++;
            printf("   loadi r15 proximo_pc\n"); //escrever no banco de registrador do so o pc atual do processo atual
            //verificar se é o proximo pc mesmo pq ele pode pegar e somar e ir ja pro proximo
            linha++;
            printf("   setsopc r15 r30\n");
            linha++;
            printf("   mov %s r26\n", q->op3->contents.variable.name); // registrador que armazena o retorno da funcao input
            linha++;
          } else { // so
            printf("   in %s\n", q->op3->contents.variable.name);
            linha++;
            printf("   out %s\n", q->op3->contents.variable.name); //so pra teste
            linha++;
          }
          
          
        } else if (strcmp("output", q->op1->contents.variable.name) == 0){
          if(select_compilation) { // program
            //syscall
            //printf("  loadi r16 3\n");
            //linha++;
            printf("   disablewriteproc\n");
            linha++; 
            printf("   pop r14 r31 0\n");
            linha++;
            printf("   enablewriteproc\n");
            linha++;
            printf("   subi r31 r31 1\n");
            linha++;
            // uma instrucao para alterar a  flag e executar o so 
            printf("   disablewriteproc\n"); 
            linha++;
            printf("   syscall 3\n"); // altera a flag e escreve no registrador do so
            linha++;
            printf("   loadi r15 proximo_pc\n"); //escrever no banco de registrador do so o pc atual do processo atual
            linha++;
            //verificar se é o proximo pc mesmo pq ele pode pegar e somar e ir ja pro proximo
            printf("   setsopc r15 r30\n");
            linha++;

          } else {
            printf("   pop r28 r31 0\n");
            linha++;
            printf("   subi r31 r31 1\n");
            linha++;
            printf("   out r28\n"); //testar no fpga
            linha++;
          }
           
            //printf("   out r%d\n", p_reg); //rever to confusa
            //p_reg = (p_reg + 1)%n;
          if(p_reg == 0) p_reg = 1;
           //linha++;
        } else if (strcmp("cpymemtorb", q->op1->contents.variable.name) == 0){ // posso usar a mesma instrucao e usar a flag do so pra definir em qual banco vai escrever 
          //printf("   pop r14 r31 0\n"); // pega a posicao da memoria que vai copiar
          //printf("   subi r31 r31 0\n");
          printf("   enablewriteproc\n"); // para permitir o deslocamento da posicao do banco de registradores
          linha++;
         // printf("   pop r13 r31 0\n"); // pega a posicao da memoria que vai copiar 
          count = 0;
          while(count < 32) {
              printf("   load r%d r0 %d\n", count, (current_program + count));
              linha++;
              count++;
          } 
          printf("   disablewriteproc\n");
          linha++;
        } else if (strcmp("cpyrbtomem", q->op1->contents.variable.name) == 0){
          //printf("   pop r14 r31 0\n"); // pega a posicao da memoria que vai escrever 
          //printf("   subi r31 r31 0\n");
          printf("   enablereadproc\n"); //para permitir ler do banco de registradores dos programas
          linha++;
          //printf("   pop r13 r31 0\n"); // pega a posicao do rb que vai copiar
          //printf("   store r13 r14 0\n");
          count = 0;
          while (count < 32) {
            printf("   store r%d r0 %d\n", count, (current_program + count));
            linha++;
            count++;  
          }
          
          printf("   disablereadproc\n");
          linha++;
        } else if (strcmp("getinterruption", q->op1->contents.variable.name) == 0){
          printf("   mov %s r26\n", q->op3->contents.variable.name); //r26 reg que armazena as interrupçoes
          linha++;
        } else if (strcmp("getoutput", q->op1->contents.variable.name) == 0){
          printf("   mov %s r14 \n", q->op3->contents.variable.name); // r14 reg que armazena i/o processos
          linha++;
        } else if (strcmp("setinput", q->op1->contents.variable.name) == 0){
          printf("   pop r14 r31 0\n"); // pega o parametro ou seja a entrada
          linha++;
          printf("   subi r31 r31 1\n");
          linha++;
          printf("   enablewriteproc\n"); //pra salvar no banco de registradores do programa em execução
          linha++;
          printf("   mov r26 r14\n"); //copia a entrada pro reg que armazena i/o processos
          linha++;
          printf("   disablewriteproc\n");
          linha++;
          //dar um load pra uma posicao especifica da memoria ?
        } else if (strcmp("getpc", q->op1->contents.variable.name) == 0){
          printf("   mov %s r15\n", q->op3->contents.variable.name); //r15 reg salva o pc do processo atual
          linha++;
        } else if (strcmp("setpc", q->op1->contents.variable.name) == 0){
          printf("   pop r15 r31 0\n"); //pega o parametro e copia pro reg que salva o pc do processo atual
          linha++;
          printf("   subi r31 r31 1\n");
          linha++;
        } else if (strcmp("execprocess", q->op1->contents.variable.name) == 0) {
          printf("   loadi r30 proximo_pc\n"); //salva o pc do so 
          linha++;
          printf("   setprocpc r15 r30\n");
          linha++;
          printf("   enablewriteproc\n");
          linha++;
          printf("   enablereadproc\n");
          linha++;
        } else {
          //pc_salvo = linha + 4;
          printf("   loadi r30 proximo_pc\n");
          linha++;
          printf("   j %s\n",  q->op1->contents.variable.name);
          linha++;
          printf("   mov %s r29\n" ,  q->op3->contents.variable.name);
        }
      break;
      case PARAM:

        if(q->op1->kind == String){
          if(q->op1->contents.variable.t == 0){
            p_mem = st_lookup(q->op1->contents.variable.name, funcao_atual);
            printf("   load r28 r0 %d\n", p_mem);
            linha++;

          }else{
            printf("   mov r28 %s\n", q->op1->contents.variable.name);
            linha++;
          }

        }else{
          printf("   loadi r28 %d\n", q->op1->contents.val);
          linha++;
        }
        printf("   addi r31 r31 1\n");
        linha++;
        printf("   push r28 r31 0\n");
        linha++;
         if(strcmp(q->op2->contents.variable.name,"cpyrbtomem") == 0 || 
            strcmp(q->op2->contents.variable.name, "cpymemtorb") == 0) {
           current_program = q->op1->contents.val;
         }
        
      break;
      case RTN:
        if(q->op1 != NULL) {
          if(q->op1->kind == String){
            if(q->op1->contents.variable.t == 0){
              p_mem = st_lookup(q->op1->contents.variable.name, funcao_atual);
              printf("   load r29 r0 %d\n", p_mem); // carrega o retorno armazenado na memoria para o registrador 28 
              linha++;
            }else{
              printf("   mov r29 %s\n",  q->op1->contents.variable.name);
              linha++;
            }
          }else{
            printf("   loadi r29 %d\n", q->op1->contents.val);
            linha++;
          }
          
        }
        printf("   pop r30 r31 0\n");
        linha++;
        printf("   jr r30\n");
        linha++;
      break;
      case HALT:
        if(select_compilation) { // program
          //printf("   loadi r16 1\n");
          //linha++;
          // uma instrucao para alterar a  flag e executar o so 
          printf("   syscall 1\n");
          linha++;
          printf("   setsopc r15 r30\n");
          linha++;
        } else { // so
          printf("   halt\n");  
        }
        
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
          printf("   pop r28 r31 0\n");
          linha++;
          printf("   subi r31 r31 1\n");
          linha++;
          k++;
        }
        //printf("   addi r31 r31 1\n");
        //linha++;
        //printf("   push r30\n");
        //linha++;
      break;
      case ASG:
        //printf("op1: %s\n", q->op1->contents.variable.name);
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
          printf("   loadi r27 %d\n",  q->op2->contents.val);
          linha++;
        }
        p_mem = st_lookup(q->op3->contents.variable.name, funcao_atual);
        //printf("posicao da memoria do vetor %d\n", p_mem );
        //VERIFICAR SE O VETOR VAI COMEÇAR POR ZERO E SE PRECISA SOMAR
        printf("   addi %s r27 %d  \n", q->op1->contents.variable.name,p_mem ); //verificar se todas as posições de operações estao de acordo com o processador
        linha++;
      break;

      case VEC_ASG: //store vai ser a posicao da memoria o registrador de onde vai pegar o valor e o registrador que contem o valor que vai ser somado com a posição da memoria
        if(q->op2->kind == String) {
          if(q->op2->contents.variable.t == 0){
            p_mem = st_lookup(q->op2->contents.variable.name, funcao_atual);
            printf("   load r%d r0 %d \n", p_reg, p_mem);
            linha++;
            printf("   store r%d %s 0\n", p_reg, q->op1->contents.variable.name);
            p_reg = (p_reg + 1)%n;
            if(p_reg == 0) p_reg = 1;
            linha++;
          } else {
            printf("   store %s %s 0\n",  q->op2->contents.variable.name, q->op1->contents.variable.name);
            linha++;
          }
        } else {
          printf("   loadi r%d %d\n", p_reg, q->op2->contents.val);
          linha++;
          printf("   store r%d %s 0\n", p_reg, q->op1->contents.variable.name);
          linha++;
          p_reg = (p_reg + 1)%n;
          if(p_reg == 0) p_reg = 1;
        }
      break;
      case VEC: //load vai ser a posicao da memoria o registrador que vai ser armazenado o valor e o registrador qeu contem o valor que vai ser somado com a posição da memoria
        printf("   load %s %s 0 \n",q->op1->contents.variable.name, q->op2->contents.variable.name);
        linha++; // verificar quem é op1 e op2
      break;
      default:

      break;
    }
    cAssembly(q->next);
  }
}
