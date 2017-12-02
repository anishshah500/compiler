//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//Symbol Table

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "SymbolTableDef.h"

int curr_no=1;

int no_of_total_errors = 0;

FILE* fp_to_symbol_table;


int FTHashCode(char id_name[50])
{
    int temp_key=0;
    int i=0;
    while(id_name[i]!='\0')
    {
        temp_key+=(int)id_name[i];
        i++;
    }
    return temp_key%FT_SIZE;

}


int STHashCode(char id_name[50])
{
    int temp_key=0;
    int i=0;
    while(id_name[i]!='\0')
    {
        temp_key+=(int)id_name[i];
        i++;
    }
    return temp_key%SYMBOL_TABLE_SIZE;

}

FTentry* createFTEntry(char name[50],STnode* curr_table)
{
    FTentry* newnode = (FTentry*) malloc(sizeof(FTentry));
    newnode->name = (char*)malloc(50*sizeof(char));
    strcpy(newnode->name,name);
    newnode->next=NULL;
    newnode->no_of_output_list_parameters=0;
    newnode->no_of_input_list_parameters=0;
    newnode->node_to_ST=curr_table;
    return newnode;
}

STentry* createSTEntry(char name[50], char type[50])
{
    //printf("creating STentry\n");
    STentry* newnode = (STentry*) malloc(sizeof(STentry));
    newnode->name = (char*)malloc(50*sizeof(char));
    newnode->type = (char*)malloc(50*sizeof(char));
    strcpy(newnode->name,name);
    strcpy(newnode->type,type);
    newnode->next=NULL;
    newnode->no_of_variables=1;
    if(strcmp(type,"INTEGER") == 0)
        newnode->width = 4;
    else if(strcmp(type,"REAL") == 0)
        newnode->width = 8;
    else if(strcmp(type,"BOOLEAN") == 0)
        newnode->width = 1;
    else
        newnode->width=0;

    //printf("leavingSTentry\n");
    return newnode;
}

void insert_at_end(FTentry* Item,FTentry* head)
{
  FTentry* temp=head;
  while(temp->next!=NULL)
  {
    temp=temp->next;
  }
  temp->next=Item;
  return;
}

list_unit* createunit(AST_Node* id_name)
{
  list_unit* new_unit=(list_unit*)malloc(sizeof(list_unit));
  new_unit->name=id_name->p->lexeme_current_node;
  new_unit->type=id_name->child->p->name;
  new_unit->is_array=0;
  return new_unit;
}

void insert_at_endST(STentry* Item,STentry* head)
{
  STentry* temp=head;
  while(temp->next!=NULL)
  {
    temp=temp->next;
  }
  temp->next=Item;
  // printf("%s %s\n",temp->next->name,temp->next->type);
  return;
}

void insert_in_STnode(STentry* newItem,STnode* current_symbol_table)
{
    int hashIndex = STHashCode(newItem->name);
    if(!current_symbol_table->Symbol_Table[hashIndex].head)
        {
            current_symbol_table->Symbol_Table[hashIndex].head = newItem;
            current_symbol_table->Symbol_Table[hashIndex].count = 1;
            //printf("%s %s\n",current_symbol_table->Symbol_Table[hashIndex].head->name,current_symbol_table->Symbol_Table[hashIndex].head->type);
        }
        else
        {
            STentry* check = current_symbol_table->Symbol_Table[hashIndex].head;
            //printf("adding %s\n",temp->p->lexeme_current_node);
            while(check != NULL)
            {
                //printf("name of check and temp %s==%s %s==%s\n",check->name,temp->p->lexeme_current_node,check->type,type);
                if(strcmp(check->name,newItem->name) == 0)
                {
                    // printf("ERROR_V: variable %s already declared\n",newItem->name);
                    char error_string[100];
                    sprintf(error_string,"ERROR_V: variable %s already declared\n",newItem->name);
                    strcpy(semantic_errors[no_of_total_errors],error_string);
                    no_of_total_errors++;
                    return;
                    // break;
                }
                check = check->next;
            }
            insert_at_endST(newItem,current_symbol_table->Symbol_Table[hashIndex].head);

        }
}

