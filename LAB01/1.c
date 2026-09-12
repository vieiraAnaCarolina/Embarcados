/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248784*/

#include<stdio.h>
const unsigned long int sequencia = 0x034A2D19;

int extrair_paridade(unsigned char num){ /* realiza a XOR com cada bit do para e retorna a paridade */
    int paridade = 0;

    for (int i = 0; i < 8 ; i++){
        int byte = (num >> i) & 1;
        paridade ^= byte;
    }
    return paridade;
}

int par(){

    int n = 24;
    for(int i = 0; i < 4; i++){ /* realiza o shift  e aplica a mascara isolando os 8 bits representantes de cada par*/
        unsigned char valor = sequencia >> n;
        unsigned char num = valor & 0xFF;
        n = n-8;
        int paridade = extrair_paridade(num);
        printf("0X%02X - paridade par = %d\n", num, paridade);
    }
    return 0;
}

int main()
{
    par();
    return 0;
}