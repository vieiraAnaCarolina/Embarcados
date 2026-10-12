# Servo Motor (ATmega328P)

   Ana Carolina Vieira de Araújo - 248734
   Maria Letícia Gomes Braga dos Reis - 281318 

## Bibliotecas
- **Bibliotecas Padrão:** `<avr/interrupt.h>`.

## Configuração dos registradores
|      Registrador       |  Config  |                                  Justificativa                                  |
| **DDRB** (0x24)        |  0x04    | Define o pino PB2 (pino 10 digital) como saída (OC1B).                          |
| **TCCR1A** (0x80)      |  0x23    | Configura Fast PWM non-inverting (COM1B1=1) e WGM11=1, WGM10=1.                 |
| **TCCR1B** (0x81)      |  0x1A    | Define Modo 15 (Fast PWM com TOP em OCR1A) e prescaler 8.                       |
| **OCR1AH/L** (0x89/88) |  0x9C3F  | Define TOP = 39999 para fixar a frequência de 50 Hz (período de 20 ms).         |
| **OCR1BH/L** (0x8B/8A) |  0x03E8  | Controla a largura do pulso (duty cycle) para definir o ângulo do servo.        |
| **TIMSK1**   (0x6F)    |  0x01    | Habilita a interrupção por Overflow do Timer 1 para gerar a base de tempo.      |


## Explicação Fluxo Processamento

O fluxo de processamento baseia-se na geração contínua de um sinal PWM de 50 Hz (20 ms de período) no pino 10 (OC1B) através do Timer 1 em Modo 15 (Fast PWM com TOP em OCR1A).

A cada ciclo completo de 20 ms, o temporizador atinge o seu limite (TOP = 39999) e reinicia, disparando automaticamente a interrupção por estouro ISR(TIMER1_OVF_vect). No interior desta rotina, a variável global contador é incrementada em uma unidade.

No fluxo principal (main), o programa mantém-se num laço infinito a monitorizar o valor do contador. Como o objetivo é manter o eixo do servo motor posicionado em cada ângulo durante exatamente 800 ms sem recorrer a esperas ativas bloqueantes (delays), calcula-se a quantidade de ciclos de interrupção necessários: 800 ms / 20 ms = 40 ciclos.

Assim que o contador atinge o limiar de 40 interrupções, o sistema desativa temporariamente as interrupções com cli() para garantir operações seguras e atómicas, zera o contador e avança para o próximo valor do vetor pulsos[12]. 

Em seguida, os novos valores de largura de pulso correspondentes ao ângulo desejado (variando de 0° a 180° e o respetivo retorno) são gravados diretamente nos registradores de 16 bits OCR1B (escrevendo primeiro o byte alto e depois o byte baixo). As interrupções são reativadas com sei(), e o ciclo repete-se indefinidamente, permitindo o movimento fluído do eixo em passos de 30°.