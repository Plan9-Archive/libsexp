#include <u.h>
#include <libc.h>
#include <ctype.h>
#include <bio.h>

#include <sexp.h>

Biobuf B[2];

#define B0 (B + 0)
#define B1 (B + 1)

void pl(O *o[2], int paren);

char *
atomstr(O *o)
{
	if(o == nil)
		return "";
	if(o->type != Atom)
		return "";

	return o->a;
}

O *
mka(char *c)
{
	O *o;

	o = malloc(sizeof(O));

	o->a = c;
	o->type = Atom;
	return o;
}

O *
cons(O *car, O *cdr)
{
	O *o;

	o = malloc(sizeof(O));

	o->o[0] = car;
	o->o[1] = cdr;
	o->type = List;

	return o;
}

O *
car(O *o)
{
	if(o == nil)
		return nil;
	if(o->type != List)
		return nil;

	return o->o[0];
}

O *
cdr(O *o)
{
	if(o == nil)
		return nil;
	if(o->type != List)
		return nil;

	return o->o[1];
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

O *
rdatom(int c)
{
	int i;
	char *s;

	s = malloc(ATOMSZ + 1);
	if(s == nil)
		return nil;

	for(i = 0; i < ATOMSZ; ++i){
		if(!isgraph(c) || c == '(' || c == ')')
			break;
		s[i] = c;
		c = Bgetc(B0);
	}
	Bungetc(B0);
	s[i] = '\0';

	if(i == 0){
		free(s);
		return nil;
	}
	return mka(s);
}

O *
rdlist(void)
{
	O *car, *cdr;
	int c;

	c = skipspace();

	if(c == ')')
		return nil;

	Bungetc(B0);

	car = r();
	c = skipspace();
	if(c == '.'){
		cdr = r();
		c = skipspace();
		if(c != ')')
			return nil;
		return cons(car, cdr);
	}else{
		Bungetc(B0);
		cdr = rdlist();
		return cons(car, cdr);
	}
}

O *
r(void)
{
	int c;

	c = skipspace();

	if(c == '(')
		return rdlist();
	if(!isprint(c) || c == ')')
		return nil;
	return rdatom(c);
}

/* XXX should it take o, or o->o[2] ? */
void
pl(O *o[2], int paren)
{
	if(o == nil)
		return;

	if(paren)
		print("(");

	prin1(o[0]);

	if(o[1] == nil)
		goto end;
	switch(o[1]->type){
	case Nil:
		break;
	case List:
		print(" ");
		pl(o[1]->o, 0);
		break;
	case Atom:
		print(" . ");
		prin1(o[1]);
		break;
	}

end:
	if(paren)
		print(")");
}

/* XXX externally-consumable prin1 should wrap this and bioflush, */
/* XXX so we can bioprint inside.  (bioflush unsuitable; equiv to print() since recursive.) */
void
prin1(O *o)
{
	if(o == nil){
		print("nil");
		return;
	}
	switch(o->type){
	case Nil:
		print("nil");
		return;
	case List:
		pl(o->o, 1);
		return;
	case Atom:
		print("%s", o->a);
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
