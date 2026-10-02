#define F_CPU 16000000UL
#include <avr/interrupt.h>

volatile unsigned char *ponteiro_TCCR0A  = (volatile unsigned char *) 0x44;
volatile unsigned char *ponteiro_TCCR0B  = (volatile unsigned char *) 0x45;
volatile unsigned char *ponteiro_OCR0A = (volatile unsigned char *) 0x47;
volatile unsigned char *ponteiro_OCR0B = (volatile unsigned char *) 0x48;
volatile unsigned char *ponteiro_TIMSK0 = (volatile unsigned char *) 0x6E;

volatile unsigned char *ponteiro_ddrb = (volatile unsigned char *) 0x24;
volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25;
volatile unsigned char *ponteiro_ddrd    = (volatile unsigned char *) 0x2A;

int contador = 0;
int estado = 0;

void config(void){

    cli();

    *ponteiro_ddrd |= 0x20;       // PD5 (OC0B) como saída
    *ponteiro_ddrb |= 0x20;       // PB5 (LED 13) como saída
    *ponteiro_portb &= ~0x20;

    *ponteiro_OCR0A  = 249;       // TOP
    *ponteiro_OCR0B  = 0;
    *ponteiro_TCCR0A = 0x23;      // COM0B1, WGM01, WGM00
    *ponteiro_TIMSK0 = 0x02;      // OCIE0A

    *ponteiro_TCCR0B = 0x0B;      // WGM02 + prescaler 64 

    sei();
}

ISR(TIMER0_COMPA_vect){
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
                if (*ponteiro_OCR0B >= 249){
                    estado = 1;
                }
            }
            
            else if (estado == 1){
                *ponteiro_OCR0B -= 1;
                 contador = 0;
                 *ponteiro_portb &= ~0x20;
                  if (*ponteiro_OCR0B <= 0){
                     estado = 0;
                    }
            }
        }

    } 

    return 0;

}
