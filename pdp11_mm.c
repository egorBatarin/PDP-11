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

void w_write(Adress adr, word w) {
    word w1 = w & 0xff;
    word w2 = w & 0xff00;
    w2 = w2 >> 8;
    byte b1 = (byte) w1;
    byte b2 = (byte) w2;
    mem[adr] = b1;
    mem[adr + 1]  = b2;
}

void test_mem() {

    //пишем байт и читаем его
    byte b0 = 0x0a;
    b_write(3, b0); //пишем в 3 адрес массива mem символ b0
    byte bres = b_read(3); //читаем то, что в 3 адресе (должен быть b0)
    assert(b0 == bres);

    //пишем 2 байта, читаем слово
    Adress a = 5;
    byte b1 = 0x0a;
    byte b2 = 0x0b;
    word w = 0x0b0a;
    b_write(a, b1);
    b_write(a + 1, b2);
    word wres = w_read(a);
    assert(wres == w);

    //пишем слово, читаем слово
    w = 0xbbaa;
    w_write(a, w);
    wres = w_read(a);
    assert(w == wres);
}
int main() {
    test_mem();
    return 0;
}
