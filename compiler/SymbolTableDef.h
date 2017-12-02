//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//HEADER FILE
#ifndef SYMBOL_TABLE_DEF_HEADER
#define SYMBOL_TABLE_DEF_HEADER

#include <stdbool.h>
#include "astdef.h"

#define FT_SIZE 200


typedef struct FTEntryhead{
  FTentry* head;
  int count;
}FTEntryhead;

FTEntryhead Function_Table[FT_SIZE];

int number_of_AST_nodes;

int curr_no;

int no_of_total_errors;

FILE* fp_to_symbol_table;

char semantic_errors[200][100];

#endif