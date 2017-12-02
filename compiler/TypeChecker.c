//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//Semantic Type Checking

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SymbolTable.h"
#include "TypeCheckerDef.h"

int var_number=0;
//int no_of_total_errors = 0;

char* var_type[1000];

int check_ID_type(char *lexeme_current_node,STnode* symbol_table_node,char* type)
{
    if(symbol_table_node == NULL)
        return 0;
    int hashIndex = STHashCode(lexeme_current_node);
    if(!symbol_table_node->Symbol_Table[hashIndex].head)
    {
        return check_ID_type(lexeme_current_node,symbol_table_node->parent,type);
    }
    else
    {
        STentry* check = symbol_table_node->Symbol_Table[hashIndex].head;
        while(check != NULL)
        {
            if(strcmp(check->name,lexeme_current_node) == 0)
            {
                if(strcmp(type,check->type) == 0)
                    return 1;
                else
                    return 0;
            }
            check = check->next;
        }
        return check_ID_type(lexeme_current_node,symbol_table_node->parent,type);
    }
}

int check_two_id_lists(FTentry* ft_entry,AST_Node* idList)
{
    int i;
    AST_Node* iter = idList->child;
    //printf("%s\n",ft_entry->input_plist[0]->type);
    for(i = 0;i<ft_entry->no_of_input_list_parameters;i++)
    {

        if(iter != NULL && check_ID_type(iter->p->lexeme_current_node,iter->lookup,ft_entry->input_plist[i]->type))
        {
            iter = iter->next;
        }
        else
        {
            // printf("No. or type of input parameter lists don't match for %s\n",ft_entry->name);
            char error_string[100];
            sprintf(error_string,"ERROR_M:No. or type of input parameter lists don't match for %s\n",ft_entry->name);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
            return 0;
        }
    }
    if(iter == NULL)
    {
        //printf("No. and type of input parameter lists match for %s\n",ft_entry->name);
        return 1;
    }
    else
    {
        // printf("No. or type of input parameter lists don't match for %s\n",ft_entry->name);
        char error_string[100];
        sprintf(error_string,"ERROR_M:No. or type of input parameter lists don't match for %s\n",ft_entry->name);
        strcpy(semantic_errors[no_of_total_errors],error_string);
        no_of_total_errors++;
        return 0;
    }
}

FTentry* get_FTentry(AST_Node* node)
{
    int hashIndex = FTHashCode(node->p->lexeme_current_node);
    if(!Function_Table[hashIndex].head)
    {
        // printf("Not Present Module %s\n",node->p->lexeme_current_node);
        char error_string[100];
        sprintf(error_string,"ERROR_M:Not Present Module %s at line %d\n",node->p->lexeme_current_node,node->p->line_number);
        strcpy(semantic_errors[no_of_total_errors],error_string);
        no_of_total_errors++;
        return NULL;
    }
    else
    {
        FTentry* check=Function_Table[hashIndex].head;
        while(check != NULL)
        {
            if(strcmp(check->name,node->p->lexeme_current_node) == 0)
            {
                return check;
            }
            check = check->next;
        }
        return NULL;
    }

}

int function_input_parameters_check(AST_Node* moduleReuseStmt)
{
    if(moduleReuseStmt == NULL)
        return 0;
    FTentry* FTentry_check = get_FTentry(moduleReuseStmt->child);
    //printf("%s\n",FTentry_check->name);
   // printf("func name %s\n",FTentry_check->name);
    AST_Node* input_plist_check = moduleReuseStmt->child->child;
    //printf("in input check\n");
    return check_two_id_lists(FTentry_check,input_plist_check);
}

int check_two_id_lists_output(FTentry* ft_entry,AST_Node* idList)
{
    int i;
    AST_Node* iter = idList->child;
    for(i = 0;i<ft_entry->no_of_output_list_parameters;i++)
    {
        if(iter != NULL && check_ID_type(iter->p->lexeme_current_node,iter->lookup,ft_entry->output_plist[i]->type))
        {
            iter = iter->next;
        }

        else
        {
            // printf("No. or type of output parameter lists don't match for %s\n",ft_entry->name);
            char error_string[100];
            sprintf(error_string,"ERROR_M:No. or type of output parameter lists don't match for %s\n",ft_entry->name);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
            return 0;
        }
    }
    if(iter == NULL)
    {
        //printf("No. and type of output parameter lists match for %s\n",ft_entry->name);
        return 1;
    }
    else
    {
        // printf("No. or type of output parameter lists don't match for %s\n",ft_entry->name);
        char error_string[100];
        sprintf(error_string,"ERROR_M:No. or type of output parameter lists don't match for %s\n",ft_entry->name);
        strcpy(semantic_errors[no_of_total_errors],error_string);
        no_of_total_errors++;
        return 0;
    }
}

