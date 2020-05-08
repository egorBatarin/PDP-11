#include <stdio.h>
#include <assert.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

#define MEMSIZE (64*1024)

byte mem[MEMSIZE];

void b_write(adr a, byte b) {
    mem[a] = b;
}

byte b_read(adr a) {
    return mem[a];
}

word w_read(adr a) {
    word w = ((word)mem[a + 1]) << 8;
    w = w | mem[a];
    return w;
}

void w_write(adr a, word w) {
    word w1 = w & 0xff;
    word w2 = w & 0xff00;
    w2 = w2 >> 8;
    byte b1 = (byte) w1;
    byte b2 = (byte) w2;
    mem[a] = b1;
    mem[a + 1]  = b2;
}

void test_mem() {

    //пишем байт и читаем его
    byte b0 = 0x0a;
    b_write(3, b0); //пишем в 3 адрес массива mem символ b0
    byte bres = b_read(3); //читаем то, что в 3 адресе (должен быть b0)
    assert(b0 == bres);

    //пишем 2 байта, читаем слово
    adr a = 5;
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
