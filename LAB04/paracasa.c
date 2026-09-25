/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#define F_CPU 16000000UL
#include <avr/io.h>        
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char *ponteiro_UBRR0H = (volatile unsigned char *) 0xC5;
volatile unsigned char *ponteiro_UBRR0L = (volatile unsigned char *) 0xC4;
volatile unsigned char *ponteiro_UCSR0A = (volatile unsigned char *) 0xC0;
volatile unsigned char *ponteiro_UCSR0B = (volatile unsigned char *) 0xC1;
volatile unsigned char *ponteiro_UCSR0C = (volatile unsigned char *) 0xC2;
volatile unsigned char *ponteiro_UDR0   = (volatile unsigned char *) 0xC6;

volatile unsigned char *ponteiro_ddrb   = (volatile unsigned char *) 0x24; 
volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25; 

volatile unsigned char rx_char = 'd';       
volatile unsigned char comando_ativo = 'd'; 
volatile unsigned char novo_dado = 0;
volatile unsigned char indice = 0;
volatile const char *msg_TXC0 = 0;

const char msg_desligado[]  = "Desligado\n";
const char msg_vermelho[]   = "Pisca vermelho\n";
const char msg_verde[]      = "Pisca verde\n";
const char msg_azul[]       = "Pisca azul\n";
const char msg_todos[]      = "Pisca todos\n";
const char msg_erro[]       = "Comando incorreto\n";

void config(void);
void enviar_mensagem(const char *msg);
void processar_entrada(void);
void atualizar_leds(void);

void config(void){
    cli();

    *ponteiro_UBRR0H = 0x00;
    *ponteiro_UBRR0L = 0x33;                 
    *ponteiro_UCSR0A = 0x00;                 
    *ponteiro_UCSR0B = 0xD8; 
    *ponteiro_UCSR0C = 0x06; 

    *ponteiro_ddrb |= 0x08; 
    *ponteiro_ddrb |= 0x10; 
    *ponteiro_ddrb |= 0x20; 

    *ponteiro_portb &= ~0x38; 

    sei();
}

void enviar_mensagem(const char *msg){
    msg_TXC0 = msg;
    indice = 0;
    *ponteiro_UDR0 = msg_TXC0[indice]; 
}

ISR(USART_RX_vect){
    unsigned char dado_recebido = *ponteiro_UDR0;
    
    if (dado_recebido != '\n' && dado_recebido != '\r') {
        rx_char = dado_recebido;
        novo_dado = 1;
    }
}

ISR(USART_TX_vect){
    indice++;
    if(msg_TXC0[indice] != '\0') { 
        *ponteiro_UDR0 = msg_TXC0[indice]; 
    }
}

void processar_entrada(void){
    switch (rx_char){
        case 'd':
            comando_ativo = 'd';
            enviar_mensagem(msg_desligado);
            break;
        case 'r':
            comando_ativo = 'r';
            enviar_mensagem(msg_vermelho);
            break;
        case 'g':
            comando_ativo = 'g';
            enviar_mensagem(msg_verde);
            break;
        case 'b':
            comando_ativo = 'b';
            enviar_mensagem(msg_azul);
            break;
        case 't':
            comando_ativo = 't';
            enviar_mensagem(msg_todos);
            break;
        default:
            enviar_mensagem(msg_erro);
            break;
    }
}

void atualizar_leds(void){
    switch (comando_ativo){
        case 'd':
            *ponteiro_portb &= ~0x38;
            break;
            
        case 'r': 
            *ponteiro_portb |= 0x10;  
            _delay_ms(200);  
            *ponteiro_portb &= ~0x10; 
            _delay_ms(200);  
            break;
                    
        case 'g': 
            *ponteiro_portb |= 0x08;  
            _delay_ms(200);  
            *ponteiro_portb &= ~0x08; 
            _delay_ms(200);  
            break;

        case 'b': 
            *ponteiro_portb |= 0x20;  
            _delay_ms(200);  
            *ponteiro_portb &= ~0x20; 
            _delay_ms(200);  
            break;

        case 't': 
            *ponteiro_portb |= 0x38;  
            _delay_ms(200);  
            *ponteiro_portb &= ~0x38; 
            _delay_ms(200);  
            break;
    }
}

int main (void){
    config();

    while(1){

        if (novo_dado == 1){
            novo_dado = 0;
            processar_entrada();
        }

        atualizar_leds();
    }

    return 0;
}