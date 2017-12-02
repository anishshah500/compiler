//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//HEADER FILE
#ifndef TYPE_CHECKER_HEADER
#define TYPE_CHECKER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "parserDef.h"
#include "astdef.h"
#include "SymbolTableDef.h"
#include "TypeCheckerDef.h"

char* get_ID_type(char* lexeme_current_node, STnode* symbol_table_node);
void type_checking_main(FILE* testcase,FILE* parsetree);

#endif
