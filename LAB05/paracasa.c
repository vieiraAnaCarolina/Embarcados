/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

/*
 1) a cada  0,5 s (500 ciclos do temporizador) é realizada a mudança de estado do led 13
 2) a cada 0,78 s (780 ciclos do temporizador) é realizada a mudança de estado do led 12
 3) a cada 5 s é realizada a interupção TIMER0_COMPA_vect e é transmitida a mensagem 

*/

/*
os valores escolhidos foram:
    parser: 64
    valor máximo: 250

    Frequência = 16 MHZ/ 64 = 0,25 MHZ

    Período = 1/frequencia = 4x10⁻⁶ 

    Tempo gasto = T x valor maximo = 4x10⁻⁶ x 250 = 1x10⁻³ = 1 ms
    O valor de 1ms foi escolhido, para podermos determinar mais facilmente a quantidade de ciclos para o envio da mensagem e o tempo que os leds ficam em cada estado

    Quantidade de ciclos:
        0,5/1x10⁻³ = 500 ciclos
        0,78/1x10⁻³ = 780 ciclos

*/ 

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

volatile unsigned char *ponteiro_TCCR0A = (volatile unsigned char *) 0x44;
volatile unsigned char *ponteiro_TCCR0B = (volatile unsigned char *) 0x45;
volatile unsigned char *ponteiro_TIMSK0 = (volatile unsigned char *) 0x6E;
volatile unsigned char *ponteiro_OCR0A  = (volatile unsigned char *) 0x47;
// Leds

volatile unsigned char *ponteiro_ddrb  = (volatile unsigned char *) 0x24; // DDRB
volatile unsigned char *ponteiro_portb = (volatile unsigned char *) 0x25; // PORTB

int indice = 0;
int tempo_led12 = 0;
int tempo_led13 =0;
char msg[] = "Atividade 5 – Interrupcoes periodicas do temporizador permitem a temporização de processos do sistema sem espera ativa! \n\n";

void config(void){
    cli();

    *ponteiro_UBRR0H = 0x00;                  // Baud rate 19200
    *ponteiro_UBRR0L = 0x33;                  // Baud rate 19200 
    *ponteiro_UCSR0A = 0x00;                  // Normal-speed 
    *ponteiro_UCSR0B = 0x48;                  // Ativa TXEN0 (Bit 3), TXCIE0
    *ponteiro_UCSR0C = 0x06;                  // Modo assíncrono, 8 bits de dados, sem paridade, 1 stop bit
    
    *ponteiro_OCR0A  = 0xFA; // 250
    *ponteiro_TIMSK0 = 0x02;
    *ponteiro_TCCR0B = 0x03; // prescaler = 64
    *ponteiro_TCCR0A = 0x02; 


    // Configuração dos pinos da PORTB como SAÍDA:
    *ponteiro_ddrb |= 0x20; // led 13
    *ponteiro_ddrb |= 0x10; // led 12
    

    // Todos leds desligados
    *ponteiro_portb &= ~0x20; // led 13
    *ponteiro_portb &= ~0x10; // led 12

    sei();
}

ISR(TIMER0_COMPA_vect){
    tempo_led12++;
    tempo_led13++;

    if(tempo_led13 >= 500){ // muda o estado do led 13 a cada 0,5 s
        *ponteiro_portb ^=0x20;
        tempo_led13 = 0;
    }  

    if(tempo_led12 >= 780){ // muda o estado do led 12 a cada 0,78 s
        *ponteiro_portb ^=0x10;
        tempo_led12 = 0;
    }  
}


ISR(USART_TX_vect){
    indice++;
    if(msg[indice] == '\0'){ // percorre toda a mensagem até o seu final
        indice = 0;
    }

    else{
        *ponteiro_UDR0 = msg[indice];
    }
}

int main(void){
    config();

    while(1){

        if(indice == 0){// envia a mensagem em intervalos de 5 s
            *ponteiro_UDR0 = msg[indice];
            _delay_ms(5000); // espera 5 s
        }    
        
    }
    return 0;
}
