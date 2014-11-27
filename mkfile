</$objtype/mkfile

LIB=/$objtype/lib/libsexp.a
LIBSRC=/sys/src/libsexp
INCL=/sys/include

install:V: $LIB

$LIB:V: libsrc
	cd $LIBSRC; mk install

libsrc:V:
	mkdir -p $LIBSRC
	cp mkfile.lib $LIBSRC/mkfile
	cp sexp.c $LIBSRC/sexp.c
	cp sexp.h $INCL/sexp.h

nuke:V:
	rm -r $LIBSRC
	rm $INCL/sexp.h

test: test.c $INCL/sexp.h $LIB
	$CC test.c
	$LD -o test test.$O
