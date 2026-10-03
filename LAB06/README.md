Para o dimmer, considerar que a tensão média controla a interpretação do brilho que nós temos.
O brilho vai ser mudado com a mudança da tensão média Vm que ele recebe.
A tensão média é calculada por Vm = V x DutyCycle.

O valor que é guardado em OCR0B determina a largura do pulso em nível alto dentro de cada período da onda.
Ou seja, se temos uma frequência de 1000 Hz, significa que esse valor de 0 a 250 (máx) vai se repetir 1000 vezes em 1 segundo. Só que, e o tempo que vai ficar ligado está relacionado com o valor que guardamos no comparador.

Se eu colocar 50, desses 250 ele fica ligado 50, e se eu botar 1, desses 250 ele fica ligado 1. Ou seja, vai parecer ter mais brilho quando deixamos o valor do comparador maior. - e quem controla esse valor será o OCR0B -.



Para a Atividade 3 foi utilizado o Fast PWM Mode e o valor 8 para o prescaler, uma vez que este gera o melhor valor de resolução DC e o maior período possível:

    TpwM = N(OCRA + 1)/fclk

    0,02 = 8x(OCRA + 1)/fclk
    
    OCRA = 39999

    Resolução DC:

    1/2^n = 1/39999 = 2.5 x 10 ^(-5)


No programa foi definido as larguras do pulso associadas a cada ângulo, tais valores foram definidos pelas fórmulas:

    pulso(theta) = 0,5 + theta/90, definida a partir da relação linear 0,5 ms → 0°, 1,5 ms → 90°, 2,5 ms → 180°

    OCR1B = pulso/0,5x10⁻⁶ , em que 0,5x10⁻⁶ corresponde a 8/16x10⁶ (prescaler/Fcpu)


No programa da atividade 3 a cada 800 ms (40 ciclos de interrupções) é realizada a escrida de um valor de faixa nos registrados OCR1BH E OCR1BL, permitindo a movimentação do motor para o angulo correspondentes, ao realizar todo o ciclo de movimento do motor, este é reiniciado (o código reinicia a leitura do array)
