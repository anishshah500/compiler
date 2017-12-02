//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//AST generation

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "astdef.h"
#include "parser.h"

int node_count=0;

// pt_node* astroot;
AST_Node* modulestmt_ast(pt_node* traverse_node);
AST_Node* get_arithmeticExpr_ast(pt_node* traverse_node);
AST_Node* get_AST_Node(pt_node* par)
{
	AST_Node* a1 = (AST_Node*) malloc(sizeof(AST_Node));
	a1->p=par;
	a1->next = NULL;
	a1->child = NULL;
	a1->lookup=NULL;
	a1->function_enclosing=NULL;
	a1->type=NULL;
	a1->var_name=NULL;
	return a1;
}


AST_Node* moduleid_ast(pt_node* traverse_node)
{
	// printf("in%s\n",traverse_node->name);
	AST_Node* temp=get_AST_Node(traverse_node->children[2]);
	return temp;
}
AST_Node* moduledec_ast(pt_node* traverse_node)
{
	AST_Node* temp=get_AST_Node(traverse_node);
	AST_Node* temp2;
	if(traverse_node->no_of_children==0)
	{
		// printf("in NULL %s\n",traverse_node->name);
		return NULL;
	}
	else
	{
		// printf("in not NULL %s\n",traverse_node->name);
		temp->child=moduleid_ast(traverse_node->children[0]);
		temp2=moduledec_ast(traverse_node->children[1]);
		if(temp2!=NULL)
		{
			temp->child->next=temp2->child;
			free(temp2);
		}
	}
	return temp;
	
}


//moduleReuseStmt

AST_Node* get_N3_ast(pt_node* traverse_node)
{
    if(traverse_node->no_of_children == 0)
        return NULL;
    AST_Node* temp = get_AST_Node(traverse_node->children[1]);
    temp->next = get_N3_ast(traverse_node->children[2]);
    return temp;
}

AST_Node* get_idList_ast(pt_node* traverse_node)
{
    AST_Node* temp = get_AST_Node(traverse_node->children[0]);
    temp->next = get_N3_ast(traverse_node->children[1]);
    return temp;
}

AST_Node* get_optional_ast(pt_node* traverse_node)
{
    AST_Node* temp = get_AST_Node(traverse_node);
    temp->child = get_idList_ast(traverse_node->children[1]);
    return temp;
}

AST_Node* get_moduleReuseStmt_ast(pt_node* traverse_node)
{
    AST_Node* temp= get_AST_Node(traverse_node->children[3]);
    
    if(traverse_node->children[0]->no_of_children == 0)
    {
        temp->child =get_AST_Node(traverse_node->children[6]);
        temp->child->child= get_idList_ast(traverse_node->children[6]);
        return temp;
    }
    
    else
    {
    	temp->child =get_AST_Node(traverse_node->children[6]);
    	temp->child->child= get_idList_ast(traverse_node->children[6]);
        temp->child->next = get_optional_ast(traverse_node->children[0]);
        return temp;
    }
}

//iterativeStmt
// AST_Node* get_arithematicOrBooleanExpr_ast(pt_node* traverse_node)
// {
// 	AST_Node* temp=get_AST_Node(traverse_node);
// 	return temp;
// }
AST_Node* get_arithematicOrBooleanExpr_ast(pt_node* traverse_node);
AST_Node* get_iterativeStmt_ast(pt_node* traverse_node)
{
	AST_Node* temp;
    if(strcmp(traverse_node->children[0]->name,"FOR") == 0)
    {
    	// printf("in for %s\n",traverse_node->children[0]->name);
        temp= get_AST_Node(traverse_node->children[0]);
        temp->next = get_AST_Node(traverse_node->children[2]);
        temp->next->child = get_AST_Node(traverse_node->children[4]->children[0]);
        temp->next->child->next = get_AST_Node(traverse_node->children[4]->children[2]);
        temp->next->next = modulestmt_ast(traverse_node->children[7]);
        // printf("for mein %s\n",traverse_node->children[4]->children[0]->lexeme_current_node);
        return temp;
    }
    else
    //if(strcmp(traverse_node->children[0]->name,"WHILE") == 0)
    {
    	// printf("in while %s\n",traverse_node->children[0]->name);
        temp = get_AST_Node(traverse_node->children[0]);
        temp->next = get_arithematicOrBooleanExpr_ast(traverse_node->children[2]);
        temp->next->next = modulestmt_ast(traverse_node->children[5]);
        return temp;
    }
}

