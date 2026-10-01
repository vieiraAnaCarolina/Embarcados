#define F_CPU 16000000UL
#include <avr/interrupt.h>

//ta confundindo OC0B e OCR0B
//verificar quem realmente tem que estar na saída
//lógica main certa

volatile unsigned char *ponteiro_TCCR0A  = (volatile unsigned char *) 0x44;
volatile unsigned char *ponteiro_TCCR0B  = (volatile unsigned char *) 0x45;
volatile unsigned char *ponteiro_OCR0A = (volatile unsigned char *) 0x47;
volatile unsigned char *ponteiro_OCR0B = (volatile unsigned char *) 0x48;
volatile unsigned char *ponteiro_OC0B = (volatile unsigned char *) 0x48;
volatile unsigned char *ponteiro_TIMSK0 = (volatile unsigned char *) 0x6E;

volatile unsigned char *ponteiro_ddrb = (volatile unsigned char *) 0x24;
volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25;
volatile unsigned char *ponteiro_ddrd    = (volatile unsigned char *) 0x2A;

int contador = 0;
int estado = 0;

void config(void){

    cli();

    *ponteiro_TCCR0A = 0x23;
    *ponteiro_TCCR0B = 0x0B;     

    *ponteiro_ddrd |= 0x20;       //pd5 como saída

    *ponteiro_OCR0A  = 0xF9;      //249
    *ponteiro_OCR0B  = 0x00;      //começa em 0
    
    *ponteiro_ddrb |= 0x20;       //led 13 como saída
    *ponteiro_portb &= ~0x20;     //led começa desligado

    *ponteiro_TIMSK0 = 0x04;      //ativa interrupçao canal a

    sei();
}

ISR(TIMER0_COMPB_vect){
    contador++;
}

int main(void){

    config();

    while(1){
        if (contador >= 4){
            if (estado == 0){
                *ponteiro_OCR0B += 1;
                contador = 0;
                *ponteiro_portb |= 0x20;
                if (*ponteiro_OCR0B == 250){
                    estado = 1;
                }
            }
            
            else if (estado == 1){
                *ponteiro_OCR0B -= 1;
                 contador = 0;
                 *ponteiro_portb &= ~0x20;
                  if (*ponteiro_OCR0B == 0){
                     estado = 0;
                    }
            }
        }

    } 

    return 0;

}