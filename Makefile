LIBS=-lm
LEXICAL=lex ac.lex.c
LEX_DEP=ac.l ac.syn.h
SYNTAX=syn ac.syn.c ac.syn.h
SYN_DEP=ac.y


ac: ac.lex.c ac.syn.c
	gcc -O3 -o $@ $? symtab.c ast.c $(LIBS)

lex: $(LEX_DEP)
ac.lex.c: $(LEX_DEP)
syn: $(SYN_DEP)
ac.syn.c: $(SYN_DEP)

$(LEXICAL):
	lex -o $@ $?

$(SYNTAX):
	yacc --defines -o $@ $?

clean:
	rm ac ac.lex.c ac.syn.c
