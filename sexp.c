#include <u.h>
#include <libc.h>

#include "thp.h"

Cell *
cons(O car, O cdr)
{
	Cell *c;

	c = malloc(sizeof(Cell));

	c->car = car;
	c->cdr = cdr;

	return c;
}

void
pl(Cell *o, int paren)
{
	if(o == nil)
		return;

	if(paren)
		print("(");

	prin1(o->car);

	switch(o->cdr.type){
	case Nil:
		break;
	case List:
		print(" ");
		pl(o->cdr.o, 0);
		break;
	case Atom:
		print(" . ");
		prin1(o->cdr);
		break;
	}

	if(paren)
		print(")");
}

void
prin1(O o)
{
	switch(o.type){
	case Nil:
		print("nil");
		return;
	case List:
		pl(o.o, 1);
		return;
	case Atom:
		print("%s", o.a);
		return;
	default:
		print("invalid type");
		return;
	}
}

void
main(void)
{
	O o;
	O a, b;
	O n;

	a.type = Atom;
	a.a = "hello";

	n.type = Nil;
	n.o = nil;

	b.type = List;
	b.o = cons(a, n);

	o.type = List;
	o.o = cons(a, b);

	prin1(o);
	print("\n");

	exits("");
}
