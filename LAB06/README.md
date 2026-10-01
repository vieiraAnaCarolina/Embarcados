Para o dimmer, considerar que a tensão média controla a interpretação do brilho que nós temos.
O brilho vai ser mudado com a mudança da tensão média Vm que ele recebe.
A tensão média é calculada por Vm = V x DutyCycle.

O valor que é guardado em OCR0B determina a largura do pulso em nível alto dentro de cada período da onda.
Ou seja, se temos uma frequência de 1000 Hz, significa que esse valor de 0 a 250 (máx) vai se repetir 1000 vezes em 1 segundo. Só que, e o tempo que vai ficar ligado está relacionado com o valor que guardamos no comparador.

Se eu colocar 50, desses 250 ele fica ligado 50, e se eu botar 1, desses 250 ele fica ligado 1. Ou seja, vai parecer ter mais brilho quando deixamos o valor do comparador maior. - e quem controla esse valor será o OCR0B -.




