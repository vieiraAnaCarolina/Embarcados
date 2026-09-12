/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char estado = 0;

volatile unsigned char *ponteiro_ddrb  = (volatile unsigned char *) 0x24; 
volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25; 
volatile unsigned char *ponteiro_ddrd  = (volatile unsigned char *) 0x2A; 
volatile unsigned char *ponteiro_portd = (volatile unsigned char *) 0x2B; 
volatile unsigned char *ponteiro_pind  = (volatile unsigned char *) 0x29; 

volatile unsigned char *ponteiro_EICRA = (volatile unsigned char *) 0x69; 
volatile unsigned char *ponteiro_EIMSK = (volatile unsigned char *) 0x3D; 

void config(void) {
    cli();

    *ponteiro_ddrb  |= 0x20;   
    *ponteiro_portb &= ~0x20;  
    
    *ponteiro_ddrd  &= ~0x04;  
    *ponteiro_portd |= 0x04;  


    *ponteiro_EICRA |= 0x02;
    *ponteiro_EICRA &= ~0x01;

    *ponteiro_EIMSK |= 0x01;   

    sei();
}

ISR(INT0_vect) {
    estado = 1; 
    *ponteiro_EIMSK &= ~0x01;
}


unsigned char filtragem(void) {
    _delay_ms(20);
    
    if ((*ponteiro_pind & 0x04) == 0) {
        return 1;
    }
    
    return 0; 
}

void acionamento_led(void) {
    if (estado == 1) {
        if (filtragem() == 1) {
            *ponteiro_portb ^= 0x20; 
            estado = 0;
            *ponteiro_EIMSK |= 0x01;
        }
        
        estado = 0; 
        *ponteiro_EIMSK |= 0x01;
        
    }
}

int main(void) {
    config();

    while (1) {
        acionamento_led(); 
    }

    return 0;
}
