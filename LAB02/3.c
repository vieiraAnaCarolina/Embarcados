/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <util/delay.h>

int main(void) {
unsigned char *ponteiro_ddrb = (unsigned char *) 0x24; //BIT 5 LED DDRB
unsigned char *ponteiro_portb = (unsigned char *) 0x25; //BIT 4 LED PORTB

unsigned char *ponteiro_ddrd = (unsigned char *) 0x2A; //BIT 2 BOTAO DDRD
unsigned char *ponteiro_pind = (unsigned char *) 0x29; //BIT 2 BOTAO PIND

//LIGA DDBR DO BOTÃO E DO LED
*ponteiro_ddrb |= 0x20;  //PERMITE ENTRADA BIT 5 LED
*ponteiro_ddrd &= ~0x04; //em 0 = entrada para não dar problema


while (1) {
if ((*ponteiro_pind & 0x04) == 0) { //SE O BOTÃO É PRECIONADO (SE 2 FOR 0)
*ponteiro_portb |= 0x20; // BIT 5 DO LED É 1 (LIGA O LED)
} else { //SE NÃO (SE 2 FOR 1)
*ponteiro_portb &= ~0x20; // BIT 5 DO LED É 0 (DESLIGA O LED)
}
}

return 0;
}
