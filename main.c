// main.c
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"
#include "symtab.h"
#include "semantic.h"
#include "runtime.h"
#include "utils.h"

// Flex reentrant functions
extern int yylex_init(void **scanner);
extern int yylex_destroy(void *scanner);
extern void yyset_in(FILE *in_str, void *yyscanner);

// Parser/lexer globals
extern AstNode *ast_root;

int main(int argc, char *argv[]) {
    void* scanner;
    FILE *input = NULL;
    
    // Initialize Flex scanner
    if (yylex_init(&scanner) != 0) {
        fprintf(stderr, "Failed to initialize lexer\n");
        return 1;
    }

    // Set input source
    if (argc > 1) {
        if ((input = fopen(argv[1], "r")) == NULL) {
            perror(argv[1]);
            yylex_destroy(scanner);
            return 1;
        }
        yyset_in(input, scanner);
    } else {
        yyset_in(stdin, scanner);
    }

    // Initialize symbol tables
    init_scope_management();
    ast_root = NULL;

    // Run parser
    int parse_result = yyparse(scanner);

    // Cleanup input file
    if (input) fclose(input);

    // Semantic analysis
    bool semantic_ok = (parse_result == 0) && analyze_semantics(ast_root);

    // Interpretation
    Environment *global_env = NULL;
    if (semantic_ok && ast_root) {
        global_env = create_environment(NULL, "global");
        if (global_env) {
            RuntimeValue result = interpret(ast_root, global_env);
            free_runtime_value_contents(&result);
            free_environment(global_env);
        }
    }

    // Cleanup
    if (ast_root) free_ast(ast_root);
    cleanup_scope_management();
    yylex_destroy(scanner);

    return (parse_result || !semantic_ok) ? 1 : 0;
}