int function_output_parameters_check(AST_Node* moduleReuseStmt)
{
    if(moduleReuseStmt == NULL)
        return 0;
    FTentry* FTentry_check = get_FTentry(moduleReuseStmt->child);
    //printf("func name %s\n",FTentry_check->name);
    AST_Node* output_plist_check = moduleReuseStmt->child->child->next;
    if(output_plist_check == NULL && FTentry_check->no_of_output_list_parameters == 0)
    {
        //printf("Output list matched\n");
        return 1;
    }
    else if(output_plist_check != NULL && FTentry_check->no_of_output_list_parameters > 0)
    {
        return check_two_id_lists_output(FTentry_check,output_plist_check);
    }
    else
    {
        // printf("Output list not matched\n");
        char error_string[100];
        sprintf(error_string,"ERROR_M:Output list not matched\n");
        strcpy(semantic_errors[no_of_total_errors],error_string);
        no_of_total_errors++;
        return 0;
    }
}

void check_id_of_declare(AST_Node* declareStmt,char *iterative_variable,int* redeclare_errors)
{
    AST_Node* iter = declareStmt->child->child;
    while(iter != NULL)
    {
        if(strcmp(iter->p->lexeme_current_node,iterative_variable) == 0)
        {
            *redeclare_errors=(*redeclare_errors)+1;
            // printf("ERROR_V: Iterative Variable %s declared at line %d\n",iterative_variable,iter->p->line_number);
            char error_string[100];
            sprintf(error_string,"ERROR_V: Iterative Variable %s declared at line %d\n",iterative_variable,iter->p->line_number);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
            return;
        }
        iter = iter->next;
    }
    return;
}

void for_statement_check(AST_Node* statements,char* for_variable,int* redeclare_errors)
{
    if(statements!=NULL)
    {
        AST_Node* iter = statements->child;
        while(iter != NULL)
        {
            if(strcmp("declareStmt",iter->p->name) == 0)
            {
                check_id_of_declare(iter,for_variable,redeclare_errors);
            }

            if(strcmp("iterativeStmt",iter->p->name) == 0)
            {
                for_statement_check(iter->child->next->next,for_variable,redeclare_errors);
            }

            if(strcmp("conditionalStmt",iter->p->name)==0)
            {
                AST_Node* iter2=iter->child->next->child;
                while(iter2!=NULL)
                {
                    for_statement_check(iter2->child,for_variable,redeclare_errors);
                    iter2=iter2->next;
                }
                if(iter->child->next->next!=NULL)
                    for_statement_check(iter->child->next->next->child,for_variable,redeclare_errors);
            }
            iter = iter->next;
        }
    }
}

char* get_ID_type(char* lexeme_current_node, STnode* symbol_table_node)
{
    //printf("my id %s\n",lexeme_current_node);
    if(symbol_table_node == NULL)
    {
        //printf("my id reached null %s\n",lexeme_current_node);
        return NULL;
    }
    int hashIndex = STHashCode(lexeme_current_node);
    if(!symbol_table_node->Symbol_Table[hashIndex].head)
    {
        //printf("my id not foudn in lookup_table %s\n",lexeme_current_node);
        return get_ID_type(lexeme_current_node,symbol_table_node->parent);
    }
    else
    {
        STentry* check = symbol_table_node->Symbol_Table[hashIndex].head;
        while(check != NULL)
        {
            if(strcmp(check->name,lexeme_current_node) == 0)
            {
                return check->type;
            }
            check = check->next;
        }
        return get_ID_type(lexeme_current_node,symbol_table_node->parent);
    }
}

