//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "parserDef.h"

int token_pos=0;

pt_node* get_tree_node(char* name)
{
    pt_node *n = (pt_node*)malloc(sizeof(struct parse_tree_node));
    n->no_of_children = 0;
    n->line_number = 0;
    n->parent=NULL;
    char word[50];
    strcpy(word,"----");
    n->value_not=&word[0];
    n->value_int=0;
    n->value_double=0;
    n->name = name;
    return n;
}


PDAelem* initialize_elem(char* name,int term_or_not)
{
    PDAelem* myelem =(PDAelem*)malloc(sizeof(struct PDAelem));
    myelem->elem_node=get_tree_node(name);
    myelem->elem=name;
    myelem->is_term=term_or_not;
    return myelem;
}


// Stack is full when top is equal to the last index
int isFull()
{   return mystack->top == STACK_MAXSIZE - 1; }
 
// Stack is empty when top is equal to -1
int isEmpty()
{   return mystack->top == -1;  }
 
// Function to add an item to stack.  It increases top by 1
void push(PDAelem* elem)
{
    if (isFull())
        return;
    mystack->stk[++mystack->top] = elem;
}
 
// Function to remove an item from stack.  It decreases top by 1
PDAelem* pop()
{
    if (isEmpty())
        return NULL;
    return mystack->stk[mystack->top--];
}

PDAelem* top()
{
    if (isEmpty())
        return NULL;
    return mystack->stk[mystack->top];
}

void get_follow(char* k);
//defining the structure

int no_of_grammar_firsts = 0;
int no_of_grammar_follows = 0;
int no_of_nullables = 0;
int no_of_firsts = 0;
int no_of_follows = 0;

//function to find the nullable variables
void find_nullables()
{
    int i = 0;
    for(i= 0;i<no_of_rules;i++)
    {
        if(n[i].next->is_terminal == 1 && strcmp(n[i].next->name,"eps") == 0)
        {
            nullables[no_of_nullables++] = n[i].name;
        }
    }
}


//function to allocate memory to a grammar symbol
gnode* get_node(char name[100], int is_terminal)
{
    gnode* n = malloc(sizeof(struct grammar_node));
    n -> name = name;
    n -> is_terminal = is_terminal;
    n -> next = NULL;
    return n;
}

first_node* get_first_node(char name[100], int no_of_firsts)
{
    first_node *n = malloc(sizeof(struct first_grammar_node));
    n->no_of_firsts = no_of_firsts;
    n->name = name;
    return n;
}

