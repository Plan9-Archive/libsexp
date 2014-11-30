#include <u.h>
#include <libc.h>
#include <ctype.h>
#include <bio.h>

#include <sexp.h>

Biobuf B[2];

#define B0 (B + 0)
#define B1 (B + 1)

void pl(Cell *o, int paren);

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

O
cons(O car, O cdr)
{
	Cell *c;

	c = malloc(sizeof(Cell));

	c->car = car;
	c->cdr = cdr;

	return mko(c);
}

int
skipspace(void)
{
	int c;

	while((c = Bgetc(B0)) >= 0){
		if(!isspace(c))
			break;
	}
	return c;
}

O
rdatom(int c)
{
	int i;
	char *s;

	s = malloc(ATOMSZ + 1);
	if(s == nil)
		return Nilcell();

	for(i = 0; i < ATOMSZ; ++i){
		if(!isalnum(c)){
			break;
		}
		s[i] = c;
		c = Bgetc(B0);
	}
	Bungetc(B0);
	s[i] = '\0';

	if(i == 0){
		free(s);
		return Nilcell();
	}
	return mka(s);
}

O
rdlist(void)
{
	O car, cdr;
	int c;

	c = skipspace();

	if(c == ')')
		return Nilcell();

	Bungetc(B0);

	car = r();
	c = skipspace();
	if(c == '.'){
		cdr = r();
		c = skipspace();
		if(c != ')')
			return Nilcell();
		return cons(car, cdr);
	}else{
		Bungetc(B0);
		cdr = rdlist();
		return cons(car, cdr);
	}
}

O
r(void)
{
	int c;

	c = skipspace();

	if(c == '(')
		return rdlist();
	if(!isalnum(c))
		return Nilcell();
	return rdatom(c);
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
initsexp(void)
{
	Binit(B0, 0, OREAD);
	Binit(B1, 1, OWRITE);
}
