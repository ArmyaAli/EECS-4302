# Makefile
# 2024-09-22
# Lab 1 Makefile modified group 10s directory structure

DIST=bin
SRC=src
OBJ_FILES=$(addprefix $(DIST)/, \
	 main.o \
	 scanner.o \
	 helper.o \
	 constants.o \
	 parser.o \
	 messages.o \
	 expr.o \
	 print.o \
	 decl.o \
	 library.o \
	 type.o \
	 param_list.o \
	 stmt.o)

INC=include

$(DIST)/bminor: $(OBJ_FILES)
	gcc -I$(INC) $^ -o $(DIST)/bminor

$(DIST)/%.o: $(SRC)/%.c $(INC)/*.h
	gcc -I$(INC) -Wall -g -c $< -o $@

$(SRC)/scanner.c: $(INC)/token.h $(SRC)/scanner.flex
	flex -o $(SRC)/scanner.c $(SRC)/scanner.flex

$(SRC)/parser.c $(INC)/token.h: $(SRC)/parser.bison
	bison --defines=$(INC)/token.h --output=$(SRC)/parser.c -v $(SRC)/parser.bison 

clean:
	rm -f $(SRC)/scanner.c $(DIST)/*.o bminor
