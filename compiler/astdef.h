//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//HEADER FILE
#ifndef ASTDEF_HEADER
#define ASTDEF_HEADER

#include <stdbool.h>
#include "parserDef.h"

#define SYMBOL_TABLE_SIZE 200
#define MAX_CHILD 20

struct STentry
{
    char* name;
    char* type;
    int offset;
    int width;
    int no_of_variables;
    struct STentry* next;
};

typedef struct STentry STentry;

typedef struct STEntryhead
{
    STentry* head;
    int count;
}STEntryhead;

typedef struct STnode{
	STEntryhead Symbol_Table[SYMBOL_TABLE_SIZE];
	int offset;
	int no_of_children;
	struct STnode* children[MAX_CHILD];
	struct STnode* parent;
}STnode;

struct list_unit
{
    char* type;
    char* name;
    int is_array;
};

typedef struct list_unit list_unit;

struct FTentry
{
    char *name;
    list_unit* input_plist[50];
    int no_of_input_list_parameters;
    list_unit* output_plist[50];
    int no_of_output_list_parameters;
    struct FTentry* next;
    STnode* node_to_ST;
};

typedef struct FTentry FTentry;

struct AST_Node
{
	// char *name;
 //    char token[50];
 //    char* value_not;
 //    int value_int;
 //    double value_double;
 //    int no_of_children;
 //    char is_leaf_node[5];
 //    char lexeme_current_node[50];
 //    int line_number;
	char* type;
	char* var_name;
	STnode* lookup;
	FTentry* function_enclosing;
    struct AST_Node *next;
    struct AST_Node *child;
    pt_node* p;
};

typedef struct AST_Node AST_Node;


AST_Node* astroot;

int node_count;

#endif