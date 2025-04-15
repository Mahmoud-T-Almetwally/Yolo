// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include "ast.h"

// Declare global variables as extern here
extern bool debug;
extern AstNode *ast_root; // Also good practice to declare globals here

#endif // GLOBALS_H