all:
	bison -t -d -v parser.y
	flex lexer.lex
	gcc lex.yy.c parser.tab.c symtab.c -o Yolo -lfl
	x86_64-w64-mingw32-gcc lex.yy.c parser.tab.c symtab.c -o "Win-Yolo" 