</$objtype/mkfile

sexp: sexp.c sexp.h
	$CC sexp.c
	$LD -o sexp sexp.$O
