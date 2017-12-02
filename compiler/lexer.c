//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexerDef.h"

struct Keywordhash hashArray[HASH_SIZE];

int HashCode(char id_check[50])
{
	int temp_key=0;
	int i=0;
	while(id_check[i]!='\0')
	{
		temp_key+=(int)id_check[i];
		i++;
	}
	return temp_key%HASH_SIZE;

}

struct KeywordItem* createNode(char word[50],char class[50])
{
	struct KeywordItem* newnode = (struct KeywordItem*) malloc(sizeof(struct KeywordItem));
	strcpy(newnode->keyword,word);
	strcpy(newnode->TOKEN_CLASS,class);
	newnode->next=NULL;
	return newnode;
}

void insert(char word[50],char class[50]) {

   //get the hash 
   int hashIndex = HashCode(word);

   struct KeywordItem* newItem=createNode(word,class);
   //move in array until an empty or deleted cell
   if(!hashArray[hashIndex].head)
   {
   		hashArray[hashIndex].head=newItem;
   		hashArray[hashIndex].count=1;
   		return;
   }
   /* adding new node to the list */
    newItem->next = (hashArray[hashIndex].head);
    /*
     * update the head of the list and no of
     * nodes in the current bucket
     */
    hashArray[hashIndex].head = newItem;
    hashArray[hashIndex].count++;
    return;
}

struct KeywordItem *search(char word[50]) {
   //get the hash 
   int hashIndex = HashCode(word); 

   struct KeywordItem* myItem;
   myItem=hashArray[hashIndex].head;
   if(!myItem)
   {
   		return NULL;
   }

   while(myItem!=NULL)
   {
   		if(strcmp(myItem->keyword,word)==0)
   			return myItem;
   		myItem=myItem->next;
   }

   return NULL;        
}

void initialize_hash()
{
	char word[50],class[50];
	strcpy(word,"integer");
	strcpy(class,"INTEGER");
	insert(word,class);
	strcpy(word,"real");
	strcpy(class,"REAL");
	insert(word,class);
	strcpy(word,"boolean");
	strcpy(class,"BOOLEAN");
	insert(word,class);
	strcpy(word,"of");
	strcpy(class,"OF");
	insert(word,class);
	strcpy(word,"array");
	strcpy(class,"ARRAY");
	insert(word,class);
	strcpy(word,"start");
	strcpy(class,"START");
	insert(word,class);
	strcpy(word,"end");
	strcpy(class,"END");
	insert(word,class);
	strcpy(word,"declare");
	strcpy(class,"DECLARE");
	insert(word,class);
	strcpy(word,"module");
	strcpy(class,"MODULE");
	insert(word,class);
	strcpy(word,"driver");
	strcpy(class,"DRIVER");
	insert(word,class);
	strcpy(word,"program");
	strcpy(class,"PROGRAM");
	insert(word,class);
	strcpy(word,"get_value");
	strcpy(class,"GET_VALUE");
	insert(word,class);
	strcpy(word,"print");
	strcpy(class,"PRINT");
	insert(word,class);
	strcpy(word,"use");
	strcpy(class,"USE");
	insert(word,class);
	strcpy(word,"with");
	strcpy(class,"WITH");
	insert(word,class);
	strcpy(word,"parameters");
	strcpy(class,"PARAMETERS");
	insert(word,class);
	strcpy(word,"true");
	strcpy(class,"TRUE");
	insert(word,class);
	strcpy(word,"false");
	strcpy(class,"FALSE");
	insert(word,class);
	strcpy(word,"takes");
	strcpy(class,"TAKES");
	insert(word,class);
	strcpy(word,"input");
	strcpy(class,"INPUT");
	insert(word,class);
	strcpy(word,"returns");
	strcpy(class,"RETURNS");
	insert(word,class);
	strcpy(word,"AND");
	strcpy(class,"AND");
	insert(word,class);
	strcpy(word,"OR");
	strcpy(class,"OR");
	insert(word,class);
	strcpy(word,"for");
	strcpy(class,"FOR");
	insert(word,class);
	strcpy(word,"in");
	strcpy(class,"IN");
	insert(word,class);
	strcpy(word,"switch");
	strcpy(class,"SWITCH");
	insert(word,class);
	strcpy(word,"case");
	strcpy(class,"CASE");
	insert(word,class);
	strcpy(word,"break");
	strcpy(class,"BREAK");
	insert(word,class);
	strcpy(word,"default");
	strcpy(class,"DEFAULT");
	insert(word,class);
	strcpy(word,"while");
	strcpy(class,"WHILE");
	insert(word,class);
}

