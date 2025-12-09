%{
#include <stdio.h>
#include <string.h>
#include "/home/annie/compiler-lab/src/utils/def.h"
#include "/home/annie/compiler-lab/src/ast/ast.h"
#include "/home/annie/compiler-lab/src/semantic/semantic.h"

extern int yylineno;
extern char *yytext;
extern FILE *yyin;

void yyerror(const char *s);
int yylex(void);

ASTNode *root = NULL;  // AST根节点
%}

%define parse.error verbose
%locations
%define api.value.type {YYSTYPE}

/* 终结符（单词种类码）声明 - 使用新名字避免冲突 */
%token <type_int> TOKEN_INT TOKEN_CHAR
%token <type_float> TOKEN_FLOAT
%token <type_id> TOKEN_ID TOKEN_STRING

%token TOKEN_TYPE_INT TOKEN_TYPE_FLOAT TOKEN_TYPE_CHAR TOKEN_TYPE_VOID
%token IF ELSE WHILE FOR BREAK CONTINUE RETURN CONST
%token INC DEC
%token PLUS MINUS STAR DIV MOD
%token ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token EQ NE GT GE LT LE AND OR NOT
%token LP RP LC RC LB RB COMMA SEMI DOT

/* 非终结符语义值类型声明 */
%type <ptr> program ext_def_list ext_def specifier ext_dec_list var_dec
%type <ptr> func_dec comp_st stmt_list stmt def_list def dec_list dec
%type <ptr> local_def
%type <ptr> exp args var_list param_dec

/* 运算符优先级和结合性 */
%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left STAR DIV MOD
%right NOT UMINUS UPLUS
%right INC DEC
%left ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

program
    : ext_def_list { 
        root = $1;  // 设置全局根节点
        display(root, 0);  // 显示抽象语法树
        $$ = root;
    }
    ;

ext_def_list
    : ext_def ext_def_list {
        ASTNode *t = mknode(2, EXT_DEF_LIST, yylineno, $1, $2);
        $$ = t;
    }
    | /* empty */ {
        $$ = NULL;
    }
    ;

ext_def
    : specifier ext_dec_list SEMI {
        ASTNode *t = mknode(2, EXT_VAR_DEF, yylineno, $1, $2);
        $$ = t;
    }
    // 添加带初始化的全局变量定义
    | specifier var_dec ASSIGN exp SEMI {
        ASTNode *t = mknode(2, EXT_VAR_DEF, yylineno, $1, 
            mknode(2, INIT_DEC, yylineno, $2, $4));
        $$ = t;
    }
    | specifier func_dec comp_st {
        ASTNode *t = mknode(3, FUNC_DEF, yylineno, $1, $2, $3);
        $$ = t;
    }
    | error SEMI {
        $$ = NULL;
    }
    ;

specifier
    : TOKEN_TYPE_INT {
        ASTNode *t = mknode(0, TYPE_NODE_INT, yylineno);
        strcpy(t->type_id, "int");
        t->type = 1;  // 类型编码
        $$ = t;
    }
    | TOKEN_TYPE_FLOAT {
        ASTNode *t = mknode(0, TYPE_NODE_FLOAT, yylineno);
        strcpy(t->type_id, "float");
        t->type = 2;
        $$ = t;
    }
    | TOKEN_TYPE_CHAR {
        ASTNode *t = mknode(0, TYPE_NODE_CHAR, yylineno);
        strcpy(t->type_id, "char");
        t->type = 3;
        $$ = t;
    }
    | TOKEN_TYPE_VOID {
        ASTNode *t = mknode(0, TYPE_NODE_VOID, yylineno);
        strcpy(t->type_id, "void");
        t->type = 0;
        $$ = t;
    }
    ;

ext_dec_list
    : var_dec {
        ASTNode *t = mknode(1, EXT_DEC_LIST, yylineno, $1);
        $$ = t;
    }
    | var_dec COMMA ext_dec_list {
        ASTNode *t = mknode(2, EXT_DEC_LIST, yylineno, $1, $3);
        $$ = t;
    }
    ;

var_dec
    : TOKEN_ID {
        ASTNode *t = mknode(0, ID_NODE, yylineno);
        strcpy(t->type_id, $1);
        $$ = t;
    }
    | var_dec LB TOKEN_INT RB {
        // 数组声明：处理多维数组
        ASTNode *t = mknode(2, ARRAY_DEC, yylineno, $1, mknode(0, INT_NODE, yylineno));
        t->ptr[1]->type_int = $3;
        $$ = t;
    }
    ;