void put_input_list(AST_Node* module_name,FTentry* node)
{
  AST_Node* iter=module_name->child;
  //node->no_of_output_list_parameters=0;
  // printf("hi\n");
  while(iter!=NULL)
  {
    //printf("id and type %s %s\n",iter->p->name,iter->child->p->name );
    list_unit* new_unit=createunit(iter);
    if(strcmp(iter->child->p->name,"ARRAY")==0)
    {
        new_unit->is_array=1;
    }
    else
    {
        node->input_plist[node->no_of_input_list_parameters]=new_unit;
        node->no_of_input_list_parameters++;
    }

    char type[50];
    if(new_unit->is_array==1)
        strcpy(type,iter->child->next->next->p->name);
    else
        strcpy(type,iter->child->p->name);
    STentry* newItem = createSTEntry(iter->p->lexeme_current_node,type);

    //printf("hello %s\n",newItem->type);
    newItem->offset=node->node_to_ST->offset;
    //printf("hello %s\n",newItem->type);
    if(new_unit->is_array==1)
        {
            node->node_to_ST->offset+=((iter->child->next->child->next->p->value_int-iter->child->next->child->p->value_int+1)*newItem->width);
            newItem->no_of_variables=iter->child->next->child->next->p->value_int-iter->child->next->child->p->value_int+1;
        }
    else
        node->node_to_ST->offset+=newItem->width;

    insert_in_STnode(newItem,node->node_to_ST);
    iter=iter->next;
  }
  return;
}

void put_output_list(AST_Node* module_name,FTentry* node)
{
  AST_Node* iter=module_name->child;

  while(iter!=NULL)
  {
    list_unit* new_unit=createunit(iter);
    if(strcmp(iter->child->p->name,"ARRAY")==0)
    {
        new_unit->is_array=1;
    }
    else
    {
        node->output_plist[node->no_of_output_list_parameters]=new_unit;
        node->no_of_output_list_parameters++;
    }
    char type[50];
    if(new_unit->is_array==1)
        strcpy(type,iter->child->next->next->p->name);
    else
        strcpy(type,iter->child->p->name);
    STentry* newItem = createSTEntry(iter->p->lexeme_current_node,type);
    newItem->offset=node->node_to_ST->offset;
    if(new_unit->is_array==1)
        {
            node->node_to_ST->offset+=((iter->child->next->child->next->p->value_int-iter->child->next->child->p->value_int+1)*newItem->width);
            newItem->no_of_variables=iter->child->next->child->next->p->value_int-iter->child->next->child->p->value_int+1;
        }
    else
        node->node_to_ST->offset+=newItem->width;
    insert_in_STnode(newItem,node->node_to_ST);
    iter=iter->next;
  }
  return;
}

FTentry* insert_in_FT_def(AST_Node* module_name,STnode** curr_symbol_table)
{
    //printf("Adding definition of %s\n",module_name->p->lexeme_current_node);

        int hashIndex = FTHashCode(module_name->p->lexeme_current_node);
        struct FTentry* newItem=createFTEntry(module_name->p->lexeme_current_node,*curr_symbol_table);
        if(!Function_Table[hashIndex].head)
        {

            put_input_list(module_name->child,newItem);

            if(module_name->child->next != NULL && strcmp(module_name->child->next->p->name,"output_plist")==0)
            {

                put_output_list(module_name->child->next,newItem);
            }
            Function_Table[hashIndex].head=newItem;
            Function_Table[hashIndex].count=1;
            return newItem;
        }
      FTentry* check=Function_Table[hashIndex].head;
      while(check!=NULL)
      {

        if(strcmp(check->name,module_name->p->lexeme_current_node)==0)
        {

          if(check->no_of_input_list_parameters==0 && check->no_of_output_list_parameters==0)
          {
            //printf("say hi\n");
            break;
          }
          else
          {
            // printf("ERROR: Previously defined module %s\n",module_name->p->lexeme_current_node);
            char error_string[100];
            sprintf(error_string,"ERROR_M: Previously defined module %s at line %d\n",module_name->p->lexeme_current_node,module_name->p->line_number);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
            return NULL;
          }
        }
        check=check->next;
      }
      if(check==NULL)
      {
        put_input_list(module_name->child,newItem);
        if(strcmp(module_name->child->next->p->name,"output_plist")==0)
        {
          put_output_list(module_name->child->next,newItem);
        }
        insert_at_end(newItem,Function_Table[hashIndex].head);
        Function_Table[hashIndex].count++;
        return newItem;
      }
      else
      {
        //printf("%s\n",check->name);
        put_input_list(module_name->child,check);
        if(strcmp(module_name->child->next->p->name,"output_plist")==0)
        {
          put_output_list(module_name->child->next,check);
        }
        //curr_symbol_table=&(check->node_to_ST);
        return check;
      }

}

