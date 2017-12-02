//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//ASM code generation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TypeChecker.h"
#include "CodegenDef.h"
#include "SymbolTable.h"
#include "ast.h"

int global_label_count = 0;
int case_number = 0;
int condition_jump=0;
void print_driver_module_declare1(AST_Node* declareStmt)
{
    AST_Node* type = declareStmt->child;
    if(strcmp(type->p->name,"INTEGER") == 0)
    {
        AST_Node* curr = type->child;
        while(curr != NULL)
        {
            // printf("%s: resd %d\n",curr->p->lexeme_current_node,1);
            fprintf(output_asm,"%s: resd %d\n",curr->p->lexeme_current_node,1);
            curr = curr->next;
        }
    }

    else if(strcmp(type->p->name,"BOOLEAN") == 0)
    {
        AST_Node* curr = type->child;
        while(curr != NULL)
        {
            // printf("%s: resd %d\n",curr->p->lexeme_current_node,1);
            fprintf(output_asm,"%s: resd %d\n",curr->p->lexeme_current_node,1);
            curr = curr->next;
        }
    }

    else if(strcmp(type->p->name,"REAL") == 0)
    {
        AST_Node* curr = type->child;
        while(curr != NULL)
        {
            // printf("%s: resd %d\n",curr->p->lexeme_current_node,1);
            fprintf(output_asm,"%s: resd %d\n",curr->p->lexeme_current_node,1);
            curr = curr->next;
        }
    }

    else if(strcmp(type->p->name,"ARRAY") == 0)
    {
        int range = type->next->child->next->p->value_int - type->next->child->p->value_int + 1;

        if(strcmp(type->next->next->p->name,"INTEGER") == 0)
        {
            // printf("%s: resd %d\n",type->child->p->lexeme_current_node,range);
            fprintf(output_asm,"%s: resd %d\n",type->child->p->lexeme_current_node,range);
        }

        else if(strcmp(type->next->next->p->name,"BOOLEAN") == 0)
        {
            // printf("%s: resd %d\n",type->child->p->lexeme_current_node,range);
            fprintf(output_asm,"%s: resd %d\n",type->child->p->lexeme_current_node,range);
        }

        else if(strcmp(type->next->next->p->name,"REAL") == 0)
        {
            // printf("%s: resd %d\n",type->child->p->lexeme_current_node,range);
            fprintf(output_asm,"%s: resd %d\n",type->child->p->lexeme_current_node,range);
        }

        else
        {
            return;
        }
    }

    else
    {
        return;
    }
}

void print_ioStmt_code(AST_Node* node)
{
    AST_Node* type = node->child;
    AST_Node* id = node->child->child;

    char* var_type;
    var_type=get_ID_type(id->p->lexeme_current_node,id->lookup);

    int size = 0;
      //printf("%s\n",node->child->child->p->name);

    if(strcmp(var_type,"INTEGER") == 0)
        size = 4;

    else if(strcmp(var_type,"BOOLEAN") == 0)
        size = 1;

    else
        size = 8;


    if(strcmp(type->p->name,"GET_VALUE") == 0)
    {
        // printf("\npush rbp\nmov rsi,%s\nmov rdi,format_inp\nmov rax,0\ncall scanf\npop rbp\n",id->p->lexeme_current_node);
        fprintf(output_asm,"\npush rbp\nmov rsi,%s\nmov rdi,format_inp\nmov rax,0\ncall scanf\npop rbp\n",id->p->lexeme_current_node);
    }

    else if(strcmp(type->p->name,"PRINT") == 0)
    {

        // printf("\npush rbp\nmov rsi,[%s]\nmov rdi,format_out\nmov rax,0\ncall printf\npop rbp\n",id->p->lexeme_current_node);
        fprintf(output_asm,"\npush rbp\nmov rsi,[%s]\nmov rdi,format_out\nmov rax,0\ncall printf\npop rbp\n",id->p->lexeme_current_node);
    }

    else
    {
        return;
    }
}

void print_driver_module_declare(AST_Node* node)
{
    if(node!=NULL && node->p!=NULL)
    {
        //printf("%s\n",node->p->name);
        if(strcmp(node->p->name,"declareStmt") == 0)
        {
            print_driver_module_declare1(node);
        }

        else
        {
            AST_Node* iter=node->child;
            while(iter!=NULL)
            {
                print_driver_module_declare(iter);
                iter=iter->next;
            }
        }
    }
}

