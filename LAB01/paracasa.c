/* Maria Letícia Gomes Braga dos Reis - 281318 e Ana Carolina Vieira de Araújo - 248734 */

#include <stdio.h>

/* Trecho do discurso */
char texto[] = "A Constituição certamente não é perfeita. Ela própria o confessa ao admitir a reforma. Quanto a ela, discordar, sim. Divergir, sim. Descumprir, jamais. Afrontá-la, nunca.\nTraidor da Constituição é traidor da Pátria. Conhecemos o caminho maldito. Rasgar a Constituição, trancar as portas do Parlamento, garrotear a liberdade, mandar os patriotas para a cadeia, o exílio e o cemitério.\nQuando após tantos anos de lutas e sacrifícios promulgamos o Estatuto do Homem da Liberdade e da Democracia bradamos por imposição de sua honra.\nTemos ódio à ditadura. Ódio e nojo.\nAmaldiçoamos a tirania aonde quer que ela desgrace homens e nações. Principalmente na América Latina.\n\nUlysses Guimarães, Presidente da Assembleia Nacional Constituinte.\n";

/**
 * Conta o número de palavras presentes na cadeia de caracteres com base
 * em delimitadores de espaço, quebras de linha e hífens.
 * 
 * @param cadeia_de_caracteres Ponteiro para a string a ser analisada.
 * @return Total estimado de palavras no texto.
 */

int conta_palavras(char * cadeia_de_caracteres)
{
    int numero_de_palavras = 0;
    int i = 0;

    /* Percorre a cadeia de caracteres até encontrar o terminador nulo '\0' */
    while (texto[i] != '\0') {
        i++; /* Avança para o próximo caractere */

        /* A cada espaço em branco encontrado, contabiliza-se uma palavra */
        if (texto[i] == ' ') {
            numero_de_palavras++;
        }
        /* Caso encontre hífen, considera-se a separação de termos compostos */
        else if (texto[i] == '-') {
            numero_de_palavras = numero_de_palavras + 2;
        }
        /* Ao encontrar uma quebra de linha válida (que não seja linha vazia ou final), contabiliza nova palavra */
        else if (texto[i] == '\n' && texto[i+1] != '\n' && texto[i+1] != '\0') {
            numero_de_palavras++;
        }
    }

    return numero_de_palavras;
}

/**
 * Localiza e imprime todas as palavras iniciadas com a letra 'c' ou 'C'
 * 
 */

void imprime_palavras_c(char * cadeia_de_caracteres)
{
    int i = 0;

    /* Itera por todos os caracteres do texto */
    while (texto[i] != '\0') {
        /* Verifica se o caractere atual é 'c' ou 'C' */
        if (texto[i] == 'c' || texto[i] == 'C') {
            /* Checa se o caractere está no início de uma palavra (precedido por espaço ou quebra de linha) */
            if (i == 0 || texto[i-1] == ' ' || texto[i-1] == '\n') {
                /* Imprime os caracteres sequenciais da palavra até encontrar pontuação ou delimitador */
                while (texto[i] != '\n' && texto[i] != ' ' && texto[i] != '.' && texto[i] != ',' && texto[i] != '\0') {
                    printf("%c", texto[i]);
                    i++;
                }
                printf("\n"); /* Quebra de linha ao término da palavra impressa */
            }
            else {
                i++; /* Avança se a letra 'c' estiver no meio/fim de uma palavra */
            }
        }
        else {
            i++; /* Avança para o próximo caractere */
        }
    }
}

int main() 
{
    /* Exibe o texto completo */
    printf("%s\n", texto);

    /* Calcula e exibe a contagem total de palavras */
    printf("O número de palavras no texto é: %d\n", conta_palavras(texto));

    /* Lista as palavras iniciadas pela letra 'c'/'C' */
    printf("As palavra que começam com a letra c no texto são:\n");
    imprime_palavras_c(texto);

    return 0;
}