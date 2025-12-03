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

#define YYSTYPE YYSTYPE

#define INT TOKEN_INT
#define CHAR TOKEN_CHAR
#define FLOAT TOKEN_FLOAT
#define ID TOKEN_ID
#define STRING TOKEN_STRING
#define TYPE_INT TOKEN_TYPE_INT
#define TYPE_FLOAT TOKEN_TYPE_FLOAT
#define TYPE_CHAR TOKEN_TYPE_CHAR
#define TYPE_VOID TOKEN_TYPE_VOID

#include <stdio.h>
#include <string.h>
#include "/home/annie/compiler-lab/src/utils/def.h"
#include "/home/annie/compiler-lab/src/ast/ast.h"

extern int yylineno;
extern char *yytext;
extern FILE *yyin;

void yyerror(const char *s);
int yylex(void);

#line 97 "/home/annie/compiler-lab/build/parser.tab.c"

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
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_CHAR = 4,                       /* CHAR  */
  YYSYMBOL_FLOAT = 5,                      /* FLOAT  */
  YYSYMBOL_ID = 6,                         /* ID  */
  YYSYMBOL_STRING = 7,                     /* STRING  */
  YYSYMBOL_TYPE_INT = 8,                   /* TYPE_INT  */
  YYSYMBOL_TYPE_FLOAT = 9,                 /* TYPE_FLOAT  */
  YYSYMBOL_TYPE_CHAR = 10,                 /* TYPE_CHAR  */
  YYSYMBOL_TYPE_VOID = 11,                 /* TYPE_VOID  */
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
#define YYLAST   966

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  87
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  188

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
       0,    66,    66,    73,    77,    83,    88,    93,    97,   103,
     109,   115,   121,   130,   134,   141,   146,   155,   161,   170,
     174,   181,   188,   192,   199,   203,   209,   214,   214,   222,
     226,   233,   236,   243,   247,   254,   258,   263,   267,   270,
     274,   278,   282,   286,   292,   297,   301,   305,   309,   315,
     319,   323,   329,   334,   339,   344,   349,   354,   359,   364,
     369,   374,   379,   384,   389,   394,   399,   404,   409,   414,
     419,   424,   429,   434,   439,   444,   449,   454,   459,   462,
     468,   474,   478,   483,   488,   493,   501,   505
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
  "\"end of file\"", "error", "\"invalid token\"", "INT", "CHAR", "FLOAT",
  "ID", "STRING", "TYPE_INT", "TYPE_FLOAT", "TYPE_CHAR", "TYPE_VOID", "IF",
  "ELSE", "WHILE", "FOR", "BREAK", "CONTINUE", "RETURN", "CONST", "INC",
  "DEC", "PLUS", "MINUS", "STAR", "DIV", "MOD", "ASSIGN", "ADD_ASSIGN",
  "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "EQ", "NE", "GT",
  "GE", "LT", "LE", "AND", "OR", "NOT", "LP", "RP", "LC", "RC", "LB", "RB",
  "COMMA", "SEMI", "DOT", "UMINUS", "UPLUS", "LOWER_THAN_ELSE", "$accept",
  "program", "ext_def_list", "ext_def", "specifier", "ext_dec_list",
  "var_dec", "func_dec", "var_list", "param_dec", "comp_st", "def_list",
  "def", "local_def", "dec_list", "dec", "stmt_list", "stmt", "exp",
  "args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-103)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-35)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      39,    -8,  -103,  -103,  -103,  -103,    22,  -103,    39,    21,
    -103,  -103,  -103,     3,    -3,   -22,    27,     0,  -103,   318,
      69,    71,   195,  -103,  -103,    71,    35,    31,  -103,  -103,
    -103,    40,   318,   318,   318,   318,   318,   318,   341,    34,
    -103,  -103,   -42,    36,    41,    42,    45,    50,    51,   129,
      71,  -103,   240,    25,    52,    43,   240,   371,    48,  -103,
      25,   310,   920,   920,   920,   920,   920,   637,  -103,  -103,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
    -103,  -103,  -103,   318,   318,    87,  -103,  -103,  -103,   401,
     -25,    53,    56,  -103,    44,    71,  -103,    25,  -103,  -103,
    -103,  -103,  -103,  -103,   551,    62,  -103,   246,   246,   920,
     920,   920,    60,    60,    60,    60,    60,    60,   907,   907,
     201,   201,   201,   201,   880,   853,   609,   664,   691,   318,
      71,    63,   431,  -103,   318,  -103,    71,  -103,   -15,    53,
     318,  -103,  -103,   285,   285,   461,  -103,   318,   153,   580,
    -103,   318,  -103,    98,  -103,   318,   491,   318,   521,   826,
     285,   718,   318,   745,   314,  -103,   285,   772,   285,   285,
     799,  -103,   285,  -103,  -103,   285,  -103,  -103
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     9,    10,    11,    12,     0,     2,     0,     0,
       8,     1,     3,    15,     0,    13,     0,     0,     5,     0,
       0,     0,     0,     7,    18,     0,     0,    20,    83,    85,
      84,    82,     0,     0,     0,     0,     0,     0,     0,     0,
      15,    14,    13,     0,     0,     0,     0,     0,     0,     0,
       0,    38,     0,    25,     0,     0,     0,     0,    21,    17,
       0,     0,    71,    73,    76,    75,    77,     0,    72,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,    16,    51,     0,     0,     0,    49,    50,    40,     0,
      31,     0,    29,    36,     0,     0,    24,    25,    35,    23,
      33,    37,    19,    80,    86,     0,    78,    66,    67,    68,
      69,    70,    52,    53,    54,    55,    56,    57,    60,    61,
      63,    65,    62,    64,    59,    58,     0,     0,     0,     0,
       0,     0,     0,    39,     0,    26,     0,    22,    31,     0,
       0,    79,    81,     0,     0,     0,    27,     0,     0,    28,
      30,     0,    87,    41,    43,     0,     0,     0,     0,    32,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,    45,     0,    46,    47,     0,    48,    44
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -103,  -103,   105,  -103,    20,    93,    -2,  -103,    57,  -103,
     100,    96,   -21,    24,  -102,  -103,   -27,     7,   -19,   -29
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     6,     7,     8,    50,    14,   100,    16,    26,    27,
      51,   106,   103,    54,   101,   102,    55,    56,    57,   115
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,    53,   144,   149,    20,    19,    21,    15,     2,     3,
       4,     5,   161,    62,    63,    64,    65,    66,    67,    42,
       9,    20,    11,    58,    20,   104,    21,    13,     9,   110,
      99,    20,   107,     2,     3,     4,     5,    25,   156,    -4,
       1,    10,   114,    24,   160,    17,    18,     2,     3,     4,
       5,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,    22,    39,   105,   137,   138,   142,    40,    59,    60,
      25,    91,    61,    93,    94,    92,   107,    95,   109,   147,
      28,    29,    30,    31,    20,     2,     3,     4,     5,    96,
      97,   108,   145,   148,   146,   151,    89,    32,    33,    34,
      35,   170,   157,    12,    41,   140,    23,   112,    52,   141,
     155,   162,     0,     0,     0,   159,     0,   105,    36,    37,
       0,   114,    28,    29,    30,    31,   139,     0,   166,   168,
       0,     0,   169,     0,   148,     0,   171,     0,   173,    32,
      33,    34,    35,   177,     0,   180,    28,    29,    30,    31,
     163,   164,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    37,     0,    32,    33,    34,    35,   175,    98,     0,
       0,     0,     0,   181,     0,   183,   184,     0,     0,   186,
       0,     0,   187,     0,    36,    37,    43,     0,    28,    29,
      30,    31,   167,     2,     3,     4,     5,    44,     0,    45,
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
       0,     0,     0,    28,    29,    30,    31,    28,    29,    30,
      31,    28,    29,    30,    31,     0,    36,    37,     0,    22,
      32,    33,    34,    35,    32,    33,    34,    35,    32,    33,
      34,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    37,   113,     0,    36,    37,   179,     0,    36,
      37,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,     0,     0,     0,    89,     0,     0,
      90,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,     0,     0,     0,    89,     0,     0,
     111,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,     0,     0,     0,    89,     0,     0,
     143,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,     0,     0,     0,    89,     0,     0,
     158,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,     0,     0,     0,    89,     0,     0,
     165,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,     0,     0,     0,    89,     0,     0,
     172,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,     0,     0,     0,    89,     0,     0,
     174,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,     0,     0,     0,    89,     0,   150,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,     0,     0,     0,     0,     0,    89,     0,   -32,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
       0,     0,     0,     0,     0,    89,   152,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,     0,     0,
     116,     0,     0,    89,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,   153,     0,     0,
      89,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,     0,     0,   154,     0,     0,    89,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,     0,
       0,   176,     0,     0,    89,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,     0,     0,   178,     0,
       0,    89,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,     0,     0,   182,     0,     0,    89,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
       0,     0,   185,     0,     0,    89,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,     0,     0,     0,
       0,     0,    89,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,     0,     0,     0,     0,     0,     0,    89,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,     0,
       0,     0,     0,     0,     0,     0,    89,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      68,    69,    83,    84,    85,    86,     0,    75,    76,    77,
      78,    79,    80,    89,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89
};