follow_node* get_follow_node(char name[100], int no_of_follows)
{
    follow_node *n = malloc(sizeof(struct follow_grammar_node));
    n->no_of_follows = no_of_follows;
    n->name = name;
    return n;
}
//making the grammar
void make_grammar()
{
    //Rule 1
    n[0].name = "program";
    n[0].is_terminal = 0;
    n[0].next = get_node("moduleDeclarations",0);
    n[0].next -> next = get_node("otherModules",0);
    n[0].next -> next -> next = get_node("driverModule",0);
    n[0].next -> next -> next -> next = get_node("otherModules",0);

    //Rule 2
    n[1].name = "moduleDeclarations";
    n[1].is_terminal = 0;
    n[1].next = get_node("moduleDeclaration",0);
    n[1].next -> next = get_node("moduleDeclarations",0);

    //Rule 3
    n[2].name = "moduleDeclarations";
    n[2].is_terminal = 0;
    n[2].next = get_node("eps",1);

    //Rule 4
    n[3].name = "moduleDeclaration";
    n[3].is_terminal = 0;
    n[3].next = get_node("DECLARE",1);
    n[3].next -> next = get_node("MODULE",1);
    n[3].next -> next -> next = get_node("ID",1);
    n[3].next -> next -> next -> next = get_node("SEMICOL",1);

    //Rule 5
    n[4].name = "otherModules";
    n[4].is_terminal = 0;
    n[4].next = get_node("module",0);
    n[4].next -> next = get_node("otherModules",0);

    //Rule 6
    n[5].name = "otherModules";
    n[5].is_terminal = 0;
    n[5].next = get_node("eps",1);

    //Rule 7
    n[6].name = "driverModule";
    n[6].is_terminal = 0;
    n[6].next = get_node("DRIVERDEF",1);
    n[6].next -> next = get_node("DRIVER",1);
    n[6].next -> next -> next = get_node("PROGRAM",1);
    n[6].next -> next -> next -> next = get_node("DRIVERENDDEF",1);
    n[6].next -> next -> next -> next -> next = get_node("moduleDef",0);

    //Rule 8
    n[7].name = "module";
    n[7].is_terminal = 0;
    n[7].next = get_node("DEF",1);
    n[7].next -> next = get_node("MODULE",1);
    n[7].next -> next -> next = get_node("ID",1);
    n[7].next -> next -> next -> next = get_node("ENDDEF",1);
    n[7].next -> next -> next -> next -> next = get_node("TAKES",1);
    n[7].next -> next -> next -> next -> next -> next = get_node("INPUT",1);
    n[7].next -> next -> next -> next -> next -> next -> next= get_node("SQBO",1);
    n[7].next -> next -> next -> next -> next -> next -> next -> next= get_node("input_plist",0);
    n[7].next -> next -> next -> next -> next -> next -> next -> next -> next= get_node("SQBC",1);
    n[7].next -> next -> next -> next -> next -> next -> next -> next -> next -> next= get_node("SEMICOL",1);
    n[7].next -> next -> next -> next -> next -> next -> next -> next -> next -> next -> next= get_node("ret",0);
    n[7].next -> next -> next -> next -> next -> next -> next -> next -> next -> next -> next-> next = get_node("moduleDef",0);

    //Rule 9
    n[8].name = "ret";
    n[8].is_terminal = 0;
    n[8].next = get_node("RETURNS",1);
    n[8].next -> next = get_node("SQBO",1);
    n[8].next -> next -> next = get_node("output_plist",0);
    n[8].next -> next -> next -> next = get_node("SQBC",1);
    n[8].next -> next -> next -> next -> next = get_node("SEMICOL",1);

    //Rule 10
    n[9].name = "ret";
    n[9].is_terminal = 0;
    n[9].next = get_node("eps",1);

    //Rule 11
    n[10].name = "input_plist";
    n[10].is_terminal = 0;
    n[10].next = get_node("ID",1);
    n[10].next -> next = get_node("COLON",1);
    n[10].next -> next -> next = get_node("dataType",0);
    n[10].next -> next -> next -> next = get_node("N1",0);

    //Rule 12
    n[11].name = "N1";
    n[11].is_terminal = 0;
    n[11].next = get_node("COMMA",1);
    n[11].next -> next = get_node("ID",1);
    n[11].next -> next -> next = get_node("COLON",1);
    n[11].next -> next -> next -> next = get_node("dataType",0);
    n[11].next -> next -> next -> next -> next = get_node("N1",0);

    //Rule 13
    n[12].name = "N1";
    n[12].is_terminal = 0;
    n[12].next = get_node("eps",1);

    //Rule 14
    n[13].name = "output_plist";
    n[13].is_terminal = 0;
    n[13].next = get_node("ID",1);
    n[13].next -> next = get_node("COLON",1);
    n[13].next -> next -> next = get_node("type",0);
    n[13].next -> next -> next -> next = get_node("N2",0);

    //Rule 15
    n[14].name = "N2";
    n[14].is_terminal = 0;
    n[14].next = get_node("COMMA",1);
    n[14].next -> next = get_node("ID",1);
    n[14].next -> next -> next = get_node("COLON",1);
    n[14].next -> next -> next -> next = get_node("type",0);
    n[14].next -> next -> next -> next -> next = get_node("N2",0);

    //Rule 16
    n[15].name = "N2";
    n[15].is_terminal = 0;
    n[15].next = get_node("eps",1);

    //Rule 17
    n[16].name = "dataType";
    n[16].is_terminal = 0;
    n[16].next = get_node("INTEGER",1);

    //Rule 18
    n[17].name = "dataType";
    n[17].is_terminal = 0;
    n[17].next = get_node("REAL",1);

    //Rule 19
    n[18].name = "dataType";
    n[18].is_terminal = 0;
    n[18].next = get_node("BOOLEAN",1);

    //Rule 20
    n[19].name = "dataType";
    n[19].is_terminal = 0;
    n[19].next = get_node("ARRAY",1);
    n[19].next -> next = get_node("SQBO",1);
    n[19].next -> next -> next = get_node("range",0);
    n[19].next -> next -> next -> next = get_node("SQBC",1);
    n[19].next -> next -> next -> next -> next = get_node("OF",1);
    n[19].next -> next -> next -> next -> next -> next = get_node("type",0);

    //Rule 21
    n[20].name = "type";
    n[20].is_terminal = 0;
    n[20].next = get_node("INTEGER",1);

    //Rule 22
    n[21].name = "type";
    n[21].is_terminal = 0;
    n[21].next = get_node("REAL",1);

    //Rule 23
    n[22].name = "type";
    n[22].is_terminal = 0;
    n[22].next = get_node("BOOLEAN",1);

    //Rule 24
    n[23].name = "moduleDef";
    n[23].is_terminal = 0;
    n[23].next = get_node("START",1);
    n[23].next -> next = get_node("statements",0);
    n[23].next -> next -> next = get_node("END",1);

    //Rule 25
    n[24].name = "statements";
    n[24].is_terminal = 0;
    n[24].next = get_node("statement",0);
    n[24].next -> next = get_node("statements",0);

    //Rule 26
    n[25].name = "statements";
    n[25].is_terminal = 0;
    n[25].next = get_node("eps",1);

    //Rule 27
    n[26].name = "statement";
    n[26].is_terminal = 0;
    n[26].next = get_node("ioStmt",0);

    //Rule 28
    n[27].name = "statement";
    n[27].is_terminal = 0;
    n[27].next = get_node("simpleStmt",0);

    //Rule 29
    n[28].name = "statement";
    n[28].is_terminal = 0;
    n[28].next = get_node("declareStmt",0);

    //Rule 30
    n[29].name = "statement";
    n[29].is_terminal = 0;
    n[29].next = get_node("conditionalStmt",0);

    //Rule 31
    n[30].name = "statement";
    n[30].is_terminal = 0;
    n[30].next = get_node("iterativeStmt",0);

    //Rule 32
    n[31].name = "ioStmt";
    n[31].is_terminal = 0;
    n[31].next = get_node("GET_VALUE",1);
    n[31].next -> next = get_node("BO",1);
    n[31].next -> next -> next = get_node("ID",1);
    n[31].next -> next -> next -> next = get_node("BC",1);
    n[31].next -> next -> next -> next -> next = get_node("SEMICOL",1);

    //Rule 33
    n[32].name = "ioStmt";
    n[32].is_terminal = 0;
    n[32].next = get_node("PRINT",1);
    n[32].next -> next = get_node("BO",1);
    n[32].next -> next -> next = get_node("var",0);
    n[32].next -> next -> next -> next = get_node("BC",1);
    n[32].next -> next -> next -> next -> next = get_node("SEMICOL",1);

    //Rule 34
    n[33].name = "var";
    n[33].is_terminal = 0;
    n[33].next = get_node("ID",1);
    n[33].next -> next = get_node("whichId",0);

    //Rule 35
    n[34].name = "var";
    n[34].is_terminal = 0;
    n[34].next = get_node("NUM",1);

    //Rule 36
    n[35].name = "var";
    n[35].is_terminal = 0;
    n[35].next = get_node("RNUM",1);

    //Rule 37
    n[36].name = "whichId";
    n[36].is_terminal = 0;
    n[36].next = get_node("SQBO",1);
    n[36].next -> next = get_node("ID",1);
    n[36].next -> next -> next = get_node("SQBC",1);

    //Rule 38
    n[37].name = "whichId";
    n[37].is_terminal = 0;
    n[37].next = get_node("eps",1);

    //Rule 39
    n[38].name = "simpleStmt";
    n[38].is_terminal = 0;
    n[38].next = get_node("assignmentStmt",0);

    //Rule 40
    n[39].name = "simpleStmt";
    n[39].is_terminal = 0;
    n[39].next = get_node("moduleReuseStmt",0);

    //Rule 41
    n[40].name = "assignmentStmt";
    n[40].is_terminal = 0;
    n[40].next = get_node("ID",1);
    n[40].next -> next = get_node("whichStmt",0);

    //Rule 42
    n[41].name = "whichStmt";
    n[41].is_terminal = 0;
    n[41].next = get_node("lvalueIDStmt",0);

    //Rule 43
    n[42].name = "whichStmt";
    n[42].is_terminal = 0;
    n[42].next = get_node("lvalueARRStmt",0);

    //Rule 44
    n[43].name = "lvalueIDStmt";
    n[43].is_terminal = 0;
    n[43].next = get_node("ASSIGNOP",1);
    n[43].next -> next = get_node("expression",0);
    n[43].next -> next -> next = get_node("SEMICOL",1);

    //Rule 45
    n[44].name = "lvalueARRStmt";
    n[44].is_terminal = 0;
    n[44].next = get_node("SQBO",1);
    n[44].next -> next = get_node("index",0);
    n[44].next -> next -> next = get_node("SQBC",1);
    n[44].next -> next -> next -> next = get_node("ASSIGNOP",1);
    n[44].next -> next -> next -> next -> next = get_node("expression",0);
    n[44].next -> next -> next -> next -> next -> next = get_node("SEMICOL",1);

    //Rule 46
    n[45].name = "index";
    n[45].is_terminal = 0;
    n[45].next = get_node("NUM",1);

    //Rule 47
    n[46].name = "index";
    n[46].is_terminal = 0;
    n[46].next = get_node("ID",1);

    //Rule 48
    n[47].name = "moduleReuseStmt";
    n[47].is_terminal = 0;
    n[47].next = get_node("optional",0);
    n[47].next -> next = get_node("USE",1);
    n[47].next -> next -> next = get_node("MODULE",1);
    n[47].next -> next -> next -> next = get_node("ID",1);
    n[47].next -> next -> next -> next->next = get_node("WITH",1);
    n[47].next -> next -> next -> next -> next->next = get_node("PARAMETERS",1);
    n[47].next -> next -> next -> next -> next -> next->next = get_node("idList",0);
    n[47].next -> next -> next -> next -> next -> next -> next->next = get_node("SEMICOL",1);

    //Rule 49
    n[48].name = "optional";
    n[48].is_terminal = 0;
    n[48].next = get_node("SQBO",1);
    n[48].next -> next = get_node("idList",0);
    n[48].next -> next -> next = get_node("SQBC",1);
    n[48].next -> next -> next -> next = get_node("ASSIGNOP",1);

    //Rule 50
    n[49].name = "optional";
    n[49].is_terminal = 0;
    n[49].next = get_node("eps",1);

    //Rule 51
    n[50].name = "idList";
    n[50].is_terminal = 0;
    n[50].next = get_node("ID",1);
    n[50].next -> next = get_node("N3",0);

    //Rule 52
    n[51].name = "N3";
    n[51].is_terminal = 0;
    n[51].next = get_node("COMMA",1);
    n[51].next -> next = get_node("ID",1);
    n[51].next -> next -> next = get_node("N3",0);

    //Rule 53
    n[52].name = "N3";
    n[52].is_terminal = 0;
    n[52].next = get_node("eps",1);

    //Rule 54
    n[53].name = "expression";
    n[53].is_terminal = 0;
    n[53].next = get_node("arithematicOrBooleanExpr",0);

    //Rule 55
    n[54].name = "expression";
    n[54].is_terminal = 0;
    n[54].next = get_node("MINUS",1);
    n[54].next -> next = get_node("BO",1);
    n[54].next -> next -> next = get_node("arithematicExpr",0);
    n[54].next -> next -> next -> next = get_node("BC",1);

    //Rule 56
    n[55].name = "arithematicOrBooleanExpr";
    n[55].is_terminal = 0;
    n[55].next = get_node("AnyTerm",0);
    n[55].next -> next = get_node("N7",0);

    //Rule 57
    n[56].name = "arithematicOrBooleanExpr";
    n[56].is_terminal = 0;
    n[56].next = get_node("BO",1);
    n[56].next -> next = get_node("arithematicOrBooleanExpr",0);
    n[56].next -> next -> next = get_node("BC",1);

    //Rule 58
    n[57].name = "N7";
    n[57].is_terminal = 0;
    n[57].next = get_node("logicalOp",0);
    n[57].next -> next = get_node("AnyTerm",0);
    n[57].next -> next -> next = get_node("N7",0);

    //Rule 59
    n[58].name = "N7";
    n[58].is_terminal = 0;
    n[58].next = get_node("eps",1);

    //Rule 60
    n[59].name = "AnyTerm";
    n[59].is_terminal = 0;
    n[59].next = get_node("arithematicExpr",0);
    n[59].next -> next = get_node("N8",0);

    //Rule 61
    n[60].name = "N8";
    n[60].is_terminal = 0;
    n[60].next = get_node("relationalOp",0);
    n[60].next -> next = get_node("arithematicExpr",0);
    n[60].next -> next -> next = get_node("N8",0);

    //Rule 62
    n[61].name = "N8";
    n[61].is_terminal = 0;
    n[61].next = get_node("eps",1);

    //Rule 63
    n[62].name = "arithematicExpr";
    n[62].is_terminal = 0;
    n[62].next = get_node("term",0);
    n[62].next -> next = get_node("N4",0);

    //Rule 64
    n[63].name = "N4";
    n[63].is_terminal = 0;
    n[63].next = get_node("op1",0);
    n[63].next -> next = get_node("term",0);
    n[63].next -> next -> next = get_node("N4",0);

    //Rule 65
    n[64].name = "N4";
    n[64].is_terminal = 0;
    n[64].next = get_node("eps",1);

    //Rule 66
    n[65].name = "term";
    n[65].is_terminal = 0;
    n[65].next = get_node("factor",0);
    n[65].next -> next = get_node("N5",0);

    //Rule 67
    n[66].name = "N5";
    n[66].is_terminal = 0;
    n[66].next = get_node("op2",0);
    n[66].next -> next = get_node("factor",0);
    n[66].next -> next -> next = get_node("N5",0);

    //Rule 68
    n[67].name = "N5";
    n[67].is_terminal = 0;
    n[67].next = get_node("eps",1);

    //Rule 69
    n[68].name = "factor";
    n[68].is_terminal = 0;
    n[68].next = get_node("var",0);

    //Rule 70
    n[69].name = "op1";
    n[69].is_terminal = 0;
    n[69].next = get_node("PLUS",1);

    //Rule 71
    n[70].name = "op1";
    n[70].is_terminal = 0;
    n[70].next = get_node("MINUS",1);

    //Rule 72
    n[71].name = "op2";
    n[71].is_terminal = 0;
    n[71].next = get_node("MUL",1);

    //Rule 73
    n[72].name = "op2";
    n[72].is_terminal = 0;
    n[72].next = get_node("DIV",1);

    //Rule 74
    n[73].name = "logicalOp";
    n[73].is_terminal = 0;
    n[73].next = get_node("AND",1);

    //Rule 75
    n[74].name = "logicalOp";
    n[74].is_terminal = 0;
    n[74].next = get_node("OR",1);

    //Rule 76
    n[75].name = "relationalOp";
    n[75].is_terminal = 0;
    n[75].next = get_node("LT",1);

    //Rule 77
    n[76].name = "relationalOp";
    n[76].is_terminal = 0;
    n[76].next = get_node("LE",1);

    //Rule 78
    n[77].name = "relationalOp";
    n[77].is_terminal = 0;
    n[77].next = get_node("GT",1);

    //Rule 79
    n[78].name = "relationalOp";
    n[78].is_terminal = 0;
    n[78].next = get_node("GE",1);

    //Rule 80
    n[79].name = "relationalOp";
    n[79].is_terminal = 0;
    n[79].next = get_node("EQ",1);

    //Rule 81
    n[80].name = "relationalOp";
    n[80].is_terminal = 0;
    n[80].next = get_node("NE",1);

    //Rule 82
    n[81].name = "declareStmt";
    n[81].is_terminal = 0;
    n[81].next = get_node("DECLARE",1);
    n[81].next -> next = get_node("idList",0);
    n[81].next -> next -> next = get_node("COLON",1);
    n[81].next -> next -> next -> next = get_node("dataType",0);
    n[81].next -> next -> next -> next -> next = get_node("SEMICOL",1);

    //Rule 83
    n[82].name = "conditionalStmt";
    n[82].is_terminal = 0;
    n[82].next = get_node("SWITCH",1);
    n[82].next -> next = get_node("BO",1);
    n[82].next -> next -> next = get_node("ID",1);
    n[82].next -> next -> next -> next = get_node("BC",1);
    n[82].next -> next -> next -> next -> next = get_node("START",1);
    n[82].next -> next -> next -> next -> next -> next = get_node("caseStmts",0);
    n[82].next -> next -> next -> next -> next -> next -> next = get_node("default",0);
    n[82].next -> next -> next -> next -> next -> next -> next -> next = get_node("END",1);

    //Rule 84
    n[83].name = "caseStmts";
    n[83].is_terminal = 0;
    n[83].next = get_node("CASE",1);
    n[83].next -> next = get_node("value",0);
    n[83].next -> next -> next = get_node("COLON",1);
    n[83].next -> next -> next -> next = get_node("statements",0);
    n[83].next -> next -> next -> next -> next = get_node("BREAK",1);
    n[83].next -> next -> next -> next -> next -> next = get_node("SEMICOL",1);
    n[83].next -> next -> next -> next -> next -> next -> next = get_node("N9",0);

    //Rule 85
    n[84].name = "N9";
    n[84].is_terminal = 0;
    n[84].next = get_node("CASE",1);
    n[84].next -> next = get_node("value",0);
    n[84].next -> next -> next = get_node("COLON",1);
    n[84].next -> next -> next -> next = get_node("statements",0);
    n[84].next -> next -> next -> next -> next = get_node("BREAK",1);
    n[84].next -> next -> next -> next -> next -> next = get_node("SEMICOL",1);
    n[84].next -> next -> next -> next -> next -> next -> next = get_node("N9",0);

    //Rule 86
    n[85].name = "N9";
    n[85].is_terminal = 0;
    n[85].next = get_node("eps",1);

    //Rule 87
    n[86].name = "value";
    n[86].is_terminal = 0;
    n[86].next = get_node("NUM",1);

    //Rule 88
    n[87].name = "value";
    n[87].is_terminal = 0;
    n[87].next = get_node("TRUE",1);

    //Rule 89
    n[88].name = "value";
    n[88].is_terminal = 0;
    n[88].next = get_node("FALSE",1);

    //Rule 90
    n[89].name = "default";
    n[89].is_terminal = 0;
    n[89].next = get_node("DEFAULT",1);
    n[89].next -> next = get_node("COLON",1);
    n[89].next -> next -> next = get_node("statements",0);
    n[89].next -> next -> next -> next = get_node("BREAK",1);
    n[89].next -> next -> next -> next -> next = get_node("SEMICOL",1);

    //Rule 91
    n[90].name = "default";
    n[90].is_terminal = 0;
    n[90].next = get_node("eps",1);

    //Rule 92
    n[91].name = "iterativeStmt";
    n[91].is_terminal = 0;
    n[91].next = get_node("FOR",1);
    n[91].next -> next = get_node("BO",1);
    n[91].next -> next -> next = get_node("ID",1);
    n[91].next -> next -> next -> next = get_node("IN",1);
    n[91].next -> next -> next -> next -> next = get_node("range",0);
    n[91].next -> next -> next -> next -> next -> next = get_node("BC",1);
    n[91].next -> next -> next -> next -> next -> next -> next = get_node("START",1);
    n[91].next -> next -> next -> next -> next -> next -> next -> next= get_node("statements",0);
    n[91].next -> next -> next -> next -> next -> next -> next -> next -> next= get_node("END",1);

    //Rule 93
    n[92].name = "iterativeStmt";
    n[92].is_terminal = 0;
    n[92].next = get_node("WHILE",1);
    n[92].next -> next = get_node("BO",1);
    n[92].next -> next -> next = get_node("arithematicOrBooleanExpr",0);
    n[92].next -> next -> next -> next = get_node("BC",1);
    n[92].next -> next -> next -> next -> next = get_node("START",1);
    n[92].next -> next -> next -> next -> next -> next = get_node("statements",0);
    n[92].next -> next -> next -> next -> next -> next -> next = get_node("END",1);

    //Rule 94
    n[93].name = "range";
    n[93].is_terminal = 0;
    n[93].next = get_node("NUM",1);
    n[93].next -> next = get_node("RANGEOP",1);
    n[93].next -> next -> next = get_node("NUM",1);
}

