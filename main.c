// main.c (Revised Again)
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
    int final_status = 0; // 0 for success, 1 for failure

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
    ast_root = NULL; // Initialize AST root

    // Run parser
    printf("--- Parsing ---\n");
    int parse_result = yyparse(scanner);

    // Cleanup input file early
    if (input) fclose(input);

    bool semantic_ok = false;
    bool ast_structurally_valid = false; // Flag if AST seems okay structurally post-parse

    // --- Proceed only if parsing reported success ---
    if (parse_result == 0) {
        // Check the AST root immediately after successful parse
        if (ast_root == NULL) {
            compiler_warning(0, "Parsing succeeded but AST root is NULL (empty input?).");
            semantic_ok = true; // Treat empty input as semantically okay
            ast_structurally_valid = true; // NULL is valid for empty
        } else if (ast_root->node_type == NODE_PROGRAM) {
            // AST root looks like a program node, proceed with analysis
            ast_structurally_valid = true; // Structure seems okay initially
            printf("--- Semantic Analysis ---\n");
            semantic_ok = analyze_semantics(ast_root); // analyze_semantics might still find errors

            if (semantic_ok) {
                printf("--- Interpretation ---\n");
                Environment *global_env = create_environment(NULL, "global");
                if (global_env) {
                    RuntimeValue result = interpret(ast_root, global_env);
                    if (result.is_error) {
                        // Use a more specific error message if available from RuntimeValue
                        fprintf(stderr, "Runtime Error occurred during interpretation.\n");
                        final_status = 1;
                    }
                    free_runtime_value_contents(&result);
                    free_environment(global_env);
                } else {
                    fprintf(stderr, "Error: Failed to create global runtime environment.\n");
                    semantic_ok = false; // Mark as failure
                    final_status = 1;
                }
            } else {
                 fprintf(stderr, "Semantic analysis failed. Skipping interpretation.\n");
                 // If semantics failed but structure was okay, AST might still be valid for freeing
                 final_status = 1;
            }
        } else {
            // Parser returned 0, but ast_root is not NULL and not NODE_PROGRAM. THIS IS THE PROBLEM CASE.
            compiler_error(ast_root->lineno, // Use line if possible, might be garbage
                           "Internal Error: Parser succeeded but AST root is not NODE_PROGRAM (type=%d). AST is corrupt.",
                           ast_root->node_type);
            ast_structurally_valid = false; // Mark AST as definitely invalid
            semantic_ok = false;
            final_status = 1;
        }

        // Free AST only if parsing succeeded AND the AST root seemed structurally valid after parse.
        if (ast_structurally_valid && ast_root != NULL) {
             printf("--- Freeing AST ---\n");
             free_ast(ast_root);
             ast_root = NULL; // Avoid dangling pointer
        } else if (ast_root != NULL) { // ast_root exists but was deemed invalid
             compiler_warning(0, "AST root node deemed invalid after parse; skipping free_ast to prevent crash.");
             // Do NOT free potentially invalid ast_root
        }

    } else {
        fprintf(stderr, "Parsing failed. Skipping semantic analysis and interpretation.\n");
        final_status = 1;
        // Do NOT free ast_root here, it's likely invalid or NULL
    }

    // --- Always perform these cleanups ---
    printf("--- Cleaning up Symbol Table & Lexer ---\n");
    cleanup_scope_management();
    yylex_destroy(scanner);

    printf("--- Finished ---\n");
    return final_status;
}