int conditionalStmt_check(AST_Node* node)
{
    char type[50];
    strcpy(type,get_ID_type(node->child->p->lexeme_current_node,node->child->lookup));

    if(strcmp(type,"BOOLEAN") == 0)
    {
        AST_Node* iter = node->child->next->child;
        while(iter != NULL)
        {
            if(strcmp(iter->p->name,"TRUE") != 0 && strcmp(iter->p->name,"FALSE") != 0)
            {
                // printf("Conditional Statement Case %s, Iterator Boolean but case variable %s not Boolean\n",node->child->p->lexeme_current_node,iter->p->name);
                char error_string[100];
                sprintf(error_string,"Conditional Statement Case %s, Iterator Boolean but case variable %s not Boolean\n",node->child->p->lexeme_current_node,iter->p->name);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;
                return 0;
            }
            else
            {
                //printf("%s\n",iter->p->name);
            }
            iter = iter->next;
        }

        AST_Node* default_node = node->child->next->next;
        if(default_node == NULL)
        {
            // printf("Default statement is not there for Conditional Statement Case %s\n",node->child->p->lexeme_current_node);
            char error_string[100];
            sprintf(error_string,"Default statement is not there for Conditional Statement Case %s\n",node->child->p->lexeme_current_node);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
            return 1;
        }
        else
        {
            //printf("ERROR: default statement there in Conditional Statement case %s\n",node->child->p->lexeme_current_node);
            return 0;
        }
    }

    else if(strcmp(type,"INTEGER") == 0)
    {
        AST_Node* iter = node->child->next->child;
        while(iter != NULL)
        {
            printf("%s\n",iter->p->name);
            if(strcmp(iter->p->name,"NUM") != 0)
            {
                // printf("Conditional Statement Case %s, Iterator Integer but case variable %s not Integer\n",node->child->p->lexeme_current_node,iter->p->name);
                char error_string[100];
                sprintf(error_string,"Conditional Statement Case %s, Iterator Integer but case variable %s not Integer\n",node->child->p->lexeme_current_node,iter->p->name);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;
                return 0;
            }
            iter = iter->next;
        }

        AST_Node* default_node = node->child->next->next;
        if(default_node == NULL)
        {
            // printf("Default statement is missing for Conditional Statement Case %s\n",node->child->p->lexeme_current_node);
            char error_string[100];
            sprintf(error_string,"Default statement is missing for Conditional Statement Case %s\n",node->child->p->lexeme_current_node);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
            return 0;
        }
        else
        {
            //printf("Conditional Statement case %s is correct\n",node->child->p->lexeme_current_node);
            return 1;
        }
    }

    else
    {
        // printf("The case identifier is not integer or boolean\n");
        char error_string[100];
        sprintf(error_string,"The case identifier is not integer or boolean\n");
        strcpy(semantic_errors[no_of_total_errors],error_string);
        no_of_total_errors++;
        return 0;
    }
}

int get_size(char* lexeme_current_node, STnode* symbol_table_node)
{
    if(symbol_table_node == NULL)
        return -1;
    int hashIndex = STHashCode(lexeme_current_node);
    if(!symbol_table_node->Symbol_Table[hashIndex].head)
    {
        return get_size(lexeme_current_node,symbol_table_node->parent);
    }
    else
    {
        STentry* check = symbol_table_node->Symbol_Table[hashIndex].head;
        while(check != NULL)
        {
            if(strcmp(check->name,lexeme_current_node) == 0)
            {
                return check->no_of_variables;
            }
            check = check->next;
        }
        return get_size(lexeme_current_node,symbol_table_node->parent);
    }
}
int check_arr(AST_Node* node)
{
    if(node!=NULL)
    {
        // printf("array element %s\n",node->p->lexeme_current_node);

        if(node->child != NULL && strcmp(node->p->name,"ID") == 0)
        {
            return get_size(node->p->lexeme_current_node,node->lookup);
        }
        if(node->child==NULL)
        {
            return 0;
        }
        int range1=0;
        int range2=0;

        if(node->child!=NULL && node->child->next!=NULL)
        {
            range1=check_arr(node->child);
            range2=check_arr(node->child->next);
        }
        if(range1==0 && range2>0)
            return range2;
        else if(range2==0 && range1>0)
            return range1;
        else if(range2==0 && range1==0)
            return 0;
        else if(range1>0 && range2>0 && range1!=range2)
            return -1;
        else if(range1<0 || range2<0)
            return -1;
    }

}