int nullable(char* n)
{
    int i = 0;
    for(i = 0;i<no_of_nullables;i++)
    {
        if(strcmp(n,nullables[i]) == 0)
             return 1;
    }
    return 0;
}

//prints the current grammar
void print_grammar()
{
    int i = 0;
    for(i = 0;i < no_of_rules;i++)
    {
        printf("<%s> -> ",n[i].name);
        gnode* k;
        k = n[i].next;
        while(k != NULL)
        {
            if(k->is_terminal == 0)
            printf("<%s> ",k->name);
            else
            printf("%s ",k->name);
            k = k -> next;
        }
        printf("\n");
    }
}

//function to get the first sets
void get_first(char *k,int include_null)
{
    int i = 0;

    for(i = 0;i<no_of_rules;i++)
    {
        //matching the left non terminal
        if(strcmp(n[i].name,k) == 0)
        {
            gnode *next_symbol = n[i].next;


            //if next symbol is terminal
            if(next_symbol->is_terminal == 1)
            {

                if(include_null==1)
                {
                    if(strcmp(next_symbol->name,"eps") != 0)
                    {
                        results_first[no_of_firsts++] = next_symbol->name;
                        continue;
                    }
                }
                else
                {
                    results_first[no_of_firsts++] = next_symbol->name;
                    continue;
                }

            }

            else
            {

                //next symbol is nullable, consider till a non nullable or till the end
                while(next_symbol != NULL && nullable(next_symbol->name))
                {
                    get_first(next_symbol->name,1);
                    if(next_symbol->next == NULL)
                    {
                        break;
                    }
                    if(next_symbol->next->is_terminal == 1)
                    {
                        results_first[no_of_firsts++] = next_symbol->next->name;
                        break;
                    }
                    next_symbol = next_symbol->next;
                }

                //next symbol is non terminal and not nullable

                if(!nullable(next_symbol->name))
                {
                    get_first(next_symbol->name,0);

                }
            }
        }
    }
}


