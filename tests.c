#include "foo.h"
#include <stdio.h>
void test_mem() {

    //���� 1: ����� ���� � ������ ���

    byte b0 = 0x0a;
    b_write(3, b0); //����� � 3 ����� ������� mem ������ b0
    byte bres = b_read(3); //������ ��, ��� � 3 ������ (������ ���� b0)
    assert(b0 == bres);

    //���� 2: ����� 2 �����, ������ �����

    adr a = 5;
    byte b1 = 0x0a;
    byte b2 = 0x0b;
    word w = 0x0b0a;
    b_write(a, b1);
    b_write(a + 1, b2);
    word wres = w_read(a);
    assert(wres == w);

    //���� 3: ����� �����, ������ �����

    w = 0xbbaa;
    w_write(a, w);
    wres = w_read(a);
    assert(w == wres);

    //���� 4: ��������� ����������������� ������� load_file ��� 01_sum.pdp.txt
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
   //���� 5: ��������� ����������������� ������� load_file ��� 01_sum_mode1.pdp.txt
/*
    printf("%x\n", b_read(64)); //5
    printf("%x\n", b_read(65)); //0
    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
*/


   //���� 6: ��������� ����������������� ������� load_file ��� 01_sum_neg.pdp.txt
/*
    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
    printf("%x\n", b_read(514)); //3
*/

    //���� 7: ���������, ��� ������� sixbittodata ����������� 16 ������ ������������������ � �������� ����������� ����

    word wer = 0xffce; // 1111111111(001110), 6 �������, ��� 1
    struct Argument result;
    reg[6] = 1002;
    w_write(1002, 5);
    result = sixbittodata(wer);
    assert(result.adress == 1002);
    assert(result.val == 5);
}

