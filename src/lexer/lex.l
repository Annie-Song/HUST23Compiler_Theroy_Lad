%option yylineno
%option noyywrap

%{
#include <string.h>
#include "/home/annie/compiler-lab/src/utils/def.h"
#include "/home/annie/compiler-lab/build/parser.tab.h"

int yycolumn = 1;

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

extern YYSTYPE yylval;
%}

/* 正则表达式定义 */
digit       [0-9]
letter      [a-zA-Z_]
id          {letter}({letter}|{digit})*
int_const   {digit}+
float_const {digit}+\.{digit}*|{digit}*\.{digit}+
char_const  \'([^\\'\n]|\\[abfnrtv\\'"])\'
string_const \"([^\\"\n]|\\[abfnrtv\\'"])*\"

/* 注释 */
line_comment  "//"[^\n]*
block_comment "/"\*([^*]|\*+[^*/])*\*+"/"

%%

"int"       { return TOKEN_TYPE_INT; }
"float"     { return TOKEN_TYPE_FLOAT; }
"char"      { return TOKEN_TYPE_CHAR; }
"void"      { return TOKEN_TYPE_VOID; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"for"       { return FOR; }
"break"     { return BREAK; }
"continue"  { return CONTINUE; }
"return"    { return RETURN; }
"const"     { return CONST; }

{id}        { 
    strcpy(yylval.type_id, yytext); 
    return TOKEN_ID; 
}

{int_const} { 
    yylval.type_int = atoi(yytext); 
    return TOKEN_INT; 
}

{float_const} { 
    yylval.type_float = atof(yytext); 
    return TOKEN_FLOAT; 
}

{char_const} { 
    // 提取字符值，处理转义字符
    char ch = yytext[1];
    if (ch == '\\') {
        switch(yytext[2]) {
            case 'n': ch = '\n'; break;
            case 't': ch = '\t'; break;
            case 'r': ch = '\r'; break;
            case '\\': ch = '\\'; break;
            case '\'': ch = '\''; break;
            case '\"': ch = '\"'; break;
            case '0': ch = '\0'; break;
        }
    }
    yylval.type_int = (int)ch;
    return TOKEN_CHAR; 
}

{string_const} {
    // 去掉引号并处理转义字符
    strncpy(yylval.type_id, yytext+1, strlen(yytext)-2);
    yylval.type_id[strlen(yytext)-2] = '\0';
    return TOKEN_STRING;
}

"++"        { return INC; }
"--"        { return DEC; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return STAR; }
"/"         { return DIV; }
"%"         { return MOD; }
"="         { return ASSIGN; }
"+="        { return ADD_ASSIGN; }
"-="        { return SUB_ASSIGN; }
"*="        { return MUL_ASSIGN; }
"/="        { return DIV_ASSIGN; }
"%="        { return MOD_ASSIGN; }
"=="        { return EQ; }
"!="        { return NE; }
">"         { return GT; }
">="        { return GE; }
"<"         { return LT; }
"<="        { return LE; }
"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }

";"         { return SEMI; }
","         { return COMMA; }
"("         { return LP; }
")"         { return RP; }
"{"         { return LC; }
"}"         { return RC; }
"["         { return LB; }
"]"         { return RB; }
"."         { return DOT; }


{line_comment}  { /* 忽略行注释 */ }
{block_comment} { /* 忽略块注释 */ }


[ \t\r]+    { /* 忽略空格、制表符、回车 */ }
\n          { yycolumn = 1; }

.           { 
    printf("Lexical error at line %d, column %d: invalid character '%s'\n", 
           yylineno, yycolumn, yytext); 
}

%%

/* 不需要main函数，由Bison调用 */