STnode* get_main_parent(STnode* node)
{
    if(node!=NULL)
    {
    if(node->parent==NULL)
        return node;
    else
        return get_main_parent(node->parent);
    }
}

void make_update(STnode* node,int offset_update)
{
    if(node!=NULL)
    {
        node->offset=offset_update;
        int x;
        for(x=0;x<node->no_of_children;x++)
        {
            make_update(node->children[x],offset_update);
        }
    }
}

void update_offset(STnode* node,int offset_update)
{
    if(node!=NULL)
    {
        STnode* main=get_main_parent(node);
        make_update(main,offset_update);
    }
}

void insert_in_ST(AST_Node* declare_Stmt, STnode* current_symbol_table)
{

    int range=1;
    char type[50];
    if(strcmp(declare_Stmt->child->p->name,"ARRAY")!=0)
        strcpy(type,declare_Stmt->child->p->name);
    else
    {
        strcpy(type,declare_Stmt->child->next->next->p->name);
        range=declare_Stmt->child->next->child->next->p->value_int-declare_Stmt->child->next->child->p->value_int+1;
    }

    AST_Node* temp = declare_Stmt->child->child;
    while(temp != NULL)
    {

        STentry* newItem = createSTEntry(temp->p->lexeme_current_node,type);
        if(strcmp(declare_Stmt->child->p->name,"ARRAY")!=0)
        {
            newItem->offset=current_symbol_table->offset;
            update_offset(current_symbol_table,current_symbol_table->offset+newItem->width);
            //current_symbol_table->offset+=newItem->width;
        }
        else
        {

            newItem->offset=current_symbol_table->offset;
            newItem->no_of_variables=range;
            update_offset(current_symbol_table,current_symbol_table->offset+range*newItem->width);
            // current_symbol_table->offset+=range*newItem->width;
        }
        insert_in_STnode(newItem, current_symbol_table);
        temp=temp->next;
    }
}

void insert_in_FT_dec(AST_Node* module_name,STnode* node)
{

      int hashIndex = FTHashCode(module_name->p->lexeme_current_node);
      struct FTentry* newItem=createFTEntry(module_name->p->lexeme_current_node,node);

      if(!Function_Table[hashIndex].head)
      {
        Function_Table[hashIndex].head=newItem;
        Function_Table[hashIndex].count=1;
        return;
      }
      FTentry* check=Function_Table[hashIndex].head;
      while(check!=NULL)
      {
        if(strcmp(check->name,module_name->p->lexeme_current_node)==0)
        {
          // printf("ERROR: Previously declare module %s\n",module_name->p->lexeme_current_node);
          char error_string[100];
            sprintf(error_string,"ERROR_M: Previously declare module %s at line %d\n",module_name->p->lexeme_current_node,module_name->p->line_number);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
          return;
        }
        check=check->next;
      }
    insert_at_end(newItem,Function_Table[hashIndex].head);
    /*
     * update the head of the list and no of
     * nodes in the current bucket
     */
    Function_Table[hashIndex].count++;
    return;
}

void Function_node_modedec(AST_Node* node)
{
  AST_Node* iter = node->child;
  while(iter != NULL)
  {
    STnode* current_function_symbol_table;
    current_function_symbol_table = (STnode*)malloc(sizeof(STnode));
    current_function_symbol_table->offset = 0;
    current_function_symbol_table->no_of_children=0;
    current_function_symbol_table->parent=NULL;
    insert_in_FT_dec(iter,current_function_symbol_table);
    iter = iter->next;
  }
}

int check_ID_in_ST(char* lexeme_current_node, STnode* symbol_table_node)
{
    int hashIndex = STHashCode(lexeme_current_node);
    if(!symbol_table_node->Symbol_Table[hashIndex].head)
    {
        //printf("Not Found\n");
        return 0;
    }
    else
    {
        STentry* check = symbol_table_node->Symbol_Table[hashIndex].head;
        while(check != NULL)
        {
            if(strcmp(check->name,lexeme_current_node) == 0)
            {
                //printf("FOUND\n");
                return 1;
            }
            check = check->next;
        }
        //printf("Not Found\n");
        return 0;
    }

}

