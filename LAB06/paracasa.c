/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */
#define F_CPU 16000000UL
#include <avr/interrupt.h>

volatile unsigned char *ponteiro_DDRB   = (volatile unsigned char *) 0x24;

volatile unsigned char *ponteiro_TCCR1A = (volatile unsigned char *) 0x80;
volatile unsigned char *ponteiro_TCCR1B = (volatile unsigned char *) 0x81;
volatile unsigned char *ponteiro_TIMSK1 = (volatile unsigned char *) 0x6F;

volatile unsigned char *ponteiro_OCR1AH = (volatile unsigned char *) 0x89;
volatile unsigned char *ponteiro_OCR1AL = (volatile unsigned char *) 0x88;
volatile unsigned char *ponteiro_OCR1BH = (volatile unsigned char *) 0x8B;
volatile unsigned char *ponteiro_OCR1BL = (volatile unsigned char *) 0x8A;



int contador = 0;
int indice = 0;

unsigned int pulsos[12] = {1000,1667,2333,3000,3667,4333,5000,4333,3667,3000,2333,1667}; /* 0 , 30   60   90  120  150  180  150  120  90   60  30  */

void config(void){

    cli();

    *ponteiro_DDRB |= 0x04; // PB2 = OC1B = pino 10 como saída

    *ponteiro_TCCR1A = 0x23;
    *ponteiro_TCCR1B = 0x1A;     

    /* 1001|1100|0011|1111 = 39999 */

    *ponteiro_OCR1AH |= 0x9C;
    *ponteiro_OCR1AL |= 0x3F;

    /* 0000 0011 1110 1000  -> 0 graus*/
    *ponteiro_OCR1BH |= 0x03;
    *ponteiro_OCR1BL |= 0xE8;

    *ponteiro_TIMSK1 = 0x01;

    sei();
}

ISR(TIMER0_COMPA_vect){ /* acontece a cada 20 ms*/
    contador++;
}

int main(void){

    config();

    while(1){

        if(contador >= 40){
            contador = 0;

            indice++;
            if(indice >= 12){ /* se ja tiver encerrado um ciclo, reinicia*/
                indice = 0;
            }

            /* escreve no ponteiro OCR1B*/
            *ponteiro_OCR1BH = (pulsos[indice] >> 8) & 0xFF;
            *ponteiro_OCR1BL = pulsos[indice] & 0xFF;

            }
        }
        return 0;
}
