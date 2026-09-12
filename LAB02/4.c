/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <util/delay.h>

int main(void) {
    
    unsigned char *ponteiro_ddrb  = (unsigned char *) 0x24; //BIT 5 LED DDRB    
    unsigned char *ponteiro_portb = (unsigned char *) 0x25; //BIT 5 LED PORTB
    unsigned char *ponteiro_ddrd  = (unsigned char *) 0x2A; //BIT 2 BOTAO DDRD
    unsigned char *ponteiro_pind  = (unsigned char *) 0x29; //BIT 2 BOTAO PIND 

    // Configurações dos pinos
    *ponteiro_ddrb  |= 0x20;  // Liga o BIT 5 do ddrb do led para 1 (SAIDA)
     *ponteiro_ddrd  &= ~0x04; // Liga o BIT 2 do ddrd do botão para 0 (ENTRADA)

    unsigned char estado_anterior = 0x04;
    
    while (1) {
        
        unsigned char estado_atual = (*ponteiro_pind & 0x04);
        //Apenas troca o LED quando botão apertado (estado atual 0)
        if (estado_anterior == 0x04 && estado_atual == 0){ 
            _delay_ms(20); 
            *ponteiro_portb ^= 0x20;
            
        } 
        
        //Enquanto o botão estiver apertado ele vai se manter em 0, quando soltar o botão o anterior volta para 0x04
        estado_anterior = estado_atual; 
    }
    return 0;
}
