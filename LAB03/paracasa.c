/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned int contador_de_pulsos;

volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25; // PORTB (pinos 8 a 13)
volatile unsigned char *ponteiro_portc = (volatile unsigned char *) 0x28; // PORTC (pinos A0 a A5)
volatile unsigned char *ponteiro_portd = (volatile unsigned char *) 0x2B; // PORTD (pinos 0 a 7)

volatile unsigned char *ponteiro_ddrb  = (volatile unsigned char *) 0x24; // DDRB
volatile unsigned char *ponteiro_ddrc  = (volatile unsigned char *) 0x27; // DDRC
volatile unsigned char *ponteiro_ddrd  = (volatile unsigned char *) 0x2A; // DDRD

config(){
    cli();
    
    //configurar os leds como saida e o A3 como entrada 
    // habilitar os pinos de interrupção (os que têm que ser habilitados estão no mapeamento)

    sei();
}

// Rotina de serviço de interrupção
ISR(PCINT1_vect) {
   int contador_de_pulsos++;
}

void forma_dezena(unsigned char digito) {

    // Apagar todos os displays
    *ponteiro_portb &= ~0x05; // Limpa os bits 0 e 2      0b00000101
    *ponteiro_portc &= ~0x12; // Limpa os bits 1 e 4      0b00010010
    *ponteiro_portd &= ~0x2C; // Limpa os bits 2, 3 e 5   0b00101100

    // Leds que serão acessos dependendo do número
    switch (digito) {
        case 0:  // a, b, c, d, e, f
            *ponteiro_portb |= 0x01;   // 0b00000001
            *ponteiro_portc |= 0x12;   // 0b00010010
            *ponteiro_portd |= 0x2C;   // 0b00101100
            break;

        case 1:  // b, c
            *ponteiro_portb |= 0x01;   // 0b00000001
            *ponteiro_portc |= 0x10;   // 0b00010000
            break;

        case 2:  // a, b, d, e, g
            *ponteiro_portb |= 0x04;   // 0b00000100
            *ponteiro_portc |= 0x12;   // 0b00010010
            *ponteiro_portd |= 0x24;   // 0b00100100
            break;

        case 3:  // a, b, c, d, g
            *ponteiro_portb |= 0x05;   // 0b00000101
            *ponteiro_portc |= 0x12;   // 0b00010010
            *ponteiro_portd |= 0x20;   // 0b00100000
            break;

        case 4:  // b, c, f, g
            *ponteiro_portb |= 0x05;   // 0b00000101
            *ponteiro_portc |= 0x10;   // 0b00010000
            *ponteiro_portd |= 0x08;   // 0b00001000
            break;

        case 5:  // a, c, d, f, g
            *ponteiro_portb |= 0x05;   // 0b00000101
            *ponteiro_portc |= 0x02;   // 0b00000010
            *ponteiro_portd |= 0x28;   // 0b00101000
            break;

        case 6:  // a, c, d, e, f, g
            *ponteiro_portb |= 0x05;   // 0b00000101
            *ponteiro_portc |= 0x02;   // 0b00000010
            *ponteiro_portd |= 0x2C;   // 0b00101100
            break;

        case 7:  // a, b, c
            *ponteiro_portb |= 0x01;   // 0b00000001
            *ponteiro_portc |= 0x12;   // 0b00010010
            break;

        case 8:  // a, b, c, d, e, f, g
            *ponteiro_portb |= 0x05;   // 0b00000101
            *ponteiro_portc |= 0x12;   // 0b00010010
            *ponteiro_portd |= 0x2C;   // 0b00101100
            break;

        case 9:  // a, b, c, d, f, g
            *ponteiro_portb |= 0x05;   // 0b00000101
            *ponteiro_portc |= 0x12;   // 0b00010010
            *ponteiro_portd |= 0x28;   // 0b00101000
            break;
    }
}

void forma_centena(unsigned char digito) {

    // Apagar todos os displays
    *ponteiro_portb &= ~0x2A; // Limpa os bits 1, 3 e 5  (0b00101010)
    *ponteiro_portc &= ~0x11; // Limpa os bits 0 e 5     (0b00010001)
    *ponteiro_portd &= ~0x90; // Limpa os bits 4 e 7     (0b10010000)

    // Leds que serão acessos dependendo do número
    switch (digito) {
        case 0:  // a, b, c, d, e, f
            *ponteiro_portb |= 0x2A;   // 0b00101010
            *ponteiro_portc |= 0x20;   // 0b00100000 
            *ponteiro_portd |= 0x90;   // 0b10010000
            break;

        case 1:  // b, c
            *ponteiro_portb |= 0x02;   // 0b00000010
            *ponteiro_portd |= 0x10;   // 0b00010000
            break;

        case 2:  // a, b, d, e, g
            *ponteiro_portb |= 0x0A;   // 0b00001010
            *ponteiro_portc |= 0x21;   // 0b00100001
            *ponteiro_portd |= 0x80;   // 0b10000000
            break;

        case 3:  // a, b, c, d, g
            *ponteiro_portb |= 0x0A;   // 0b00001010
            *ponteiro_portc |= 0x21;   // 0b00100001
            *ponteiro_portd |= 0x10;   // 0b00010000
            break;

        case 4:  // b, c, f, g
            *ponteiro_portb |= 0x22;   // 0b00100010
            *ponteiro_portc |= 0x01;   // 0b00000001
            *ponteiro_portd |= 0x10;   // 0b00010000
            break;

        case 5:  // a, c, d, f, g
            *ponteiro_portb |= 0x28;   // 0b00101000
            *ponteiro_portc |= 0x21;   // 0b00100001
            *ponteiro_portd |= 0x10;   // 0b00010000
            break;

        case 6:  // a, c, d, e, f, g
            *ponteiro_portb |= 0x28;   // 0b00101000
            *ponteiro_portc |= 0x21;   // 0b00100001
            *ponteiro_portd |= 0x90;   // 0b10010000
            break;

        case 7:  // a, b, c
            *ponteiro_portb |= 0x02;   // 0b00000010
            *ponteiro_portc |= 0x20;   // 0b00100000
            *ponteiro_portd |= 0x10;   // 0b00010000
            break;

        case 8:  // a, b, c, d, e, f, g
            *ponteiro_portb |= 0x2A;   // 0b00101010
            *ponteiro_portc |= 0x21;   // 0b00100001
            *ponteiro_portd |= 0x90;   // 0b10010000
            break;

        case 9:  // a, b, c, d, f, g
            *ponteiro_portb |= 0x2A;   // 0b00101010
            *ponteiro_portc |= 0x21;   // 0b00100001
            *ponteiro_portd |= 0x10;   // 0b00010000
            break;
    }
}

main (){

    
}