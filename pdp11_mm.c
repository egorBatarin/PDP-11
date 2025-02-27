#include <stdio.h>
#include "foo.h" //������ ���� ����������, ���� �� ������ ������ �������� � ������� ������ ���
#include <string.h>
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
	{0170000, 0010000, "mov",	do_mov}, // (1111)000000000000 & w = (1)000000000000,   w = (0001).. - � ������� ��������� 4 ���� (01 � 8-������)
	{0170000, 0060000, "add",	do_add}, // (1111)000000000000 & w = (110)000000000000, w = (0110).. - � ������� ��������� 4 ���� (06 � 8-������)
	{0170000, 0000000, "halt",	do_halt} // (1111)000000000000 & w = (0000)000000000000,w = (0000).. - � ������� ��������� 4 ���� (00 � 8-������)
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
			trace (" [sixbittodata : R%06o] ", n);
			break;
		case Rn1:
			arg.adress = reg[n]; //� �������� ����� �����
			arg.val = w_read(arg.adress); // ������ � ����� ������ ��������
			trace (" [sixbittodata :(R%06o)] ", n);
			break;
		case Rn2:
			arg.adress = reg[n];
			arg.val = w_read(arg.adress);
            reg[n] += 2; //��� �� �� �����, ������ �������������� �������� � ��������
            if (n == 7)
				trace(" [sixbittodata :#%06o] ", arg.val);
			else
				trace(" [sixbittodata :(R%06o)+] ", n);
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



void load_file(const char * file_name) { //����� ������������ ������ � ���� ����� � main, ��� ��������� ��������� ����� � RAM PDP-11
    FILE * fin = fopen(file_name, "r"); //������ �� ���������� �����
	unsigned int start; //����� ���������� �����
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
  char tracechecker;

int main(int argc, char * argv[]) {

    if (argv[2] != NULL) {
        if(strcmp(argv[2],"-t") == 0)
            tracechecker = 1;
        else
            tracechecker = 0;
    }
    else
        tracechecker = 0;


    load_file(argv[1]); // � �������� argv[1] ����� ��������� �����-������ ��������� ���� .txt ���������� � ��������� ������ ������ ����� ������
    trace("--------Running---------\n");
    run();
    //test_mem(); // �������-����������� (�����, ���� ���, load_file ����������� ��������� � ��������)
    return 0;
}