char* check_expression(AST_Node* node)
{
    if(node!=NULL && node->p!=NULL)
    {
        // printf("expression %s\n",node->p->name);
        if(node->child != NULL && strcmp(node->p->name,"ID") == 0 && (strcmp(node->child->p->name,"ID") == 0 || strcmp(node->child->p->name,"NUM") == 0))
        {
            // printf("array element %s %s\n",node->p->lexeme_current_node,node->child->p->lexeme_current_node);
            return get_ID_type(node->p->lexeme_current_node,node->lookup);
        }

        // if(node->child != NULL && ((strcmp(node->child->p->name,"ID") == 0) || (strcmp(node->child->p->name,"NUM") == 0)))

        // {
        //     return get_ID_type(node->p->lexeme_current_node,node->lookup);
        // }

        // if(strcmp(node->child->p->name,"NUM") == 0)
        // {
        //     char* type;
        //     type=(char*) malloc(50*sizeof(char));
        //     strcpy(type,"INTEGER");
        //     return type;
        // }

        if(node->child==NULL)
        {
            if(strcmp(node->p->name,"ID")==0)
            {
                return get_ID_type(node->p->lexeme_current_node,node->lookup);
            }
            else if(strcmp(node->p->name,"NUM")==0)
            {
                char* type;
                type=(char*) malloc(50*sizeof(char));
                strcpy(type,"INTEGER");
                return type;
            }
            else if(strcmp(node->p->name,"RNUM")==0)
            {
                char* type;
                type=(char*) malloc(50*sizeof(char));
                strcpy(type,"REAL");
                return type;
            }
            else
            {
                char* type;
                type=(char*) malloc(50*sizeof(char));
                strcpy(type,"BOOLEAN");
                return type;
            }
        }
        char* type1;
        char* type2;
        type1=check_expression(node->child);
        type2=check_expression(node->child->next);
        if(type1==NULL || type2==NULL)
        {
            return NULL;
        }
        if((strcmp(node->p->name,"PLUS")==0 || strcmp(node->p->name,"MINUS")==0 || strcmp(node->p->name,"MUL")==0 || strcmp(node->p->name,"DIV")==0))
        {
            if(strcmp(type1,"INTEGER")==0 && strcmp(type2,"INTEGER")==0)
            {
                char* type;
                type=(char*) malloc(50*sizeof(char));
                strcpy(type,"INTEGER");
                node->type=type;
                char* var_name;
                var_name=(char*)malloc(50*sizeof(char));
                sprintf(var_name,"expr%d",var_number);
                node->var_name=var_name;
                //printf("this expr variable is%s\n",var_name);
                var_type[var_number]=type;
                var_number++;
                return type;
            }
            else if(strcmp(type1,"REAL")==0 && strcmp(type2,"REAL")==0)
            {
                char* type;
                type=(char*) malloc(50*sizeof(char));
                strcpy(type,"REAL");
                node->type=type;
                char* var_name;
                var_name=(char*)malloc(50*sizeof(char));
                sprintf(var_name,"expr%d",var_number);
                node->var_name=var_name;
                var_type[var_number]=type;
                var_number++;
                return type;
            }
            else
            {
                // printf("ERROR_T: There is mismatch in types\n");
                // no_of_total_errors++;
                return NULL;
            }
        }
        else if((strcmp(node->p->name,"LT")==0 || strcmp(node->p->name,"LE")==0 || strcmp(node->p->name,"GT")==0 || strcmp(node->p->name,"GE")==0 || strcmp(node->p->name,"NE")==0 || strcmp(node->p->name,"EQ")==0))
        {
            if(strcmp(type1,"INTEGER")==0 && strcmp(type2,"INTEGER")==0)
            {
                char* type;
                type=(char*) malloc(50*sizeof(char));
                strcpy(type,"BOOLEAN");
                node->type=type;
                char* var_name;
                var_name=(char*)malloc(50*sizeof(char));
                sprintf(var_name,"expr%d",var_number);
                node->var_name=var_name;
                var_type[var_number]=type;
                var_number++;
                return type;
            }
            else if(strcmp(type1,"REAL")==0 && strcmp(type2,"REAL")==0)
            {
                char* type;
                type=(char*) malloc(50*sizeof(char));
                strcpy(type,"BOOLEAN");
                node->type=type;
                char* var_name;
                var_name=(char*)malloc(50*sizeof(char));
                sprintf(var_name,"expr%d",var_number);
                node->var_name=var_name;
                var_type[var_number]=type;
                var_number++;
                return type;
            }
            else
            {
                // printf("ERROR_T: There is mismatch in types\n");
                // no_of_total_errors++;
                return NULL;
            }
        }
        else if((strcmp(node->p->name,"AND")==0 || strcmp(node->p->name,"OR")==0))
        {
            if(strcmp(type1,"BOOLEAN")==0 && strcmp(type2,"BOOLEAN")==0)
            {
                char* type;
                type=(char*) malloc(50*sizeof(char));
                strcpy(type,"BOOLEAN");
                node->type=type;
                char* var_name;
                var_name=(char*)malloc(50*sizeof(char));
                sprintf(var_name,"expr%d",var_number);
                node->var_name=var_name;
                var_type[var_number]=type;
                var_number++;
                return type;
            }
            else
            {
                // printf("ERROR_T: There is mismatch in types\n");
                // no_of_total_errors++;
                return NULL;
            }
        }
        else
        {
            // printf("ERROR_T: There is mismatch in types\n");
            // no_of_total_errors++;
            return NULL;
        }
    }
}