int check_ID(AST_Node* ASTnode,STnode* symbol_table_node)
{
    if(symbol_table_node == NULL)
    {
        //printf("name of lexeme %s\n",ASTnode->p->lexeme_current_node);
        //printf("Not Found\n");
        return 0;
    }

    if(check_ID_in_ST(ASTnode->p->lexeme_current_node,symbol_table_node))
    {
        //printf("FOUND\n");
        return 1;
    }

    else
        check_ID(ASTnode,symbol_table_node->parent);
}

void check_for_assignment(AST_Node* ASTnode, STnode* symbol_table_node)
{

    AST_Node* iter = ASTnode->child;

    while(iter != NULL)
    {
        if(strcmp(iter->p->name,"ID") == 0)
        {
            if(check_ID(iter,iter->lookup) == 0)
            {
                // printf("ERROR_V: variable %s not declared\n",iter->p->lexeme_current_node);
                char error_string[100];
                sprintf(error_string,"ERROR_V: variable %s not declared at line %d\n",iter->p->lexeme_current_node,iter->p->line_number);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;
            }
            // else
            // {
            //     printf("This identifier is declared previously %s\n",iter->p->lexeme_current_node);
            // }
        }
        check_for_assignment(iter,iter->lookup);
        iter = iter->next;
    }
}

void check_for_modulereuse2(AST_Node* ASTnode, STnode* symbol_table_node)
{

    AST_Node* iter = ASTnode->child;

    while(iter != NULL)
    {
        if(strcmp(iter->p->name,"ID") == 0)
        {
            if(check_ID(iter,iter->lookup) == 0)
            {
                // printf("ERROR_V: variable %s not declared\n",iter->p->lexeme_current_node);
                char error_string[100];
                sprintf(error_string,"ERROR_V: variable %s not declared at line %d\n",iter->p->lexeme_current_node,iter->p->line_number);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;
            }
            // else
            // {
            //     printf("This identifier is declared previously %s\n",iter->p->lexeme_current_node);
            // }
        }
        check_for_modulereuse2(iter,iter->lookup);
        iter = iter->next;
    }
}

void check_ID_in_FT(AST_Node* node)
{
    //printf("checking definition of %s\n",node->p->lexeme_current_node);
    int hashIndex = FTHashCode(node->p->lexeme_current_node);
    // printf("%s\n", );
    if(!Function_Table[hashIndex].head)
    {
        // printf("hi\n");
        // printf("Not Present Module %s\n",node->p->lexeme_current_node);
        char error_string[100];
        sprintf(error_string,"ERROR_M:Not Present Module %s at line %d\n",node->p->lexeme_current_node,node->p->line_number);
        strcpy(semantic_errors[no_of_total_errors],error_string);
        no_of_total_errors++;
    }
    else
    {
        FTentry* check=Function_Table[hashIndex].head;
        while(check != NULL)
        {
            if(strcmp(check->name,node->p->lexeme_current_node) == 0)
            {
                //printf("Present Module %s\n",node->p->lexeme_current_node);
                return;
            }
            check = check->next;
        }
        // printf("hi2 \n");
        // printf("Not Present Module %s\n",node->p->lexeme_current_node);
        char error_string[100];
        sprintf(error_string,"ERROR_M:Not Present Module %sat line %d\n",node->p->lexeme_current_node,node->p->line_number);
        strcpy(semantic_errors[no_of_total_errors],error_string);
        no_of_total_errors++;
    }

}

void check_for_modulereuse(AST_Node* ASTnode, STnode* symbol_table_node)
{
    check_ID_in_FT(ASTnode->child);

    AST_Node* iter = ASTnode->child->child;

    while(iter != NULL)
    {
        if(strcmp(iter->p->name,"ID") == 0)
        {
            if(check_ID(iter,iter->lookup) == 0)
            {
                // printf("ERROR_V: variable %s not declared\n",iter->p->lexeme_current_node);
                char error_string[100];
                sprintf(error_string,"ERROR_V: variable %s not declared at line %d\n",iter->p->lexeme_current_node,iter->p->line_number);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;
            }
            // else
            // {
            //     printf("This identifier is declared previously %s\n",iter->p->lexeme_current_node);
            // }
        }
        check_for_modulereuse2(iter,iter->lookup);
        iter = iter->next;
    }
}