int curr_line=1;
int state=1;//State-1 is starting state
int errors=0;

//Array to store all tokens sequentially
//TOKEN* token_arr[MAX_NO_OF_TOKENS];
int no_of_tokens=0;

//function to read input
int read_input(FILE *fp)
{
	if(fp == NULL)
	{
		printf("File does not exist!");
		return -1;
	}
	int	buffsize1=fread(buffer,sizeof(char),(size_t)MAX_BUFFER,fp);
		return buffsize1;
}

//function to iniitialize new token
TOKEN* initialize_token()
{
	TOKEN* new_tok=(TOKEN*) malloc(sizeof(TOKEN));
	new_tok->line_no=-1;
	new_tok->token_complete=0;
	memset(new_tok->lexeme,'\0',sizeof(new_tok->lexeme));
	memset(new_tok->TOKEN_TYPE,'\0',sizeof(new_tok->TOKEN_TYPE));
	return new_tok;
}


//function to change corresponding states
int set_state(TOKEN* new_tok,char current,int position_i)
{
	switch(state)
	{
		case 1://start state
		{
			switch(current)
			{
				case EOF:
				{
					state=1;
					return 0;
				}
				case ' ':
				{
					state=1;
					return 0;
				}
				case '\n':
				{
					state=1;
					return 0;
				}
				case '\t':
				{
					state=1;
					return 2;
				}
				//Single symbols
				case '+':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"PLUS");
					state=1;
					return 1;
				}
				break;
				case '-':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"MINUS");
					state=1;
					return 1;
				}
				break;
				case '/':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"DIV");
					state=1;
					return 1;
				}
				break;
				case ';':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"SEMICOL");
					state=1;
					return 1;
				}
				break;
				case ',':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"COMMA");
					state=1;
					return 1;
				}
				break;
				case '[':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"SQBO");
					state=1;
					return 1;
				}
				break;
				case ']':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"SQBC");
					state=1;
					return 1;
				}
				break;
				case '(':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"BO");
					state=1;
					return 1;
				}
				break;
				case ')':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"BC");
					state=1;
					return 1;
				}
				break;

				//Symbols with 2 characters, if not found report error.
				case '=':
				{
					new_tok->lexeme[position_i]=current;
					state=2;
					return 0;
				}
				break;
				case '!':
				{
					new_tok->lexeme[position_i]=current;
					state=3;
					return 0;
				}
				break;

				//Symbols with 1 or 2 characters, multiple meanings.
				case '*':
				{
					new_tok->lexeme[position_i]=current;
					state=4;
					return 0;
				}
				break;
				case '<':
				{
					new_tok->lexeme[position_i]=current;
					state=5;
					return 0;
				}
				break;
				case '>':
				{
					new_tok->lexeme[position_i]=current;
					state=6;
					return 0;
				}
				break;
				case ':':
				{
					new_tok->lexeme[position_i]=current;
					state=7;
					return 0;
				}
				break;

				//Symbol with 2 characters, if only 1 character found reports error.
				case '.':
				{
					new_tok->lexeme[position_i]=current;
					state=8;
					return 0;
				}

				//TO READ IDENTIFIER OR KEYWORD
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'g':
				case 'h':
				case 'i':
				case 'j':
				case 'k':
				case 'l':
				case 'm':
				case 'n':
				case 'o':
				case 'p':
				case 'q':
				case 'r':
				case 's':
				case 't':
				case 'u':
				case 'v':
				case 'w':
				case 'x':
				case 'y':
				case 'z':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				case 'G':
				case 'H':
				case 'I':
				case 'J':
				case 'K':
				case 'L':
				case 'M':
				case 'N':
				case 'O':
				case 'P':
				case 'Q':
				case 'R':
				case 'S':
				case 'T':
				case 'U':
				case 'V':
				case 'W':
				case 'X':
				case 'Y':
				case 'Z':
				{
					new_tok->lexeme[position_i]=current;
					state=9;
					return 0;
				}
				break;

				//TO READ REAL NUMBER OR INTEGER
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					new_tok->lexeme[position_i]=current;
					state=10;
					return 0;
				}
				case '0':
				{
					new_tok->lexeme[position_i]=current;
					state=11;
					return 0;
				}

				// If found any non-permitted character.
				default:
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"ERR");
					char error_string[100];
					sprintf(error_string,"ERROR_2: Unknown Symbol %s at line %d\n",new_tok->lexeme,new_tok->line_no);
					strcpy(error_name[errors],error_string);
					errors++;
					state=1;
					return 1;
				}	
			}
		}
		break;
		//For EQUAL_TO Operator
		case 2:
		{
			switch(current)
			{
				case '=':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"EQ");
					state=1;
					return 1;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"ERR");
					char error_string[100];
					sprintf(error_string,"ERROR_3: Unknown pattern %s in line %d\n",new_tok->lexeme,new_tok->line_no);
					strcpy(error_name[errors],error_string);
					errors++;
					state=-1;
					return 1;
				}
			}
		}
		break;

		//For NOT_EQUAL_TO Operator
		case 3:
		{
			switch(current)
			{
				case '=':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"NE");
					state=1;
					return 1;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"ERR");
					char error_string[100];
					sprintf(error_string,"ERROR_3: Unknown pattern %s in line %d\n",new_tok->lexeme,new_tok->line_no);
					strcpy(error_name[errors],error_string);
					errors++;
					state=-1;
					return 1;
				}
			}
		}
		break;

		//FOR MULTIPLIER and COMMENT
		case 4:
		{
			switch(current)
			{
				case '*':
				{
					new_tok->lexeme[position_i-1]='\0';
					state=17;
					return 0;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"MUL");
					state=-1;
					return 1;
				}
			}
		}
		break;

		//For LESS THAN operator and its variants
		case 5:
		{
			switch(current)
			{
				case '=':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"LE");
					state=1;
					return 1;
				}
				break;
				case '<':
				{
					new_tok->lexeme[position_i]=current;
					state=19;
					return 0;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"LT");
					state=-1;
					return 1;
				}
			}	
		}
		break;

		//For GREATER THAN operator and its variants
		case 6:
		{
			switch(current)
			{
				case '=':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"GE");
					state=1;
					return 1;
				}
				break;
				case '>':
				{
					new_tok->lexeme[position_i]=current;
					state=20;
					return 0;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"GT");
					state=-1;
					return 1;
				}
			}	
		}
		break;

		//FOR ASSIGN operator
		case 7:
		{
			switch(current)
			{
				case '=':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"ASSIGNOP");
					state=1;
					return 1;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"COLON");
					state=-1;
					return 1;
				}
			}
		}
		break;

		//For RangeOp operator
		case 8:
		{
			switch(current)
			{
				case '.':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"RANGEOP");
					state=1;
					return 1;
				}
				break;
				default:
				{
					char error_string[100];
					strcpy(new_tok->TOKEN_TYPE,"ERR");
					sprintf(error_string,"ERROR_3: Unknown pattern %s in line %d\n",new_tok->lexeme,new_tok->line_no);
					strcpy(error_name[errors],error_string);
					errors++;
					state=-1;
					return 1;
				}
			}
		}
		break;

		//Case to consider Identifiers and keywords.
		case 9:
		{
			switch(current)
			{
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'g':
				case 'h':
				case 'i':
				case 'j':
				case 'k':
				case 'l':
				case 'm':
				case 'n':
				case 'o':
				case 'p':
				case 'q':
				case 'r':
				case 's':
				case 't':
				case 'u':
				case 'v':
				case 'w':
				case 'x':
				case 'y':
				case 'z':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				case 'G':
				case 'H':
				case 'I':
				case 'J':
				case 'K':
				case 'L':
				case 'M':
				case 'N':
				case 'O':
				case 'P':
				case 'Q':
				case 'R':
				case 'S':
				case 'T':
				case 'U':
				case 'V':
				case 'W':
				case 'X':
				case 'Y':
				case 'Z':
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case '_':
				{
					new_tok->lexeme[position_i]=current;
					state=9;
					return 0;
				}
				break;
				default:
				{
					//check Identifier or Keyword from HASH TABLE
					struct KeywordItem * mystring;
					mystring=search(new_tok->lexeme);
					if(mystring==NULL)
					{
						if(strlen(new_tok->lexeme)<=8)
							strcpy(new_tok->TOKEN_TYPE,"ID");
						else
						{
							strcpy(new_tok->TOKEN_TYPE,"ERR");
							char error_string[100];
							printf("ERROR\n");
							sprintf(error_string,"ERROR_1 : Identifier at line %d is longer than the prescribed length\n",new_tok->line_no);
							strcpy(error_name[errors],error_string);
							errors++;
						}
					}
					else
						strcpy(new_tok->TOKEN_TYPE,mystring->TOKEN_CLASS);
					state=-1;
					return 1;
				}
			}
		}
		break;

		//Case to consider both types of numbers.
		case 10:
		{
			switch(current)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					new_tok->lexeme[position_i]=current;
					state=10;
					return 0;
				}
				break;
				case '.':
				{
					new_tok->lexeme[position_i]=current;
					state=12;
					return 0;
				}
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"NUM");
					state=-1;
					return 1;
				}
			}
		}
		break;
		//For considering range and Real number
		case 11:
		{
			switch(current)
			{
				case '.':
				{
					new_tok->lexeme[position_i]=current;
					state=12;
					return 0;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"NUM");
					state=-1;
					return 1;
				}
			}
		}
		break;
		case 12:
		{
			switch(current)
			{
				case '.':
				{
					//Confirmed that it is range divide RANGEOP and NUM
					new_tok->lexeme[position_i-1]='\0';
					strcpy(new_tok->TOKEN_TYPE,"NUM");
					state=-2;
					return 1;
				}
				break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					new_tok->lexeme[position_i]=current;
					state=13;
					return 0;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"ERR");
					char error_string[100];
					printf("ERROR\n");
					sprintf(error_string,"ERROR_3: Unknown pattern %s in line %d\n",new_tok->lexeme,new_tok->line_no);
					strcpy(error_name[errors],error_string);
					errors++;
					state=-1;
					return 1;
				}
			}
		}
		break;
		case 13:
		{
			switch(current)
			{
				case 'e':
				case 'E':
				{
					new_tok->lexeme[position_i]=current;
					state=14;
					return 0;
				}
				break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					new_tok->lexeme[position_i]=current;
					state=13;
					return 0;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"RNUM");
					state=-1;
					return 1;
				}
			}
		}
		break;
		case 14:
		{
			switch(current)
			{
				case '-':
				case '+':
				{
					new_tok->lexeme[position_i]=current;
					state=15;
					return 0;
				}
				break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					new_tok->lexeme[position_i]=current;
					state=16;
					return 0;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"ERR");
					char error_string[100];
					printf("ERROR\n");
					sprintf(error_string,"ERROR_3: Unknown pattern %s in line %d\n",new_tok->lexeme,new_tok->line_no);
					strcpy(error_name[errors],error_string);
					errors++;
					state=-1;
					return 1;
				}
			}
		}
		break;
		case 15:
		{
			switch(current)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					new_tok->lexeme[position_i]=current;
					state=16;
					return 0;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"ERR");
					char error_string[100];
					sprintf(error_string,"ERROR_3: Unknown pattern %s in line %d\n",new_tok->lexeme,new_tok->line_no);
					strcpy(error_name[errors],error_string);
					errors++;
					state=-1;
					return 1;
				}
			}
		}
		break;
		case 16:
		{
			switch(current)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					new_tok->lexeme[position_i]=current;
					state=16;
					return 0;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"RNUM");
					state=-1;
					return 1;
				}
			}
		}
		break;
		case 17:
		{
			switch(current)
			{
				case '*':
				{
					state=18;
					return 0;
				}
				break;
				default:
				{
					state=17;
					return 0;
				}
			}
		}
		break;
		case 18:
		{
			switch(current)
			{
				case '*':
				{
					state=-3;
					return 0;
				}
				break;
				default:
				{
					state=17;
					return 0;
				}
			}
		}
		break;
		case 19:
		{
			switch(current)
			{
				case '<':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"DRIVERDEF");
					state=1;
					return 1;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"DEF");
					state=-1;
					return 1;
				}
			}	
		}
		case 20:
		{
			switch(current)
			{
				case '>':
				{
					new_tok->lexeme[position_i]=current;
					strcpy(new_tok->TOKEN_TYPE,"DRIVERENDDEF");
					state=1;
					return 1;
				}
				break;
				default:
				{
					strcpy(new_tok->TOKEN_TYPE,"ENDDEF");
					state=-1;
					return 1;
				}
			}	
		}			
		default:
			return 0;
	}
}

