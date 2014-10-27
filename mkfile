</$objtype/mkfile

sexp: sexp.c thp.h
	$CC sexp.c
	$LD -o sexp sexp.$O