//initializing follow sets
void initialize_follow()
{
    follow_node current_node;
    current_node.name = "program";
    current_node.no_of_follows = 1;
    current_node.follows[0] = "EOF";
    follow_results[0] = current_node;

    no_of_grammar_follows++;

    follow_results[no_of_grammar_follows].name = "moduleDeclarations";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "moduleDeclaration";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "otherModules";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "driverModule";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "module";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "ret";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "input_plist";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "N1";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "output_plist";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "N2";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "dataType";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "type";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "moduleDef";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "statements";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "statement";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "ioStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "var";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "whichId";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "simpleStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "assignmentStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "whichStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "lvalueIDStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "lvalueARRStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "index";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "moduleReuseStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "optional";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "idList";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "N3";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "expression";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "arithematicOrBooleanExpr";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "N7";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "AnyTerm";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "N8";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "arithematicExpr";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "N4";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "term";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "N5";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "factor";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "op1";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "op2";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "logicalOp";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "relationalOp";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "declareStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "conditionalStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "caseStmts";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "N9";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "value";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "default";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "iterativeStmt";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;

    follow_results[no_of_grammar_follows].name = "range";
    follow_results[no_of_grammar_follows++].no_of_follows = 0;
}

//checks if eps is in first set
int eps_in_first(char *k)
{
    int i;
    for(i = 0;i<no_of_grammar_firsts;i++)
    {
        if(strcmp(first_results[i].name,k) == 0)
        {
            int j;
            for(j = 0;j<first_results[i].no_of_firsts;j++)
            {
                if(strcmp(first_results[i].firsts[j],"eps") == 0)
                {
                    return 1;
                }
            }
            return 0;
        }
    }
    return 0;
}