void add_to_ST_statements(AST_Node* statements,STnode* current_function_symbol_table);
void check_io(AST_Node* ASTnode,STnode* symbol_table_node)
{
    if(strcmp(ASTnode->child->p->name,"GET_VALUE")==0)
    {
        if(check_ID(ASTnode->child->child,ASTnode->child->child->lookup)==0)
        {
            // printf("ERROR_V: variable %s not declared\n",ASTnode->child->child->p->lexeme_current_node);
            char error_string[100];
            sprintf(error_string,"ERROR_V: variable %s not declared at line %d\n",ASTnode->child->child->p->lexeme_current_node,ASTnode->child->child->p->line_number);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
        }
        // else
        // {
        //     printf("This identifier is declared previously %s\n",ASTnode->child->child->p->lexeme_current_node);
        // }
    }
    else
    {
        if(check_ID(ASTnode->child->child,ASTnode->child->child->lookup)==0)
        {
            // printf("ERROR_V: variable %s not declared\n",ASTnode->child->child->p->lexeme_current_node);
            char error_string[100];
            sprintf(error_string,"ERROR_V: variable %s not declared at line %d\n",ASTnode->child->child->p->lexeme_current_node,ASTnode->child->child->p->line_number);
            strcpy(semantic_errors[no_of_total_errors],error_string);
            no_of_total_errors++;
        }
        // else
        // {
        //     printf("This identifier is declared previously %s\n",ASTnode->child->child->p->lexeme_current_node);
        // }
    }
}

void check_for_iterative(AST_Node* ASTnode,STnode* symbol_table_node)
{
    if(check_ID(ASTnode->child->next,ASTnode->child->next->lookup) == 0)
    {
        // printf("ERROR_V: variable %s not declared\n",ASTnode->child->next->p->lexeme_current_node);
        char error_string[100];
        sprintf(error_string,"ERROR_V: variable %s not declared at line %d\n",ASTnode->child->next->p->lexeme_current_node,ASTnode->child->next->p->line_number);
        strcpy(semantic_errors[no_of_total_errors],error_string);
        no_of_total_errors++;
        // return 0;
    }
    // else
    // {
    //     printf("This identifier is declared previously %s\n",ASTnode->child->next->p->lexeme_current_node);
    // }
    if(ASTnode->child->next->next!=NULL)
    add_to_ST_statements(ASTnode->child->next->next,ASTnode->child->next->next->lookup);
    // AST_Node* statements = ASTnode->child->next->next;
}

void check_aorb_expr(AST_Node* ASTnode,STnode* symbol_table_node)
{
    AST_Node* iter = ASTnode->child;

    while(iter != NULL)
    {
        if(strcmp(iter->p->name,"ID") == 0)
        {
            if(check_ID(iter,iter->lookup) == 0)
            {
                // printf("ERROR_V: variable %s not declared\n",iter->p->lexeme_current_node);
                char error_string[100];
                sprintf(error_string,"ERROR_V: variable %s not declared at line %d\n",iter->p->lexeme_current_node,iter->p->line_number);
                strcpy(semantic_errors[no_of_total_errors],error_string);
                no_of_total_errors++;
            }
            // else
            // {
            //     printf("This identifier is declared previously %s\n",iter->p->lexeme_current_node);
            // }
        }
        check_aorb_expr(iter,iter->lookup);
        iter = iter->next;
    }
}

void check_while_iterative(AST_Node* ASTnode,STnode* symbol_table_node)
{
    check_aorb_expr(ASTnode->child->next,ASTnode->child->next->lookup);
    if(ASTnode->child->next->next!=NULL)
    add_to_ST_statements(ASTnode->child->next->next,ASTnode->child->next->next->lookup);
    // AST_Node* statements = ASTnode->child->next->next;
}

void check_for_conditional(AST_Node* ASTnode,STnode* symbol_table_node)
{
    if(check_ID(ASTnode->child,ASTnode->child->lookup) == 0)
    {
        // printf("ERROR_V: variable %s not declared\n",ASTnode->child->p->lexeme_current_node);
        char error_string[100];
        sprintf(error_string,"ERROR_V: variable %s not declared at liine %d\n",ASTnode->child->p->lexeme_current_node,ASTnode->child->p->line_number);
        strcpy(semantic_errors[no_of_total_errors],error_string);
        no_of_total_errors++;
        // return 0;
    }
    // else
    // {
    //     printf("This identifier is declared previously %s\n",ASTnode->child->p->lexeme_current_node);
    // }
    AST_Node* iter=ASTnode->child->next->child;
    while(iter!=NULL)
    {
        add_to_ST_statements(iter->child,iter->child->lookup);
        iter=iter->next;
    }
    //add_to_ST_statements(ASTnode->child->next->next,ASTnode->child->next->next->lookup);
    // AST_Node* statements = ASTnode->child->next->next;
}

