#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>

// Configurações (considerando 1ms)
volatile unsigned char *ponteiro_TCCR0A = (volatile unsigned char *) 0x44;
volatile unsigned char *ponteiro_TCCR0B = (volatile unsigned char *) 0x45;
volatile unsigned char *ponteiro_TIMSK0 = (volatile unsigned char *) 0x6E;
volatile unsigned char *ponteiro_OCR0A  = (volatile unsigned char *) 0x47;

unsigned char *ponteiro_ddrb  = (unsigned char *) 0x24; //BIT 5 LED DDRB    
unsigned char *ponteiro_portb = (unsigned char *) 0x25; //BIT 5 LED PORTB
unsigned char *ponteiro_ddrd  = (unsigned char *) 0x2A; //BIT 2 BOTAO DDRD
unsigned char *ponteiro_pind  = (unsigned char *) 0x29; //BIT 2 BOTAO PIND 

int botao = 0;
int toggle = 0;
int tempo = 20;
int contador = 0;
int estado = 0;
int estado_b = 0;
int filtrado = 0;

void config(void){

    cli();
    
    *ponteiro_ddrb  |= 0x20;  // Liga o BIT 5 do ddrb do led para 1 (SAIDA)
    *ponteiro_ddrd  &= ~0x04; // Liga o BIT 2 do ddrd do botão para 0 (ENTRADA)

    *ponteiro_OCR0A  = 0xFA;  // 250
    *ponteiro_TIMSK0 = 0x02;
    *ponteiro_TCCR0B = 0x03;  // prescaler = 64
    *ponteiro_TCCR0A = 0x02; 
    *ponteiro_portb &= ~0x20;

    sei();

}


int detecta_borda(int entrada){
        if ((estado == 0)&&(entrada == 1)){
            estado = 0;
            toggle = 1;
        }
        else if  ((estado == 0)&& entrada == 0){
            estado = 1;
            toggle = 0;
        }
        else if ((estado == 1)&& entrada == 0){
            estado = 1;
            toggle = 0;
        }
        else if  ((estado == 1)&& entrada == 1){
            estado = 2;
            toggle = 0;
        }
        else if  ((estado == 2)&& entrada == 1){
            estado = 2;
            toggle = 0;
        }
        else if  ((estado == 2)&& entrada == 0){
            estado = 3;
            toggle = 1;
        }
        else if  ((estado ==3) && entrada == 0){
            estado = 3;
            toggle = 1;
        }
        else if  ((estado == 3)&& entrada == 1){
            estado = 0;
            toggle = 1;
        }
        return toggle;
    }
    

int debounce(volatile unsigned char botao){
        if ((estado_b == 0)&&(botao == 4)){
            estado_b = 0;
            filtrado = 1;
     
        }
        else if ((estado_b == 0)&& botao == 0){
            estado_b = 1;
            filtrado = 1;
            contador = 0;
        }
        else if ((estado_b == 1)&& (botao == 0) && (contador >= tempo)){
            estado_b = 2;
            filtrado = 0;
            contador = 0;
        }
        else if (((estado_b == 1)&& botao == 4) && (contador >= tempo)){
            estado_b = 0;
            filtrado = 1;
            contador = 0;
        }
         else if  ((estado_b == 1)&& contador < tempo){
            estado_b = 1;
            filtrado = 1;
           
        }
         else if  ((estado_b == 2)&& botao == 4){
            estado_b = 3;
            filtrado = 0;
            contador = 0;
        }
         else if  ((estado_b == 2)&& botao == 0){
            estado_b = 2;
            filtrado = 0;
            
        }
         else if  ((estado_b == 3)&& (botao == 0) && (contador >= tempo)){
            estado_b = 2;
            filtrado = 0;
            contador = 0;
        }
         else if  ((estado_b == 3)&& (botao == 4) && (contador >= tempo)){
            estado_b = 0;
            filtrado = 1;
            contador = 0;
        }
         else if  ((estado_b == 3)&& contador < tempo){
            estado_b = 3;
            filtrado = 0;
          
        }
        return filtrado;
    }
     


ISR(TIMER0_COMPA_vect){
    contador++;
}

int main(void){
    config();
    while(1){
      
        int saida =  debounce(*ponteiro_pind & 0x04);
        
        int led = detecta_borda(saida);
        

        if (led == 0){
            *ponteiro_portb &= ~0x20;
        } 
        else{
            *ponteiro_portb |= 0x20; 
        }
          
    }

}

