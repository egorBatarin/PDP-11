#include <stdio.h>
#include <stdlib.h>
#include "foo.h"

struct Argument ss, dd;

void trace(const char *  format, ...) {
    va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
};

void do_mov() {
    w_write(dd.adress, ss.val); // ������ ����� ss.val � ����� � ������� dd.adress
};

void do_halt() {
//trace("------halted------\n");
//trace(" R0 = %06o \n R1 = %06o \n R2 = %06o \n R3 = %06o \n R4 = %06o \n R5 = %06o \n R6 = %06o \n R7 = %06o \n" ,
	//reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]); // ������� ��� ��������
	exit(0);
};

void do_add() {
    w_write (dd.adress, ss.val + dd.val); // � ��� ������ ���������� ��� �������� � ���� �� ����������
};


void run() {
    pc = 01000;
    while(1) {
        word w = w_read(pc);
        //trace("pc = %06o word = %06o: \n", pc, w);
        pc += 2;
        int i;
        for(i = 0; i < 3; i++) {
            if ((w & cmd[i].mask) == cmd[i].opcode) {
                if (i == 0| i == 1) { // ���� ��� mov ��� add, ��
                    dd = sixbittodata(w); //�������������� ������ 6 ��� (destination)
                    ss = sixbittodata(w >> 6); // ����� �������������� ��������� 6 ��� (source)
                }
                cmd[i].do_func(); // � ����� ������ ��������� �������
            }
        }
    }
};



