#define F_CPU 16000000UL
#include <avr/interrupt.h>

volatile unsigned char *ponteiro_TCCR0A  = (volatile unsigned char *) 0x44;
volatile unsigned char *ponteiro_TCCR0B  = (volatile unsigned char *) 0x45;
volatile unsigned char *ponteiro_OCR0A = (volatile unsigned char *) 0x47;
volatile unsigned char *ponteiro_OCR0B = (volatile unsigned char *) 0x48;
volatile unsigned char *ponteiro_OC0B = (volatile unsigned char *) 0x2A;


void config(void){

    cli();

    *ponteiro_TCCR0A = 0x23;
    *ponteiro_TCCR0B = 0x0B;     

    *ponteiro_OC0B |= 0x20;        //0C0B como saída
    
    *ponteiro_OCR0A  = 0xF9;         //249
    *ponteiro_OCR0B  = 0x32;         //50


    sei();
}

int main(void){

    config();

    while(1){

        } 

}