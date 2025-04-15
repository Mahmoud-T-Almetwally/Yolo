# Makefile for Compiler Project ( Builds Compiler Executable & Syntax Analyzer Library )

CC = gcc
# Common flags: Enable warnings, debug info, Position Independent Code, Include dir
CFLAGS = -Wall -Wno-unused-function -g -fPIC -I.
# Linker flags for the main compiler executable ONLY
COMPILER_LDFLAGS = -lm -lfl # Link math library for pow(), flex library if needed by main

# Bison/Flex executable names might vary
BISON = bison
FLEX = flex

# --- Source Files ---
# Compiler specific sources
COMPILER_ONLY_SRCS = main.c semantic.c runtime.c 
# Sources shared by both compiler and library
SHARED_SRCS = ast.c symtab.c utils.c globals.c parser.tab.c lex.yy.c

# --- Object Files ---
COMPILER_ONLY_OBJS = $(COMPILER_ONLY_SRCS:.c=.o)
SHARED_OBJS = $(SHARED_SRCS:.c=.o)

# All object files for the compiler executable
COMPILER_ALL_OBJS = $(COMPILER_ONLY_OBJS) $(SHARED_OBJS)
# All object files for the shared library
LIB_OBJS = $(SHARED_OBJS)

# --- Generated Files ---
PARSER_C = parser.tab.c
PARSER_H = parser.tab.h
LEXER_C = lex.yy.c
LEXER_H = lex.yy.h # We will generate this header now

# --- Header Files (Dependencies) ---
# Manually created headers (add others like runtime.h if needed by shared components)
MANUAL_HEADERS = ast.h symtab.h types.h utils.h globals.h semantic.h runtime.h
# All potentially relevant headers for dependency checking
ALL_HEADERS = $(MANUAL_HEADERS) $(PARSER_H) $(LEXER_H)

# --- Target Names ---
COMPILER_TARGET = Yolo

# Detect OS for shared library naming and flags
OS := $(shell uname -s)
ifeq ($(OS),Darwin)
    SHARED_LIB_TARGET = Yolo.dylib
    SHARED_LIB_LDFLAGS = -dynamiclib
else ifeq ($(OS),Linux)
    SHARED_LIB_TARGET = Yolo.so
    SHARED_LIB_LDFLAGS = -shared
else # Default/Windows MinGW? (Untested - might need adjustments)
    SHARED_LIB_TARGET = Yolo.dll
    SHARED_LIB_LDFLAGS = -shared
endif

# --- Rules ---

# Default target: build both the compiler and the shared library
all: $(COMPILER_TARGET) $(SHARED_LIB_TARGET)

# Rule to link the compiler executable
$(COMPILER_TARGET): $(COMPILER_ALL_OBJS)
	@echo "Linking Compiler Executable: $(COMPILER_TARGET)"
	$(CC) $(CFLAGS) $^ -o $@ $(COMPILER_LDFLAGS)

# Rule to link the shared library
$(SHARED_LIB_TARGET): $(LIB_OBJS)
	@echo "Linking Shared Library: $(SHARED_LIB_TARGET)"
	# Note: No COMPILER_LDFLAGS here unless libm/libfl needed by interface/parser actions
	$(CC) $(CFLAGS) $(SHARED_LIB_LDFLAGS) $^ -o $@

# Rule to compile C source files into object files
# Depends on all headers for simplicity (recompiles .c if any header changes)
# Alternatively, create more specific rules if rebuild times are an issue.
%.o: %.c $(ALL_HEADERS) Makefile
	@echo "Compiling $< -> $@"
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to run Bison: generates parser.tab.c and parser.tab.h
# Dependencies include necessary headers for parser actions
$(PARSER_C) $(PARSER_H): parser.y ast.h symtab.h types.h utils.h runtime.h globals.h
	@echo "Running Bison on parser.y"
	$(BISON) -d -v -o $(PARSER_C) parser.y # -d creates .h, -v creates .output, -o specifies output C file

# Rule to run Flex: generates lex.yy.c AND lex.yy.h
# Depends on parser.tab.h (token definitions)
$(LEXER_C) $(LEXER_H): lexer.lex $(PARSER_H)
	@echo "Running Flex on lexer.lex"
	$(FLEX) --header-file=$(LEXER_H) -o $(LEXER_C) lexer.lex

# Clean up generated files, object files, and targets
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(COMPILER_TARGET) $(SHARED_LIB_TARGET) \
	      $(COMPILER_ALL_OBJS) \
	      $(PARSER_C) $(PARSER_H) $(LEXER_C) $(LEXER_H) \
	      lex.backup parser.output *.o *~ core.*
	@echo "Clean complete."


.PHONY: all clean