//add the first set if it doesn't contain eps
void add_first_not_eps(char *k1,char *first)
{
    int i;
    for(i = 0;i<no_of_grammar_follows;i++)
    {
        if(strcmp(follow_results[i].name,k1) == 0)
        {
            int j;
            for(j = 0;j<no_of_grammar_firsts;j++)
            {
                if(strcmp(first_results[j].name,first) == 0)
                {
                    int k;
                    for(k = 0;k<first_results[j].no_of_firsts;k++)
                    {
                        if(strcmp(first_results[j].firsts[k],"eps") != 0)
                            follow_results[i].follows[follow_results[i].no_of_follows++] = first_results[j].firsts[k];
                    }
                    return;
                }
            }
        }
    }
}

//adds the follow of LHS NT to k1
void add_follow(char *k1, char* curr_lhs_name)
{
    if(strcmp(k1,curr_lhs_name) == 0)
        return;
    int i;
    for(i = 0;i<no_of_grammar_follows;i++)
    {
        if(strcmp(follow_results[i].name,curr_lhs_name) == 0)
        {
            if(follow_results[i].no_of_follows == 0)
            {
                get_follow(curr_lhs_name);
            }
            int j;
            for(j = 0;j<no_of_grammar_follows;j++)
            {

                if(strcmp(follow_results[j].name,k1) == 0)
                {
                    int k;
                    for(k = 0;k<follow_results[i].no_of_follows;k++)
                    {
                        
                        if(strcmp(follow_results[i].follows[k],"eps") != 0)
                        {
                            follow_results[j].follows[follow_results[j].no_of_follows++] = follow_results[i].follows[k];
                        }
                    }
                }
            }
        }
    }
}

