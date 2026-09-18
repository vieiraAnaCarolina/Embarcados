/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char estado = 0;

volatile unsigned char *ponteiro_ddrb  = (volatile unsigned char *) 0x24; 
volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25; 

volatile unsigned char *ponteiro_ddrd  = (volatile unsigned char *) 0x2A; 
volatile unsigned char *ponteiro_pind  = (volatile unsigned char *) 0x29; 

volatile unsigned char *ponteiro_EICRA = (volatile unsigned char *) 0x69; 
volatile unsigned char *ponteiro_EIMSK = (volatile unsigned char *) 0x3D; 
volatile unsigned char *ponteiro_EIFR =  (volatile unsigned char *) 0x1C;

void config(void) {
    cli();                               // desativa as interrupções globais 

    *ponteiro_ddrb  |= 0x20;             // led como saida
    *ponteiro_portb &= ~0x20;            // led começa desligado
    *ponteiro_ddrd  &= ~0x04;            // pd2 como entrada

    *ponteiro_EICRA |= 0x02;
    *ponteiro_EICRA &= ~0x01;
    *ponteiro_EIMSK |= 0x01;   

    sei();                               // habilita as interrupções globais
}



ISR(INT0_vect) {
    estado = 1;                          // borda de descida foi detectada
    *ponteiro_EIMSK &= ~0x01;            // desabilita INT0 
}


unsigned char filtragem(void) {
    _delay_ms(20);
    
    if ((*ponteiro_pind & 0x04) == 0) {  // se continuar em nível lógico baixo, a borda é válida
        return 1;
    }
    
    return 0; 
}

void acionamento_led(void) {
    if (estado == 1) {
        if (filtragem() == 1) {
            *ponteiro_portb ^= 0x20; 
        }
        
        estado = 0;                      // retorna ao estado inicial
        *ponteiro_EIFR  |= 0x01;          // zera as flags de interrupção
        *ponteiro_EIMSK |= 0x01;         // reabilita a interrupção INT0
        
    }
}

int main(void) {
    config();

    while (1) {
        acionamento_led(); 
    }

    return 0;
}