//conditionalStmt

AST_Node* get_default_ast(pt_node* traverse_node)
{
    AST_Node *temp = get_AST_Node(traverse_node);
    temp->child = modulestmt_ast(traverse_node->children[2]);
    return temp;
}
AST_Node* get_N9_ast(pt_node* traverse_node)
{
	if(traverse_node->no_of_children == 0)
	{
		//printf("N9 is null\n");
        return NULL;
	}
    else
    {
    	//printf("N9 is not null\n");
    	AST_Node *temp = get_AST_Node(traverse_node->children[1]->children[0]);
    	temp->child = modulestmt_ast(traverse_node->children[3]);
      	temp->next = get_N9_ast(traverse_node->children[6]);
        return temp;
    }
}

AST_Node* get_caseStmts_ast(pt_node* traverse_node)
{
    AST_Node* temp = get_AST_Node(traverse_node);
    temp->child = get_AST_Node(traverse_node->children[1]->children[0]);
    //printf("conditional case %s\n",traverse_node->children[1]->children[0]->name);
    temp->child->child = modulestmt_ast(traverse_node->children[3]);
    //printf("id is %s and N( children %d\n",traverse_node->children[1]->children[0]->name,)
    temp->child->next = get_N9_ast(traverse_node->children[6]);
    return temp;
}


AST_Node* get_conditionalStmt_ast(pt_node* traverse_node)
{
    AST_Node* temp = get_AST_Node(traverse_node);
    temp= get_AST_Node(traverse_node->children[2]);
    //printf("conditional %s\n",traverse_node->children[5]->name);
    temp->next = get_caseStmts_ast(traverse_node->children[5]);
    if(traverse_node->children[6]->no_of_children > 0)
        temp->next->next = get_default_ast(traverse_node->children[6]);

    return temp;
}

//ioStmt
AST_Node* get_var_ast(pt_node* traverse_node)
{
    if(traverse_node->no_of_children == 1)
    {
    	// printf("my var is %s\n",traverse_node->children[0]->name);
        AST_Node* temp = get_AST_Node(traverse_node->children[0]);
        return temp;
    }

    else
    {
    	// printf("var is %s\n",traverse_node->name);
        AST_Node* temp = get_AST_Node(traverse_node->children[0]);
        if(traverse_node->children[1]->no_of_children!=0)
        	temp->child = get_AST_Node(traverse_node->children[1]->children[1]);
        return temp;
    }
}

AST_Node* get_ioStmt_ast(pt_node* traverse_node)
{
    if(strcmp(traverse_node->children[0]->name,"GET_VALUE") == 0)
    {
        //AST_Node* temp = get_AST_Node(traverse_node);
        AST_Node* temp=get_AST_Node(traverse_node->children[0]);
        //temp->child = get_AST_Node(traverse_node->children[0]);
        temp->child = get_AST_Node(traverse_node->children[2]);
        return temp;
    }

    if(strcmp(traverse_node->children[0]->name,"PRINT") == 0)
    {
    	// printf("This is print %s\n",traverse_node->children[2]->name);
        AST_Node* temp=get_AST_Node(traverse_node->children[0]);
        temp->child = get_var_ast(traverse_node->children[2]);
        return temp;
    }
}

