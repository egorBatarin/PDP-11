#include <stdio.h>
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

const Command cmd[] = {
	{(word)1111000000000000,   (word)1000000000000, "mov",	do_mov}, // (1111)000000000000 & w = (1)000000000000,   w = (0001).. - � ������� ��������� 4 ���� (01 � 8-������)
	{(word)1111000000000000, (word)110000000000000, "add",	do_add}, // (1111)000000000000 & w = (110)000000000000, w = (0110).. - � ������� ��������� 4 ���� (06 � 8-������)
	{(word)1111000000000000,               (word)0, "halt",	do_halt} // (1111)000000000000 & w = (0000)000000000000,w = (0000).. - � ������� ��������� 4 ���� (00 � 8-������)
};

struct Argument sixbittodata(word w) {
// ������ ������� ����� 16 ������ ������������������, ������������� ��������� 6 ��� (��� destination ���� ������ �� ���� w ��� ��� source ���� ������ w >> 6)
// ������������ �������������� ���� � ������ n �������� � mode, ����� ��������� �������� ����, ��� �������� � mode, � ����� ���������� ��������
    struct Argument arg;
	int n = w & 7; //3 ���� �� ����� ��������
	int mode = (w >> 3) & 7; //3 ���� �� ����� ����
	switch (mode) {
	    unsigned int x;
		case Rn0:
			arg.adress = n; // ����� - ����� ��������
			arg.val = reg[n]; //�������� - ��, ��� ����� � ��������
			break;
		case Rn1:
			arg.adress = reg[n]; //� �������� ����� �����
			arg.val = w_read(arg.adress); // ������ � ����� ������ ��������
			break;
		case Rn2:
			arg.adress = reg[n];
			arg.val = w_read(arg.adress);
            reg[n] += 2; //��� �� �� �����, ������ �������������� �������� � ��������
            break;
        case Rn3:
			arg.adress = reg[n]; //� �������� ����� �����
			arg.adress = w_read(arg.adress); //� ������ RAM PDP-11 �� ����� ������ ���� ����� ����� (��� ��������)
			arg.val = w_read(arg.adress); // �� ��������� ������ ������ ��������
            reg[n] += 2; //�� ��������...
            break;
        case Rn4:
            reg[n] -= 2; // ������� ��������������
            arg.adress = reg[n]; // ���������� ���������� �����
            arg.val = w_read(arg.adress); // �� ���� ��������� � ������ RAM, �� ������� ����� ��������
            break;
        case Rn5:
            reg[n] -= 2; // ������� ��������������
            arg.adress = reg[n]; // �� ������������������� ����� ������� ����
            arg.adress = w_read(arg.adress); // �� ���� ���� (���� ���� � ��������� �����)
            arg.val = w_read(arg.adress); // �� ���� ���� (���� - � ��������� ��������)
            break;
        case Rn6:
            x = w_read(pc); // ������ ������� �����
            pc += 2; // ��� ������
            arg.adress = x + pc; // � ����� ����� ��� ����� ���� pc
            arg.val = w_read(arg.adress); // ������ �������� �� ����������� ������
            break;
        case Rn7:
            x = w_read(pc);
            pc += 2;
            arg.adress = x + pc;
            arg.adress = w_read(arg.adress); // ��� �� �� �����, ������ ������ ����� �������� ������ ��-������...
            arg.val = w_read(arg.adress);
            break;
	}
	return arg;
};


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

void load_file(const char * file_name) { //����� ������������ ������ � ���� ����� � main, ��� ��������� ��������� ����� � RAM PDP-11
    FILE * fin = fopen(file_name, "r"); //������ �� ���������� �����
	adr start; //����� ���������� �����
	unsigned int i;
	unsigned int data; //����������� ��� 16-������ ����� (+���� ����� 8 ����)
	unsigned int n; // ����� ����

	while(fscanf(fin,"%x%x",&start, &n) == 2) { // ������ �� fin ���� ���� ������ ����� � ���-�� ����
		for (i = 0; i < n; i++) { // ������ ��� i = 0 ����� �������� ������ ������ �� start
	     fscanf (fin, "%x", &data); //������ � ����������������� ����
	     byte cdata = (byte) data; //�������� unsigned int, �������� ������ ������ 8 ���, ���������� ���� � ���� 16-������ ������
		 b_write (start + i, cdata); // ���������� ���� �� i-�� �����
		}
	}
	fclose(fin);
};


int main(int argc, char * argv[]) {
    test_mem(); // �������-����������� (�����, ���� ���, load_file ����������� ��������� � ��������)
    //load_file(argv[1]); // � �������� argv[1] ����� ��������� �����-������ ��������� ���� .txt ���������� � ��������� ������ ������ ����� ������

    return 0;
}