int in_parent(char* lexeme_name,STnode* lookup_table)
{
    if(lookup_table!=NULL)
    {
        if(lookup_table->parent==NULL)
        {
            int hashIndex = STHashCode(lexeme_name);
            if(!lookup_table->Symbol_Table[hashIndex].head)
            {
                return 0;
            }
            else
            {
                STentry* check = lookup_table->Symbol_Table[hashIndex].head;
                while(check != NULL)
                {
                    if(strcmp(check->name,lexeme_name) == 0)
                    {
                        return 1;
                    }
                    check = check->next;
                }
                return 0;
            }
        }
        else
        {
            int hashIndex = STHashCode(lexeme_name);
            if(!lookup_table->Symbol_Table[hashIndex].head)
            {
                return in_parent(lexeme_name,lookup_table->parent);
            }
            else
            {
                STentry* check = lookup_table->Symbol_Table[hashIndex].head;
                while(check != NULL)
                {
                    if(strcmp(check->name,lexeme_name) == 0)
                    {
                        return 0;
                    }
                    check = check->next;
                }
                return in_parent(lexeme_name,lookup_table->parent);
            }
        }
    }
}

int check_assigned_value(char *name, AST_Node* statements)
{
    AST_Node* current_statement = statements->child;
    //printf("output name %s\n",name);
    while(current_statement != NULL)
    {
        if(strcmp(current_statement->p->name,"assignmentStmt") == 0)
        {
            AST_Node* variable = current_statement->child->child;
            if(strcmp(variable->p->lexeme_current_node,name) == 0)
            {
                if(in_parent(variable->p->lexeme_current_node,variable->lookup))
                return 1;
            }
        }
        else if(strcmp(current_statement->p->name,"ioStmt") == 0 && strcmp(current_statement->child->p->name,"GET_VALUE")==0)
        {
            AST_Node* variable = current_statement->child->child;
            if(strcmp(variable->p->lexeme_current_node,name) == 0)
            {
                if(in_parent(variable->p->lexeme_current_node,variable->lookup))
                return 1;
            }
        }
        else if(strcmp(current_statement->p->name,"iterativeStmt")==0)
        {
            if(current_statement->child->next->next!=NULL)
                if(check_assigned_value(name,current_statement->child->next->next)==1)
                    return 1;
        }
        else if(strcmp(current_statement->p->name,"conditionalStmt")==0)
        {
            AST_Node* iter=current_statement->child->next->child;
            while(iter!=NULL)
            {
                if(check_assigned_value(name,iter->child)==1)
                    return 1;
                iter=iter->next;
            }
        }
        current_statement = current_statement->next;
    }
    return 0;
}


int function_outputplist_parameters_assigned(AST_Node* node)
{
    FTentry* FTentry_check = node->function_enclosing;
    //printf("%s function\n",FTentry_check->name);
    
    if(FTentry_check->no_of_output_list_parameters == 0)
        return 1;
    else
    {
        //printf("%s %s\n",node->child->next->p->name,FTentry_check->name);
        AST_Node* statements = node->child->next->next;
        if(statements == NULL)
            return 0;
        int i;
        //printf("hi\n");
        for(i = 0;i < FTentry_check->no_of_output_list_parameters;i++)
        {
            if(check_assigned_value(FTentry_check->output_plist[i]->name,statements) == 0)
            {
                return 0;
            }
        }
        return 1;
    }
    //return 1;
}

