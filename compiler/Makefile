# Makefile
# 2024-09-22
# Lab 1 Makefile modified group 10s directory structure

DIST=bin
SRC=src
INC=$(SRC)/include

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
	 stmt.o \
	 symbol.o \
	 hash_table.o \
	 resolve.o \
	 stack.o \
	 global.o \
	 pipeline.o \
	 typecheck.o \
	 scope.o \
	 codegen.o \
	 codegen_helper.o \
	 first_pass.o \
	 second_pass.o \
	 write_stream.o)


$(DIST)/bminor: $(OBJ_FILES)
	gcc -I$(INC) $^ -o $(DIST)/bminor

$(DIST)/%.o: $(SRC)/%.c $(INC)/*.h
	gcc -I$(INC) -Wall -g -c $< -o $@

$(SRC)/scanner.c: $(INC)/token.h $(SRC)/scanner.flex
	flex -o $(SRC)/scanner.c $(SRC)/scanner.flex

$(SRC)/parser.c $(INC)/token.h: $(SRC)/parser.bison
	bison --defines=$(INC)/token.h --output=$(SRC)/parser.c -v $(SRC)/parser.bison 

clean:
	rm -f $(INC)/token.h $(SRC)/scanner.c $(SRC)/parser.c $(SRC)/parser.output $(DIST)/*.o $(DIST)/bminor

# NOTE(Ali): usage: make codegen ARG1=asm.s
codegen: 
	gcc -g $(ARG1) src/library.c -o $(ARG1).out
	./$(ARG1).out