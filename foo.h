#ifndef FOO_H

#define FOO_H
#include <stdarg.h>
#include <assert.h> // �������� ��� ���������� ������ � �������
typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

#define MEMSIZE (64*1024) //������ ����������� ������
#define pc reg[7] //�� ���� ��������� ������� R7

              // ������ ���������:
#define Rn0 0 // �����������
#define Rn1 1 // ��������-�����������
#define Rn2 2 // ����������������
#define Rn3 3 // �������� ����������������
#define Rn4 4 // ����������������
#define Rn5 5 // �������� ����������������
#define Rn6 6 // ���������
#define Rn7 7 // �������� ���������

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
}; //������� ���������� ss � dd ���� struct Argument

extern struct Argument ss, dd;

#endif FOO_H

