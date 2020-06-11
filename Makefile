LIBS=-lm
LEXICAL=lex ac.lex.c
LEX_DEP=ac.l ac.syn.h
LEX_NAME=ac.lex.c
SYNTAX=syn ac.syn.c ac.syn.h
SYN_DEP=ac.y
SYN_NAME=ac.syn.c


ac: ac.lex.c ac.syn.c
	gcc -O3 $? symtab.c ast.c -o $@ $(LIBS)

lex: $(LEX_DEP)
ac.lex.c: $(LEX_DEP)
syn: $(SYN_DEP)
ac.syn.c: $(SYN_DEP)
ac.syn.h: $(SYN_DEP)

$(LEXICAL):
	flex -o $(LEX_NAME) $?

$(SYNTAX):
	bison --defines -o $(SYN_NAME) $?

clean:
	rm ac ac.lex.c ac.syn.*
