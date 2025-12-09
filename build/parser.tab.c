/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "/home/annie/compiler-lab/src/parser/parser.y"

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

#line 88 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOKEN_INT = 3,                  /* TOKEN_INT  */
  YYSYMBOL_TOKEN_CHAR = 4,                 /* TOKEN_CHAR  */
  YYSYMBOL_TOKEN_FLOAT = 5,                /* TOKEN_FLOAT  */
  YYSYMBOL_TOKEN_ID = 6,                   /* TOKEN_ID  */
  YYSYMBOL_TOKEN_STRING = 7,               /* TOKEN_STRING  */
  YYSYMBOL_TOKEN_TYPE_INT = 8,             /* TOKEN_TYPE_INT  */
  YYSYMBOL_TOKEN_TYPE_FLOAT = 9,           /* TOKEN_TYPE_FLOAT  */
  YYSYMBOL_TOKEN_TYPE_CHAR = 10,           /* TOKEN_TYPE_CHAR  */
  YYSYMBOL_TOKEN_TYPE_VOID = 11,           /* TOKEN_TYPE_VOID  */
  YYSYMBOL_IF = 12,                        /* IF  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_WHILE = 14,                     /* WHILE  */
  YYSYMBOL_FOR = 15,                       /* FOR  */
  YYSYMBOL_BREAK = 16,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 17,                  /* CONTINUE  */
  YYSYMBOL_RETURN = 18,                    /* RETURN  */
  YYSYMBOL_CONST = 19,                     /* CONST  */
  YYSYMBOL_INC = 20,                       /* INC  */
  YYSYMBOL_DEC = 21,                       /* DEC  */
  YYSYMBOL_PLUS = 22,                      /* PLUS  */
  YYSYMBOL_MINUS = 23,                     /* MINUS  */
  YYSYMBOL_STAR = 24,                      /* STAR  */
  YYSYMBOL_DIV = 25,                       /* DIV  */
  YYSYMBOL_MOD = 26,                       /* MOD  */
  YYSYMBOL_ASSIGN = 27,                    /* ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 28,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 29,                /* SUB_ASSIGN  */
  YYSYMBOL_MUL_ASSIGN = 30,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 31,                /* DIV_ASSIGN  */
  YYSYMBOL_MOD_ASSIGN = 32,                /* MOD_ASSIGN  */
  YYSYMBOL_EQ = 33,                        /* EQ  */
  YYSYMBOL_NE = 34,                        /* NE  */
  YYSYMBOL_GT = 35,                        /* GT  */
  YYSYMBOL_GE = 36,                        /* GE  */
  YYSYMBOL_LT = 37,                        /* LT  */
  YYSYMBOL_LE = 38,                        /* LE  */
  YYSYMBOL_AND = 39,                       /* AND  */
  YYSYMBOL_OR = 40,                        /* OR  */
  YYSYMBOL_NOT = 41,                       /* NOT  */
  YYSYMBOL_LP = 42,                        /* LP  */
  YYSYMBOL_RP = 43,                        /* RP  */
  YYSYMBOL_LC = 44,                        /* LC  */
  YYSYMBOL_RC = 45,                        /* RC  */
  YYSYMBOL_LB = 46,                        /* LB  */
  YYSYMBOL_RB = 47,                        /* RB  */
  YYSYMBOL_COMMA = 48,                     /* COMMA  */
  YYSYMBOL_SEMI = 49,                      /* SEMI  */
  YYSYMBOL_DOT = 50,                       /* DOT  */
  YYSYMBOL_UMINUS = 51,                    /* UMINUS  */
  YYSYMBOL_UPLUS = 52,                     /* UPLUS  */
  YYSYMBOL_LOWER_THAN_ELSE = 53,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 54,                  /* $accept  */
  YYSYMBOL_program = 55,                   /* program  */
  YYSYMBOL_ext_def_list = 56,              /* ext_def_list  */
  YYSYMBOL_ext_def = 57,                   /* ext_def  */
  YYSYMBOL_specifier = 58,                 /* specifier  */
  YYSYMBOL_ext_dec_list = 59,              /* ext_dec_list  */
  YYSYMBOL_var_dec = 60,                   /* var_dec  */
  YYSYMBOL_func_dec = 61,                  /* func_dec  */
  YYSYMBOL_var_list = 62,                  /* var_list  */
  YYSYMBOL_param_dec = 63,                 /* param_dec  */
  YYSYMBOL_comp_st = 64,                   /* comp_st  */
  YYSYMBOL_def_list = 65,                  /* def_list  */
  YYSYMBOL_def = 66,                       /* def  */
  YYSYMBOL_local_def = 67,                 /* local_def  */
  YYSYMBOL_dec_list = 68,                  /* dec_list  */
  YYSYMBOL_dec = 69,                       /* dec  */
  YYSYMBOL_stmt_list = 70,                 /* stmt_list  */
  YYSYMBOL_stmt = 71,                      /* stmt  */
  YYSYMBOL_exp = 72,                       /* exp  */
  YYSYMBOL_args = 73                       /* args  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   946

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  86
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  186

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   308


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    57,    57,    65,    69,    75,    80,    85,    89,    95,
     101,   107,   113,   122,   126,   133,   138,   147,   153,   162,
     166,   173,   180,   184,   191,   195,   201,   208,   212,   219,
     223,   230,   233,   240,   244,   251,   254,   258,   261,   265,
     269,   273,   277,   282,   287,   291,   295,   299,   304,   308,
     312,   318,   323,   328,   333,   338,   343,   348,   353,   358,
     363,   368,   373,   378,   383,   388,   393,   398,   403,   408,
     413,   418,   423,   428,   433,   438,   443,   448,   451,   457,
     463,   467,   472,   477,   482,   490,   494
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOKEN_INT",
  "TOKEN_CHAR", "TOKEN_FLOAT", "TOKEN_ID", "TOKEN_STRING",
  "TOKEN_TYPE_INT", "TOKEN_TYPE_FLOAT", "TOKEN_TYPE_CHAR",
  "TOKEN_TYPE_VOID", "IF", "ELSE", "WHILE", "FOR", "BREAK", "CONTINUE",
  "RETURN", "CONST", "INC", "DEC", "PLUS", "MINUS", "STAR", "DIV", "MOD",
  "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN",
  "MOD_ASSIGN", "EQ", "NE", "GT", "GE", "LT", "LE", "AND", "OR", "NOT",
  "LP", "RP", "LC", "RC", "LB", "RB", "COMMA", "SEMI", "DOT", "UMINUS",
  "UPLUS", "LOWER_THAN_ELSE", "$accept", "program", "ext_def_list",
  "ext_def", "specifier", "ext_dec_list", "var_dec", "func_dec",
  "var_list", "param_dec", "comp_st", "def_list", "def", "local_def",
  "dec_list", "dec", "stmt_list", "stmt", "exp", "args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-102)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-35)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     160,   -41,  -102,  -102,  -102,  -102,    31,  -102,   160,    27,
    -102,  -102,  -102,    -8,   -14,   -26,     0,    28,  -102,    76,
      42,    44,   195,  -102,  -102,    44,    29,    25,  -102,  -102,
    -102,    36,    76,    76,    76,    76,    76,    76,   334,    38,
    -102,  -102,   -43,    34,    49,    58,    72,    37,    66,   144,
      44,  -102,   240,   169,    73,    79,   240,   364,    75,  -102,
     169,     6,    81,    81,    81,    81,    81,   630,  -102,  -102,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
    -102,  -102,  -102,    76,    76,    84,  -102,  -102,  -102,   394,
     -21,    80,    83,    44,    87,    44,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,   544,    85,  -102,   246,   246,    81,    81,
      81,    88,    88,    88,    88,    88,    88,   900,   900,   201,
     201,   201,   201,   873,   846,   602,   657,   684,    76,    86,
     424,  -102,    76,  -102,    44,  -102,  -102,    -6,    80,    76,
    -102,  -102,   285,   285,   454,    76,   153,   573,  -102,    76,
    -102,   125,  -102,    76,   484,    76,   514,   819,   285,   711,
      76,   738,   310,  -102,   285,   765,   285,   285,   792,  -102,
     285,  -102,  -102,   285,  -102,  -102
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     9,    10,    11,    12,     0,     2,     0,     0,
       8,     1,     3,    15,     0,    13,     0,     0,     5,     0,
       0,     0,     0,     7,    18,     0,     0,    20,    82,    84,
      83,    81,     0,     0,     0,     0,     0,     0,     0,     0,
      15,    14,    13,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    25,     0,     0,     0,     0,    21,    17,
       0,     0,    70,    72,    75,    74,    76,     0,    71,    73,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,    16,    50,     0,     0,     0,    48,    49,    39,     0,
      31,     0,    29,     0,     0,     0,    24,    35,    23,    33,
      36,    19,    79,    85,     0,    77,    65,    66,    67,    68,
      69,    51,    52,    53,    54,    55,    56,    59,    60,    62,
      64,    61,    63,    58,    57,     0,     0,     0,     0,     0,
       0,    38,     0,    26,     0,    27,    22,    31,     0,     0,
      78,    80,     0,     0,     0,     0,     0,    28,    30,     0,
      86,    40,    42,     0,     0,     0,     0,    32,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,    44,
       0,    45,    46,     0,    47,    43
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -102,  -102,   131,  -102,    24,   120,    -2,  -102,    92,  -102,
     127,   101,  -102,    50,  -101,  -102,    64,   166,   -19,    13
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     6,     7,     8,   103,    14,   100,    16,    26,    27,
      51,    52,    53,    54,   101,   102,    55,    56,    57,   114
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,    19,   145,    20,   148,    21,   142,    15,    10,    28,
      29,    30,    31,    62,    63,    64,    65,    66,    67,    42,
      20,   159,    21,    58,     9,    20,    32,    33,    34,    35,
      99,    11,     9,    13,    17,    18,     2,     3,     4,     5,
      20,    25,   113,   158,    22,    39,    50,    36,    37,   112,
      40,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    24,    59,    60,   136,   137,   140,   105,    61,    28,
      29,    30,    31,    92,    25,    91,    96,    28,    29,    30,
      31,    93,     2,     3,     4,     5,    32,    33,    34,    35,
      94,    68,    69,   147,    32,    33,    34,    35,    75,    76,
      77,    78,    79,    80,    95,    97,   104,    36,    37,   154,
     109,    20,   107,   157,   108,    36,    37,    89,   150,   143,
     113,   144,   146,   138,    89,   155,   164,   166,   168,    12,
     167,    41,   147,    23,   169,   139,   171,    28,    29,    30,
      31,   175,   111,   178,   106,     0,    28,    29,    30,    31,
      -4,     1,   160,     0,    32,    33,    34,    35,     2,     3,
       4,     5,     0,    32,    33,    34,    35,     2,     3,     4,
       5,     0,     0,     0,     0,    36,    37,     0,     0,     0,
       0,     0,     0,    98,    36,    37,    43,     0,    28,    29,
      30,    31,   165,     2,     3,     4,     5,    44,     0,    45,
      46,    47,    48,    49,     0,    32,    33,    34,    35,     0,
       0,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    36,    37,     0,    22,
     -25,    43,     0,    28,    29,    30,    31,    89,     2,     3,
       4,     5,    44,     0,    45,    46,    47,    48,    49,     0,
      32,    33,    34,    35,     0,     0,    68,    69,     0,     0,
      72,    73,    74,    75,    76,    77,    78,    79,    80,     0,
       0,    36,    37,     0,    22,   -34,    43,     0,    28,    29,
      30,    31,    89,     2,     3,     4,     5,    44,     0,    45,
      46,    47,    48,    49,     0,    32,    33,    34,    35,     0,
       0,     0,     0,    28,    29,    30,    31,     0,   161,   162,
       0,     0,     0,     0,     0,     0,    36,    37,     0,    22,
      32,    33,    34,    35,   173,     0,     0,     0,     0,     0,
     179,     0,   181,   182,     0,     0,   184,     0,     0,   185,
       0,    36,    37,   177,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,     0,     0,     0,
      89,     0,     0,    90,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,     0,     0,     0,
      89,     0,     0,   110,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,     0,     0,     0,
      89,     0,     0,   141,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,     0,     0,     0,
      89,     0,     0,   156,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,     0,     0,     0,
      89,     0,     0,   163,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,     0,     0,     0,
      89,     0,     0,   170,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,     0,     0,     0,
      89,     0,     0,   172,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,     0,     0,     0,
      89,     0,   149,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,     0,     0,     0,     0,     0,    89,
       0,   -32,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,     0,     0,     0,     0,     0,    89,   151,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,     0,     0,   115,     0,     0,    89,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,     0,     0,
     152,     0,     0,    89,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,   153,     0,     0,
      89,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,   174,     0,     0,    89,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,     0,
       0,   176,     0,     0,    89,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,     0,     0,   180,     0,
       0,    89,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,     0,     0,   183,     0,     0,    89,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
       0,     0,     0,     0,     0,    89,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,     0,     0,     0,     0,
       0,     0,    89,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,     0,     0,     0,     0,     0,     0,     0,    89,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,     0,     0,     0,    89
};

static const yytype_int16 yycheck[] =
{
      19,    27,   103,    46,   105,    48,    27,     9,    49,     3,
       4,     5,     6,    32,    33,    34,    35,    36,    37,    21,
      46,    27,    48,    25,     0,    46,    20,    21,    22,    23,
      49,     0,     8,     6,    42,    49,     8,     9,    10,    11,
      46,    17,    61,   144,    44,     3,    22,    41,    42,    43,
       6,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    43,    43,    48,    93,    94,    95,    53,    42,     3,
       4,     5,     6,    49,    60,    47,    49,     3,     4,     5,
       6,    42,     8,     9,    10,    11,    20,    21,    22,    23,
      42,    20,    21,   105,    20,    21,    22,    23,    27,    28,
      29,    30,    31,    32,    42,    49,    52,    41,    42,   138,
      56,    46,    49,   142,    45,    41,    42,    46,    43,    49,
     149,    48,    45,    49,    46,    49,   155,   156,    13,     8,
     159,    21,   144,    16,   163,    95,   165,     3,     4,     5,
       6,   170,    60,   172,    53,    -1,     3,     4,     5,     6,
       0,     1,   149,    -1,    20,    21,    22,    23,     8,     9,
      10,    11,    -1,    20,    21,    22,    23,     8,     9,    10,
      11,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    41,    42,     1,    -1,     3,     4,
       5,     6,    49,     8,     9,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    -1,    20,    21,    22,    23,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    41,    42,    -1,    44,
      45,     1,    -1,     3,     4,     5,     6,    46,     8,     9,
      10,    11,    12,    -1,    14,    15,    16,    17,    18,    -1,
      20,    21,    22,    23,    -1,    -1,    20,    21,    -1,    -1,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    41,    42,    -1,    44,    45,     1,    -1,     3,     4,
       5,     6,    46,     8,     9,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    -1,    20,    21,    22,    23,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    44,
      20,    21,    22,    23,   168,    -1,    -1,    -1,    -1,    -1,
     174,    -1,   176,   177,    -1,    -1,   180,    -1,    -1,   183,
      -1,    41,    42,    43,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    49,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    48,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    48,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    46,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    -1,
      43,    -1,    -1,    46,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,
      46,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    43,    -1,    -1,    46,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    43,    -1,    -1,    46,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    46,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    46,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    -1,    -1,    46,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     8,     9,    10,    11,    55,    56,    57,    58,
      49,     0,    56,     6,    59,    60,    61,    42,    49,    27,
      46,    48,    44,    64,    43,    58,    62,    63,     3,     4,
       5,     6,    20,    21,    22,    23,    41,    42,    72,     3,
       6,    59,    60,     1,    12,    14,    15,    16,    17,    18,
      58,    64,    65,    66,    67,    70,    71,    72,    60,    43,
      48,    42,    72,    72,    72,    72,    72,    72,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    46,
      49,    47,    49,    42,    42,    42,    49,    49,    49,    72,
      60,    68,    69,    58,    70,    58,    65,    49,    45,    70,
      49,    62,    43,    72,    73,    43,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    49,    67,
      72,    49,    27,    49,    48,    68,    45,    60,    68,    48,
      43,    47,    43,    43,    72,    49,    49,    72,    68,    27,
      73,    71,    71,    49,    72,    49,    72,    72,    13,    72,
      49,    72,    49,    71,    43,    72,    43,    43,    72,    71,
      43,    71,    71,    43,    71,    71
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    54,    55,    56,    56,    57,    57,    57,    57,    58,
      58,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    64,    64,    65,    65,    66,    67,    67,    68,
      68,    69,    69,    70,    70,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    73,    73
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     5,     3,     2,     1,
       1,     1,     1,     1,     3,     1,     4,     4,     3,     3,
       1,     2,     4,     3,     2,     0,     3,     2,     4,     1,
       3,     1,     3,     2,     0,     2,     2,     1,     3,     2,
       5,     7,     5,     9,     8,     8,     8,     9,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     2,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: ext_def_list  */
