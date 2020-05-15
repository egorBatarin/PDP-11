#include "foo.h"
#include <stdio.h>
void test_mem() {

    //Тест 1: пишем байт и читаем его

    byte b0 = 0x0a;
    b_write(3, b0); //пишем в 3 адрес массива mem символ b0
    byte bres = b_read(3); //читаем то, что в 3 адресе (должен быть b0)
    assert(b0 == bres);

    //Тест 2: пишем 2 байта, читаем слово

    adr a = 5;
    byte b1 = 0x0a;
    byte b2 = 0x0b;
    word w = 0x0b0a;
    b_write(a, b1);
    b_write(a + 1, b2);
    word wres = w_read(a);
    assert(wres == w);

    //Тест 3: пишем слово, читаем слово

    w = 0xbbaa;
    w_write(a, w);
    wres = w_read(a);
    assert(w == wres);

    //Тест 4: проверяем работоспособность функции load_file для 01_sum.pdp.txt
/*
    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
    printf("%x\n", b_read(514)); //2
    printf("%x\n", b_read(515)); //0
    printf("%x\n", b_read(516)); //c1
    printf("%x\n", b_read(517)); //15
    printf("%x\n", b_read(518)); //03
    printf("%x\n", b_read(519)); //0
    printf("%x\n", b_read(520)); //01
    printf("%x\n", b_read(521)); //60
    printf("%x\n", b_read(522)); //00
    printf("%x\n", b_read(523)); //00
*/
   //Тест 5: проверяем работоспособность функции load_file для 01_sum_mode1.pdp.txt
/*
    printf("%x\n", b_read(64)); //5
    printf("%x\n", b_read(65)); //0
    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
*/


   //Тест 6: проверяем работоспособность функции load_file для 01_sum_neg.pdp.txt
/*
    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
    printf("%x\n", b_read(514)); //3
*/

    //Тест 7: проверяем, как успешно sixbittodata раскодирует 16 битные последовательности в косвенно регистровом моде

    word wer = 0xffce; // 1111111111(001110), 6 регистр, мод 1
    struct Argument result;
    reg[6] = 1002;
    w_write(1002, 5);
    result = sixbittodata(wer);
    assert(result.adress == 1002);
    assert(result.val == 5);
}

