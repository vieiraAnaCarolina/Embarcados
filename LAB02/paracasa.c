/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <util/delay.h>

int main(void) {

unsigned char *ponteiro_ddrb  = (volatile unsigned char *) 0x24; // DDRB
unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25; // PORTB
unsigned char *ponteiro_ddrd  = (volatile unsigned char *) 0x2A; // DDRD
unsigned char *ponteiro_portd = (volatile unsigned char *) 0x2B; // PORTD
unsigned char *ponteiro_pind  = (volatile unsigned char *) 0x29; // PIND

// Configuração dos pinos da PORTB como SAÍDA:
*ponteiro_ddrb |= 0x02; // LED VERMELHO
*ponteiro_ddrb |= 0x04; // LED VERDE
*ponteiro_ddrb |= 0x08; // LED AZUL
*ponteiro_ddrb |= 0x10; // LED INDICADOR

// Botão como entrada
*ponteiro_ddrd &= ~0x08;

// Todos leds desligados
*ponteiro_portb &= ~0x02; 
*ponteiro_portb &= ~0x04; 
*ponteiro_portb &= ~0x08; 
*ponteiro_portb &= ~0x10; 

unsigned char estado_anterior = 0x08;
 
while(1){
    unsigned char estado_atual = (*ponteiro_pind & 0x08);
    
    if (estado_atual == 0) {
            *ponteiro_portb |= 0x10;
        } else {
            *ponteiro_portb &= ~0x10;
        }
    
        unsigned char vermelho = ((*ponteiro_portb & 0x02) != 0); 
        unsigned char verde = ((*ponteiro_portb & 0x04) != 0);
        unsigned char azul = ((*ponteiro_portb & 0x08) != 0);

        // Verifica quais leds estão ligados para decidir quais os próximos que serão
        // VERMELHO -> VERDE AZUL -> BRANCO -> NENHUM
        if ((estado_anterior == 0x08) && (estado_atual == 0)) {
            _delay_ms(20); 

            if (!vermelho && !verde && !azul) {
                *ponteiro_portb |= 0x02;
            }
         
            else if (vermelho && !verde && !azul) {
                *ponteiro_portb &= ~0x02;
                *ponteiro_portb |= 0x04;  
            }
           
            else if (!vermelho && verde && !azul) {
                *ponteiro_portb &= ~0x04;
                *ponteiro_portb |= 0x08; 
            }
            
            else if (!vermelho && !verde && azul) {
                *ponteiro_portb |= 0x02;  
                *ponteiro_portb |= 0x04; 
                                         
            }
          
            else if (vermelho && verde && azul) {
                *ponteiro_portb &= ~0x0E; 
            }
        }

        estado_anterior = estado_atual;
    }

    return 0;
}
