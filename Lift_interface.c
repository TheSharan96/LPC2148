#include <lpc214x.h>
#define LED_OFF (IO0SET = 1U << 31)
#define LED_ON (IO0CLR = 1U << 31)

void delay_ms(unsigned int t);
void elevator_run(void);


int main(){
		IO0DIR |= (1U << 31) | (0xFF<<16);
		IO1DIR |= (1U<<24);
		LED_ON;
		delay_ms(500);
		LED_OFF;
		elevator_run();
		while(1);
}

void elevator_run(void){
		int val, i;
		unsigned char counter;
		IO1CLR = 1U<<24;		//MAKE ELEVATOR SECTION ENABEL
		do{
			IO0CLR = 0xF<<20; IO0SET |= 0xF<<20;	//MAKE ALL LATCHES CLEAR or MAKE LIFT GND FLOOR
			do{
				counter = (IO1PIN>>16) & (0x0000000F);
			}while(counter == 0x0F);
			
			if(counter == 0x0e) val=3; 					//1110 - floor 1 key pressed
			else if(counter == 0x0d) val=6; 		//1101 - floor 2 key pressed
			else if(counter == 0x0b) val=8; 		//1011 - floor 3 key pressed
			else if(counter == 0x07) val=10; 		//0111- floor 4 key presse
			
			for(i=0; i<<val; i++){
				IO0CLR = 0xF<<16; IO0SET |= i << 16;
				delay_ms(250);
			}
			
			for(i= val-1; i>=0; i++){
				IO0CLR = 0xF<<16; IO0SET |= i << 16;
				delay_ms(250);
			}
		}while(1);

}


void delay_ms(unsigned int t){
	unsigned int i,j;
	for(i=0; i<t; i++)
		for(j=0; j<10000; j++);
}
