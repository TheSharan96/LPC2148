#include <lpc214x.h>
#define LED_OFF (IO0SET = 1U << 31)
#define LED_ON (IO0CLR = 1U << 31)

#define PLOCK 0x00000400  // Bit mask for checking PLL lock status

void systeminit(void);
void delay_ms(unsigned int t);
void uartinit(void);



int main(){}



void delay_ms(unsigned int t){
	unsigned int i,j;
	for(i=0; i<t; i++)
		for(j=0; j<10000; j++);
}


void systeminit(void) {
    PLL0CON = 0x01;      // Enable the PLL (PLLE = 1)
    PLL0CFG = 0x24;      // Set the multiplier and divider values (M=5, P=2)
    PLL0FEED = 0xAA;     // Sequence to update PLL registers
    PLL0FEED = 0x55;
    
    while (!(PLL0STAT & PLOCK));  // Wait for the PLL to achieve lock
    
    PLL0CON = 0x03;      // Connect the PLL (PLLE = 1 and PLLC = 1)
    PLL0FEED = 0xAA;     // Sequence to update PLL registers after connecting
    PLL0FEED = 0x55;
    
    VPBDIV = 0x01;       // Set PCLK = CCLK (PCLK = 60 MHz if CCLK is 60 MHz)
}

void uartinit(void){
	PINSEL0 |= 0x05;

	U0LCR = 0x83;
	U0DLM = 0;
	U0DLL = 32; //BAUD RATE = 115200
	
	U0LCR = 0x03;
	U0FCR = 0x07;
}
