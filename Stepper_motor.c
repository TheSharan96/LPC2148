#include <lpc214x.h>

#define LED_OFF (IO0SET = 1U << 31)  // Macro to turn off the LED by setting pin 31
#define LED_ON (IO0CLR = 1U << 31)   // Macro to turn on the LED by clearing pin 31

#define PLOCK 0x00000400  // Bit mask for checking PLL lock status

// Function prototypes
void systeminit(void);       // Function to initialize the system
void delay_ms(unsigned int t); // Function to create a delay in milliseconds

int main() {
    unsigned int no_clk = 100;   // Counter for the first sequence
    unsigned int no_aclk = 100;  // Counter for the second sequence
    
    // Set the direction of pins: 
    // Pin 31 for LED, pins 16-19 for output (LEDs), and pin 30 for strobe
    IO0DIR |= (1U<<31) | (0xFF<<16) | (1U<<30); 
    
    // Turn on LED, wait for 500 ms, then turn it off
    LED_ON;
    delay_ms(500);
    LED_OFF;

    systeminit(); // Call system initialization function

    // First sequence: Light up LEDs in a specific order
    do {
        IO0CLR = 0xF << 16;          // Clear all output pins (LEDs)
        IO0SET |= 1 << 16;          // Set pin 16 (LED 1) high
        delay_ms(10);                // Delay for 10 ms
        if (--no_clk == 0) break;    // Decrement counter and break if it reaches 0

        IO0CLR = 0xF << 16;          // Clear all output pins (LEDs)
        IO0SET |= 1 << 17;          // Set pin 17 (LED 2) high
        delay_ms(10);                // Delay for 10 ms
        if (--no_clk == 0) break;    // Decrement counter and break if it reaches 0

        IO0CLR = 0xF << 16;          // Clear all output pins (LEDs)
        IO0SET |= 1 << 18;          // Set pin 18 (LED 3) high
        delay_ms(10);                // Delay for 10 ms
        if (--no_clk == 0) break;    // Decrement counter and break if it reaches 0

        IO0CLR = 0xF << 16;          // Clear all output pins (LEDs)
        IO0SET |= 1 << 19;          // Set pin 19 (LED 4) high
        delay_ms(10);                // Delay for 10 ms
        if (--no_clk == 0) break;    // Decrement counter and break if it reaches 0
    } while (1); // Repeat until break

    // Second sequence: Light up LEDs in reverse order
    do {
        IO0CLR = 0xF << 16;          // Clear all output pins (LEDs)
        IO0SET |= 1 << 19;          // Set pin 19 (LED 4) high
        delay_ms(10);                // Delay for 10 ms
        if (--no_aclk == 0) break;   // Decrement counter and break if it reaches 0

        IO0CLR = 0xF << 16;          // Clear all output pins (LEDs)
        IO0SET |= 1 << 18;          // Set pin 18 (LED 3) high
        delay_ms(10);                // Delay for 10 ms
        if (--no_aclk == 0) break;   // Decrement counter and break if it reaches 0

        IO0CLR = 0xF << 16;          // Clear all output pins (LEDs)
        IO0SET |= 1 << 17;          // Set pin 17 (LED 2) high
        delay_ms(10);                // Delay for 10 ms
        if (--no_aclk == 0) break;   // Decrement counter and break if it reaches 0

        IO0CLR = 0xF << 16;          // Clear all output pins (LEDs)
        IO0SET |= 1 << 16;          // Set pin 16 (LED 1) high
        delay_ms(10);                // Delay for 10 ms
        if (--no_aclk == 0) break;   // Decrement counter and break if it reaches 0
    } while (1); // Repeat until break

    IO0CLR = 0xFF << 16; // Clear all LEDs at the end
    while (1);           // Infinite loop to keep the program running
}

void delay_ms(unsigned int t) {
    unsigned int i, j;
    for (i = 0; i < t; i++)         // Loop for the specified number of milliseconds
        for (j = 0; j < 10000; j++); // Delay loop (approximate)
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