void update_Symbol_Table(AST_Node* statements,STnode* current_function_symbol_table,FTentry* current_function)
{
    if(statements!=NULL)
    {
            AST_Node* iter1=statements->child;
            while(iter1!=NULL)
            {
                //printf("looping in %s\n",iter1->p->name);

                if(strcmp(iter1->p->name,"statements") == 0)
                {
                    STnode* new_child;
                    new_child = (STnode*)malloc(sizeof(STnode));
                    new_child->offset = current_function_symbol_table->offset;
                    new_child->no_of_children=0;
                    new_child->parent=current_function_symbol_table;
                    int i=0;
                    for(i=0;i<MAX_CHILD;i++)
                    {
                        new_child->children[i]=NULL;
                    }
                    for(i=0;i<SYMBOL_TABLE_SIZE;i++)
                    {
                        new_child->Symbol_Table[i].count=0;
                        new_child->Symbol_Table[i].head=NULL;
                    }
                    current_function_symbol_table->children[current_function_symbol_table->no_of_children++]=new_child;
                    iter1->lookup=new_child;

                }
                else
                iter1->lookup=current_function_symbol_table;
                iter1->function_enclosing=current_function;
                update_Symbol_Table(iter1,iter1->lookup,current_function);
                iter1=iter1->next;
            }
    }
}

void add_to_ST_statements(AST_Node* statements,STnode* current_function_symbol_table)
{
    if(statements!=NULL)
    {
        if(strcmp(statements->p->name,"statements")==0)
        {
            AST_Node* curr_statement = statements->child;

            while(curr_statement != NULL)
            {
                //printf("adding in %s\n",curr_statement->p->name);
                if(strcmp(curr_statement->p->name,"declareStmt") == 0)
                {
                    //printf("going in insertST\n");
                    insert_in_ST(curr_statement,curr_statement->lookup);
                    //printf("ending in st\n");
                }
                if(strcmp(curr_statement->p->name,"ioStmt")==0)
                {
                    check_io(curr_statement,curr_statement->lookup);
                }
                if(strcmp(curr_statement->p->name,"iterativeStmt")==0 && curr_statement->child!=NULL && strcmp(curr_statement->child->p->name,"FOR")==0)
                {
                    check_for_iterative(curr_statement,curr_statement->lookup);
                }
                if(strcmp(curr_statement->p->name,"assignmentStmt")==0)
                {
                    check_for_assignment(curr_statement,curr_statement->lookup);
                }
                if(strcmp(curr_statement->p->name,"moduleReuseStmt")==0)
                {
                    check_for_modulereuse(curr_statement,curr_statement->lookup);
                }
                if(strcmp(curr_statement->p->name,"conditionalStmt")==0)
                {
                    check_for_conditional(curr_statement,curr_statement->lookup);
                }
                if(strcmp(curr_statement->p->name,"iterativeStmt")==0 && curr_statement->child!=NULL && strcmp(curr_statement->child->p->name,"WHILE")==0)
                {
                    check_while_iterative(curr_statement,curr_statement->lookup);
                }
                curr_statement = curr_statement->next;
            }
        }
    }
}

