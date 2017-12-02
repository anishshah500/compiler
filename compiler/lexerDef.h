//BATCH 81
//Kunal Todi 2013B3A7480P
//Anish Shah 2013B1A7856P
//HEADER FILE
#ifndef LEXERDEF_HEADER
#define LEXERDEF_HEADER

#define MAX_BUFFER 50
#define MAX_LEXEME 50
#define MAX_NO_OF_TOKENS 1000
#define HASH_SIZE 30

struct KeywordItem {
   char keyword[50];
   char TOKEN_CLASS[50];
   struct KeywordItem* next;
};

struct Keywordhash{
    struct KeywordItem* head;
    int count;
};

//Token Information
typedef struct token{
    char lexeme[MAX_LEXEME];
    int line_no;
    char TOKEN_TYPE[30];
    int token_complete;
}TOKEN;

TOKEN* token_arr[MAX_NO_OF_TOKENS];

char error_name[200][100];


int no_of_tokens;

//defining buffer, size and global caharacter pointers
unsigned char buffer[MAX_BUFFER];
int buffsize;
char prev1,prev2,curr;
int curr_line;
int state;//State-1 is starting state
int errors;
#endif