#include <xc.h>
#include "macaw.h"
#include "async.h"
#include "jwire.h"
#include "interrupts.h"
#include "interval.h"
#include "serial.h"
#include "globals.h"

#pragma config OSC = IRCIO7    // Oscillator Selection bits (Internal oscillator block, port function on RA7, clock on RA6)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)

void onSetup();
void onLoop();

int main() {
    //Set clock to 2MHZ (FOSC = 8MHZ)
    OSCCONbits.IRCF = 7;

   // AsyncBegin();
   // onSetup();

    GIE = 1;
    PEIE = 1;

    TRISC = 0b00011000; //RC3-RC4 INPUTS
    SSPM3 = 1; //set as I2C master
    SSPM2 = 0;
    SSPM1 = 0;
    SSPM0 = 0;
    SMP = 1; //disable slew rate control (running at 100khz)
    SSPEN = 1; //enable serial port

    SEN = 1; //create START condition
    SSPADD = 0x09; //set baud rate to 100khz
    SSPBUF = 0xA2; //buffer in number
}