void Function_node_moddef(AST_Node* node)
{
  AST_Node* iter = node->child;
  FTentry* to_add_id;

  while(iter != NULL)
  {

    STnode* current_function_symbol_table;
    current_function_symbol_table = (STnode*)malloc(sizeof(STnode));
    current_function_symbol_table->offset = 0;
    current_function_symbol_table->no_of_children=0;
    current_function_symbol_table->parent=NULL;

    int i=0;
    for(i=0;i<MAX_CHILD;i++)
    {
        current_function_symbol_table->children[i]=NULL;
    }
    for(i=0;i<SYMBOL_TABLE_SIZE;i++)
    {
        current_function_symbol_table->Symbol_Table[i].count=0;
        current_function_symbol_table->Symbol_Table[i].head=NULL;
    }
    iter->lookup=current_function_symbol_table;

    to_add_id=insert_in_FT_def(iter,&current_function_symbol_table);
    if(to_add_id==NULL)
    {
        iter=iter->next;
        continue;
    }
    iter->function_enclosing=to_add_id;
    AST_Node* ip_list_ptr = iter->child;

    if(ip_list_ptr->next != NULL)
    {

        if(strcmp(ip_list_ptr->next->p->name,"output_plist") == 0)
        {
            if(ip_list_ptr->next->next != NULL && strcmp(ip_list_ptr->next->next->p->name,"statements") == 0)
            {
                ip_list_ptr->next->next->function_enclosing=to_add_id;
                ip_list_ptr->next->next->lookup=to_add_id->node_to_ST;
                update_Symbol_Table(ip_list_ptr->next->next,to_add_id->node_to_ST,ip_list_ptr->next->next->function_enclosing);
                add_to_ST_statements(ip_list_ptr->next->next,to_add_id->node_to_ST);
            }
        }


        if(strcmp(ip_list_ptr->next->p->name,"statements") == 0)
        {
            ip_list_ptr->next->function_enclosing=to_add_id;
            ip_list_ptr->next->lookup=to_add_id->node_to_ST;
            update_Symbol_Table(ip_list_ptr->next,to_add_id->node_to_ST,ip_list_ptr->next->function_enclosing);
            add_to_ST_statements(ip_list_ptr->next,to_add_id->node_to_ST);
        }
    }
    iter = iter->next;
  }
}


void print_ST(STnode* temp1)
{
    if(temp1!=NULL)
    { 
        int j,x;
        printf("offset %d\n",temp1->offset);
        printf("children %d\n",temp1->no_of_children);
        for(j = 0;j<SYMBOL_TABLE_SIZE;j++)
        {
            if(temp1->Symbol_Table[j].count>0)
            {
                STentry* k1 = temp1->Symbol_Table[j].head;
                while(k1!=NULL)
                {
                    printf("%s %s %d %d\n",k1->name,k1->type,k1->width,k1->offset);
                    k1 = k1->next;
                }
            }
        }
        for(x=0;x<temp1->no_of_children;x++)
        {
            print_ST(temp1->children[x]);
        }
    }
}

int number_of_AST_nodes = 0;

void traverseAST(AST_Node* node)
{
    //number_of_AST_nodes++;

  if(node!=NULL && node->p!=NULL)
  {
    // printf("starting %s\n",node->p->name);
    if(strcmp(node->p->name,"moduleDeclarations") == 0)
    {
        Function_node_modedec(node);
    }
    else if(strcmp(node->p->name,"otherModules") == 0)
    {
        Function_node_moddef(node);
    }
    else if(strcmp(node->p->name,"driverModule")==0)
    {
        STnode* current_function_symbol_table;
        current_function_symbol_table = (STnode*)malloc(sizeof(STnode));
        current_function_symbol_table->offset = 0;
        current_function_symbol_table->no_of_children=0;
        current_function_symbol_table->parent=NULL;

        int i=0;
        for(i=0;i<MAX_CHILD;i++)
        {
            current_function_symbol_table->children[i]=NULL;
        }
        for(i=0;i<SYMBOL_TABLE_SIZE;i++)
        {
            current_function_symbol_table->Symbol_Table[i].count=0;
            current_function_symbol_table->Symbol_Table[i].head=NULL;
        }
        node->lookup=current_function_symbol_table;
        if(node->child!=NULL)
        {
            node->child->lookup=current_function_symbol_table;
        }

        update_Symbol_Table(node->child,current_function_symbol_table,NULL);
        add_to_ST_statements(node->child,current_function_symbol_table);
        //print_ST(node->lookup);
    }
    else
    {
        AST_Node* iter=node->child;
        while(iter!=NULL)
        {
            traverseAST(iter);
            iter=iter->next;
        }
    }
    //printf("finished %s\n",node->p->name);
  }
  return;
}

