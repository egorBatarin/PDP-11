#ifndef FOO_H

#define FOO_H

typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

#define MEMSIZE (64*1024) //размер оперативной памяти
#define pc reg[7] //то бишь последний регистр R7

extern byte mem[MEMSIZE];

void b_write(adr a, byte b);
byte b_read(adr a);
word w_read(adr a);
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




#endif FOO_H

