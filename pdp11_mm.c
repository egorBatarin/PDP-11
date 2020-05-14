#include <stdio.h>
#include <assert.h> // средство для сохранения зрения и времени
#include "foo.h" //создал свою библиотеку, чтоб не писать всякие тайпдефы и екстёрны каждый раз

byte mem[MEMSIZE]; //решил, что отдельными байтами проще

void b_write(adr a, byte b) {
    mem[a] = b; // просто добавляем байт b в адрес с номером a
}

byte b_read(adr a) {
    return mem[a]; // а тут его просто читаем
}

word w_read(adr a) {
    word w = ((word)mem[a + 1]) << 8; // берем старший байт (у нас little endian)
    // потом приводим к типу word (так как у нас был байт, а не слово)
    // смещаем вперед на 8 бит, первые 8 бит заполняются нулями
    w = w | mem[a]; // складываем (на первые 8 нулевых бит аккуратно наложится )
    return w;
}

void w_write(adr a, word w) {
    word w1 = w & 0xff; //отрезаем 8 последних бит
    word w2 = w & 0xff00; //отрезаем 8 первых бит
    w2 = w2 >> 8; // смещаем в начало
    byte b1 = (byte) w1; //приводим все слова к байтам
    byte b2 = (byte) w2;
    mem[a] = b1; // тут дело техники...
    mem[a + 1]  = b2;
}

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
*/

   //Тест 5: проверяем работоспособность функции load_file для 01_sum_mode1.pdp.txt
/*
    printf("%x\n", b_read(64)); //5
    printf("%x\n", b_read(65)); //0
    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
*/


   //Тест 6: проверяем работоспособность функции load_file для 01_sum_neg.pdp.txt

    printf("%x\n", b_read(512)); //c0
    printf("%x\n", b_read(513)); //15
    printf("%x\n", b_read(514)); //3

}

void load_file(const char * file_name) { //будем использовать только в этом файле в main
    FILE * fin = fopen(file_name, "r"); //читаем из текстового файла
	adr start; //адрес начального блока
	unsigned int i;
	unsigned int data; //считываемые два 16-ричных числа (+нули после 8 бита)
	unsigned int n; // число байт

	while(fscanf(fin,"%x%x",&start, &n) == 2) { // читаем из fin пока дают читать адрес и кол-во байт
		for (i = 0; i < n; i++) { // каждый раз i = 0 чтобы начинать запись именно с adress
	     fscanf (fin, "%x", &data); //читаем в шестнадцатиричном виде
	     byte cdata = (byte) data; //обрезаем unsigned int, оставляя только первые 8 бит, кодирующие инфу о двух 16-ричных числах
		 b_write (start + i, cdata); // записываем байт на i-ое место
		}
	}
	fclose(fin);
};


int main(int argc, char * argv[]) {
    load_file(argv[1]); // в качестве argv[1] будет выступать какой-нибудь текстовый тест .txt написанный первым через пробел
    test_mem(); // палочка-выручалочка
    return 0;
}
