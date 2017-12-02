//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//HEADER FILE
#ifndef CODEGEN_HEADER
#define CODEGEN_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "parserDef.h"
#include "astdef.h"
#include "SymbolTableDef.h"
#include "TypeCheckerDef.h"
#include "CodegenDef.h"

void codegen_main(FILE* testcase,FILE* output_asm,FILE* parsetree);

#endif