//declarestmt
AST_Node* get_newdataType_ast(pt_node* traverse_node)
{
    if(traverse_node->no_of_children == 1)
    {
        AST_Node* temp=get_AST_Node(traverse_node->children[0]);
        return temp;
    }

    else
    {
        //array
        AST_Node* temp = get_AST_Node(traverse_node->children[0]);
        temp->next =get_AST_Node(traverse_node->children[2]);
        temp->next->child = get_AST_Node(traverse_node->children[2]->children[0]);
        temp->next->child->next = get_AST_Node(traverse_node->children[2]->children[2]);
        temp->next->next = get_AST_Node(traverse_node->children[5]->children[0]);
        return temp;
    }
}

AST_Node* get_declareStmt_ast(pt_node* traverse_node)
{
    //AST_Node* temp = get_AST_Node(traverse_node);
    AST_Node* temp;
    temp = get_newdataType_ast(traverse_node->children[3]);
    temp->child=get_idList_ast(traverse_node->children[1]);;
    // temp = get_idList_ast(traverse_node->children[1]);
    // temp->next = get_AST_Node(traverse_node->children[3]);
    // temp->next->child = get_newdataType_ast(traverse_node->children[3]);
    return temp;
}

AST_Node* get_N8_ast(pt_node* traverse_node)
{
	AST_Node* temp_child;
	if(traverse_node->no_of_children==0)
		return NULL;
	AST_Node* temp=get_AST_Node(traverse_node->children[0]->children[0]);
	temp->child=get_arithmeticExpr_ast(traverse_node->children[1]);
	temp_child=get_N8_ast(traverse_node->children[2]);
	if(temp_child==NULL)
	{
		return temp;
	}
	else
	{
		AST_Node* t1;
		t1=temp_child->child;
		temp_child->child=temp->child;
		temp_child->child->next=t1;
		temp->child=temp_child;
		return temp;
	}
}

AST_Node* get_anyterm_ast(pt_node* traverse_node)
{
	AST_Node* temp1;
	AST_Node* temp2;
	// printf("anyterm is %s\n",traverse_node->name);
	temp1=get_arithmeticExpr_ast(traverse_node->children[0]);
	temp2=get_N8_ast(traverse_node->children[1]);
	if(temp2==NULL)
		return temp1;
	else
	{
		AST_Node* temp_child;
		temp_child=temp2->child;
		temp2->child=temp1;
		temp2->child->next=temp_child;
		return temp2;
	}
}

AST_Node* get_N7_ast(pt_node* traverse_node)
{
	AST_Node* temp_child;
	if(traverse_node->no_of_children==0)
		return NULL;
	AST_Node* temp=get_AST_Node(traverse_node->children[0]->children[0]);
	temp->child=get_anyterm_ast(traverse_node->children[1]);
	temp_child=get_N7_ast(traverse_node->children[2]);
	if(temp_child==NULL)
	{
		return temp;
	}
	else
	{
		AST_Node* t1;
		t1=temp_child->child;
		temp_child->child=temp->child;
		temp_child->child->next=t1;
		temp->child=temp_child;
		return temp;
	}
}

AST_Node* get_arithematicOrBooleanExpr_ast(pt_node* traverse_node)
{
	AST_Node* temp1;
	AST_Node* temp2;
	// printf("aorb is %s\n",traverse_node->name);
	temp1=get_anyterm_ast(traverse_node->children[0]);
	temp2=get_N7_ast(traverse_node->children[1]);
	if(temp2==NULL)
		return temp1;
	else
	{
		AST_Node* temp_child;
		temp_child=temp2->child;
		temp2->child=temp1;
		temp2->child->next=temp_child;
		return temp2;
	}
}

AST_Node* get_factor_ast(pt_node* traverse_node)
{
	AST_Node* temp;
	if(strcmp(traverse_node->children[0]->name,"BO")==0)
	{
		temp=get_arithematicOrBooleanExpr_ast(traverse_node->children[2]);
		return temp;
	}
	else
	{
		// printf("factor is %s\n",traverse_node->name);
		temp=get_var_ast(traverse_node->children[0]);
		return temp;
	}
}