//adds terminal to the follow set of k1
void add_follow_terminal(char* k1,char* terminal)
{
    int i;
    for(i = 0;i<no_of_grammar_follows;i++)
    {
        if(strcmp(follow_results[i].name,k1) == 0)
        {
            if(strcmp(terminal,"eps") != 0)
            {
                follow_results[i].follows[follow_results[i].no_of_follows++] = terminal;
            }
        }
    }
}

//checks if the follow for this symbol has already been computer
int already_computed_follow(char *k1)
{
    int i;
    for(i = 0;i<no_of_grammar_follows;i++)
    {
        if(strcmp(follow_results[i].name,k1) == 0)
        {
            if(follow_results[i].no_of_follows != 0)
                return 1;
            return 0;
        }
    }
}

//function to find out the follow set of a NT
void get_follow(char *k)
{
    
    int i = 0;

    for(i = 0;i<no_of_rules;i++)
    {
        gnode* consider = n[i].next;
        char* curr_lhs_name = n[i].name;

        while(consider != NULL)
        {
            //matches on RHS, not eps
            if(strcmp(consider->name,k) == 0)
            {
                consider = consider->next;

                if(consider != NULL && consider->is_terminal == 1 && strcmp(consider->name,"eps") != 0)
                {
                    add_follow_terminal(k,consider->name);
                    continue;
                }

                while(consider != NULL && consider->is_terminal == 0)
                {
                    if(eps_in_first(consider->name))
                    {
                        add_first_not_eps(k,consider->name);
                        consider = consider->next;
                        continue;
                    }

                    else
                    {
                        add_first_not_eps(k,consider->name);
                        break;
                    }
                }

                if(consider == NULL)
                {
                    add_follow(k,curr_lhs_name);
                }

                else if(consider->is_terminal == 1)
                {
                    add_follow_terminal(k,consider->name);
                }
            }
            if(consider != NULL)
            consider = consider -> next;
        }

    }
}

//iniitalizing parse table,terminals and non-terminals
void initialize_parse()
{
    int i,j;
    for(i=0;i<NO_NONTERM;i++)
    {
        for(j=0;j<NO_TERM;j++)
        {
            parse_table[i][j]=-1;
        }
    }
}

void initialize_nonterm()
{
    int i;
    for(i=0;i<no_of_grammar_follows;i++)
    {
        non_term[i]=follow_results[i].name;
    }
}

void initialize_term()
{
    
    term[0]="EOF";
    term[1]="DECLARE";
    term[2]="MODULE";
    term[3]="ID";
    term[4]="SEMICOL";
    term[5]="DRIVERDEF";
    term[6]="PROGRAM";
    term[7]="DRIVERENDDEF";
    term[8]="DEF";
    term[9]="ENDDEF";
    term[10]="TAKES";
    term[11]="INPUT";
    term[12]="SQBO";
    term[13]="SQBC";
    term[14]="RETURNS";
    term[15]="COMMA";
    term[16]="COLON";
    term[17]="INTEGER";
    term[18]="REAL";
    term[19]="BOOLEAN";
    term[20]="ARRAY";
    term[21]="OF";
    term[22]="START";
    term[23]="END";
    term[24]="GET_VALUE";
    term[25]="BO";
    term[26]="BC";
    term[27]="PRINT";
    term[28]="NUM";
    term[29]="RNUM";
    term[30]="ASSIGNOP";
    term[31]="USE";
    term[32]="WITH";
    term[33]="PARAMETERS";
    term[34]="PLUS";
    term[35]="MINUS";
    term[36]="MUL";
    term[37]="DIV";
    term[38]="AND";
    term[39]="OR";
    term[40]="LT";
    term[41]="LE";
    term[42]="GT";
    term[43]="GE";
    term[44]="EQ";
    term[45]="NE";
    term[46]="SWITCH";
    term[47]="CASE";
    term[48]="BREAK";
    term[49]="TRUE";
    term[50]="FALSE";
    term[51]="DEFAULT";
    term[52]="FOR";
    term[53]="WHILE";
    term[54]="IN";
    term[55]="RANGEOP";
    term[56]="DRIVER";
}

int get_term_index(char* k)
{
    int i;
    for(i=0;i<NO_TERM;i++)
    {
        if(strcmp(term[i],k)==0)
            return i;
    }
}

int get_nonterm_index(char* k)
{
    int i;
    for(i=0;i<NO_NONTERM;i++)
    {
        if(strcmp(non_term[i],k)==0)
            return i;
    }
}

//Parse table is filled by looking of FIRSTS and FOLLOWS
void add_follow_to_table(char* rule,int nonterm_index,int rule_pos)
{
    int i,term_index;
    for(i = 0;i<no_of_grammar_follows;i++)
    {
        if(strcmp(follow_results[i].name,rule) == 0)
        {
            int k;
            for(k = 0;k<follow_results[i].no_of_follows;k++)
            {
                if(strcmp(follow_results[i].follows[k],"eps") != 0)
                {
                    term_index=get_term_index(follow_results[i].follows[k]);
                    if(parse_table[nonterm_index][term_index]==-1)
                    parse_table[nonterm_index][term_index]=rule_pos;

                }
            }

        }
    }
}

void add_first_to_table(char* rule,int nonterm_index,int rule_pos)
{
    int i,term_index;
    for(i = 0;i<no_of_grammar_firsts;i++)
    {
        if(strcmp(first_results[i].name,rule) == 0)
        {
            int k;
            for(k = 0;k<first_results[i].no_of_firsts;k++)
            {
                if(strcmp(first_results[i].firsts[k],"eps") != 0)
                {
                    term_index=get_term_index(first_results[i].firsts[k]);
                    if(parse_table[nonterm_index][term_index]==-1)
                    parse_table[nonterm_index][term_index]=rule_pos;

                }
            }

        }
    }
}