void print_other_statements(AST_Node* node);

void print_for_statement(AST_Node* node)
{
    char iterator_var[50];
    int local_count = global_label_count;
    strcpy(iterator_var,node->child->next->p->lexeme_current_node);
    //printf("%s\n",iterator_var);
    int number = node->child->next->child->p->value_int - node->child->next->child->next->p->value_int;
    // printf("\nmov %s,%d\n",iterator_var,node->child->next->child->p->value_int);
    fprintf(output_asm,"\nmov dword [%s],%d\n",iterator_var,node->child->next->child->p->value_int);
    // printf("loop%d:\n\n",global_label_count);
    fprintf(output_asm,"loop%d:\n\n",global_label_count);
    global_label_count++;
    //printf("%s\n",node->child->next->next->p->name);
    print_other_statements(node->child->next->next);
    // printf("inc %s\ncmp %s,%d\njne loop%d\n\n",iterator_var,iterator_var,node->child->next->child->next->p->value_int,local_count);
    fprintf(output_asm,"add dword [%s],1\ncmp dword [%s],%d\njne loop%d\n\n",iterator_var,iterator_var,node->child->next->child->next->p->value_int,local_count);

}


void print_start_expression(AST_Node* node)
{
    char* var1,*var2;
    char* this_var;
    int flag1 = 0;
    int flag2 = 0;
    if(node->child!=NULL)
    {
        if(strcmp(node->child->p->name,"ID")==0)
        {
            var1=node->child->p->lexeme_current_node;
            flag1 = 0;
        }
        else if(strcmp(node->child->p->name,"NUM")==0)
        {
            var1=node->child->p->lexeme_current_node;
            flag1 = 1;
        }
        else if(strcmp(node->child->p->name,"RNUM")==0)
        {
            var1=(char*)malloc(50*sizeof(char));
            snprintf(var1,5,"%f",node->child->p->value_double);
        }
        else
        {
            var1=node->child->var_name;
            print_start_expression(node->child);
        }
        if(strcmp(node->child->next->p->name,"ID")==0)
        {
            var2=node->child->next->p->lexeme_current_node;
            flag2 = 0;
        }
        else if(strcmp(node->child->next->p->name,"NUM")==0)
        {
            var2=node->child->next->p->lexeme_current_node;
            flag2 = 1;
        }
        else if(strcmp(node->child->next->p->name,"RNUM")==0)
        {
            var2=(char*)malloc(50*sizeof(char));
            snprintf(var2,5,"%f",node->child->next->p->value_double);
        }
        else
        {
            print_start_expression(node->child->next);
            var2=node->child->next->var_name;

        }
        this_var=node->var_name;
        if(strcmp(node->p->name,"PLUS")==0)
        {
            if(flag1 == 1 && flag2 == 1)
            {
                // printf("\nmov eax,%s\nadd eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,%s\nadd eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);    
            }
            else if(flag1 == 0 && flag2 == 1)
            {
                // printf("\nmov eax,[%s]\nadd eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,[%s]\nadd eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);    
            }

            else if(flag1 == 1 && flag2 == 0)
            {
                // printf("\nmov eax,%s\nadd eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,%s\nadd eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
            }
            else
            {
                // printf("\nmov eax,[%s]\nadd eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,[%s]\nadd eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
            }
        }
        else if(strcmp(node->p->name,"MINUS")==0)
        {
            if(flag1 == 1 && flag2 == 1)
            {
                // printf("\nmov eax,%s\nsub eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,%s\nsub eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);    
            }
            else if(flag1 == 0 && flag2 == 1)
            {
                // printf("\nmov eax,[%s]\nsub eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,[%s]\nsub eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);    
            }

            else if(flag1 == 1 && flag2 == 0)
            {
                // printf("\nmov eax,%s\nsub eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,%s\nsub eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
            }
            else
            {
                // printf("\nmov eax,[%s]\nsub eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,[%s]\nsub eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
            }
        }
        else if(strcmp(node->p->name,"MUL")==0)
        {
            if(flag1 == 1 && flag2 == 1)
            {
                // printf("\nmov eax,%s\nimul eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,%s\nimul eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);    
            }
            else if(flag1 == 0 && flag2 == 1)
            {
                // printf("\nmov eax,[%s]\nimul eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,[%s]\nimul eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);    
            }

            else if(flag1 == 1 && flag2 == 0)
            {
                // printf("\nmov eax,%s\nimul eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,%s\nimul eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
            }
            else
            {
                // printf("\nmov eax,[%s]\nimul eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,[%s]\nimul eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
            }
        }
        else if(strcmp(node->p->name,"DIV")==0)
        {
            if(flag1 == 1 && flag2 == 1)
            {
                // printf("\nmov eax,%s\nadd eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,%s\nidiv eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);    
            }
            else if(flag1 == 0 && flag2 == 1)
            {
                // printf("\nmov eax,[%s]\nidiv eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,[%s]\nidiv eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);    
            }

            else if(flag1 == 1 && flag2 == 0)
            {
                // printf("\nmov eax,%s\nidiv eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,%s\nidiv eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
            }
            else
            {
                // printf("\nmov eax,[%s]\nidiv eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
                fprintf(output_asm,"\nmov eax,[%s]\nidiv eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
            }
        }
        else if(strcmp(node->p->name,"LT")==0)
        {
            int next_jump=condition_jump+1;
            // printf("\ncmp %s,%s\njl jump%d\nmov [%s],0\njmp jump%d\njump%d:\nmov %s,1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            fprintf(output_asm,"\ncmp dword [%s],%s\njl jump%d\nmov [%s],0\njmp jump%d\njump%d:\nmov dword [%s],1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            condition_jump=next_jump+1;
        }
        else if(strcmp(node->p->name,"LE")==0)
        {
            int next_jump=condition_jump+1;
            // printf("\ncmp %s,%s\njle jump%d\nmov [%s],0\njmp jump%d\njump%d:\nmov %s,1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            fprintf(output_asm,"\ncmp dword [%s],%s\njle jump%d\nmov dword [%s],0\njmp jump%d\njump%d:\nmov dword [%s],1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            condition_jump=next_jump+1;
        }
        else if(strcmp(node->p->name,"GT")==0)
        {
            int next_jump=condition_jump+1;
            // printf("\ncmp %s,%s\njg jump%d\nmov [%s],0\njmp jump%d\njump%d:\nmov %s,1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            fprintf(output_asm,"\ncmp dword [%s],%s\njg jump%d\nmov dword [%s],0\njmp jump%d\njump%d:\nmov dword [%s],1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            condition_jump=next_jump+1;
        }
        else if(strcmp(node->p->name,"GE")==0)
        {
            int next_jump=condition_jump+1;
            // printf("\ncmp %s,%s\njge jump%d\nmov [%s],0\njmp jump%d\njump%d:\nmov %s,1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            fprintf(output_asm,"\ncmp dword [%s],%s\njge jump%d\nmov dword [%s],0\njmp jump%d\njump%d:\nmov dword [%s],1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            condition_jump=next_jump+1;
        }
        else if(strcmp(node->p->name,"EQ")==0)
        {
            int next_jump=condition_jump+1;
            // printf("\ncmp %s,%s\nje jump%d\nmov [%s],0\njmp jump%d\njump%d:\nmov %s,1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            fprintf(output_asm,"\ncmp dword [%s],%s\nje jump%d\nmov dword [%s],0\njmp jump%d\njump%d:\nmov dword [%s],1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            condition_jump=next_jump+1;
        }
        else if(strcmp(node->p->name,"NE")==0)
        {
            int next_jump=condition_jump+1;
            // printf("\ncmp %s,%s\njne jump%d\nmov [%s],0\njmp jump%d\njump%d:\nmov %s,1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            fprintf(output_asm,"\ncmp dword [%s],%s\njne jump%d\nmov dword [%s],0\njmp jump%d\njump%d:\nmov dword [%s],1\njump%d: \n",var1,var2,condition_jump,this_var,next_jump,condition_jump,this_var,next_jump);
            condition_jump=next_jump+1;
        }
        else if(strcmp(node->p->name,"AND")==0)
        {
            // printf("\nmov eax,%s\nand eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
            fprintf(output_asm,"\nmov eax,[%s]\nand eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
        }
        else if(strcmp(node->p->name,"OR")==0)
        {
            // printf("\nmov eax,%s\nor eax,%s\nmov [%s],eax\n\n",var1,var2,this_var);
            fprintf(output_asm,"\nmov eax,[%s]\nor eax,[%s]\nmov [%s],eax\n\n",var1,var2,this_var);
        }
    }
}

void print_while_statement(AST_Node* node)
{
    int curr_no = global_label_count;

    // printf("\nwhile%d:\n\n",curr_no);
    fprintf(output_asm,"\nwhile%d:\n\n",curr_no);
    print_start_expression(node->child->next);
    // printf("cmp %s,0 \nje final%d\n\n",node->child->next->var_name,curr_no);
    fprintf(output_asm,"cmp %s,0 \nje final%d\n\n",node->child->next->var_name,curr_no);

    print_other_statements(node->child->next->next);

    // printf("jmp while%d\n",curr_no);
    fprintf(output_asm,"jmp while%d\n",curr_no);
    // printf("final%d:\n\n",curr_no);
    fprintf(output_asm,"final%d:\n\n",curr_no);
    global_label_count++;
    return;
}


void print_iterativeStmt_code(AST_Node* node)
{
    //printf("hello\n");
    if(strcmp(node->child->p->name,"FOR") == 0)
    {
        print_for_statement(node);
    }

    else if(strcmp(node->child->p->name,"WHILE") == 0)
    {
        print_while_statement(node);
        //print_while_statement(code);
    }
}

void print_other_statements(AST_Node* node);

void print_conditional_statement(AST_Node* switch_node)
{
    int curr_switch = case_number;
    if(switch_node == NULL)
        return;
    char* name = switch_node->child->p->lexeme_current_node;
    //printf("\nmove eax,%s\n",name);
    AST_Node* caseStmt = switch_node->child->next->child;
    // printf("hello\n");
    while(caseStmt != NULL)
    {
        if(strcmp(caseStmt->p->name,"TRUE") == 0 || strcmp(caseStmt->p->name,"FALSE") == 0)
        {
            // printf("cmp %s,%s\nje %s\n",name,caseStmt->p->name,caseStmt->p->name);
            //strlwr(caseStmt->p->name);
        if(strcmp(caseStmt->p->name,"TRUE")==0)
            fprintf(output_asm,"cmp dword [%s],%s\nje true\n",name,caseStmt->p->name);
        else if(strcmp(caseStmt->p->name,"FALSE")==0)
            fprintf(output_asm,"cmp dword [%s],%s\nje false\n",name,caseStmt->p->name);
        }
        else
        {
            //printf("hello\n");
            // printf("cmp %s,%s\nje case%d\n",name,caseStmt->p->lexeme_current_node,caseStmt->p->value_int);
            fprintf(output_asm,"cmp dword [%s],%s\nje case%d\n",name,caseStmt->p->lexeme_current_node,caseStmt->p->value_int);
        }
        caseStmt = caseStmt->next;
    }

    // printf("jmp final%d:\n\n",curr_switch);
    fprintf(output_asm,"jmp final%d\n\n",curr_switch);
    caseStmt = switch_node->child->next->child;

    while(caseStmt != NULL)
    {
        if(strcmp(caseStmt->p->name,"TRUE") == 0 || strcmp(caseStmt->p->name,"FALSE") == 0)
        {
            // printf("%s:\n",caseStmt->p->name);
            if(strcmp(caseStmt->p->name,"TRUE")==0)
               fprintf(output_asm,"true:\n"); 
           else if(strcmp(caseStmt->p->name,"FALSE")==0)
               fprintf(output_asm,"false:\n"); 
            // strlwr(caseStmt->p->name);
            // fprintf(output_asm,"%s:\n",caseStmt->p->name);
            print_other_statements(caseStmt->child);
            // printf("jmp final%d\n\n",curr_switch);
            fprintf(output_asm,"jmp final%d\n\n",curr_switch);
        }
        else
        {
            // printf("\ncase%d:\n",caseStmt->p->value_int);
            fprintf(output_asm,"\ncase%d:\n",caseStmt->p->value_int);
            print_other_statements(caseStmt->child);
            // printf("jmp final%d\n\n",curr_switch);
            fprintf(output_asm,"jmp final%d\n\n",curr_switch);
        }
        caseStmt = caseStmt->next;
    }

    if(switch_node->child->next->next != NULL)
    {
        // printf("default%d:\n",case_number);
        fprintf(output_asm,"default%d:\n",case_number);
        print_other_statements(switch_node->child->next->next->child);
        // printf("jmp final%d\n\n",curr_switch);
    }
    // printf("final%d:\n",case_number);
    fprintf(output_asm,"final%d:\n",case_number);
    case_number++;
    // printf("\n");
    fprintf(output_asm,"\n");
    return;
}

void print_other_statements(AST_Node* node)
{
    if(node!=NULL && node->p!=NULL)
    {
        AST_Node* iter=node->child;
        while(iter!=NULL)
        {
            //printf("%s\n",iter->p->name);
            if(strcmp(iter->p->name,"ioStmt") == 0)
            {
                print_ioStmt_code(iter);

            }
            else if(strcmp(iter->p->name,"iterativeStmt") == 0)
            {
                //printf("hello\n");
                print_iterativeStmt_code(iter);
            }
            else if(strcmp(iter->p->name,"conditionalStmt")==0)
            {
                case_number++;
                print_conditional_statement(iter);
            }
            else if(strcmp(iter->p->name,"assignmentStmt")==0)
            {
                if(strcmp(iter->child->child->p->name,"ID") == 0 && strcmp(iter->child->child->next->p->name,"NUM") == 0)
                {
                    // printf("\nmov dword [%s],%d\n",iter->child->child->p->lexeme_current_node,iter->child->child->next->p->value_int);
                    fprintf(output_asm,"\nmov dword [%s],%d\n",iter->child->child->p->lexeme_current_node,iter->child->child->next->p->value_int);
                }
                else if (strcmp(iter->child->child->p->name,"ID") == 0 && strcmp(iter->child->child->next->p->name,"RNUM") == 0)
                {
                    // printf("\nmov dword [%s],%lf\n",iter->child->child->p->lexeme_current_node,iter->child->child->next->p->value_double);
                    fprintf(output_asm,"\nmov dword [%s],%lf\n",iter->child->child->p->lexeme_current_node,iter->child->child->next->p->value_double);
                }
                else
                {
                    print_start_expression(iter->child->child->next);
                    // printf("\nmov eax,[%s]\nmov [%s],eax\n",iter->child->child->next->var_name,iter->child->child->p->lexeme_current_node);
                    fprintf(output_asm,"\nmov eax,[%s]\nmov [%s],eax\n",iter->child->child->next->var_name,iter->child->child->p->lexeme_current_node);
                }
            }
            iter=iter->next;
        }
    }
}

void print_declare_statements_first(AST_Node* node)
{
    //printf("%s\n",node->p->name);
    AST_Node* iter=node->child;
    while(iter!=NULL)
    {
        if(strcmp(iter->p->name,"driverModule") == 0)
        {
            print_driver_module_declare(iter);
            //printf("yes\n");
            // printf("\nsection .text\nglobal main\nglobalmain:\n\n");
            fprintf(output_asm,"\nsection .text\nglobal main\nmain:\n\n");
            print_other_statements(iter->child);
            return;
        }
        iter=iter->next;
    }
}

void codegen_main(FILE* testcase,FILE* code_asm,FILE* parsetree)
{
    type_checking_main(testcase,parsetree);
    if(no_of_total_errors==0 && errors==0)
    {
        output_asm=code_asm;
        // printf("extern printf\nextern scanf\n\nsection .data\nformat_inp db \"");
        fprintf(output_asm,"extern printf\nextern scanf\nsection .data\nTRUE equ 1\nFALSE equ 0\nformat_inp db \"");
        // printf("%%d\\n\" ,0\nformat_out db `%%d\\n`,0\n");
        fprintf(output_asm,"%%d\\n\" , 0\nformat_out db `%%d\\n`, 0\n");
        // printf("\nsection .bss\n");
        fprintf(output_asm,"\nsection .bss\n");
        int i;
        for(i=0;i<var_number;i++)
        {
            if(strcmp(var_type[i],"INTEGER") == 0)
            {
                // printf("expr%d: resd %d\n",i,1);
                fprintf(output_asm,"expr%d: resd %d\n",i,1);
            }

            else if(strcmp(var_type[i],"BOOLEAN") == 0)
            {
                // printf("expr%d: resd %d\n",i,1);
                fprintf(output_asm,"expr%d: resd %d\n",i,1);
            }

            else if(strcmp(var_type[i],"REAL") == 0)
            {
                // printf("expr%d: resd %d\n",i,1);
                fprintf(output_asm,"expr%d: resd %d\n",i,1);
            }
        }

        print_declare_statements_first(astroot);
    }

}