AST_Node* get_N5_ast(pt_node* traverse_node)
{
	AST_Node* temp_child;
	if(traverse_node->no_of_children==0)
		return NULL;
	AST_Node* temp=get_AST_Node(traverse_node->children[0]->children[0]);
	temp->child=get_factor_ast(traverse_node->children[1]);
	temp_child=get_N5_ast(traverse_node->children[2]);
	if(temp_child==NULL)
	{
		return temp;
	}
	else
	{
		AST_Node* t1;
		t1=temp_child->child;
		temp_child->child=temp->child;
		temp_child->child->next=t1;
		temp->child=temp_child;
		return temp;
	}
}

AST_Node* get_term_ast(pt_node* traverse_node)
{
	AST_Node* temp1;
	AST_Node* temp2;
	// printf("term is %s\n",traverse_node->name);
	temp1=get_factor_ast(traverse_node->children[0]);
	temp2=get_N5_ast(traverse_node->children[1]);
	if(temp2==NULL)
		return temp1;
	else
	{
		AST_Node* temp_child;
		temp_child=temp2->child;
		temp2->child=temp1;
		temp2->child->next=temp_child;
		return temp2;
	}
}

AST_Node* get_N4_ast(pt_node* traverse_node)
{
	AST_Node* temp_child;
	if(traverse_node->no_of_children==0)
		return NULL;
	AST_Node* temp=get_AST_Node(traverse_node->children[0]->children[0]);
	temp->child=get_term_ast(traverse_node->children[1]);
	temp_child=get_N4_ast(traverse_node->children[2]);
	if(temp_child==NULL)
	{
		return temp;
	}
	else
	{
		AST_Node* t1;
		t1=temp_child->child;
		temp_child->child=temp->child;
		temp_child->child->next=t1;
		temp->child=temp_child;
		return temp;
	}
}

AST_Node* get_arithmeticExpr_ast(pt_node* traverse_node)
{
	AST_Node* temp1;
	AST_Node* temp2;
	// printf("get_arithmeticExpr_ast is %s\n",traverse_node->name);
	temp1=get_term_ast(traverse_node->children[0]);
	temp2=get_N4_ast(traverse_node->children[1]);
	if(temp2==NULL)
		return temp1;
	else
	{
		AST_Node* temp_child;
		temp_child=temp2->child;
		temp2->child=temp1;
		temp2->child->next=temp_child;
		return temp2;
	}
}

AST_Node* get_expression_ast(pt_node* traverse_node)
{
	AST_Node* temp;
	// printf("expression is %s\n",traverse_node->children[0]->name);
	if(strcmp(traverse_node->children[0]->name,"MINUS") == 0)
	{
		temp = get_AST_Node(traverse_node->children[0]);
		temp->child = get_arithmeticExpr_ast(traverse_node->children[2]);
	}
	else
	{
		temp = get_arithematicOrBooleanExpr_ast(traverse_node->children[0]);

	}
	return temp;
}

AST_Node* get_valueID_ast(pt_node* traverse_node)
{
	AST_Node* temp=get_AST_Node(traverse_node->children[0]);
	temp->child=get_expression_ast(traverse_node->children[1]);
	return temp;
}

AST_Node* get_valueARR_ast(pt_node* traverse_node)
{
	AST_Node* temp;
	temp=get_AST_Node(traverse_node->children[3]);
	temp->child=get_AST_Node(traverse_node->children[1]->children[0]);
	temp->child->next=get_expression_ast(traverse_node->children[4]);
	return temp;
}

AST_Node* get_assignmentStmt_ast(pt_node* traverse_node)
{
	AST_Node* temp;
	AST_Node* temp2;
	temp = get_AST_Node(traverse_node->children[0]);
	// printf("assignment %s\n",traverse_node->children[1]->name);
	if(strcmp(traverse_node->children[1]->children[0]->name,"lvalueIDStmt")==0)
	{
		temp2 = get_valueID_ast(traverse_node->children[1]->children[0]);
		AST_Node* temp1;
		temp1=temp2->child;
		temp2->child=temp;
		temp2->child->next=temp1;
		return temp2;
	}
	//return temp;
	else
	{
		temp2=get_valueARR_ast(traverse_node->children[1]->children[0]);
		AST_Node* temp_child1;
		AST_Node* temp_child2;
		temp_child1=temp2->child;
		temp_child2=temp2->child->next;
		temp2->child=temp;
		temp2->child->child=temp_child1;
		temp2->child->child->next=NULL;
		temp2->child->next=temp_child2;
		// /free(temp2->child->next);
		//free(temp2->child->child->next);
		return temp2;
	}

}

