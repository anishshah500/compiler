//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//HEADER FILE
#ifndef PARSERDEF_HEADER
#define PARSERDEF_HEADER

#include <stdbool.h>
#include "lexerDef.h"
#define STACK_MAXSIZE 200
#define no_of_rules 94
#define NO_NONTERM 51
#define NO_TERM 57
struct parse_tree_node
{
    char *name;
    char token[50];
    char* value_not;
    int value_int;
    double value_double;
    int no_of_children;
    struct parse_tree_node* children[20];
    struct parse_tree_node* parent;
    char is_leaf_node[5];
    char lexeme_current_node[50];
    int line_number;
};

typedef struct parse_tree_node pt_node;

typedef struct PDAelem{
    struct parse_tree_node* elem_node;
    char* elem;
    int is_term;
}PDAelem;

typedef struct PDAstack
{
    PDAelem* stk[STACK_MAXSIZE];
    int top;

}PDAstack;

struct grammar_node
{
    char *name;
    int is_terminal;
    struct grammar_node* next;
    int side;
};

typedef struct grammar_node gnode;

struct first_grammar_node
{
    char *name;
    char *firsts[100];
    int no_of_firsts;
};

typedef struct first_grammar_node first_node;

struct follow_grammar_node
{
    char *name;
    char *follows[1000];
    int no_of_follows;
};

typedef struct follow_grammar_node follow_node;

struct parse_tree_node *root;
struct parse_tree_node *current_pt_node;
struct parse_tree_node *temp;

int parse_table[100][100];
char* non_term[100];
char* term[100];
char* read_token;
int token_pos;
PDAstack* mystack;

first_node first_results[no_of_rules];

follow_node follow_results[no_of_rules];

//array with rules
gnode n[no_of_rules];

//array of nullable variables
char *nullables[no_of_rules];

//array of first set results
char *results_first[100];

//array of follow set results
char *results_follow[100];

int no_of_grammar_firsts;
int no_of_grammar_follows;
int no_of_nullables;
int no_of_firsts;
int no_of_follows;
int number_of_PT_nodes;
/* a simple rule is used to differentiate between terminals and nonterminals
 * nonterminal ids begin at a large number
 * so identifiers larger than this number represent nonterminals and the others are terminals
 */

#endif