func_dec
    : TOKEN_ID LP var_list RP {
        ASTNode *t = mknode(2, FUNC_DEC, yylineno, 
                           mknode(0, ID_NODE, yylineno), $3);
        strcpy(t->ptr[0]->type_id, $1);
        $$ = t;
    }
    | TOKEN_ID LP RP {
        ASTNode *t = mknode(1, FUNC_DEC, yylineno, 
                           mknode(0, ID_NODE, yylineno));
        strcpy(t->ptr[0]->type_id, $1);
        $$ = t;
    }
    ;

var_list
    : param_dec COMMA var_list {
        ASTNode *t = mknode(2, VAR_LIST, yylineno, $1, $3);
        $$ = t;
    }
    | param_dec {
        ASTNode *t = mknode(1, VAR_LIST, yylineno, $1);
        $$ = t;
    }
    ;

param_dec
    : specifier var_dec {
        ASTNode *t = mknode(2, PARAM_DEC, yylineno, $1, $2);
        $$ = t;
    }
    ;

comp_st
    : LC def_list stmt_list RC {
        ASTNode *t = mknode(2, COMP_ST, yylineno, $2, $3);
        $$ = t;
    }
    | LC stmt_list RC {  // 添加这个规则：没有变量声明的复合语句
        ASTNode *t = mknode(1, COMP_ST, yylineno, $2);
        $$ = t;
    }
    ;

def_list
    : def def_list {
        ASTNode *t = mknode(2, DEF_LIST, yylineno, $1, $2);
        $$ = t;
    }
    | /* empty */ {
        $$ = NULL;
    }
    ;

def
    : specifier dec_list SEMI {
        ASTNode *t = mknode(2, DEF, yylineno, $1, $2);
        $$ = t;
    }
    ;

local_def
    : specifier dec_list {
        ASTNode *t = mknode(2, DEF, yylineno, $1, $2);
        $$ = t;
    }
    | specifier var_dec ASSIGN exp {
        ASTNode *t = mknode(2, DEF, yylineno, $1, mknode(2, INIT_DEC, yylineno, $2, $4));
        $$ = t;
    }
    ;

dec_list
    : dec {
        ASTNode *t = mknode(1, DEC_LIST, yylineno, $1);
        $$ = t;
    }
    | dec COMMA dec_list {
        ASTNode *t = mknode(2, DEC_LIST, yylineno, $1, $3);
        $$ = t;
    }
    ;

dec
    : var_dec {
        $$ = $1;
    }
    | var_dec ASSIGN exp {
        ASTNode *t = mknode(2, INIT_DEC, yylineno, $1, $3);
        $$ = t;
    }
    ;

stmt_list
    : stmt stmt_list {
        ASTNode *t = mknode(2, STMT_LIST, yylineno, $1, $2);
        $$ = t;
    }
    | /* empty */ {
        $$ = NULL;
    }
    ;

stmt
    // 添加局部变量声明
    : local_def SEMI {  // 例如：int a = 0, b = 1;
        $$ = $1;
    }
    | exp SEMI {
        ASTNode *t = mknode(1, EXP_STMT, yylineno, $1);
        $$ = t;
    }
    | comp_st {
        $$ = $1;
    }
    | RETURN exp SEMI {
        ASTNode *t = mknode(1, RETURN_STMT, yylineno, $2);
        $$ = t;
    }
    | RETURN SEMI {
        ASTNode *t = mknode(0, RETURN_STMT, yylineno);
        $$ = t;
    }
    | IF LP exp RP stmt %prec LOWER_THAN_ELSE {
        ASTNode *t = mknode(2, IF_STMT, yylineno, $3, $5);
        $$ = t;
    }
    | IF LP exp RP stmt ELSE stmt {
        ASTNode *t = mknode(3, IF_ELSE_STMT, yylineno, $3, $5, $7);
        $$ = t;
    }
    | WHILE LP exp RP stmt {
        ASTNode *t = mknode(2, WHILE_STMT, yylineno, $3, $5);
        $$ = t;
    }
    // FOR 开始
    | FOR LP exp SEMI exp SEMI exp RP stmt {  // for (exp; exp; exp) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, $3, $5, $7, $9);
        $$ = t;
    }
    // 添加 for 语句的各种变体
    | FOR LP SEMI exp SEMI exp RP stmt {  // for (; exp; exp) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, NULL, $4, $6, $8);
        $$ = t;
    }
    | FOR LP exp SEMI SEMI exp RP stmt {  // for (exp; ; exp) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, $3, NULL, $6, $8);
        $$ = t;
    }
    | FOR LP exp SEMI exp SEMI RP stmt {  // for (exp; exp; ) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, $3, $5, NULL, $8);
        $$ = t;
    }
    | FOR LP local_def SEMI exp SEMI exp RP stmt {  // for (int i=0; i<n; i++) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, $3, $5, $7, $9);
        $$ = t;
    }
    // FOR 结束
    | BREAK SEMI {
        ASTNode *t = mknode(0, BREAK_STMT, yylineno);
        $$ = t;
    }
    | CONTINUE SEMI {
        ASTNode *t = mknode(0, CONTINUE_STMT, yylineno);
        $$ = t;
    }
    | error SEMI {
        $$ = NULL;
    }
    ;

