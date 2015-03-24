#include <u.h>
#include <libc.h>

#include <sexp.h>

void
main(void)
{
	O *o;

	initsexp();

	o = r();
	prin1(o);
	print("\n");

	exits("");
}
