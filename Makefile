all:
	bison -t -d -v parser.y
	flex lexer.lex
	gcc lex.yy.c parser.tab.c -o Yolo -lfl