void function_output_parameters_assignment_check_otherModules(AST_Node* otherModules)
{
    if(otherModules == NULL)
        return;
    int k = 0;
    AST_Node* iter = otherModules->child;
    while(iter != NULL)
    {
        if(function_outputplist_parameters_assigned(iter) == 0)
        {
            // printf("function %s doesn't have output parameters assigned\n",iter->p->lexeme_current_node);
            char error_string[100];
            sprintf(error_string,"ERROR_M:function %s doesn't have output parameters assigned at line %d\n",iter->p->lexeme_current_node,iter->p->line_number);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
            k++;
            //return 0;
        }
        else
        {
            //printf("function %s has output parameters assigned\n",iter->p->lexeme_current_node);
        }
        iter = iter->next;
    }
    //printf("All functions have output parameters assigned correctly\n");
    if(k == 0)
    {
        //printf("All functions have output parameters assigned correctly\n");
        //return 1;
    }
    else
    {
        //printf("Issue with %d functions\n",k);
        //return 0;
    }
}

/*char* check_right_array_type(AST_Node* op)
{
    char *left_type;
    left_type = (char*)malloc(50*sizeof(char));
    char *right_type;
    right_type = (char*)malloc(50*sizeof(char));
    if(strcmp(op->child->p->name,"ID") != 0)
    {
        left_type = check_right_array_type(op->child);
    }
    else
    {
        left_type = op->child->p->;
    }
    if(strcmp(op->child->next->p->name,"ID") != 0)
    {
        right_type = check_right_array_type(op->child->next);
    }
    else
    {
        right_type = op->child->next->type;
    }
    if(strcmp(left_type,right_type) == 0)
    {
        return left_type;
    }
    else
    {
        char *k;
        k = (char*)malloc(50*sizeof(char));
        k = "mismatch";
        return k;
    }

}*/

void check_right_arr(AST_Node* temp,char* left_type)
{
    if(temp == NULL)
        return;

    //printf("%s %s\n",temp->p->name,left_type);

    if(strcmp(temp->p->name,"NUM") == 0)
    {
        if(strcmp(left_type,"INTEGER") != 0)
        {
            //printf("Mismatch in array types in line %d\n",temp->p->line_number);
            return;   
        }
    }

    if(strcmp(temp->p->name,"RNUM") == 0)
    {
        if(strcmp(left_type,"REAL") != 0)
        {
            //printf("Mismatch in array types in line %d\n",temp->p->line_number);
            return;   
        }
    }

    if(strcmp(temp->p->name,"ID") == 0)
    {
        //printf("%s\n",get_ID_type(temp->p->lexeme_current_node,temp->lookup));
        if(strcmp(left_type,get_ID_type(temp->p->lexeme_current_node,temp->lookup)) != 0)
        {
            // printf("Mismatch in array types in line %d\n",temp->p->line_number);
            char error_string[100];
            sprintf(error_string,"Mismatch in array types in line %d\n",temp->p->line_number);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
            return;
        }
    }

    else
    {
        if(temp->child != NULL)
        check_right_arr(temp->child,left_type);
        if(temp->child != NULL && temp->child->next != NULL)
        check_right_arr(temp->child->next,left_type);
    }
}

void check_if_arr_assignment(AST_Node* assignmentStmt)
{
    // if(strcmp(assignmentStmt->child->p->name,"ASSIGNOP") == 0)
    // {
    //      char *left_type = assignmentStmt->child->child->p->type;
    //      char *right_type = get_right_array_type(assignmentStmt->child);
    // }
    char *left_type;
    //left_type = (char*)malloc(50*sizeof(char));
    //printf("%s\n",assignmentStmt->child->child->p->lexeme_current_node);
    left_type = get_ID_type(assignmentStmt->child->child->p->lexeme_current_node,assignmentStmt->child->child->lookup);
    if(left_type == NULL)
    {
        //printf("%s is returning NULL\n",assignmentStmt->child->child->p->lexeme_current_node);
        return;
    }
    //printf("%s\n",left_type);
    AST_Node* temp = assignmentStmt->child->child->next;
    check_right_arr(temp,left_type);
}