#line 57 "/home/annie/compiler-lab/src/parser/parser.y"
                   { 
        root = (yyvsp[0].ptr);  // 设置全局根节点
        display(root, 0);  // 显示抽象语法树
        (yyval.ptr) = root;
    }
#line 1808 "parser.tab.c"
    break;

  case 3: /* ext_def_list: ext_def ext_def_list  */
#line 65 "/home/annie/compiler-lab/src/parser/parser.y"
                           {
        ASTNode *t = mknode(2, EXT_DEF_LIST, yylineno, (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1817 "parser.tab.c"
    break;

  case 4: /* ext_def_list: %empty  */
#line 69 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        (yyval.ptr) = NULL;
    }
#line 1825 "parser.tab.c"
    break;

  case 5: /* ext_def: specifier ext_dec_list SEMI  */
#line 75 "/home/annie/compiler-lab/src/parser/parser.y"
                                  {
        ASTNode *t = mknode(2, EXT_VAR_DEF, yylineno, (yyvsp[-2].ptr), (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 1834 "parser.tab.c"
    break;

  case 6: /* ext_def: specifier var_dec ASSIGN exp SEMI  */
#line 80 "/home/annie/compiler-lab/src/parser/parser.y"
                                        {
        ASTNode *t = mknode(2, EXT_VAR_DEF, yylineno, (yyvsp[-4].ptr), 
            mknode(2, INIT_DEC, yylineno, (yyvsp[-3].ptr), (yyvsp[-1].ptr)));
        (yyval.ptr) = t;
    }
#line 1844 "parser.tab.c"
    break;

  case 7: /* ext_def: specifier func_dec comp_st  */
#line 85 "/home/annie/compiler-lab/src/parser/parser.y"
                                 {
        ASTNode *t = mknode(3, FUNC_DEF, yylineno, (yyvsp[-2].ptr), (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1853 "parser.tab.c"
    break;

  case 8: /* ext_def: error SEMI  */
#line 89 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        (yyval.ptr) = NULL;
    }
#line 1861 "parser.tab.c"
    break;

  case 9: /* specifier: TOKEN_TYPE_INT  */
#line 95 "/home/annie/compiler-lab/src/parser/parser.y"
                     {
        ASTNode *t = mknode(0, TYPE_NODE_INT, yylineno);
        strcpy(t->type_id, "int");
        t->type = 1;  // 类型编码
        (yyval.ptr) = t;
    }
#line 1872 "parser.tab.c"
    break;

  case 10: /* specifier: TOKEN_TYPE_FLOAT  */
#line 101 "/home/annie/compiler-lab/src/parser/parser.y"
                       {
        ASTNode *t = mknode(0, TYPE_NODE_FLOAT, yylineno);
        strcpy(t->type_id, "float");
        t->type = 2;
        (yyval.ptr) = t;
    }
#line 1883 "parser.tab.c"
    break;

  case 11: /* specifier: TOKEN_TYPE_CHAR  */
#line 107 "/home/annie/compiler-lab/src/parser/parser.y"
                      {
        ASTNode *t = mknode(0, TYPE_NODE_CHAR, yylineno);
        strcpy(t->type_id, "char");
        t->type = 3;
        (yyval.ptr) = t;
    }
#line 1894 "parser.tab.c"
    break;

  case 12: /* specifier: TOKEN_TYPE_VOID  */
#line 113 "/home/annie/compiler-lab/src/parser/parser.y"
                      {
        ASTNode *t = mknode(0, TYPE_NODE_VOID, yylineno);
        strcpy(t->type_id, "void");
        t->type = 0;
        (yyval.ptr) = t;
    }
#line 1905 "parser.tab.c"
    break;

  case 13: /* ext_dec_list: var_dec  */
#line 122 "/home/annie/compiler-lab/src/parser/parser.y"
              {
        ASTNode *t = mknode(1, EXT_DEC_LIST, yylineno, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1914 "parser.tab.c"
    break;

  case 14: /* ext_dec_list: var_dec COMMA ext_dec_list  */
#line 126 "/home/annie/compiler-lab/src/parser/parser.y"
                                 {
        ASTNode *t = mknode(2, EXT_DEC_LIST, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1923 "parser.tab.c"
    break;

  case 15: /* var_dec: TOKEN_ID  */
#line 133 "/home/annie/compiler-lab/src/parser/parser.y"
               {
        ASTNode *t = mknode(0, ID_NODE, yylineno);
        strcpy(t->type_id, (yyvsp[0].type_id));
        (yyval.ptr) = t;
    }
#line 1933 "parser.tab.c"
    break;

  case 16: /* var_dec: var_dec LB TOKEN_INT RB  */
#line 138 "/home/annie/compiler-lab/src/parser/parser.y"
                              {
        // 数组声明：处理多维数组
        ASTNode *t = mknode(2, ARRAY_DEC, yylineno, (yyvsp[-3].ptr), mknode(0, INT_NODE, yylineno));
        t->ptr[1]->type_int = (yyvsp[-1].type_int);
        (yyval.ptr) = t;
    }
#line 1944 "parser.tab.c"
    break;

  case 17: /* func_dec: TOKEN_ID LP var_list RP  */
#line 147 "/home/annie/compiler-lab/src/parser/parser.y"
                              {
        ASTNode *t = mknode(2, FUNC_DEC, yylineno, 
                           mknode(0, ID_NODE, yylineno), (yyvsp[-1].ptr));
        strcpy(t->ptr[0]->type_id, (yyvsp[-3].type_id));
        (yyval.ptr) = t;
    }
#line 1955 "parser.tab.c"
    break;

  case 18: /* func_dec: TOKEN_ID LP RP  */
#line 153 "/home/annie/compiler-lab/src/parser/parser.y"
                     {
        ASTNode *t = mknode(1, FUNC_DEC, yylineno, 
                           mknode(0, ID_NODE, yylineno));
        strcpy(t->ptr[0]->type_id, (yyvsp[-2].type_id));
        (yyval.ptr) = t;
    }
#line 1966 "parser.tab.c"
    break;

  case 19: /* var_list: param_dec COMMA var_list  */
#line 162 "/home/annie/compiler-lab/src/parser/parser.y"
                               {
        ASTNode *t = mknode(2, VAR_LIST, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1975 "parser.tab.c"
    break;

  case 20: /* var_list: param_dec  */
#line 166 "/home/annie/compiler-lab/src/parser/parser.y"
                {
        ASTNode *t = mknode(1, VAR_LIST, yylineno, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1984 "parser.tab.c"
    break;

  case 21: /* param_dec: specifier var_dec  */
#line 173 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(2, PARAM_DEC, yylineno, (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1993 "parser.tab.c"
    break;

  case 22: /* comp_st: LC def_list stmt_list RC  */
#line 180 "/home/annie/compiler-lab/src/parser/parser.y"
                               {
        ASTNode *t = mknode(2, COMP_ST, yylineno, (yyvsp[-2].ptr), (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2002 "parser.tab.c"
    break;

  case 23: /* comp_st: LC stmt_list RC  */
#line 184 "/home/annie/compiler-lab/src/parser/parser.y"
                      {  // 添加这个规则：没有变量声明的复合语句
        ASTNode *t = mknode(1, COMP_ST, yylineno, (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2011 "parser.tab.c"
    break;

  case 24: /* def_list: def def_list  */
#line 191 "/home/annie/compiler-lab/src/parser/parser.y"
                   {
        ASTNode *t = mknode(2, DEF_LIST, yylineno, (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2020 "parser.tab.c"
    break;

  case 25: /* def_list: %empty  */
#line 195 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        (yyval.ptr) = NULL;
    }
#line 2028 "parser.tab.c"
    break;

  case 26: /* def: specifier dec_list SEMI  */
#line 201 "/home/annie/compiler-lab/src/parser/parser.y"
                              {
        ASTNode *t = mknode(2, DEF, yylineno, (yyvsp[-2].ptr), (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2037 "parser.tab.c"
    break;

  case 27: /* local_def: specifier dec_list  */
#line 208 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, DEF, yylineno, (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2046 "parser.tab.c"
    break;

  case 28: /* local_def: specifier var_dec ASSIGN exp  */
#line 212 "/home/annie/compiler-lab/src/parser/parser.y"
                                   {
        ASTNode *t = mknode(2, DEF, yylineno, (yyvsp[-3].ptr), mknode(2, INIT_DEC, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr)));
        (yyval.ptr) = t;
    }
#line 2055 "parser.tab.c"
    break;

  case 29: /* dec_list: dec  */
#line 219 "/home/annie/compiler-lab/src/parser/parser.y"
          {
        ASTNode *t = mknode(1, DEC_LIST, yylineno, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2064 "parser.tab.c"
    break;

  case 30: /* dec_list: dec COMMA dec_list  */
#line 223 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, DEC_LIST, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2073 "parser.tab.c"
    break;

  case 31: /* dec: var_dec  */
#line 230 "/home/annie/compiler-lab/src/parser/parser.y"
              {
        (yyval.ptr) = (yyvsp[0].ptr);
    }
#line 2081 "parser.tab.c"
    break;

  case 32: /* dec: var_dec ASSIGN exp  */
#line 233 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, INIT_DEC, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2090 "parser.tab.c"
    break;

  case 33: /* stmt_list: stmt stmt_list  */
#line 240 "/home/annie/compiler-lab/src/parser/parser.y"
                     {
        ASTNode *t = mknode(2, STMT_LIST, yylineno, (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2099 "parser.tab.c"
    break;

  case 34: /* stmt_list: %empty  */
#line 244 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        (yyval.ptr) = NULL;
    }
#line 2107 "parser.tab.c"
    break;

  case 35: /* stmt: local_def SEMI  */
#line 251 "/home/annie/compiler-lab/src/parser/parser.y"
                     {  // 例如：int a = 0, b = 1;
        (yyval.ptr) = (yyvsp[-1].ptr);
    }
#line 2115 "parser.tab.c"
    break;

  case 36: /* stmt: exp SEMI  */
#line 254 "/home/annie/compiler-lab/src/parser/parser.y"
               {
        ASTNode *t = mknode(1, EXP_STMT, yylineno, (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2124 "parser.tab.c"
    break;

  case 37: /* stmt: comp_st  */
#line 258 "/home/annie/compiler-lab/src/parser/parser.y"
              {
        (yyval.ptr) = (yyvsp[0].ptr);
    }
#line 2132 "parser.tab.c"
    break;

  case 38: /* stmt: RETURN exp SEMI  */
#line 261 "/home/annie/compiler-lab/src/parser/parser.y"
                      {
        ASTNode *t = mknode(1, RETURN_STMT, yylineno, (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2141 "parser.tab.c"
    break;

  case 39: /* stmt: RETURN SEMI  */
#line 265 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        ASTNode *t = mknode(0, RETURN_STMT, yylineno);
        (yyval.ptr) = t;
    }
#line 2150 "parser.tab.c"
    break;

  case 40: /* stmt: IF LP exp RP stmt  */
#line 269 "/home/annie/compiler-lab/src/parser/parser.y"
                                              {
        ASTNode *t = mknode(2, IF_STMT, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2159 "parser.tab.c"
    break;

  case 41: /* stmt: IF LP exp RP stmt ELSE stmt  */
#line 273 "/home/annie/compiler-lab/src/parser/parser.y"
                                  {
        ASTNode *t = mknode(3, IF_ELSE_STMT, yylineno, (yyvsp[-4].ptr), (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2168 "parser.tab.c"
    break;

  case 42: /* stmt: WHILE LP exp RP stmt  */
#line 277 "/home/annie/compiler-lab/src/parser/parser.y"
                           {
        ASTNode *t = mknode(2, WHILE_STMT, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2177 "parser.tab.c"
    break;

  case 43: /* stmt: FOR LP exp SEMI exp SEMI exp RP stmt  */
#line 282 "/home/annie/compiler-lab/src/parser/parser.y"
                                           {  // for (exp; exp; exp) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, (yyvsp[-6].ptr), (yyvsp[-4].ptr), (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2186 "parser.tab.c"
    break;

  case 44: /* stmt: FOR LP SEMI exp SEMI exp RP stmt  */
#line 287 "/home/annie/compiler-lab/src/parser/parser.y"
                                       {  // for (; exp; exp) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, NULL, (yyvsp[-4].ptr), (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2195 "parser.tab.c"
    break;

  case 45: /* stmt: FOR LP exp SEMI SEMI exp RP stmt  */
#line 291 "/home/annie/compiler-lab/src/parser/parser.y"
                                       {  // for (exp; ; exp) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, (yyvsp[-5].ptr), NULL, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2204 "parser.tab.c"
    break;

  case 46: /* stmt: FOR LP exp SEMI exp SEMI RP stmt  */
#line 295 "/home/annie/compiler-lab/src/parser/parser.y"
                                       {  // for (exp; exp; ) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, (yyvsp[-5].ptr), (yyvsp[-3].ptr), NULL, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2213 "parser.tab.c"
    break;

  case 47: /* stmt: FOR LP local_def SEMI exp SEMI exp RP stmt  */
#line 299 "/home/annie/compiler-lab/src/parser/parser.y"
                                                 {  // for (int i=0; i<n; i++) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, (yyvsp[-6].ptr), (yyvsp[-4].ptr), (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2222 "parser.tab.c"
    break;

  case 48: /* stmt: BREAK SEMI  */
#line 304 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(0, BREAK_STMT, yylineno);
        (yyval.ptr) = t;
    }
#line 2231 "parser.tab.c"
    break;

  case 49: /* stmt: CONTINUE SEMI  */
#line 308 "/home/annie/compiler-lab/src/parser/parser.y"
                    {
        ASTNode *t = mknode(0, CONTINUE_STMT, yylineno);
        (yyval.ptr) = t;
    }
#line 2240 "parser.tab.c"
    break;

  case 50: /* stmt: error SEMI  */
#line 312 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        (yyval.ptr) = NULL;
    }
#line 2248 "parser.tab.c"
    break;

  case 51: /* exp: exp ASSIGN exp  */
#line 318 "/home/annie/compiler-lab/src/parser/parser.y"
                     {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "=");
        (yyval.ptr) = t;
    }
#line 2258 "parser.tab.c"
    break;

  case 52: /* exp: exp ADD_ASSIGN exp  */
#line 323 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "+=");
        (yyval.ptr) = t;
    }
#line 2268 "parser.tab.c"
    break;

  case 53: /* exp: exp SUB_ASSIGN exp  */
#line 328 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "-=");
        (yyval.ptr) = t;
    }
#line 2278 "parser.tab.c"
    break;

  case 54: /* exp: exp MUL_ASSIGN exp  */
#line 333 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "*=");
        (yyval.ptr) = t;
    }
#line 2288 "parser.tab.c"
    break;

  case 55: /* exp: exp DIV_ASSIGN exp  */
#line 338 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "/=");
        (yyval.ptr) = t;
    }
#line 2298 "parser.tab.c"
    break;

  case 56: /* exp: exp MOD_ASSIGN exp  */
#line 343 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "%=");
        (yyval.ptr) = t;
    }
#line 2308 "parser.tab.c"
    break;

  case 57: /* exp: exp OR exp  */
#line 348 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "||");
        (yyval.ptr) = t;
    }
#line 2318 "parser.tab.c"
    break;

  case 58: /* exp: exp AND exp  */
#line 353 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "&&");
        (yyval.ptr) = t;
    }
#line 2328 "parser.tab.c"
    break;

  case 59: /* exp: exp EQ exp  */
#line 358 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "==");
        (yyval.ptr) = t;
    }
#line 2338 "parser.tab.c"
    break;

  case 60: /* exp: exp NE exp  */
#line 363 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "!=");
        (yyval.ptr) = t;
    }
#line 2348 "parser.tab.c"
    break;

  case 61: /* exp: exp LT exp  */
#line 368 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "<");
        (yyval.ptr) = t;
    }
#line 2358 "parser.tab.c"
    break;

  case 62: /* exp: exp GT exp  */
#line 373 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, ">");
        (yyval.ptr) = t;
    }
#line 2368 "parser.tab.c"
    break;

  case 63: /* exp: exp LE exp  */
#line 378 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "<=");
        (yyval.ptr) = t;
    }
#line 2378 "parser.tab.c"
    break;

  case 64: /* exp: exp GE exp  */
#line 383 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, ">=");
        (yyval.ptr) = t;
    }
#line 2388 "parser.tab.c"
    break;

  case 65: /* exp: exp PLUS exp  */
#line 388 "/home/annie/compiler-lab/src/parser/parser.y"
                   {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "+");
        (yyval.ptr) = t;
    }
#line 2398 "parser.tab.c"
    break;

  case 66: /* exp: exp MINUS exp  */
#line 393 "/home/annie/compiler-lab/src/parser/parser.y"
                    {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "-");
        (yyval.ptr) = t;
    }
#line 2408 "parser.tab.c"
    break;

  case 67: /* exp: exp STAR exp  */
#line 398 "/home/annie/compiler-lab/src/parser/parser.y"
                   {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "*");
        (yyval.ptr) = t;
    }
#line 2418 "parser.tab.c"
    break;

  case 68: /* exp: exp DIV exp  */
#line 403 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "/");
        (yyval.ptr) = t;
    }
#line 2428 "parser.tab.c"
    break;

  case 69: /* exp: exp MOD exp  */
#line 408 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "%");
        (yyval.ptr) = t;
    }
#line 2438 "parser.tab.c"
    break;

  case 70: /* exp: INC exp  */
#line 413 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "++");
        (yyval.ptr) = t;
    }
#line 2448 "parser.tab.c"
    break;

  case 71: /* exp: exp INC  */
#line 418 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(1, POST_INC_EXP, yylineno, (yyvsp[-1].ptr));
        strcpy(t->type_id, "++");
        (yyval.ptr) = t;
    }
#line 2458 "parser.tab.c"
    break;

  case 72: /* exp: DEC exp  */
#line 423 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "--");
        (yyval.ptr) = t;
    }
#line 2468 "parser.tab.c"
    break;

  case 73: /* exp: exp DEC  */
#line 428 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(1, POST_DEC_EXP, yylineno, (yyvsp[-1].ptr));
        strcpy(t->type_id, "--");
        (yyval.ptr) = t;
    }
#line 2478 "parser.tab.c"
    break;

  case 74: /* exp: MINUS exp  */
#line 433 "/home/annie/compiler-lab/src/parser/parser.y"
                             {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "-");
        (yyval.ptr) = t;
    }
#line 2488 "parser.tab.c"
    break;

  case 75: /* exp: PLUS exp  */
#line 438 "/home/annie/compiler-lab/src/parser/parser.y"
                           {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "+");
        (yyval.ptr) = t;
    }
#line 2498 "parser.tab.c"
    break;

  case 76: /* exp: NOT exp  */
#line 443 "/home/annie/compiler-lab/src/parser/parser.y"
              {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "!");
        (yyval.ptr) = t;
    }
#line 2508 "parser.tab.c"
    break;

  case 77: /* exp: LP exp RP  */
#line 448 "/home/annie/compiler-lab/src/parser/parser.y"
                {
        (yyval.ptr) = (yyvsp[-1].ptr);
    }
#line 2516 "parser.tab.c"
    break;

  case 78: /* exp: TOKEN_ID LP args RP  */
#line 451 "/home/annie/compiler-lab/src/parser/parser.y"
                          {
        ASTNode *t = mknode(2, FUNC_CALL, yylineno, 
                           mknode(0, ID_NODE, yylineno), (yyvsp[-1].ptr));
        strcpy(t->ptr[0]->type_id, (yyvsp[-3].type_id));
        (yyval.ptr) = t;
    }
#line 2527 "parser.tab.c"
    break;

  case 79: /* exp: TOKEN_ID LP RP  */
#line 457 "/home/annie/compiler-lab/src/parser/parser.y"
                     {
        ASTNode *t = mknode(1, FUNC_CALL, yylineno, 
                           mknode(0, ID_NODE, yylineno));
        strcpy(t->ptr[0]->type_id, (yyvsp[-2].type_id));
        (yyval.ptr) = t;
    }
#line 2538 "parser.tab.c"
    break;

  case 80: /* exp: exp LB exp RB  */
#line 463 "/home/annie/compiler-lab/src/parser/parser.y"
                    {
        ASTNode *t = mknode(2, ARRAY_ACCESS, yylineno, (yyvsp[-3].ptr), (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2547 "parser.tab.c"
    break;

  case 81: /* exp: TOKEN_ID  */
#line 467 "/home/annie/compiler-lab/src/parser/parser.y"
               {
        ASTNode *t = mknode(0, ID_NODE, yylineno);
        strcpy(t->type_id, (yyvsp[0].type_id));
        (yyval.ptr) = t;
    }
#line 2557 "parser.tab.c"
    break;

  case 82: /* exp: TOKEN_INT  */
#line 472 "/home/annie/compiler-lab/src/parser/parser.y"
                {
        ASTNode *t = mknode(0, INT_NODE, yylineno);
        t->type_int = (yyvsp[0].type_int);
        (yyval.ptr) = t;
    }
#line 2567 "parser.tab.c"
    break;

  case 83: /* exp: TOKEN_FLOAT  */
#line 477 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        ASTNode *t = mknode(0, FLOAT_NODE, yylineno);
        t->type_float = (yyvsp[0].type_float);
        (yyval.ptr) = t;
    }
#line 2577 "parser.tab.c"
    break;

  case 84: /* exp: TOKEN_CHAR  */
#line 482 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(0, CHAR_NODE, yylineno);
        t->type_int = (yyvsp[0].type_int);  // 字符值存储在type_int中
        (yyval.ptr) = t;
    }
#line 2587 "parser.tab.c"
    break;

  case 85: /* args: exp  */
#line 490 "/home/annie/compiler-lab/src/parser/parser.y"
          {
        ASTNode *t = mknode(1, ARGS, yylineno, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2596 "parser.tab.c"
    break;

  case 86: /* args: exp COMMA args  */
#line 494 "/home/annie/compiler-lab/src/parser/parser.y"
                     {
        ASTNode *t = mknode(2, ARGS, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2605 "parser.tab.c"
    break;


#line 2609 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 500 "/home/annie/compiler-lab/src/parser/parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
}
