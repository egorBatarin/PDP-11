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

    //���� 4: ��������� ����������������� ������� load_file ��� 01_sum.pdp.txt
/*
    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
    printf("%x\n", b_read(514)); //2
*/

   //���� 5: ��������� ����������������� ������� load_file ��� 01_sum_mode1.pdp.txt
/*
    printf("%x\n", b_read(64)); //5
    printf("%x\n", b_read(65)); //0
    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
*/


   //���� 6: ��������� ����������������� ������� load_file ��� 01_sum_neg.pdp.txt

    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
    printf("%x\n", b_read(514)); //3

}

void load_file(const char * file_name) { //����� ������������ ������ � ���� ����� � main
    FILE * fin = fopen(file_name, "r"); //������ �� ���������� �����
	adr start; //����� ���������� �����
	unsigned int i;
	unsigned int data; //����������� ��� 16-������ ����� (+���� ����� 8 ����)
	unsigned int n; // ����� ����

	while(fscanf(fin,"%x%x",&start, &n) == 2) { // ������ �� fin ���� ���� ������ ����� � ���-�� ����
		for (i = 0; i < n; i++) { // ������ ��� i = 0 ����� �������� ������ ������ � adress
	     fscanf (fin, "%x", &data); //������ � ����������������� ����
	     byte cdata = (byte) data; //�������� unsigned int, �������� ������ ������ 8 ���, ���������� ���� � ���� 16-������ ������
		 b_write (start + i, cdata); // ���������� ���� �� i-�� �����
		}
	}
	fclose(fin);
};


int main(int argc, char * argv[]) {
    load_file(argv[1]); // � �������� argv[1] ����� ��������� �����-������ ��������� ���� .txt ���������� ������ ����� ������
    test_mem(); // �������-�����������
    return 0;
}
