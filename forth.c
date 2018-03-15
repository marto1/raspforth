extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

#define ESCAPE_SEQ 0x20
#define MAX_WORD_LENGTH 64
#define MAX_INTEGER_LENGTH 10
#define STACK_SIZE 40

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0
//((250,000,000/115200)/8)-1 = 270

static int stack[STACK_SIZE];
static unsigned long long types = 0;

inline char isdigit ( unsigned char c )
{
    switch (c){
	case 48:
	case 49:
	case 50:
	case 51:
        case 52:
	case 53:
	case 54:
	case 55:
        case 56:
	case 57:	
	    return 1;
	    break;
        default:
	    return 0;
	    break;
    }
}

char isnumber ( unsigned char* buf, unsigned char index ) 
{
    unsigned char scan;
    if (!isdigit(buf[0]) && (buf[0] != 43) && (buf[0] != 45)){
	return 0;
    }
    if (index > MAX_INTEGER_LENGTH){
	return 0;
    }
    if ((index == 1) && ((buf[0] == 43) || (buf[0] == 45))){
	return 0;
    }
    for (scan = index - 1; scan > 0 ;scan--) 
    {
	if (!isdigit(buf[scan])) return 0;
    }
    return 1;
}

void handle_number ( unsigned char* buf, unsigned char index ) {
    unsigned char i, neg;
    int integer=0;
    
    if (buf[0] == '-'){
	i = 1;
	neg = 1;
    } else {
	i = 0;
	neg = 0;
    }

    for (i; i < index ;i++) 
    {
	integer = 10*integer - (buf[i] - '0');
    }
    if (!neg) {
	integer = -integer;
    }
}

void handle_symbol ( unsigned char* buf, unsigned char index ) {

}

void handle_word ( unsigned char* buf, unsigned char index )
{
    if (isnumber(buf, index)) {
        handle_number(buf, index);
    } else {
	handle_symbol(buf, index);
    }
}

void uart_putc ( unsigned int c )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;
    }
    PUT32(AUX_MU_IO_REG,c);
}
//------------------------------------------------------------------------
void uart_puts ( char* str )
{
    unsigned int rb = 0;
    while(str[rb] != 0)
    {
	uart_putc(str[rb]);
	rb++;
    }
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}
//------------------------------------------------------------------------
int parse(unsigned char * buffer, unsigned char index, unsigned char c) {
    if (index > MAX_WORD_LENGTH) {
	return -1;
    }
    if (c == ESCAPE_SEQ) {
	return 1;
    }
    buffer[index] = c;
    return 0;
}
int notmain ( unsigned int earlypc )
{
    unsigned int ra;
    unsigned char word_index = 0;
    /* unsigned char i; */
    unsigned char word_buf[MAX_WORD_LENGTH];


    PUT32(AUX_ENABLES,1);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,270);

    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    PUT32(GPFSEL1,ra);

    PUT32(GPPUD,0);
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);

    PUT32(AUX_MU_CNTL_REG,3);

    uart_puts("Forth interpreter ready. Type words to list words.\n");

    while(1)
    {
        while(1)
        {
            if(GET32(AUX_MU_LSR_REG)&0x01) break;
        }
        ra=GET32(AUX_MU_IO_REG);
	/* uart_putc(ra); */
	hexstring(ra);
	switch (parse(word_buf, word_index, (unsigned char)ra)){
	case 0:
	    uart_puts("PUT\n");
	    word_index++;
	    break;
	case 1:
	    /* for (i = 0; i < word_index; i++) { */
	    /* 	uart_putc(word_buf[i]); */
	    /* } */
	    /* uart_putc('\n'); */
	    handle_word(word_buf, word_index);
	    word_index = 0;
	    break;
	case -1:
	    uart_puts("Fatal. Resetting..\n");
	    break;
	}
	
	
    }

    return(0);
}
