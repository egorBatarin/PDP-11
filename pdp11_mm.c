#include <stdio.h>
#include <assert.h> // �������� ��� ���������� ������ � �������
#include "foo.h" //������ ���� ����������, ���� �� ������ ������ �������� � ������� ������ ���

byte mem[MEMSIZE]; //�����, ��� ���������� ������� �����

void b_write(adr a, byte b) {
    mem[a] = b; // ������ ��������� ���� b � ����� � ������� a
}

byte b_read(adr a) {
    return mem[a]; // � ��� ��� ������ ������
}

word w_read(adr a) {
    word w = ((word)mem[a + 1]) << 8; // ����� ������� ���� (� ��� little endian)
    // ����� �������� � ���� word (��� ��� � ��� ��� ����, � �� �����)
    // ������� ������ �� 8 ���, ������ 8 ��� ����������� ������
    w = w | mem[a]; // ���������� (�� ������ 8 ������� ��� ��������� ��������� )
    return w;
}

void w_write(adr a, word w) {
    word w1 = w & 0xff; //�������� 8 ��������� ���
    word w2 = w & 0xff00; //�������� 8 ������ ���
    w2 = w2 >> 8; // ������� � ������
    byte b1 = (byte) w1; //�������� ��� ����� � ������
    byte b2 = (byte) w2;
    mem[a] = b1; // ��� ���� �������...
    mem[a + 1]  = b2;
}

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
}

void load_file(const char * file_name) { //����� ������������ ������ � ���� ����� � main

};

int main() {
    test_mem(); // �������-�����������
    return 0;
}