AST_Node* modulepartstmt_ast(pt_node* traverse_node)
{
	pt_node* temp_child1;
	temp_child1=traverse_node->children[0];
	if(strcmp(temp_child1->name,"simpleStmt")==0)
	{
		temp_child1=temp_child1->children[0];
	}
	AST_Node* stmt=get_AST_Node(temp_child1);
	if(strcmp(temp_child1->name,"declareStmt")==0)
	{
		stmt->child=get_declareStmt_ast(temp_child1);
	}
	if(strcmp(temp_child1->name,"ioStmt")==0)
	{
		// printf("hi tis is iostmt\n");
		stmt->child=get_ioStmt_ast(temp_child1);
	}
	if(strcmp(temp_child1->name,"conditionalStmt")==0)
	{
		stmt->child=get_conditionalStmt_ast(temp_child1);
	}
	if(strcmp(temp_child1->name,"iterativeStmt")==0)
	{
		// printf("me in iter\n"); 	
		stmt->child=get_iterativeStmt_ast(temp_child1);
		// ->p->name);
	}
	if(strcmp(temp_child1->name,"moduleReuseStmt")==0)
	{
		// printf("me in iter\n"); 	
		stmt->child=get_moduleReuseStmt_ast(temp_child1);
		// ->p->name);
	}
	if(strcmp(temp_child1->name,"assignmentStmt")==0)
	{
		// printf("me in iter\n"); 	
		stmt->child=get_assignmentStmt_ast(temp_child1);
		// ->p->name);
	}
	//temp2=modulestmt_ast(traverse_node->children[1]);
	// printf("in %s\n",temp_child1->name);
	//AST_Node* temp=get_AST_Node(traverse_node->children[2]);
	return stmt;
}

AST_Node* modulestmt_ast(pt_node* traverse_node)
{
	if(traverse_node->no_of_children==0)
		return NULL;
	//pt_node* temp_child1;
	else
	{
		AST_Node* stmt=get_AST_Node(traverse_node);
		AST_Node* temp2;
		// printf("in not NULL %s\n",traverse_node->name);
		stmt->child=modulepartstmt_ast(traverse_node->children[0]);
		temp2=modulestmt_ast(traverse_node->children[1]);
		if(temp2!=NULL)
		{
			stmt->child->next=temp2->child;
			free(temp2);
		}
		return stmt;
	}
}
AST_Node* moduledriver_ast(pt_node* traverse_node)
{
	AST_Node* temp=get_AST_Node(traverse_node);
	pt_node* temp_child=traverse_node->children[4]->children[1];
	if(temp_child->no_of_children==0)
	{
		// printf("in NULL %s\n",temp_child->name);
		return temp;
	}
	else
	{
		// printf("in not NULL %s\n",temp_child->name);
		temp->child=modulestmt_ast(temp_child);


	}
	return temp;
}

AST_Node* get_dataType_ast(pt_node* traverse_node)
{
    if(traverse_node->no_of_children == 1)
    {
        AST_Node* temp=get_AST_Node(traverse_node->children[0]);
        return temp;
    }

    else
    {
        //array
        AST_Node* temp = get_AST_Node(traverse_node->children[0]);
        temp->next =get_AST_Node(traverse_node->children[2]);
        temp->next->child = get_AST_Node(traverse_node->children[2]->children[0]);
        temp->next->child->next = get_AST_Node(traverse_node->children[2]->children[2]);
        temp->next->next = get_AST_Node(traverse_node->children[5]->children[0]);
        return temp;
    }
}

