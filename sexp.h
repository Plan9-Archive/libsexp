#pragma src "/sys/src/libsexp"
#pragma lib "libsexp.a"

enum
{
	Nil = 0,
	List = 1,
	Atom = 2,
	ATOMSZ = 64,
};

typedef struct Cell Cell;
typedef struct O O;

struct O
{
	union {
		Cell *o;
		char *a;
	};
	int type;
};

struct Cell
{
	O car, cdr;
};

/* sexp.c */
char *atomstr(O o);
O cons(O car, O cdr);
O car(O o);
O cdr(O o);
O r(void);
void prin1(O o);
void initsexp(void);