void fill_table()
{

    int a = 0;
    int i,j,k;
    int nonterm_index,term_index;
    char* current;
    for(a = 0;a<no_of_rules;a++)
    {
        current = n[a].name;
        nonterm_index=get_nonterm_index(current);
        gnode *next_symbol = n[a].next;
        while(next_symbol!=NULL)
        {
            if(next_symbol->is_terminal==1)
            {
                if(strcmp(next_symbol->name,"eps")==0)
                {
                    add_follow_to_table(current,nonterm_index,a);
                }
                else
                {
                    term_index=get_term_index(next_symbol->name);
                    if(parse_table[nonterm_index][term_index]==-1)
                    parse_table[nonterm_index][term_index]=a;
                }
                break;
            }
            else
            {
                if(eps_in_first(next_symbol->name))
                {
                    add_first_to_table(next_symbol->name,nonterm_index,a);
                }
                else
                {
                    add_first_to_table(next_symbol->name,nonterm_index,a);
                    break;
                }
            }

            next_symbol=next_symbol->next;
        }
    }
}

int number_of_PT_nodes = 0;
//Printing parse tree
void print_parse_tree(pt_node* start,FILE* outfp)
{

    if(start!=NULL)
    {
        number_of_PT_nodes++;
        if(start->no_of_children>0)
            print_parse_tree(start->children[0],outfp);
        if(strcmp(start->token,"NUM")==0)
            fprintf(outfp,"%-9.9s\t%9d\t\t%-9.9s\t%9d\t\t%-30s\t%-9.9s\t%s\n",start->lexeme_current_node,start->line_number,start->token,start->value_int,start->parent->name,start->is_leaf_node,start->name);
        else if(strcmp(start->token,"RNUM")==0)
            fprintf(outfp,"%-9.9s\t%9d\t\t%-9.9s\t%9lf\t\t%-30s\t%-9.9s\t%s\n",start->lexeme_current_node,start->line_number,start->token,start->value_double,start->parent->name,start->is_leaf_node,start->name);
        else
            fprintf(outfp,"%-9.9s\t%9d\t\t%-9.9s\t%9.9s\t\t%-30s\t%-9.9s\t%s\n",start->lexeme_current_node,start->line_number,start->token,"----",start->parent->name,start->is_leaf_node,start->name);
        
        int i;
        for(i=1;i<start->no_of_children;i++)
        {
            print_parse_tree(start->children[i],outfp);
        }
    }
}

