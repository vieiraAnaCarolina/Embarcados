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

volatile unsigned char *ponteiro_pcicr = (volatile unsigned char *) 0x68;
volatile unsigned char *ponteiro_pcmsk2 = (volatile unsigned char *) 0x6D; 

void config(void) {
    cli();                                  // desativa as interrupções globais 

    *ponteiro_ddrb  |= 0x20;   
    *ponteiro_portb &= ~0x20;  
    
    *ponteiro_ddrd  &= ~0x04;               // pd2 habilitado como entrada
    *ponteiro_portd |= 0x04;  

    *ponteiro_pcicr |= 0x04;                // ativa o PCIE2*/
    *ponteiro_pcmsk2 |= 0x04;               // bit 2 do ponteiro

    sei();                                  // habilita as interrupções globais
}


ISR(PCINT2_vect) {
    estado = 1;                          // borda de descida foi detectada
    *ponteiro_pcmsk2 &= ~0x04; 
}

unsigned char filtragem(void) {
    _delay_ms(20);
    
    if ((*ponteiro_pind & 0x04) == 0) {     // se continuar em nível lógico baixo, a borda é válida
        return 1;
    }
    
    return 0; 
}

void acionamento_led(void) {
    if (estado == 1) {
        if (filtragem() == 1) {
            *ponteiro_portb ^= 0x20; 
        }
        
        estado = 0;                         // retorna ao estado inicial 
        *ponteiro_pcmsk2 |= 0x04;           // reabilita a interrupção INT0
        
    }
}

int main(void) {
    config();

    while (1) {
        acionamento_led(); 
    }

    return 0;
}