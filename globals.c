// globals.c
#include "globals.h" // Include the header

// Definitions (actual storage for the variables)
AstNode* ast_root = NULL;
int yylineno = 1; // Initialize line number
bool debug = false; // Initialize debug flag
// ... other definitions