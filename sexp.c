#include <u.h>
#include <libc.h>
#include <ctype.h>

#include "thp.h"

char buf[256];
int m, n;

enum
{
	BUFSZ = 255,
};

Cell *
cons(O car, O cdr)
{
	Cell *c;

	c = malloc(sizeof(Cell));

	c->car = car;
	c->cdr = cdr;

	return c;
}

O
Nilcell(void)
{
	O o;

	o.o = nil;
	o.type = Nil;
	return o;
}

O
mko(Cell *c)
{
	O o;

	o.o = c;
	o.type = List;
	return o;
}

O
mka(char *c)
{
	O o;
	o.a = c;
	o.type = Atom;
	return o;
}

int
readmore(void)
{
	if(m < n)
		return n - m;
	m = 0;
	n = read(0, buf, BUFSZ);
	return n;
}

int
skipspace(void)
{
	for(;; ++m){
		if(m >= n){
			if(readmore() < 1)
				return -1;
		}
		if(!isspace(buf[m]))
			return readmore();
	}
}

O
rdatom(void)
{
	int i;
	char *s;
	s = malloc(ATOMSZ + 1);
	if(s == nil)
		return Nilcell();

	for(i = 0; i < ATOMSZ; ++i){
		if(readmore() < 1)
			break;
		if(!isalnum(buf[m]))
			break;
		s[i] = buf[m++];
	}
	s[i] = '\0';

	//return (O){.a = s, .type = Atom};
	return mka(s);
}

O
rdlist(void)
{
	O car, cdr;

	if(skipspace() < 1)
		return Nilcell();
	if(buf[m] == ')'){
		m += 1;
		return Nilcell();
	}

	car = r();
	if(skipspace() < 1)
		return Nilcell();
	if(buf[m] == '.'){
		m += 1;
		cdr = r();
		if(skipspace() < 1)
			return Nilcell();
		if(buf[m++] != ')')
			return Nilcell();
		return mko(cons(car, cdr));
	}
	cdr = rdlist();
	return mko(cons(car, cdr));
}

O
r(void)
{
	O car, cdr;

	if(skipspace() < 1)
		return Nilcell();

	if(isalnum(buf[m]))
		return rdatom();
	if(buf[m++] == '(')
		return rdlist();
	return Nilcell();

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

	o = r();
	prin1(o);
	print("\n");

	exits("");
}