void print_Symbol(STnode* temp1,int level,char* func_name,FILE* fp)
{
    if(temp1!=NULL)
    {
        int i;
        for(i = 0;i<SYMBOL_TABLE_SIZE;i++)
        {
            if(temp1->Symbol_Table[i].count > 0)
            {
                STentry* iter = temp1->Symbol_Table[i].head;
                while(iter != NULL)
                {
                    if(iter->no_of_variables == 1)
                    {
                        //fprintf(fp,"%-9d\t%-9.9s\t%-19.9s\t%-19.9s\t%-9d\t%-9d\t%-9d\n",curr_no++,iter->name,iter->type,func_name,level,iter->width,iter->offset);
                        fprintf(fp,"%d\t%s\t%s\t%s\t%d\t%d\t%d\n",curr_no++,iter->name,iter->type,func_name,level,iter->width,iter->offset);
                    }
                    else
                    {
                        //fprintf(fp,"%-9d\t%-9.9s\tarray(%d,%s)\t%-9.9s\t%-9d\t%-9d\t%-9d\n",curr_no++,iter->name,iter->no_of_variables,iter->type,func_name,level,iter->width*iter->no_of_variables,iter->offset);
                        fprintf(fp,"%d\t%s\tarray(%d,%s)\t%s\t%d\t%d\t%d\n",curr_no++,iter->name,iter->no_of_variables,iter->type,func_name,level,iter->width*iter->no_of_variables,iter->offset);
                    }
                    iter = iter->next;
                }
            }
        }
        int x;
        for(x=0;x<temp1->no_of_children;x++)
        {
            print_Symbol(temp1->children[x],level+1,func_name,fp);
        }
    }
}


void print_symbol_table()
{
    int i;
    FILE *fp = fopen("symbol_table_output.txt","w");

    fprintf(fp,"S.No.\tidentifier lexeme\type\tscope(module name)\tscope(nesting level)\twidth\toffset\n");
    //fprintf(fp,"%-9.9s\t%-9.9s\t%-19.9s\t%-19.9s\t%-9.9s\t%-9.9s\t%-9.9s\n","S.No.","identifier lexeme","type","scope(moduleName)","scope(nesting level)","width","offset");
    //fprint(fp,"%-9.9s\t%-9.9s\t%-9.9s\t%-9.9s\t%-9.9s\t%-9.9s\n","S.No.","identifier lexeme","type","scope(moduleName)","scope(nesting level)","width","offset");

    for(i = 0;i<FT_SIZE;i++)
    {

        if(Function_Table[i].count > 0)
        {
            FTentry* k = Function_Table[i].head;
            while(k!=NULL)
            {
                STnode* temp1=k->node_to_ST;
                print_Symbol(temp1,1,k->name,fp);

                k = k->next;
            }
        }
    }

    AST_Node* iter = astroot->child;
    while(iter != NULL)
    {
        if(strcmp(iter->p->name,"driverModule") == 0)
        {
            char *driver = (char*)malloc(50 * sizeof(char));
            strcpy(driver,"driver");
            print_Symbol(iter->lookup,1,driver,fp);
        }

        iter = iter->next;
    }

    fclose(fp);

}

//int number_of_AST_nodes = 0;

void count_number_AST_nodes(AST_Node* astnode)
{
    if(astnode == NULL)
        return;

    number_of_AST_nodes++;

    AST_Node* iter = astnode->child;

    while(iter != NULL)
    {
        count_number_AST_nodes(iter);
        iter = iter->next;
    }
}

//void count_number_PT_nodes()

void compute_parse_tree_compression()
{
    count_number_AST_nodes(astroot);
    //printf("Number of AST Nodes = %d\n",number_of_AST_nodes);
    //printf("Number of PT Nodes = %d\n",number_of_PT_nodes);
    int size_of_PT = sizeof(pt_node) * number_of_PT_nodes;
    int size_of_AST = sizeof(AST_Node) * number_of_AST_nodes;
    printf("\nParse Tree\tNumber of nodes = %d Allocated Memory = %d Bytes\n",number_of_PT_nodes,size_of_PT);
    printf("AST       \tNumber of nodes = %d Allocated Memory = %d Bytes\n",number_of_AST_nodes,size_of_AST);
    printf("Compression Percentage = ((%d-%d)/%d)*100\n",size_of_PT,size_of_AST,size_of_PT);
}

void STmain(FILE* testcase,FILE* parsetree)
{
  ASTmain(testcase,parsetree);
  if(errors==0)
  {
    traverseAST(astroot);
    count_number_AST_nodes(astroot);
}
 // printf("Number of AST Nodes = %d\n",number_of_AST_nodes);
  // printf("Number of PT Nodes = %d\n",number_of_PT_nodes);
  // int i,j;
  /*for(i = 0;i<FT_SIZE;i++)
  {

      if(Function_Table[i].count > 0)
      {
          FTentry* k = Function_Table[i].head;
          while(k!=NULL)
          {
              printf("%s\n",k->name);
              printf("node parameter %d,%d\n",k->no_of_input_list_parameters,k->no_of_output_list_parameters);
              STnode* temp1=k->node_to_ST;
              print_ST(temp1);

              k = k->next;
          }
      }
  }*/
}
