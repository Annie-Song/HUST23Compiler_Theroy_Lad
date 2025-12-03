/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_HOME_ANNIE_COMPILER_LAB_BUILD_PARSER_TAB_H_INCLUDED
# define YY_YY_HOME_ANNIE_COMPILER_LAB_BUILD_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT = 258,                     /* INT  */
    CHAR = 259,                    /* CHAR  */
    FLOAT = 260,                   /* FLOAT  */
    ID = 261,                      /* ID  */
    STRING = 262,                  /* STRING  */
    TYPE_INT = 263,                /* TYPE_INT  */
    TYPE_FLOAT = 264,              /* TYPE_FLOAT  */
    TYPE_CHAR = 265,               /* TYPE_CHAR  */
    TYPE_VOID = 266,               /* TYPE_VOID  */
    IF = 267,                      /* IF  */
    ELSE = 268,                    /* ELSE  */
    WHILE = 269,                   /* WHILE  */
    FOR = 270,                     /* FOR  */
    BREAK = 271,                   /* BREAK  */
    CONTINUE = 272,                /* CONTINUE  */
    RETURN = 273,                  /* RETURN  */
    CONST = 274,                   /* CONST  */
    INC = 275,                     /* INC  */
    DEC = 276,                     /* DEC  */
    PLUS = 277,                    /* PLUS  */
    MINUS = 278,                   /* MINUS  */
    STAR = 279,                    /* STAR  */
    DIV = 280,                     /* DIV  */
    MOD = 281,                     /* MOD  */
    ASSIGN = 282,                  /* ASSIGN  */
    ADD_ASSIGN = 283,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 284,              /* SUB_ASSIGN  */
    MUL_ASSIGN = 285,              /* MUL_ASSIGN  */
    DIV_ASSIGN = 286,              /* DIV_ASSIGN  */
    MOD_ASSIGN = 287,              /* MOD_ASSIGN  */
    EQ = 288,                      /* EQ  */
    NE = 289,                      /* NE  */
    GT = 290,                      /* GT  */
    GE = 291,                      /* GE  */
    LT = 292,                      /* LT  */
    LE = 293,                      /* LE  */
    AND = 294,                     /* AND  */
    OR = 295,                      /* OR  */
    NOT = 296,                     /* NOT  */
    LP = 297,                      /* LP  */
    RP = 298,                      /* RP  */
    LC = 299,                      /* LC  */
    RC = 300,                      /* RC  */
    LB = 301,                      /* LB  */
    RB = 302,                      /* RB  */
    COMMA = 303,                   /* COMMA  */
    SEMI = 304,                    /* SEMI  */
    DOT = 305,                     /* DOT  */
    UMINUS = 306,                  /* UMINUS  */
    UPLUS = 307,                   /* UPLUS  */
    LOWER_THAN_ELSE = 308          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_HOME_ANNIE_COMPILER_LAB_BUILD_PARSER_TAB_H_INCLUDED  */
