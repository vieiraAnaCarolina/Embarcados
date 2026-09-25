/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>

// Configurações 
volatile unsigned char *ponteiro_UBRR0H = (volatile unsigned char *) 0xC5;
volatile unsigned char *ponteiro_UBRR0L = (volatile unsigned char *) 0xC4;
volatile unsigned char *ponteiro_UCSR0A = (volatile unsigned char *) 0xC0;
volatile unsigned char *ponteiro_UCSR0B = (volatile unsigned char *) 0xC1;
volatile unsigned char *ponteiro_UCSR0C = (volatile unsigned char *) 0xC2;
volatile unsigned char *ponteiro_UDR0   = (volatile unsigned char *) 0xC6;

volatile unsigned char *ponteiro_ddrb  = (volatile unsigned char *) 0x24;
volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25;

void config(void){
    cli();

    *ponteiro_UBRR0H = 0x00;
    *ponteiro_UBRR0L = 0x10;                  // Baud rate 115200
    *ponteiro_UCSR0A = 0x02;                  // Double-speed 
    *ponteiro_UCSR0B = 0x48;                  // Ativa TXEN0 (Bit 3) e TXCIE0 (Bit 6 - Interrupção de Transmissão Completa)
    *ponteiro_UCSR0C = 0x06;                  // Modo assíncrono, 8 bits de dados, sem paridade, 1 stop bit

    *ponteiro_ddrb  |= 0x20;                  // Configura LED como saída

    sei();
}

volatile int indice = 0;
char msg_TXC0 [] = "Transmissao serial utilizando a interrupcao USART Transmit Complete. \n\n";

// Rotina de Serviço de Interrupção de Transmissão Completa (TXC)
ISR(USART_TX_vect){
    indice++;

    if(msg_TXC0[indice] == '\0') {
        indice = 0;                           // Se chegou ao fim da string, reinicia o índice
    } else {
        *ponteiro_UDR0 = msg_TXC0[indice];    // Envia o próximo caractere
    }
}

int main (void){
    config();

    while(1){
    
        if (indice == 0){
            *ponteiro_UDR0 = msg_TXC0[indice];
        }

        *ponteiro_portb |= 0x20;              // Acende o LED
        _delay_ms(500);                       // Aguarda 500ms
        *ponteiro_portb &= ~0x20;             // Apaga o LED
    }

    return 0;
}