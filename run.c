#include <stdio.h>
#include <stdlib.h> // для функции exit
#include "foo.h"

struct Argument ss, dd;

void trace(const char *  format, ...) {
    va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
};

void do_mov() {

};

void do_halt() {
trace("R0 = %06o \n R1 = %06o \n R2 = %06o \n R3 = %06o \n R4 = %06o \n R5 = %06o \n R6 = %06o \n R7 = %06o \n" ,
	reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]); //выводим все регистры и стоп машина
	exit(0);
};

void do_add() {

};


void run() {
    pc = 01000;
    while(1) {
        word w = w_read(pc);
        //trace("%06o %06o: ", pc, w);
        pc += 2;
        int i;
        for(i = 0; i < 3; i++) {
            if ((w & cmd[i].mask) == cmd[i].opcode) {

            }
        }
    }
};



