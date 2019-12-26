/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for TINY                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Adaptado para C- por Júlia P.S                   */
/****************************************************/


%{
#include "globals.h"
#include "util.h"
#include "scan.h"

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
static int yylex(void);
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"            {return IF;}
"while"         {return WHILE;}
"else"          {return ELSE;}
"void"          {return VOID;}
"return"        {return RETURN;}
"int"           {return INT;}
"="             {return ASSIGN;}
"["             {return ACOL;}
"]"             {return FCOL;}
"=="            {return EQ;}
"!="            {return DIF;}
"<"             {return LT;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
"("             {return LPAREN;}
")"             {return RPAREN;}
";"             {return SEMI;}
","             {return VIR;}
"{"             {return ACH;}
"}"             {return FCH;}
">"             {return LR;}
"<="            {return MENEQ;}
">="            {return MAIEQ;}
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
"/*"            { 
      char c1, c2;
      c2 = input();
      if(c2 == EOF) break;
      if (c2 == '\n') lineno++;
      do{
        c1 = c2;
        c2 = input();
        if(c2 == EOF) break;
        if(c2 == '\n') lineno++;
      } while (c1 != '*' || c2 != '/');
                }
.               {return ERROR;}

%%

int yywrap()
{
	return 1;
}

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d: ",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