//function to derive each character and tokenize through it
void getCharactersandtokenize(FILE* fp)
{
	int pointer=0;
	
	//read each character from buffer
	if(fp==NULL)
	{
		buffsize==read_input(fp);
		return;
	}
	prev1='\0';
	prev2='\0';
	curr='\0';
	
	//iniitialize new token
	TOKEN* new_tok=initialize_token();
	buffsize=read_input(fp);
	int check=0;
	int token_pos=0;

	//This loop updates buffer when it has been fully read
	while(buffsize==MAX_BUFFER)
	{
		int buffer_pointer=0;
		for(buffer_pointer=0;buffer_pointer<buffsize;buffer_pointer++)
		{
			if(state==-3)
			{
				new_tok->line_no=-1;
				token_pos=0;
				state=1;
			}
			//Situation when there is a RANGEOP just after a NUMBER
			if(state==-2)
			{
				new_tok=initialize_token();
				new_tok->lexeme[0]=prev2;
				new_tok->lexeme[1]=curr;
				strcpy(new_tok->TOKEN_TYPE,"RANGEOP");
				new_tok->token_complete=1;
				new_tok->line_no=curr_line;
				state=1;

				//Fill this token in token array.

				if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
				{
					token_arr[no_of_tokens]=new_tok;
					no_of_tokens++;
				}
				

			}

			//STATE -1 represents when one character is read but it has not been consumed for corresponding token.
			if(state==-1)
			{
				state=1;
				new_tok=initialize_token();
				check=0;
				token_pos=0;

				//Avoid space, new line and tab characters while tokenizing.
				if(curr!=' ' && curr!='\n' && curr!='\t')
				{
					//store line for corresponding token
					if(new_tok->line_no==-1)
					{
						new_tok->line_no=curr_line;
					}

					//change state corresponding to character and looking at DFA.
					check=set_state(new_tok,curr,token_pos);
					token_pos++;

					//This ensures completion of token and its storage.
					if(check==1)
					{
						if(buffer_pointer!=buffsize || strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
						{
							new_tok->token_complete=1;

							//Fill this token in token array.
							if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
							{
								token_arr[no_of_tokens]=new_tok;
								no_of_tokens++;
							}
						}
						
					}
				}
				else
				{
					//This else is to specifically read unusable characters.
					check=set_state(new_tok,curr,token_pos);
					if(check==1)
					{
						if(buffer_pointer!=buffsize || strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
						{
							new_tok->token_complete=1;

							//Fill this token in token array.
							if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
							{
								token_arr[no_of_tokens]=new_tok;
								no_of_tokens++;
							}
						}
						
					}
				}	

			}
			
			prev1=prev2;
			prev2=curr;
			curr=buffer[buffer_pointer];

			//update current line being read
			if(curr=='\n')
				curr_line++;

			//initialize new token for storing new array:
			//state-1 indicates it has come back to original state
			//token_complete inidicates tokenization completed for corresponding token
			if(state==1 && new_tok->token_complete==1)
			{
				new_tok=initialize_token();
				check=0;
				token_pos=0;
			}

			//Avoid space, new line and tab characters while tokenizing.
			if(curr!=' ' && curr!='\n' && curr!='\t')
			{
				//store line for corresponding token
				if(new_tok->line_no==-1)
				{
					new_tok->line_no=curr_line;
				}

				//change state corresponding to character and looking at DFA.
				check=set_state(new_tok,curr,token_pos);
				token_pos++;

				//This ensures completion of token and its storage.
				if(check==1)
				{
					if(buffer_pointer!=buffsize || strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
					{
						new_tok->token_complete=1;
						//Fill this token in token array.
						if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
							{
								token_arr[no_of_tokens]=new_tok;
								no_of_tokens++;
							}
					}
					
				}
			}
			else
			{
				//This else is to specifically read unusable characters.
				check=set_state(new_tok,curr,token_pos);

				//This ensures completion of token and its storage.
				if(check==1)
				{
					if(buffer_pointer!=buffsize || strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
					{
						new_tok->token_complete=1;
						//Fill this token in token array.
						if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
							{
								token_arr[no_of_tokens]=new_tok;
								no_of_tokens++;
							}
					}
				}
			}	
		}

		buffsize=read_input(fp);
	}

	//Last iteration left when buffer read is less than maximum buffer size
	int buffer_pointer=0;
	for(buffer_pointer=0;buffer_pointer<buffsize+1;buffer_pointer++)
	{
		if(state==-3)
			{
				new_tok->line_no=-1;
				token_pos=0;
				state=1;
			}
		//Situation when there is a RANGEOP just after a NUMBER
		if(state==-2)
			{
				new_tok=initialize_token();
				new_tok->lexeme[0]=prev2;
				new_tok->lexeme[1]=curr;
				strcpy(new_tok->TOKEN_TYPE,"RANGEOP");
				new_tok->token_complete=1;
				new_tok->line_no=curr_line;
				state=1;

				//Fill this token in token array.
				if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
				{
					token_arr[no_of_tokens]=new_tok;
					no_of_tokens++;
				}
				

			}

		//STATE -1 represents when one character is read but it has not been consumed for corresponding token.
		if(state==-1)
		{
			state=1;
			new_tok=initialize_token();
			check=0;
			token_pos=0;
			if(curr!=' ' && curr!='\n' && curr!='\t')
			{
				if(new_tok->line_no==-1)
				{
					new_tok->line_no=curr_line;
				}
				check=set_state(new_tok,curr,token_pos);
				token_pos++;
				if(check==1)
				{
					if(buffer_pointer!=buffsize || strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
					{
						new_tok->token_complete=1;

						//Fill this token in token array.
						if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
						{
							token_arr[no_of_tokens]=new_tok;
							no_of_tokens++;
						}
					}
					
				}
			}
			else
			{
				//This else is to specifically read unusable characters.
				check=set_state(new_tok,curr,token_pos);
				if(check==1)
				{
					if(buffer_pointer!=buffsize || strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
					{
						new_tok->token_complete=1;

						//Fill this token in token array.
						if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
						{
							token_arr[no_of_tokens]=new_tok;
							no_of_tokens++;
						}
					}
					
				}
			}	

		}

		//reading new character
		prev1=prev2;
		prev2=curr;
		if(buffer_pointer==buffsize)
			curr=EOF;
		else
		curr=buffer[buffer_pointer];

		//update current line being read
		if(curr=='\n')
			curr_line++;
		if(state==1 && new_tok->token_complete==1)
		{
			new_tok=initialize_token();
			check=0;
			token_pos=0;
		}
		if(curr!=' ' && curr!='\n' && curr!='\t')
		{
			if(new_tok->line_no==-1)
			{
				new_tok->line_no=curr_line;
			}
			check=set_state(new_tok,curr,token_pos);
			token_pos++;
			if(check==1)
			{
				if(buffer_pointer!=buffsize || strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
				{
					new_tok->token_complete=1;

					//Fill this token in token array.
					if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
						{
							token_arr[no_of_tokens]=new_tok;
							no_of_tokens++;
						}
				}
				
			}
		}
		else
		{
			//This else is to specifically read unusable characters.
			check=set_state(new_tok,curr,token_pos);
			if(check==1)
			{
				if(buffer_pointer!=buffsize || strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
				{
					new_tok->token_complete=1;
					//Fill this token in token array.
					if(strcmp(new_tok->TOKEN_TYPE,"ERR")!=0)
						{
							token_arr[no_of_tokens]=new_tok;
							no_of_tokens++;
						}
				}
				
			}
		}
	}
	
}

void remove_comments(FILE* fp,FILE* new_fp,int file_size)
{
	int flag=0;
	char prev,current;
	prev='\0';
	current='\0';
	int i=0;
	for(i=0;i<file_size;i++)
	{
		current=fgetc(fp);
		if(flag==0 && current!='*')
		{
			//printf("%c",current);
			fputc(current,new_fp);
		}
		else if(flag==0 && current=='*')
		{
			flag=1;
		}
		else if(flag==1 && current!='*')
		{
			fputc(prev,new_fp);
			fputc(current,new_fp);
			flag=0;
		}
		else if(flag==1 && current=='*')
		{
			flag=2;
		}
		else if(flag==2 && current !='*')
		{
			flag=2;
		}
		else if(flag==2 && current=='*')
		{
			flag=3;
		}
		else if(flag==3 && current !='*')
		{
			flag=2;
		}
		else if(flag==3 && current=='*')
		{
			flag=0;
		}
		prev=current;
	}
	rewind(fp);
}
void lexical_analyser(FILE* fp)
{
	initialize_hash();
	fpos_t position;
	FILE *fp1=fopen("cleancode.txt","w");
	//printf("hi\n");
	fseek(fp,0,SEEK_END);
	int file_size=ftell(fp);
	rewind(fp);
	remove_comments(fp,fp1,file_size);
	getCharactersandtokenize(fp);
	fclose(fp1);
	
}

