# Transmissão e recepção usando interrupção (ATmega328P)

   Ana Carolina Vieira de Araújo - 248734
   Maria Letícia Gomes Braga dos Reis - 281318 

## Bibliotecas
* **Bibliotecas Padrão:** `<avr/io.h>`, `<avr/interrupt.h>`, `<util/delay.h>`.


## Configuração dos registradores UART
|    Registrador    |Config|                                  Justificativa                                      |
| **UBRR0H** (0xC5) | 0x00 | Baud rate coube suficientemente no UBRR0L                                           |
| **UBRR0L** (0xC4) | 0x33 | Para um baud rate de 19200 bps e normal speed -> UBRRn = 51                         |
| **UCSR0A** (0xC0) | 0x00 | U2X0 zerado pois normal speed, demais bits de estados que são geridos pelo hardware |
| **UCSR0B** (0xC1) | 0xD8 | Interrupção completa, buffer vazio desabilitado, transmissor habilitado             |
| **UCSR0C** (0xC2) | 0x06 | Modo assíncrono, 8 bits de dados, sem paridade, 1 stop bit                          |
| **UDR0**   (0xC6) |      | Criação do ponteiro para leitura e escrita dos dados                                |


## Configuração dos LEDS
|    Registrador    |  Configuração  |                  Justificativa                     |
| **DDRB**   (0x24) |    (|0x08)     | Ativa o bit correspondente ao led vermelho PB1     |
| **DDRB**   (0x24) |    (|0X10)     | Ativa o bit correspondente ao led verde    PB2     |
| **DDRB**   (0x24) |    (|0X20)     | Ativa o bit correspondente ao led azul     PB3     |
| **PORTB**  (0x25) |   (&= ~0x38)   | Controle estado dos leds/incialmente apagados      |


## Estrutura geral do código

 ┣ Cabeçalhos e Definições (#define F_CPU, <avr/io.h>, <avr/interrupt.h>)
 ┣ Mapeamento de Registradores por Ponteiros
 ┃  ┣ UBRR0H / UBRR0L / UCSR0A / UCSR0B / UCSR0C / UDR0 (UART)
 ┃  ┗ DDRB / PORTB
 ┣ Variáveis Globais e Buffers de Mensagens 
 ┣ Funções de Controle
     ┣ config(void) ──────────────> Inicializa USART (19200 bps) e define pinos do PORTB como saída
     ┣ enviar_mensagem(   ) ──────> Inicia a transmissão assíncrona disparando o primeiro caractere
     ┣ processar_entrada(void)  ──> Valida o caractere recebido e seleciona a string de feedback
     ┣ atualizar_leds(void)  ─────> Gere os padrões visuais e temporizações (delays) dos LEDs
     ┣ ISR(USART_RX_vect)  ───────> Interrupção de receção: captura o comando digitado no PC
     ┗ ISR(USART_TX_vect)  ───────> Interrupção de transmissão: envia os carateres da string sequencialmente
 ┗ int main(void)
     ┣ Chama config()
     ┗ Laço infinito 
         ┣ Verifica flag 'novo_dado' ──> Executa processar_comando()
         ┗ Executa continuamente   ────> Executa atualizar_leds()


## Explicação Fluxo Processamento

O fluxo de processamento se inicia com a chegada de um byte pelo terminal. Isso vai ativar uma interrupção de recepção pela ISR(USART_RX_vect). A interrupção vai ler o novo dado, guarda o caractere recebido e faz a verificação do caractere. Se válido, a variável de novo_dado será ativada.

Pela main, se a variável novo_dado for 1, deve-se fazer o processamento da entrada. 

Para o envio das mensagens, seguimos o mesmo raciocínio realizado no exercício 2 em classe.

A função void processar_entrada(void) vai usar como argumento o caractere salvo pela interrupção de recebimento e comparar pelo switch case qual deve ser a frase printada e chama enviar_mensagem passando o ponteiro da mensagem.

A função void enviar_mensagem(const char *msg) vai ser chamada, guardar o endereço recebido em msg_TXC0 e enviar o primeiro caractere.

Assim que esse primeiro caractere termina de ser transmitido, o hardware dispara automaticamente a interrupção ISR(USART_TX_vect), que usa o mesmo ponteiro (msg_TXC0) para enviar o próximo caractere.

Importante frisar que buscando tornar a função de envio mais genérica e usar melhor a memória usamos um ponteiro base para passar o endereço de cada frase para podermos percorrer os índices com a função de envio de mensagem.

Voltando para a main, após o processamento da entrada, a função atualizar_leds() é chamada.

Sempre que um comando for válido, a função de processamento de entrada vai atualizar a variável comando_ativo. A função de atualizar o led vai usar ela para avaliar qual deve ser o funcionamento dependendo do caractere. 

Toda vez que a flag de recebimento de um novo dado for 1, haverá a mudança do comando_ativo pela função do processamento de entrada, e consequentemente o comportamento do led mudará. Essa escolha também se fez nessaria pois ao digitar qualquer outro caractere a função de escrita não vai ser chamada pois ele não será valido e o caractere antigo ainda continuara no comando_ativo permitindo que o led continue tendo o mesmo comportamento do comando válido anterior.






