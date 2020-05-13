
typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;

#define MEMSIZE (64*1024)

extern byte mem[MEMSIZE];

void b_write(adr a, byte b);
byte b_read(adr a);
word w_read(adr a);
void w_write(adr a, word w);