AST_Node* getN1_ast(pt_node* traverse_node)
{
    if(traverse_node->no_of_children == 0)
        return NULL;
    else
    {
        AST_Node* temp = get_AST_Node(traverse_node->children[1]);
        temp->child = get_dataType_ast(traverse_node->children[3]);
        temp->next = getN1_ast(traverse_node->children[4]);
        return temp;
    }
}

AST_Node* get_inputplist_ast(pt_node* traverse_node)
{
    AST_Node* temp = get_AST_Node(traverse_node);
    AST_Node* temp2;
    temp->child=get_AST_Node(traverse_node->children[0]);
    temp2=temp->child;
    temp2->child=get_dataType_ast(traverse_node->children[2]);
    // temp->next = get_dataType_ast(traverse_node->children[2]);
    temp2->next= getN1_ast(traverse_node->children[3]);
    return temp;
}

AST_Node* get_Type_ast(pt_node* traverse_node)
{
        AST_Node* temp=get_AST_Node(traverse_node->children[0]);
        return temp;
}

AST_Node* getN2_ast(pt_node* traverse_node)
{
    if(traverse_node->no_of_children == 0)
        return NULL;
    else
    {
        AST_Node* temp = get_AST_Node(traverse_node->children[1]);
        temp->child = get_Type_ast(traverse_node->children[3]);
        temp->next = getN2_ast(traverse_node->children[4]);
        return temp;
    }
}

AST_Node* get_outplist_ast(pt_node* traverse_node)
{
	AST_Node* temp = get_AST_Node(traverse_node);
    AST_Node* temp2;
    temp->child=get_AST_Node(traverse_node->children[0]);
    temp2=temp->child;
    temp2->child=get_Type_ast(traverse_node->children[2]);
    // temp->next = get_dataType_ast(traverse_node->children[2]);
    temp2->next= getN2_ast(traverse_node->children[3]);
    return temp;
	// AST_Node* temp1=get_AST_Node(traverse_node);
 //    temp1->child = get_outputplist_ast(traverse_node->children[3]);
 //    return temp;
}

AST_Node* otherid_ast(pt_node* traverse_node)
{
    AST_Node* temp = get_AST_Node(traverse_node->children[2]);
    temp->child = get_inputplist_ast(traverse_node->children[7]);
    if(traverse_node->children[10]->no_of_children == 0)
    {
        //ret is empty
        // printf("I am in hi %s\n",traverse_node->children[11]->name);
        temp->child->next = modulestmt_ast(traverse_node->children[11]->children[1]);
    }
    else
    {
        //ret is not empty
        // printf("I am in hi in not%s\n",traverse_node->children[10]->name);
        temp->child->next = get_outplist_ast(traverse_node->children[10]->children[2]);
        // printf("hi\n");
        temp->child->next->next = modulestmt_ast(traverse_node->children[11]->children[1]);
    }
    return temp;
}

AST_Node* moduleother_ast(pt_node* traverse_node)
{
    AST_Node* temp=get_AST_Node(traverse_node);
	AST_Node* temp2;
	if(traverse_node->no_of_children==0)
	{
		return NULL;
	}
	else
    {
        temp->child = otherid_ast(traverse_node->children[0]);
        temp2 = moduleother_ast(traverse_node->children[1]);
        if(temp2 != NULL)
        {
            temp->child->next = temp2->child;
            free(temp2);
        }
    }
    return temp;
}