//Error correcting
int if_in_follow(char* non_term, char* term)
{
    int i;
    i = get_nonterm_index(non_term);
    int j;
    for(j = 0;j<follow_results[i].no_of_follows;j++)
    {
        if(strcmp(follow_results[i].follows[j],term) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void error_correction()
{
    while(strcmp(top()->elem,"EOF")!=0 && top()->is_term==1)
    {
        strcpy(current_pt_node->is_leaf_node,"yes");
        current_pt_node->line_number=token_arr[token_pos]->line_no;
        strcpy(current_pt_node->lexeme_current_node,token_arr[token_pos]->lexeme);
        strcpy(current_pt_node->token,token_arr[token_pos]->TOKEN_TYPE);
        if(strcmp(token_arr[token_pos]->TOKEN_TYPE,"NUM") == 0)
        {
                current_pt_node->value_int = atoi(token_arr[token_pos]->lexeme);
        }
        if(strcmp(token_arr[token_pos]->TOKEN_TYPE,"RNUM") == 0)
        {
                current_pt_node->value_double = atof(token_arr[token_pos]->lexeme);
        }
        pop();
        current_pt_node=top()->elem_node;
    }
    char non_term[50];
    char term[50];
    char next_token[50];
    strcpy(non_term,top()->elem);
    while(strcmp(top()->elem,"EOF")!=0 && top()->is_term==0)
    {
            strcpy(current_pt_node->is_leaf_node,"yes");
            current_pt_node->line_number=-1;
            strcpy(current_pt_node->lexeme_current_node,"----");
            strcpy(current_pt_node->token,"----");
            pop();
            current_pt_node=top()->elem_node;
    }
    strcpy(term,top()->elem);
    while(strcmp(top()->elem,"EOF")!=0 && if_in_follow(non_term,term)==0)
    {
        strcpy(current_pt_node->is_leaf_node,"yes");
        current_pt_node->line_number=token_arr[token_pos]->line_no;
        strcpy(current_pt_node->lexeme_current_node,token_arr[token_pos]->lexeme);
        strcpy(current_pt_node->token,token_arr[token_pos]->TOKEN_TYPE);
        if(strcmp(token_arr[token_pos]->TOKEN_TYPE,"NUM") == 0)
        {
                current_pt_node->value_int = atoi(token_arr[token_pos]->lexeme);
        }
        if(strcmp(token_arr[token_pos]->TOKEN_TYPE,"RNUM") == 0)
        {
                current_pt_node->value_double = atof(token_arr[token_pos]->lexeme);
        }
        pop();
        current_pt_node=top()->elem_node;
        strcpy(term,top()->elem);
    }
    if(strcmp(top()->elem,"EOF")==0)
    {
        return;
    }
    if(if_in_follow(non_term,term)==1)
    {
        strcpy(next_token,term);
    }
    while(strcmp(read_token,next_token)!=0)
    {
        token_pos++;
        read_token=&token_arr[token_pos]->TOKEN_TYPE[0];
    }
}

//PDA Stack implementation
void push_rule_on_stack(gnode* elem,int i)
{
    PDAelem* node;
    if(elem==NULL)
    {  
        return ;
    }
    push_rule_on_stack(elem->next,i+1);
    if(strcmp("eps",elem->name) != 0)
    {
    node=initialize_elem(elem->name,elem->is_terminal);
    temp->children[i]=node->elem_node;
    temp->no_of_children++;
    node->elem_node->parent=temp;
    push(node);
    }
    return; 
}

int check_table(char* nonterminal,char* terminal)
{
    int nonterm_index=get_nonterm_index(nonterminal);
    int term_index=get_term_index(terminal);
    int ruleno=parse_table[nonterm_index][term_index];
    return ruleno;
}

void simulate_parser()
{
    int rule_no;
    while(strcmp(top()->elem,"EOF")!=0)
    {
        current_pt_node=top()->elem_node;
        if(top()->is_term==1)
        {
            strcpy(current_pt_node->is_leaf_node,"yes");
            current_pt_node->line_number=token_arr[token_pos]->line_no;
            strcpy(current_pt_node->lexeme_current_node,token_arr[token_pos]->lexeme);
            strcpy(current_pt_node->token,token_arr[token_pos]->TOKEN_TYPE);
            if(strcmp(token_arr[token_pos]->TOKEN_TYPE,"NUM") == 0)
            {
                current_pt_node->value_int = atoi(token_arr[token_pos]->lexeme);
            }
            if(strcmp(token_arr[token_pos]->TOKEN_TYPE,"RNUM") == 0)
            {
                current_pt_node->value_double = atof(token_arr[token_pos]->lexeme);
            }
        }
        else
        {
            strcpy(current_pt_node->is_leaf_node,"no");
            current_pt_node->line_number=-1;
            strcpy(current_pt_node->lexeme_current_node,"----");
            strcpy(current_pt_node->token,"----");
        }
        if(top()->is_term==0)
        {
            rule_no=check_table(top()->elem,read_token);

            if(rule_no==-1)
            {
            	char error_string[100];
                sprintf(error_string,"ERROR_5:The token %s for lexeme %s does not match at line %d.\n",token_arr[token_pos]->TOKEN_TYPE,token_arr[token_pos]->lexeme,token_arr[token_pos]->line_no);
                strcpy(error_name[errors],error_string);
					errors++;
                error_correction();
            }
            else
            {

                temp=top()->elem_node;
                pop();
                push_rule_on_stack(n[rule_no].next,0);
                continue;
            }
        }
        else
        {
            if(strcmp(read_token,top()->elem) == 0)
                {
                    pop();
                    current_pt_node=top()->elem_node;
                    token_pos++;
                if(token_pos<no_of_tokens)
                    read_token=&(token_arr[token_pos]->TOKEN_TYPE)[0];
                else
                    {
                        char word[20];
                        strcpy(word,"EOF");
                        read_token=&word[0];
                    }
                }
            else
            {
            	char error_string[100];
                sprintf(error_string,"ERROR_5:The token %s for lexeme %s does not match at line %d.The expected token here is %s\n",token_arr[token_pos]->TOKEN_TYPE,token_arr[token_pos]->lexeme,token_arr[token_pos]->line_no,top()->elem);
                strcpy(error_name[errors],error_string);
					errors++;
                error_correction();
            }
        }   
    }
    
}

void parse()
{
    mystack=(PDAstack*) malloc(sizeof(PDAstack));
    mystack->top=-1;

    PDAelem* end_symbol=initialize_elem("EOF",1);
    PDAelem* start_symbol=initialize_elem("program",0);
    start_symbol->elem_node->line_number=-1;
    strcpy(start_symbol->elem_node->lexeme_current_node,"----");
    strcpy(start_symbol->elem_node->token,"----");
    start_symbol->elem_node->parent=root;

    push(end_symbol);
    push(start_symbol);

    current_pt_node=top()->elem_node;
    root->children[root->no_of_children++]=current_pt_node;
    if(token_pos<no_of_tokens)
    {
        read_token=&(token_arr[token_pos]->TOKEN_TYPE)[0];
    }
    simulate_parser();
    
}

int parse_main(FILE *fp,FILE* outfp)
{
    make_grammar();
    find_nullables();

    //Loop
    int a = 0;
    int i,j,k;

    char* current;
    char previous[100];

    current = n[0].name;
    strcpy(previous ,"no name");

    //looping through each rule and getting first set of left NT
    for(a = 0;a<no_of_rules;a++)
    {
        current = n[a].name;

        if(strcmp(current,previous) != 0)
        {
            no_of_firsts = 0;
            get_first(n[a].name,0);
            int n1 = no_of_firsts;

            //removing duplicates
            for(i=0; i < n1; i++)
            {
                for(j=i+1; j < n1; )
                {
                    if(strcmp(results_first[i],results_first[j]) == 0)
                    {
                        for(k=j; k < n1;k++)
                        {
                        results_first[k] = results_first[k+1];
                        }
                        n1--;
                    }
                    else   {
                        j++;
                    }
                }
            }


            first_node current_first;
            current_first.name = n[a].name;
            current_first.no_of_firsts = n1;
            int k = 0;
            for(k = 0;k<n1;k++)
                current_first.firsts[k] = results_first[k];

            first_results[no_of_grammar_firsts++] = current_first;

        }
        strcpy(previous,current);
    }

    //initializing follow symbol
    initialize_follow();

    //getting follow for program
    get_follow("program");

    //getting follow sets for all other non terminals
    for(a = 1;a<no_of_grammar_follows;a++)
    {
        get_follow(follow_results[a].name);
        int n1 = follow_results[a].no_of_follows;

        //removing duplicates
        for(i=0; i < n1; i++)
        {
            for(j=i+1; j < n1; )
            {
                if(strcmp(follow_results[a].follows[i],follow_results[a].follows[j]) == 0)
                {
                    for(k=j; k < n1;k++)
                    {
                    follow_results[a].follows[k] = follow_results[a].follows[k+1];
                    }
                    n1--;
                }
                else   {
                    j++;
                }
            }
        }

        follow_results[a].no_of_follows = n1;
    }

    initialize_parse();
    initialize_nonterm();
    initialize_term();
    fill_table();
    
    //opening file
    
    lexical_analyser(fp);

    root=get_tree_node("ROOT");
    parse();

    print_parse_tree(root->children[0],outfp);
}

