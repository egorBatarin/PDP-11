#ifndef FOO_H

#define FOO_H
#include <stdarg.h>
#include <assert.h> // средство для сохранения зрения и времени
typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

#define MEMSIZE (64*1024) //размер оперативной памяти
#define pc reg[7] //то бишь последний регистр R7

              // режимы адресации:
#define Rn0 0 // регистровый
#define Rn1 1 // косвенно-регистровый
#define Rn2 2 // автоинкрементный
#define Rn3 3 // косвенно автоинкрементный
#define Rn4 4 // автодекрементный
#define Rn5 5 // косвенно автодекрементный
#define Rn6 6 // индексный
#define Rn7 7 // косвенно индексный

extern byte mem[MEMSIZE];

void b_write(adr a, byte b);
byte b_read(adr a);
word w_read(adr a);
word reg[8];
void w_write(adr a, word w);
void run();
void do_mov();
void do_halt();
void do_add();
void do_nothing();
void run();
typedef struct {
	word mask;
	word opcode;
	char * name;
	void (*do_func)(void);
} Command;
extern const Command cmd[];
void trace(const char *  format, ...);

struct Argument {
    word val;
    word adress;
}; //объявим переменные ss и dd типа struct Argument

extern struct Argument ss, dd;

#endif FOO_H

