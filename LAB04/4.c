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

volatile unsigned char caractere = 0;
volatile unsigned char novo_dado = 0;

void config(void){
    cli();

    *ponteiro_UBRR0H = 0x00;
    *ponteiro_UBRR0L = 0x10;                  // Baud rate 115200 
    *ponteiro_UCSR0A = 0x02;                  // Double-speed 
    *ponteiro_UCSR0B = 0x98;                  // Ativa RXEN0 (Bit 4), TXEN0 (Bit 3) e RXCIE0 (Bit 7 - Interrupção de Recepção Completa)
    *ponteiro_UCSR0C = 0x06;                  // Modo assíncrono, 8 bits de dados, sem paridade, 1 stop bit

    sei();
}


ISR(USART_RX_vect){
    caractere = *ponteiro_UDR0;               // Lê o caractere recebido do buffer serial
    novo_dado = 1;                            // Sinaliza que há um novo dado para ser ecoado
}

int main (void){
    config();

    while(1){
        // Se a interrupção recebeu um caractere, envia ele de volta 
        if (novo_dado == 1){
            // Aguarda o registrador de dados de transmissão (UDRE0 - bit 5 do UCSR0A) ficar livre
            while (!(*ponteiro_UCSR0A & 0x20)); 
            
            *ponteiro_UDR0 = caractere;       // Devolve o caractere recebido para o computador
            novo_dado = 0;                    // Reseta a flag
        }
    }

    return 0;
}