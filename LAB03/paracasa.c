/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned int contador_de_pulsos = 0;
unsigned long rpm = 0; 

volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25;   // PORTB (Pinos digitais 8 a 13)
volatile unsigned char *ponteiro_portc = (volatile unsigned char *) 0x28;   // PORTC (Pinos analógicos A0 a A5 / digitais 14 a 19)
volatile unsigned char *ponteiro_portd = (volatile unsigned char *) 0x2B;   // PORTD (Pinos digitais 0 a 7)

volatile unsigned char *ponteiro_ddrb  = (volatile unsigned char *) 0x24;   
volatile unsigned char *ponteiro_ddrc  = (volatile unsigned char *) 0x27;   
volatile unsigned char *ponteiro_ddrd  = (volatile unsigned char *) 0x2A;   

volatile unsigned char *ponteiro_PCICR   = (volatile unsigned char *) 0x68; 
volatile unsigned char *ponteiro_PCMSK0  = (volatile unsigned char *) 0x6B;
volatile unsigned char *ponteiro_PCMSK1  = (volatile unsigned char *) 0x6C; 
volatile unsigned char *ponteiro_PCMSK2  = (volatile unsigned char *) 0x6D; 
volatile unsigned char *ponteiro_PCIFR   = (volatile unsigned char *) 0x3B; 

void config(void){

    cli();
    
    *ponteiro_PCICR |= 0x02;       // Habilita PCIER2/1/0 (00000111)

    *ponteiro_ddrb |= 0x20;        // LEDB SAIDA
    *ponteiro_ddrc |= 0x20;        // LEDC SAIDA
    *ponteiro_ddrd |= 0x04;        // LEDD SAIDA

    //verificar cada um que deve ser setado como 1 nos registradores
    *ponteiro_PCMSK0 |= 0x08;      // 
    *ponteiro_PCMSK1 |= 0x08;      // 
    *ponteiro_PCMSK2 |= 0x08;      // 

    *ponteiro_ddrc &= ~0x08;       //Bit 3 do DDRC como ENTRADA

    sei();
}


ISR(PCINT1_vect) {
    contador_de_pulsos++;
   
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
    *ponteiro_portc &= ~0x21; // Limpa os bits 0 e 5     (0b00100001)
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

int main (void){
    int numero_de_pulsos = 0;
    int dezena = 0;
    int centena = 0;

    config();

    while(1){  
     
        cli();
        contador_de_pulsos = 0;
        sei();

     
        _delay_ms(1000);

        cli();
        numero_de_pulsos = contador_de_pulsos;
        sei();

        rpm = ((unsigned long)numero_de_pulsos * 60) / 98;

        centena = (rpm / 100) % 10;
        dezena  = (rpm / 10) % 10;

        forma_dezena(dezena);
        forma_centena(centena);
    }

    return 0;
}