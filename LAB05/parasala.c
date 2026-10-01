#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>

//Configurações
volatile unsigned char *ponteiro_TCCR0A = (volatile unsigned char *) 0x44;
volatile unsigned char *ponteiro_TCCR0B = (volatile unsigned char *) 0x45;
volatile unsigned char *ponteiro_TIMSK0 = (volatile unsigned char *) 0x6E;

volatile unsigned char *ponteiro_ddrb = (volatile unsigned char *) 0x24;  
volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25; 

int tempo_chegado = 0;

void config(void){
    cli();

    *ponteiro_TCCR0A = 0x00;
    *ponteiro_TCCR0B = 0x02;
    *ponteiro_TIMSK0 = 0x01;
    

    *ponteiro_ddrb = *ponteiro_ddrb | 0x20;   
    *ponteiro_portb &= ~0x20;                 
    
    
    sei();
}


ISR(TIMER0_OVF_vect){

    tempo_chegado++;

}


int main(void){

    config();

    while(1){
        if (tempo_chegado >= 2000){
            *ponteiro_portb ^= 0x20; 
            tempo_chegado = 0;
        } 
        
    }
    
    return 0;
}