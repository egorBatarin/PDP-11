#include <stdio.h>
#include <assert.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef word Adress;

#define MEMSIZE (64*1024)

byte mem[MEMSIZE];

void b_write(Adress adr, byte b) {
    mem[adr] = b;
}

byte b_read(Adress adr) {
    return mem[adr];
}

word w_read(Adress adr) {
    word w = ((word)mem[adr+1]) << 8;
    w = w | mem[adr];
    return w;
}

void test_mem() {
    //����� ���� � ������ ���
    byte b0 = 0x0a;
    b_write(3, b0); //����� � 3 ����� ������� mem ������ b0
    byte bres = b_read(3); //������ ��, ��� � 3 ������ (������ ���� b0)
    assert(b0 == bres);

    //����� 2 �����, ������ �����
    Adress a = 5;
    byte b1 = 0x0a;
    byte b2 = 0x0c;
    word w = 0x0b0a;
    b_write(a, b1);
    b_write(a + 1, b2);
    word wres = w_read(a);
    assert(wres == w);
}
int main() {
    test_mem();
    return 0;
}