static const yytype_int16 yycheck[] =
{
      19,    22,    27,   105,    46,    27,    48,     9,     8,     9,
      10,    11,    27,    32,    33,    34,    35,    36,    37,    21,
       0,    46,     0,    25,    46,    52,    48,     6,     8,    56,
      49,    46,    53,     8,     9,    10,    11,    17,   140,     0,
       1,    49,    61,    43,   146,    42,    49,     8,     9,    10,
      11,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    44,     3,    53,    93,    94,    95,     6,    43,    48,
      60,    47,    42,    42,    42,    49,   107,    42,    45,    45,
       3,     4,     5,     6,    46,     8,     9,    10,    11,    49,
      49,    49,    49,   105,    48,    43,    46,    20,    21,    22,
      23,    13,    49,     8,    21,    95,    16,    60,    22,    95,
     139,   150,    -1,    -1,    -1,   144,    -1,   107,    41,    42,
      -1,   150,     3,     4,     5,     6,    49,    -1,   157,   158,
      -1,    -1,   161,    -1,   146,    -1,   165,    -1,   167,    20,
      21,    22,    23,   172,    -1,   174,     3,     4,     5,     6,
     153,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    20,    21,    22,    23,   170,    49,    -1,
      -1,    -1,    -1,   176,    -1,   178,   179,    -1,    -1,   182,
      -1,    -1,   185,    -1,    41,    42,     1,    -1,     3,     4,
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
      -1,    -1,    -1,     3,     4,     5,     6,     3,     4,     5,
       6,     3,     4,     5,     6,    -1,    41,    42,    -1,    44,
      20,    21,    22,    23,    20,    21,    22,    23,    20,    21,
      22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    42,    43,    -1,    41,    42,    43,    -1,    41,
      42,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      49,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    48,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    48,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    -1,    -1,    46,    47,    20,    21,    22,
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
      -1,    -1,    43,    -1,    -1,    46,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    -1,    46,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      20,    21,    35,    36,    37,    38,    -1,    27,    28,    29,
      30,    31,    32,    46,    -1,    -1,    -1,    -1,    -1,    -1,
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
      60,    68,    69,    66,    70,    58,    65,    66,    49,    45,
      70,    49,    62,    43,    72,    73,    43,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    49,
      58,    67,    72,    49,    27,    49,    48,    45,    60,    68,
      48,    43,    47,    43,    43,    72,    68,    49,    49,    72,
      68,    27,    73,    71,    71,    49,    72,    49,    72,    72,
      13,    72,    49,    72,    49,    71,    43,    72,    43,    43,
      72,    71,    43,    71,    71,    43,    71,    71
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    54,    55,    56,    56,    57,    57,    57,    57,    58,
      58,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    64,    64,    65,    65,    66,    67,    67,    68,
      68,    69,    69,    70,    70,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    73,    73
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     5,     3,     2,     1,
       1,     1,     1,     1,     3,     1,     4,     4,     3,     3,
       1,     2,     4,     3,     2,     0,     3,     2,     4,     1,
       3,     1,     3,     2,     0,     2,     1,     2,     1,     3,
       2,     5,     7,     5,     9,     8,     8,     8,     9,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     3
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
#line 66 "/home/annie/compiler-lab/src/parser/parser.y"
                   { 
        display((yyvsp[0].ptr), 0);  // 显示抽象语法树
        (yyval.ptr) = (yyvsp[0].ptr);
    }
#line 1819 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 3: /* ext_def_list: ext_def ext_def_list  */
#line 73 "/home/annie/compiler-lab/src/parser/parser.y"
                           {
        ASTNode *t = mknode(2, EXT_DEF_LIST, yylineno, (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1828 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 4: /* ext_def_list: %empty  */
#line 77 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        (yyval.ptr) = NULL;
    }
#line 1836 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 5: /* ext_def: specifier ext_dec_list SEMI  */
#line 83 "/home/annie/compiler-lab/src/parser/parser.y"
                                  {
        ASTNode *t = mknode(2, EXT_VAR_DEF, yylineno, (yyvsp[-2].ptr), (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 1845 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 6: /* ext_def: specifier var_dec ASSIGN exp SEMI  */
#line 88 "/home/annie/compiler-lab/src/parser/parser.y"
                                        {
        ASTNode *t = mknode(2, EXT_VAR_DEF, yylineno, (yyvsp[-4].ptr), 
            mknode(2, INIT_DEC, yylineno, (yyvsp[-3].ptr), (yyvsp[-1].ptr)));
        (yyval.ptr) = t;
    }
#line 1855 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 7: /* ext_def: specifier func_dec comp_st  */
#line 93 "/home/annie/compiler-lab/src/parser/parser.y"
                                 {
        ASTNode *t = mknode(3, FUNC_DEF, yylineno, (yyvsp[-2].ptr), (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1864 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 8: /* ext_def: error SEMI  */
#line 97 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        (yyval.ptr) = NULL;
    }
#line 1872 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 9: /* specifier: TYPE_INT  */
#line 103 "/home/annie/compiler-lab/src/parser/parser.y"
               {
        ASTNode *t = mknode(0, TYPE_NODE_INT, yylineno);
        strcpy(t->type_id, "int");
        t->type = 1;  // 类型编码
        (yyval.ptr) = t;
    }
#line 1883 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 10: /* specifier: TYPE_FLOAT  */
#line 109 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(0, TYPE_NODE_FLOAT, yylineno);
        strcpy(t->type_id, "float");
        t->type = 2;
        (yyval.ptr) = t;
    }
#line 1894 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 11: /* specifier: TYPE_CHAR  */
#line 115 "/home/annie/compiler-lab/src/parser/parser.y"
                {
        ASTNode *t = mknode(0, TYPE_NODE_CHAR, yylineno);
        strcpy(t->type_id, "char");
        t->type = 3;
        (yyval.ptr) = t;
    }
#line 1905 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 12: /* specifier: TYPE_VOID  */
#line 121 "/home/annie/compiler-lab/src/parser/parser.y"
                {
        ASTNode *t = mknode(0, TYPE_NODE_VOID, yylineno);
        strcpy(t->type_id, "void");
        t->type = 0;
        (yyval.ptr) = t;
    }
#line 1916 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 13: /* ext_dec_list: var_dec  */
#line 130 "/home/annie/compiler-lab/src/parser/parser.y"
              {
        ASTNode *t = mknode(1, EXT_DEC_LIST, yylineno, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1925 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 14: /* ext_dec_list: var_dec COMMA ext_dec_list  */
#line 134 "/home/annie/compiler-lab/src/parser/parser.y"
                                 {
        ASTNode *t = mknode(2, EXT_DEC_LIST, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1934 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 15: /* var_dec: ID  */
#line 141 "/home/annie/compiler-lab/src/parser/parser.y"
         {
        ASTNode *t = mknode(0, ID_NODE, yylineno);
        strcpy(t->type_id, (yyvsp[0].type_id));
        (yyval.ptr) = t;
    }
#line 1944 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 16: /* var_dec: var_dec LB INT RB  */
#line 146 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        // 数组声明：处理多维数组
        ASTNode *t = mknode(2, ARRAY_DEC, yylineno, (yyvsp[-3].ptr), mknode(0, INT_NODE, yylineno));
        t->ptr[1]->type_int = (yyvsp[-1].type_int);
        (yyval.ptr) = t;
    }
#line 1955 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 17: /* func_dec: ID LP var_list RP  */
#line 155 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(2, FUNC_DEC, yylineno, 
                           mknode(0, ID_NODE, yylineno), (yyvsp[-1].ptr));
        strcpy(t->ptr[0]->type_id, (yyvsp[-3].type_id));
        (yyval.ptr) = t;
    }
#line 1966 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 18: /* func_dec: ID LP RP  */
#line 161 "/home/annie/compiler-lab/src/parser/parser.y"
               {
        ASTNode *t = mknode(1, FUNC_DEC, yylineno, 
                           mknode(0, ID_NODE, yylineno));
        strcpy(t->ptr[0]->type_id, (yyvsp[-2].type_id));
        (yyval.ptr) = t;
    }
#line 1977 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 19: /* var_list: param_dec COMMA var_list  */
#line 170 "/home/annie/compiler-lab/src/parser/parser.y"
                               {
        ASTNode *t = mknode(2, VAR_LIST, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1986 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 20: /* var_list: param_dec  */
#line 174 "/home/annie/compiler-lab/src/parser/parser.y"
                {
        ASTNode *t = mknode(1, VAR_LIST, yylineno, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 1995 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 21: /* param_dec: specifier var_dec  */
#line 181 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(2, PARAM_DEC, yylineno, (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2004 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 22: /* comp_st: LC def_list stmt_list RC  */
#line 188 "/home/annie/compiler-lab/src/parser/parser.y"
                               {
        ASTNode *t = mknode(2, COMP_ST, yylineno, (yyvsp[-2].ptr), (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2013 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 23: /* comp_st: LC stmt_list RC  */
#line 192 "/home/annie/compiler-lab/src/parser/parser.y"
                       {  // 添加这个规则：没有变量声明的复合语句
        ASTNode *t = mknode(1, COMP_ST, yylineno, (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2022 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 24: /* def_list: def def_list  */
#line 199 "/home/annie/compiler-lab/src/parser/parser.y"
                   {
        ASTNode *t = mknode(2, DEF_LIST, yylineno, (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2031 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 25: /* def_list: %empty  */
#line 203 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        (yyval.ptr) = NULL;
    }
#line 2039 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 26: /* def: specifier dec_list SEMI  */
#line 209 "/home/annie/compiler-lab/src/parser/parser.y"
                              {
        ASTNode *t = mknode(2, DEF, yylineno, (yyvsp[-2].ptr), (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2048 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 28: /* local_def: specifier var_dec ASSIGN exp  */
#line 214 "/home/annie/compiler-lab/src/parser/parser.y"
                                                        {
        ASTNode *t = mknode(2, DEF, yylineno, (yyvsp[-3].ptr), (yyvsp[-2].ptr));
        (yyval.ptr) = t;
    }
#line 2057 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 29: /* dec_list: dec  */
#line 222 "/home/annie/compiler-lab/src/parser/parser.y"
          {
        ASTNode *t = mknode(1, DEC_LIST, yylineno, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2066 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 30: /* dec_list: dec COMMA dec_list  */
#line 226 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, DEC_LIST, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2075 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 31: /* dec: var_dec  */
#line 233 "/home/annie/compiler-lab/src/parser/parser.y"
              {
        (yyval.ptr) = (yyvsp[0].ptr);
    }
#line 2083 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 32: /* dec: var_dec ASSIGN exp  */
#line 236 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, INIT_DEC, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2092 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 33: /* stmt_list: stmt stmt_list  */
#line 243 "/home/annie/compiler-lab/src/parser/parser.y"
                     {
        ASTNode *t = mknode(2, STMT_LIST, yylineno, (yyvsp[-1].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2101 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 34: /* stmt_list: %empty  */
#line 247 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        (yyval.ptr) = NULL;
    }
#line 2109 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 35: /* stmt: local_def SEMI  */
#line 254 "/home/annie/compiler-lab/src/parser/parser.y"
                     {  // 例如：int a = 0, b = 1;
        (yyval.ptr) = (yyvsp[-1].ptr);
    }
#line 2117 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 36: /* stmt: def  */
#line 258 "/home/annie/compiler-lab/src/parser/parser.y"
          {
        ASTNode *t = mknode(1, EXP_STMT, yylineno, (yyvsp[0].ptr));  // 将def包装为语句
        (yyval.ptr) = t;
    }
#line 2126 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 37: /* stmt: exp SEMI  */
#line 263 "/home/annie/compiler-lab/src/parser/parser.y"
               {
        ASTNode *t = mknode(1, EXP_STMT, yylineno, (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2135 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 38: /* stmt: comp_st  */
#line 267 "/home/annie/compiler-lab/src/parser/parser.y"
              {
        (yyval.ptr) = (yyvsp[0].ptr);
    }
#line 2143 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 39: /* stmt: RETURN exp SEMI  */
#line 270 "/home/annie/compiler-lab/src/parser/parser.y"
                      {
        ASTNode *t = mknode(1, RETURN_STMT, yylineno, (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2152 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 40: /* stmt: RETURN SEMI  */
#line 274 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        ASTNode *t = mknode(0, RETURN_STMT, yylineno);
        (yyval.ptr) = t;
    }
#line 2161 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 41: /* stmt: IF LP exp RP stmt  */
#line 278 "/home/annie/compiler-lab/src/parser/parser.y"
                                              {
        ASTNode *t = mknode(2, IF_STMT, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2170 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 42: /* stmt: IF LP exp RP stmt ELSE stmt  */
#line 282 "/home/annie/compiler-lab/src/parser/parser.y"
                                  {
        ASTNode *t = mknode(3, IF_ELSE_STMT, yylineno, (yyvsp[-4].ptr), (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2179 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 43: /* stmt: WHILE LP exp RP stmt  */
#line 286 "/home/annie/compiler-lab/src/parser/parser.y"
                           {
        ASTNode *t = mknode(2, WHILE_STMT, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2188 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 44: /* stmt: FOR LP exp SEMI exp SEMI exp RP stmt  */
#line 292 "/home/annie/compiler-lab/src/parser/parser.y"
                                           {  // for (exp; exp; exp) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, (yyvsp[-6].ptr), (yyvsp[-4].ptr), (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2197 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 45: /* stmt: FOR LP SEMI exp SEMI exp RP stmt  */
#line 297 "/home/annie/compiler-lab/src/parser/parser.y"
                                       {  // for (; exp; exp) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, NULL, (yyvsp[-4].ptr), (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2206 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 46: /* stmt: FOR LP exp SEMI SEMI exp RP stmt  */
#line 301 "/home/annie/compiler-lab/src/parser/parser.y"
                                       {  // for (exp; ; exp) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, (yyvsp[-5].ptr), NULL, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2215 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 47: /* stmt: FOR LP exp SEMI exp SEMI RP stmt  */
#line 305 "/home/annie/compiler-lab/src/parser/parser.y"
                                       {  // for (exp; exp; ) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, (yyvsp[-5].ptr), (yyvsp[-3].ptr), NULL, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2224 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 48: /* stmt: FOR LP local_def SEMI exp SEMI exp RP stmt  */
#line 309 "/home/annie/compiler-lab/src/parser/parser.y"
                                                 {  // for (int i=0; i<n; i++) stmt
        ASTNode *t = mknode(4, FOR_STMT, yylineno, (yyvsp[-6].ptr), (yyvsp[-4].ptr), (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2233 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 49: /* stmt: BREAK SEMI  */
#line 315 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(0, BREAK_STMT, yylineno);
        (yyval.ptr) = t;
    }
#line 2242 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 50: /* stmt: CONTINUE SEMI  */
#line 319 "/home/annie/compiler-lab/src/parser/parser.y"
                    {
        ASTNode *t = mknode(0, CONTINUE_STMT, yylineno);
        (yyval.ptr) = t;
    }
#line 2251 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 51: /* stmt: error SEMI  */
#line 323 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        (yyval.ptr) = NULL;
    }
#line 2259 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 52: /* exp: exp ASSIGN exp  */
#line 329 "/home/annie/compiler-lab/src/parser/parser.y"
                     {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "=");
        (yyval.ptr) = t;
    }
#line 2269 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 53: /* exp: exp ADD_ASSIGN exp  */
#line 334 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "+=");
        (yyval.ptr) = t;
    }
#line 2279 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 54: /* exp: exp SUB_ASSIGN exp  */
#line 339 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "-=");
        (yyval.ptr) = t;
    }
#line 2289 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 55: /* exp: exp MUL_ASSIGN exp  */
#line 344 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "*=");
        (yyval.ptr) = t;
    }
#line 2299 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 56: /* exp: exp DIV_ASSIGN exp  */
#line 349 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "/=");
        (yyval.ptr) = t;
    }
#line 2309 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 57: /* exp: exp MOD_ASSIGN exp  */
#line 354 "/home/annie/compiler-lab/src/parser/parser.y"
                         {
        ASTNode *t = mknode(2, ASSIGN_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "%=");
        (yyval.ptr) = t;
    }
#line 2319 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 58: /* exp: exp OR exp  */
#line 359 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "||");
        (yyval.ptr) = t;
    }
#line 2329 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 59: /* exp: exp AND exp  */
#line 364 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "&&");
        (yyval.ptr) = t;
    }
#line 2339 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 60: /* exp: exp EQ exp  */
#line 369 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "==");
        (yyval.ptr) = t;
    }
#line 2349 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 61: /* exp: exp NE exp  */
#line 374 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "!=");
        (yyval.ptr) = t;
    }
#line 2359 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 62: /* exp: exp LT exp  */
#line 379 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "<");
        (yyval.ptr) = t;
    }
#line 2369 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 63: /* exp: exp GT exp  */
#line 384 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, ">");
        (yyval.ptr) = t;
    }
#line 2379 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 64: /* exp: exp LE exp  */
#line 389 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "<=");
        (yyval.ptr) = t;
    }
#line 2389 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 65: /* exp: exp GE exp  */
#line 394 "/home/annie/compiler-lab/src/parser/parser.y"
                 {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, ">=");
        (yyval.ptr) = t;
    }
#line 2399 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 66: /* exp: exp PLUS exp  */
#line 399 "/home/annie/compiler-lab/src/parser/parser.y"
                   {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "+");
        (yyval.ptr) = t;
    }
#line 2409 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 67: /* exp: exp MINUS exp  */
#line 404 "/home/annie/compiler-lab/src/parser/parser.y"
                    {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "-");
        (yyval.ptr) = t;
    }
#line 2419 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 68: /* exp: exp STAR exp  */
#line 409 "/home/annie/compiler-lab/src/parser/parser.y"
                   {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "*");
        (yyval.ptr) = t;
    }
#line 2429 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 69: /* exp: exp DIV exp  */
#line 414 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "/");
        (yyval.ptr) = t;
    }
#line 2439 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 70: /* exp: exp MOD exp  */
#line 419 "/home/annie/compiler-lab/src/parser/parser.y"
                  {
        ASTNode *t = mknode(2, BINARY_EXP, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        strcpy(t->type_id, "%");
        (yyval.ptr) = t;
    }
#line 2449 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 71: /* exp: INC exp  */
#line 424 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "++");
        (yyval.ptr) = t;
    }
#line 2459 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 72: /* exp: exp INC  */
#line 429 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(1, POST_INC_EXP, yylineno, (yyvsp[-1].ptr));
        strcpy(t->type_id, "++");
        (yyval.ptr) = t;
    }
#line 2469 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 73: /* exp: DEC exp  */
#line 434 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "--");
        (yyval.ptr) = t;
    }
#line 2479 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 74: /* exp: exp DEC  */
#line 439 "/home/annie/compiler-lab/src/parser/parser.y"
                        {
        ASTNode *t = mknode(1, POST_DEC_EXP, yylineno, (yyvsp[-1].ptr));
        strcpy(t->type_id, "--");
        (yyval.ptr) = t;
    }
#line 2489 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 75: /* exp: MINUS exp  */
#line 444 "/home/annie/compiler-lab/src/parser/parser.y"
                             {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "-");
        (yyval.ptr) = t;
    }
#line 2499 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 76: /* exp: PLUS exp  */
#line 449 "/home/annie/compiler-lab/src/parser/parser.y"
                           {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "+");
        (yyval.ptr) = t;
    }
#line 2509 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 77: /* exp: NOT exp  */
#line 454 "/home/annie/compiler-lab/src/parser/parser.y"
              {
        ASTNode *t = mknode(1, UNARY_EXP, yylineno, (yyvsp[0].ptr));
        strcpy(t->type_id, "!");
        (yyval.ptr) = t;
    }
#line 2519 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 78: /* exp: LP exp RP  */
#line 459 "/home/annie/compiler-lab/src/parser/parser.y"
                {
        (yyval.ptr) = (yyvsp[-1].ptr);
    }
#line 2527 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 79: /* exp: ID LP args RP  */
#line 462 "/home/annie/compiler-lab/src/parser/parser.y"
                    {
        ASTNode *t = mknode(2, FUNC_CALL, yylineno, 
                           mknode(0, ID_NODE, yylineno), (yyvsp[-1].ptr));
        strcpy(t->ptr[0]->type_id, (yyvsp[-3].type_id));
        (yyval.ptr) = t;
    }
#line 2538 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 80: /* exp: ID LP RP  */
#line 468 "/home/annie/compiler-lab/src/parser/parser.y"
               {
        ASTNode *t = mknode(1, FUNC_CALL, yylineno, 
                           mknode(0, ID_NODE, yylineno));
        strcpy(t->ptr[0]->type_id, (yyvsp[-2].type_id));
        (yyval.ptr) = t;
    }
#line 2549 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 81: /* exp: exp LB exp RB  */
#line 474 "/home/annie/compiler-lab/src/parser/parser.y"
                    {
        ASTNode *t = mknode(2, ARRAY_ACCESS, yylineno, (yyvsp[-3].ptr), (yyvsp[-1].ptr));
        (yyval.ptr) = t;
    }
#line 2558 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 82: /* exp: ID  */
#line 478 "/home/annie/compiler-lab/src/parser/parser.y"
         {
        ASTNode *t = mknode(0, ID_NODE, yylineno);
        strcpy(t->type_id, (yyvsp[0].type_id));
        (yyval.ptr) = t;
    }
#line 2568 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 83: /* exp: INT  */
#line 483 "/home/annie/compiler-lab/src/parser/parser.y"
          {
        ASTNode *t = mknode(0, INT_NODE, yylineno);
        t->type_int = (yyvsp[0].type_int);
        (yyval.ptr) = t;
    }
#line 2578 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 84: /* exp: FLOAT  */
#line 488 "/home/annie/compiler-lab/src/parser/parser.y"
            {
        ASTNode *t = mknode(0, FLOAT_NODE, yylineno);
        t->type_float = (yyvsp[0].type_float);
        (yyval.ptr) = t;
    }
#line 2588 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 85: /* exp: CHAR  */
#line 493 "/home/annie/compiler-lab/src/parser/parser.y"
           {
        ASTNode *t = mknode(0, CHAR_NODE, yylineno);
        t->type_int = (yyvsp[0].type_int);  // 字符值存储在type_int中
        (yyval.ptr) = t;
    }
#line 2598 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 86: /* args: exp  */
#line 501 "/home/annie/compiler-lab/src/parser/parser.y"
          {
        ASTNode *t = mknode(1, ARGS, yylineno, (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2607 "/home/annie/compiler-lab/build/parser.tab.c"
    break;

  case 87: /* args: exp COMMA args  */
#line 505 "/home/annie/compiler-lab/src/parser/parser.y"
                     {
        ASTNode *t = mknode(2, ARGS, yylineno, (yyvsp[-2].ptr), (yyvsp[0].ptr));
        (yyval.ptr) = t;
    }
#line 2616 "/home/annie/compiler-lab/build/parser.tab.c"
    break;


#line 2620 "/home/annie/compiler-lab/build/parser.tab.c"

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

#line 511 "/home/annie/compiler-lab/src/parser/parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
}
