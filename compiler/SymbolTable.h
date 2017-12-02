//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//HEADER FILE
#ifndef ST_HEADER
#define ST_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "parserDef.h"
#include "astdef.h"
#include "SymbolTableDef.h"

void STmain(FILE* testcase,FILE* parsetree);
int FTHashCode(char id_name[50]);
int STHashCode(char id_name[50]);
void print_symbol_table();
void compute_parse_tree_compression();
void check_ID_in_FT(AST_Node* node);
#endif