AST_Node* AST_generate(pt_node* traverse_node)
{
	if(traverse_node!=NULL)
	{
		AST_Node* top_level=get_AST_Node(traverse_node);
		AST_Node* a1;
		AST_Node* temp;
		//printf("node is %s\n",traverse_node->name);
		if(strcmp(traverse_node->name,"program")==0)
		{
			// printf("in %s\n",traverse_node->name);
			a1=moduledec_ast(traverse_node->children[0]);
			if(a1!=NULL)
			{
				top_level->child=a1;
				temp=top_level->child;
			}
			a1=moduleother_ast(traverse_node->children[1]);
			if(top_level->child==NULL)
			{
				if(a1!=NULL)
				{
					top_level->child=a1;
					temp=top_level->child;
				}
			}
			else
			{
				if(a1!=NULL)
				{
					temp->next=a1;
					temp=temp->next;
				}
			}
			a1=moduledriver_ast(traverse_node->children[2]);
			if(top_level->child==NULL)
			{
				top_level->child=a1;
				temp=top_level->child;
			}
			else
			{
				temp->next=a1;
				temp=temp->next;
			}
			a1=moduleother_ast(traverse_node->children[3]);
			if(a1!=NULL)
				{
					temp->next=a1;
					temp=temp->next;
				}
		}
		return top_level;
	}
	
}


void print_parse(AST_Node* print_root)
{
	if(print_root!=NULL && print_root->p!=NULL)
    {
    	//node_count++;
    	printf("%s -> ",print_root->p->name);
    	AST_Node* iter2=print_root->child;
    	while(iter2!=NULL )
        {
        	if(iter2->p!=NULL)
        	printf("%s ",iter2->p->name);
        	iter2=iter2->next;
        }
        printf("\n");
        AST_Node* iter=print_root->child;

        while(iter!=NULL)
        {
        	print_parse(iter);
        	iter=iter->next;
        }
    }
}

void print_AST_tree(AST_Node* start)
{
	if(start == NULL)
		return;

	if(strcmp(start->p->token,"NUM")==0)
        printf("%-9.9s\t%9d\t\t%-9.9s\t%9d\t\t%-30s\t%-9.9s\t%s\n",start->p->lexeme_current_node,start->p->line_number,start->p->token,start->p->value_int,start->p->parent->name,start->p->is_leaf_node,start->p->name);
    else if(strcmp(start->p->token,"RNUM")==0)
        printf("%-9.9s\t%9d\t\t%-9.9s\t%9lf\t\t%-30s\t%-9.9s\t%s\n",start->p->lexeme_current_node,start->p->line_number,start->p->token,start->p->value_double,start->p->parent->name,start->p->is_leaf_node,start->p->name);
    else
        printf("%-9.9s\t%9d\t\t%-9.9s\t%9.9s\t\t%-30s\t%-9.9s\t%s\n",start->p->lexeme_current_node,start->p->line_number,start->p->token,"----",start->p->parent->name,start->p->is_leaf_node,start->p->name);

    AST_Node* iter = start->child;
    while(iter != NULL)
    {
    	print_AST_tree(iter);
    	iter = iter->next;
    }
}

void print_AST()
{
	printf("Preorder traversal of the Abstract Syntax Tree (AST): \n");
	print_AST_tree(astroot);
}

void count_AST(AST_Node* print_root)
{
	if(print_root!=NULL && print_root->p!=NULL)
    {
    	node_count++;
    	//printf("%s -> ",print_root->p->name);
    	AST_Node* iter2=print_root->child;
    	while(iter2!=NULL )
        {
        	if(iter2->p!=NULL)
        	//printf("%s ",iter2->p->name);
        	iter2=iter2->next;
        }
        //printf("\n");
        AST_Node* iter=print_root->child;

        while(iter!=NULL)
        {
        	//count_AST(iter);
        	iter=iter->next;
        }
    }
}

void ASTmain(FILE* fp,FILE* outfp)
{
	// FILE *fp = fopen("testcase.txt","r");
	// FILE *outfp=fopen("parseoutput.txt","w");
	parse_main(fp,outfp);
	// fclose(fp);
	// fclose(outfp);
	if(errors==0)
	{
	// printf("going in generate\n");
		astroot=AST_generate(root->children[0]);
		// printf("coming out generate\n");
		//print_parse(astroot);
		count_AST(astroot);
		//printf("%d\n",node_count);
		//print_parse_tree(root->children[0],outfp);
	}
}