exp
    : exp ASSIGN exp {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "=");
        $$ = t;
    }
    | exp ADD_ASSIGN exp {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "+=");
        $$ = t;
    }
    | exp SUB_ASSIGN exp {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "-=");
        $$ = t;
    }
    | exp MUL_ASSIGN exp {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "*=");
        $$ = t;
    }
    | exp DIV_ASSIGN exp {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "/=");
        $$ = t;
    }
    | exp MOD_ASSIGN exp {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "%=");
        $$ = t;
    }
    | exp OR exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "||");
        $$ = t;
    }
    | exp AND exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "&&");
        $$ = t;
    }
    | exp EQ exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "==");
        $$ = t;
    }
    | exp NE exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "!=");
        $$ = t;
    }
    | exp LT exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "<");
        $$ = t;
    }
    | exp GT exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, ">");
        $$ = t;
    }
    | exp LE exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "<=");
        $$ = t;
    }
    | exp GE exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, ">=");
        $$ = t;
    }
    | exp PLUS exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "+");
        $$ = t;
    }
    | exp MINUS exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "-");
        $$ = t;
    }
    | exp STAR exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "*");
        $$ = t;
    }
    | exp DIV exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "/");
        $$ = t;
    }
    | exp MOD exp {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, $1, $3);
        strcpy(t->type_id, "%");
        $$ = t;
    }
    | INC exp %prec INC {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, $2);
        strcpy(t->type_id, "++");
        $$ = t;
    }
    | exp INC %prec INC {
        ASTNode *t = mknode(1, POST_INC_EXP, yylineno, $1);
        strcpy(t->type_id, "++");
        $$ = t;
    }
    | DEC exp %prec DEC {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, $2);
        strcpy(t->type_id, "--");
        $$ = t;
    }
    | exp DEC %prec DEC {
        ASTNode *t = mknode(1, POST_DEC_EXP, yylineno, $1);
        strcpy(t->type_id, "--");
        $$ = t;
    }
    | MINUS exp %prec UMINUS {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, $2);
        strcpy(t->type_id, "-");
        $$ = t;
    }
    | PLUS exp %prec UPLUS {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, $2);
        strcpy(t->type_id, "+");
        $$ = t;
    }
    | NOT exp {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, $2);
        strcpy(t->type_id, "!");
        $$ = t;
    }
    | LP exp RP {
        $$ = $2;
    }
    | TOKEN_ID LP args RP {
        ASTNode *t = mknode(2, FUNC_CALL, yylineno, 
                           mknode(0, ID_NODE, yylineno), $3);
        strcpy(t->ptr[0]->type_id, $1);
        $$ = t;
    }
    | TOKEN_ID LP RP {
        ASTNode *t = mknode(1, FUNC_CALL, yylineno, 
                           mknode(0, ID_NODE, yylineno));
        strcpy(t->ptr[0]->type_id, $1);
        $$ = t;
    }
    | exp LB exp RB {
        ASTNode *t = mknode(2, ARRAY_ACCESS, yylineno, $1, $3);
        $$ = t;
    }
    | TOKEN_ID {
        ASTNode *t = mknode(0, ID_NODE, yylineno);
        strcpy(t->type_id, $1);
        $$ = t;
    }
    | TOKEN_INT {
        ASTNode *t = mknode(0, INT_NODE, yylineno);
        t->type_int = $1;
        $$ = t;
    }
    | TOKEN_FLOAT {
        ASTNode *t = mknode(0, FLOAT_NODE, yylineno);
        t->type_float = $1;
        $$ = t;
    }
    | TOKEN_CHAR {
        ASTNode *t = mknode(0, CHAR_NODE, yylineno);
        t->type_int = $1;  // 字符值存储在type_int中
        $$ = t;
    }
    ;

args
    : exp {
        ASTNode *t = mknode(1, ARGS, yylineno, $1);
        $$ = t;
    }
    | exp COMMA args {
        ASTNode *t = mknode(2, ARGS, yylineno, $1, $3);
        $$ = t;
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
}