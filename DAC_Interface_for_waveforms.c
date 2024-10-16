#include <lpc214x.h>
#define LED_OFF (IO0SET = 1U << 31)
#define LED_ON (IO0CLR = 1U << 31)

#define PLOCK 0x00000400  // Bit mask for checking PLL lock status

void systeminit(void);
void delay_ms(unsigned int t);


	
void delay_ms(unsigned int t){
	unsigned int i,j;
	for(i=0; i<t; i++)
		for(j=0; j<10000; j++);
}
#define SW2 (IO0PIN & (1 << 14))
#define SW3 (IO0PIN & (1 << 15))
#define SW4 (IO1PIN & (1 << 18))
#define SW5 (IO1PIN & (1 << 19))
#define SW6 (IO1PIN & (1 << 20))

short int sine_table[ ] = {512+0,512+53,512+106,512+158,512+208,512+256,512+300,512+342,512+380,512+413,512+442,
														512+467,512+486,512+503,512+510,512+511,512+510,512+503,512+486,512+467,512+442,512+413,
														512+380,512+342,512+300,512+256,512+208,512+158,512+106,512+53,512+0,512-53,512-106,512-158,
														512-208,512-256,512-300,512-342,512-380,512-413,512-442,512-467,512-486,512-503,512-510,512-511,
														512-510,512-503,512-486,512-467,512-442,512-413,512-380,512-342,512-300,512-256,512-208,512-158,
														512-106,512-53};
														
short int sine_rect_table[ ] = {512+0,512+53,512+106,512+158,512+208,512+256,512+300,512+342,512+380,512+413,
																512+442,512+467,512+486,512+503,512+510,512+511,512+510,512+503,512+486,512+467,
																512+442,512+413,512+380,512+342,512+300,512+256,512+208,512+158,512+106,512+53,512+0};
	
int main(){
		short int value,i=0;
		systeminit();
		PINSEL1 |= 0x00080000; /* P0.25 as DAC output :option 3 - 10 (bits18,19)*/
		IO0DIR |= 1U << 31 | 0x00FF0000 ; // to set P0.16 to P0.23 as o/ps 
		while(1){
		if (!SW2) /* If switch for sine wave is pressed */
		{
			while (i!=60 ){
				value = sine_table[i++];
				DACR = ( (1<<16) | (value<<6) );
				delay_ms(1);
			}
			i=0;
		}
		
		else if (!SW3){
			while ( i!=30 ){
				value = sine_rect_table[i++];
				DACR = ( (1<<16) | (value<<6) );
				delay_ms(1);
			}
			i=0;
		}
 
		else if ( !SW4) /* If switch for triangular wave is pressed */
		{
			value = 0;
			while ( value != 1023 ){
				DACR = ( (1<<16) | (value<<6) );
				value++;
			}
			while ( value != 0 ){
				DACR = ( (1<<16) | (value<<6) );
				value--;
			}
		}
		
		else if ( !SW5 ) /* If switch for sawtooth wave is pressed */
		{
			value = 0;
			while ( value != 1023 ){
				DACR = ( (1<<16) | (value<<6) );
				value++;
			}
		}

		else if ( !SW6 ) /* If switch for square wave is pressed */
		{
			value = 1023;
			DACR = ( (1<<16) | (value<<6) );
			delay_ms(1);
			value = 0;
			DACR = ( (1<<16) | (value<<6) );
			delay_ms(1);
		}
	else /* If no switch is pressed, 3.3V DC */
	{
		value = 1023;
		DACR = ( (1<<16) | (value<<6) );
	}
	}
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