void traverse_AST(AST_Node* node)
{
    if(node!=NULL && node->p!=NULL)
    {
        //printf("this ast node %s\n",node->p->name);
        if(strcmp(node->p->name,"moduleReuseStmt") == 0)
        {
             //printf("my %s\n",node->p->name);
            // printf("my %s\n",node->child->function_enclosing->name);
            if(node->child->function_enclosing!=NULL && strcmp(node->child->p->lexeme_current_node,node->child->function_enclosing->name)==0)
            {
                // printf("There cannot be recursive call to this function %s at line %d\n",node->child->p->lexeme_current_node,node->child->p->line_number);
                char error_string[100];
                sprintf(error_string,"There cannot be recursive call to this function %s at line %d\n",node->child->p->lexeme_current_node,node->child->p->line_number);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;

            }
            // printf("moduleReuseStmt %s",node->function_enclosing->name);
            check_ID_in_FT(node->child);
            if(function_input_parameters_check(node));
            if(function_output_parameters_check(node));
        }

        else if(strcmp(node->p->name,"otherModules") == 0)
        {
            function_output_parameters_assignment_check_otherModules(node);
            AST_Node* iter1=node->child;
            while(iter1!=NULL)
            {
                traverse_AST(iter1);
                iter1=iter1->next;
            }
        }
        else if(strcmp(node->p->name,"conditionalStmt") == 0)
        {
            if(conditionalStmt_check(node));
            AST_Node* iter1=node->child->next->child;
            while(iter1!=NULL)
            {
                traverse_AST(iter1->child);
                iter1=iter1->next;
            }
            //traverse_AST(node->child->next);
        }
        else if(strcmp(node->p->name,"iterativeStmt") == 0 && strcmp(node->child->p->name,"FOR") == 0)
        {
            int redeclare_errors=0;
            char for_variable[50];
            strcpy(for_variable,node->child->next->p->lexeme_current_node);
            char* type=get_ID_type(node->child->next->p->lexeme_current_node,node->child->next->lookup);
            if(strcmp(type,"INTEGER")!=0)
            {
                // printf("ERROR_V: The iterative variable %s cannot be %s\n",node->child->next->p->lexeme_current_node,type);
                char error_string[100];
                sprintf(error_string,"ERROR_V: The iterative variable %s cannot be %s\n",node->child->next->p->lexeme_current_node,type);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;
            }
            for_statement_check(node->child->next->next,for_variable,&redeclare_errors);
            if(redeclare_errors==0)
            {
                //printf("All correct\n");
            }
            traverse_AST(node->child->next->next);
        }
        else if(strcmp("iterativeStmt",node->p->name) == 0 && strcmp("WHILE",node->child->p->name)==0)
        {
            char* type;
            type=check_expression(node->child->next);

            if(type==NULL)
            {
                // printf("ERROR_T: type mismatch for expression statement in line %d\n",node->child->next->p->line_number);
                char error_string[100];
                sprintf(error_string,"ERROR_T: type mismatch for expression statement in line %d\n",node->child->next->p->line_number);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;
            }
            else
            {
                // if(check_arr(node->child->next)==-1)
                //     printf("ERROR_T: type mismatch for arrays in line %d\n",node->child->next->p->line_number);
                //printf("There is no error in expression statement in line %d\n",node->child->next->p->line_number);
            }
            //check_arr(node->child->next);
            traverse_AST(node->child->next->next);
        }
        else if(strcmp("assignmentStmt",node->p->name) == 0)
        {
            char* type_rvalue;
            char* type_lvalue;

            //check_if_arr_assignment(node);
            // printf("assignment for %s\n",node->child->child->p->lexeme_current_node);
            type_rvalue=check_expression(node->child->child->next);
            // printf("got out of assignment for %s\n",node->child->child->p->lexeme_current_node);
            if(type_rvalue==NULL)
            {
                // printf("ERROR_T: type mismatch for expression statement in line %d\n",node->child->child->next->p->line_number);
                char error_string[100];
                sprintf(error_string,"ERROR_T: type mismatch for expression statement in line %d\n",node->child->child->next->p->line_number);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;
            }
            else
            {
                type_lvalue=get_ID_type(node->child->child->p->lexeme_current_node,node->child->child->lookup);
                // printf("hi\n");
                if(type_lvalue == NULL)
                {
                    // printf("ERROR_T: type mismatch for expression statement in line %d\n",node->child->child->next->p->line_number);
                    char error_string[100];
                    sprintf(error_string,"ERROR_T: type mismatch for expression statement in line %d\n",node->child->child->next->p->line_number);
                    strcpy(semantic_errors[no_of_total_errors],error_string);
                    no_of_total_errors++;//printf("fucking returning\n");
                    return;
                }
                if(strcmp(type_lvalue,type_rvalue)==0)
                {
                    if(node->child->child->child==NULL)
                        {
                            if(check_arr(node->child)<0)
                            {
                                // printf("ERROR_T: type mismatch of elements belonging to two different types of arrays in line %d\n",node->child->p->line_number);
                                char error_string[100];
                                sprintf(error_string,"ERROR_T: type mismatch of elements belonging to two different types of arrays in line %d\n",node->child->p->line_number);
                                strcpy(semantic_errors[no_of_total_errors],error_string);
                                no_of_total_errors++;
                            }
                            // if(check_arr(node->child)==-1)
                            //     printf("ERROR_T: type mismatch for arrays in line %d\n",node->child->p->line_number);
                        }//printf("The type of lvalue and rvalue mathces at line %d\n",node->child->child->p->line_number);
                    else
                    {
                        char* type_of_index;
                        // printf("hi2\n");
                        if(strcmp("ID",node->child->child->child->p->name)==0)
                        {
                            // p//rintf("array index %s\n",node->child->child->child->p->lexeme_current_node);
                            type_of_index=get_ID_type(node->child->child->child->p->lexeme_current_node,node->child->child->child->lookup);
                            if(type_of_index==NULL)
                            {
                                char error_string[100];
                                sprintf(error_string,"ERROR_V: Variable %s his not declared before at line %d\n",node->child->child->child->p->lexeme_current_node,node->child->child->p->line_number);
                                strcpy(semantic_errors[no_of_total_errors],error_string);
                                no_of_total_errors++; 
                            }
                            else
                            {
                                if(strcmp("INTEGER",type_of_index)==0)
                                {
                                    if(check_arr(node->child)<0)
                                    {
                                        // printf("ERROR_T: type mismatch of elements belonging to two different types of arrays in line %d\n",node->child->p->line_number);
                                        char error_string[100];
                                        sprintf(error_string,"ERROR_T: type mismatch of elements belonging to two different types of arrays in line %d\n",node->child->p->line_number);
                                        strcpy(semantic_errors[no_of_total_errors],error_string);
                                        no_of_total_errors++;
                                    }
                                    // if(check_arr(node->child)==-1)
                                    // printf("ERROR_T: type mismatch for arrays in line %d\n",node->child->p->line_number);
                                    //printf("The type of lvalue and rvalue mathces at line %d\n",node->child->child->p->line_number);
                                }
                                else
                                {
                                    // printf("ERROR_T: type mismatch for expression statement, The type of lvalue and rvalue mathces but index %s has wrong type at line %d\n",node->child->child->child->p->lexeme_current_node,node->child->child->p->line_number);
                                    char error_string[100];
                                    sprintf(error_string,"ERROR_T: type mismatch for expression statement, The type of lvalue and rvalue mathces but index %s has wrong type at line %d\n",node->child->child->child->p->lexeme_current_node,node->child->child->p->line_number);
                                    strcpy(semantic_errors[no_of_total_errors],error_string);
                                    no_of_total_errors++;
                                }
                            }
                        }

                    }
                }
                else
                {
                    // printf("ERROR_T: type mismatch for expression statement in line %d\n",node->child->child->p->line_number);
                    char error_string[100];
                    sprintf(error_string,"ERROR_T: type mismatch for expression statement in line %d\n",node->child->child->p->line_number);
                    strcpy(semantic_errors[no_of_total_errors],error_string);
                    no_of_total_errors++;
                }
            }
        }
        else
        {
            AST_Node* iter=node->child;
            while(iter!=NULL)
            {
                traverse_AST(iter);
                iter=iter->next;
            }
        }
    }
    return;
}

int  check_errors()
{
    if(errors > 0)
    {
        printf("Syntactic errors\n");
        return 0;
    }

    else
    {
        printf("No Syntactic Errors\n");
        printf("No of semantic errors = %d\n",no_of_total_errors);
        return 1;
    }
}

void type_checking_main(FILE* testcase,FILE* parsetree)
{
    STmain(testcase,parsetree);
    if(errors==0)
    traverse_AST(astroot);
    //check_errors();
    
}
