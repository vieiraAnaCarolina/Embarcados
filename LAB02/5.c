/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <util/delay.h>

int main(void) {

unsigned char *ponteiro_ddrb = (unsigned char *) 0x24; //BIT 5 LED DDRB
unsigned char *ponteiro_portb = (unsigned char *) 0x25; //BIT 4 LED PORTB
unsigned char *ponteiro_ddrd = (unsigned char *) 0x2A; //BIT 2 BOTAO DDRD
unsigned char *ponteiro_pind = (unsigned char *) 0x29; //BIT 2 BOTAO PIND

// Configurações dos pinos
*ponteiro_ddrb |= 0x20; // Liga o BIT 5 do ddrb do led para 1 (SAIDA)
*ponteiro_ddrd &= ~0x04; // Liga o BIT 2 do ddrd do botão para 0 (ENTRADA)

int anterior = 0x04;

while (1) {
	int atual=*ponteiro_pind & 0x04;
if (anterior == 0x04 && atual == 0) { //BORDA DE DESCIDA
	_delay_ms(20); 
	int atual=*ponteiro_pind & 0x04;
	if((atual == 0)){ 
	    *ponteiro_portb |= 0x20; // BIT 5 DO LED É 1 (LIGA O LED)
}
}

if (anterior == 0 && atual == 0x04) { //BORDA DE SUBIDA
	_delay_ms(20); 
	int atual=*ponteiro_pind & 0x04;
	if((atual == 0x04)){
	    *ponteiro_portb &= ~0x20; // BIT 5 DO LED É 0 (DESLIGA O LED)

}
}
anterior = atual;
}
return 0;
}




