#BATCH 81
#Kunal Todi 2013B3A7480P
#Anish Shah 2013B1A7856P
#Makefile for stage1exe

CC = gcc

.PHONY: all
all: compiler

compiler:	driver.o lexer.o parser.o ast.o symbolTable.o TypeChecker.o Codegen.o
	$(CC) driver.o lexer.o parser.o ast.o symbolTable.o TypeChecker.o Codegen.o -o $@

driver.o: Codegen.h TypeChecker.h SymbolTable.h ast.h
lexer.o: lexerDef.h
parser.o: parserDef.h lexer.h
ast.o: astdef.h ast.h
symbolTable.o:SymbolTableDef.h SymbolTable.h
TypeChecker.o:TypeCheckerDef.h TypeChecker.h
Codegen.o: CodegenDef.h Codegen.h

.PHONY: install
install:
	rm -rf driver.o lexer.o parser.o ast.o symbolTable.o TypeChecker.o Codegen.o

.PHONY: clean
clean:
	rm -rf driver.o lexer.o parser.o ast.o symbolTable.o TypeChecker.o Codegen.o compiler