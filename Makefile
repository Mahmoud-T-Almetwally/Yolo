# Makefile for Compiler Project (Updated)

CC = gcc
CFLAGS = -Wall -Wno-unused-function -g  # Enable warnings, debug info
LDFLAGS = -lm -lfl # Link math library for pow()

# Bison/Flex executable names might vary (e.g., bison, flex)
BISON = bison
FLEX = flex

# Source files
SRCS = main.c ast.c symtab.c semantic.c runtime.c utils.c parser.tab.c lex.yy.c # codegen.c
# Object files derived from C sources
OBJS = $(SRCS:.c=.o)

# Generated files from Bison/Flex
PARSER_C = parser.tab.c
PARSER_H = parser.tab.h
LEXER_C = lex.yy.c

# Target executable name
TARGET = Yolo

# Default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Rule to compile C source files (including generated ones) into object files
# Added runtime.h dependency
%.o: %.c ast.h symtab.h types.h utils.h semantic.h runtime.h $(PARSER_H) # Dependencies
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to run Bison: generates parser.tab.c and parser.tab.h from parser.y
$(PARSER_C) $(PARSER_H): parser.y ast.h symtab.h types.h utils.h runtime.h
	$(BISON) -d -v parser.y # -d creates .h, -v creates .output verbose report

# Rule to run Flex: generates lex.yy.c from lexer.lex
# Depends on parser.tab.h because lexer needs token definitions
$(LEXER_C): lexer.lex $(PARSER_H)
	$(FLEX) -o lex.yy.c lexer.lex

# Clean up generated files and object files
clean:
	rm -f $(TARGET) $(OBJS) $(PARSER_C) $(PARSER_H) $(LEXER_C) lex.backup parser.output
	rm -f *~ core.* # General cleanup

# Phony targets
.PHONY: all clean