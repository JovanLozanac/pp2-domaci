#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* read_line()
{
    char* red = NULL;
    int brojac = 0;
    while(1)
    {
        char slovo = getchar();
        if(slovo == EOF || slovo == '\n') break;
        red = realloc(red, brojac+1);
        if(!red)
        {
            printf("%s", "MEM_GRESKA");
            return 0;
        }
        red[brojac++] = slovo;
    }
    if(brojac)
    {
        red = realloc(red, brojac+1);
        if(!red)
        {
            printf("%s", "MEM_GRESKA");
            return 0;
        }
        red[brojac] = '\0';
    }
    return red;
}

char** read_numbers()
{
    char** redovi = NULL;
    int brojac = 0;
    while(1)
    {
        char *red = read_line();
        if(!red) break;
        redovi = realloc(redovi, (brojac+1) * sizeof(char*));
        if(!redovi)
        {
            printf("%s", "MEM_GRESKA");
            return 0;
        }
        redovi[brojac++] = red;
    }
    redovi = realloc(redovi, (brojac+1) * sizeof(char*));
    if(!redovi)
    {
        printf("%s", "MEM_GRESKA");
        return 0;
    }
    redovi[brojac] = NULL;
    return redovi;
}

double convert (char *number, int base)
{
    double zbir = 0;
    char brojevi[20] = {};
    char nula = '0';
    for(int i = 0; i<10; i++) brojevi[i] = nula++;
    char slovo = 'A';
    for(int i = 10; i<20; i++) brojevi[i] = slovo++;

    int indeksTacke = -1;
    for(int i = 0; number[i]; i++)
    {
        if(number[i] == '.')
        {
            indeksTacke = i;
            break;
        }
        if(!number[i+1]) indeksTacke = i+1;
    }

    for(int i = 0; number[i]; i++)
    {
        if(i == indeksTacke) continue;
        int stepen = indeksTacke-i;
        if(i < indeksTacke) stepen--;
        if(number[i] >= '0' && number [i] <= '9') zbir += (number[i] - '0') * pow(base, stepen);
        else zbir += (number[i] - 'A' + 10) * pow(base, stepen);
    }
    return zbir;
}

void main() {
    char **brojevi = read_numbers();
    if(!brojevi[0]) printf("%s", "GRESKA");
    else
    {
        for(int i = 0; brojevi[i]; i++)
        {
            int trenutni = 0;
            int indeksBroja = 0;
            int indeksOtvoreneZagrade = -1;
            int indeksZatvoreneZagrade = -1;
            while(1)
            {
                if(!brojevi[i][trenutni]) break;

                if(brojevi[i][trenutni] == '(') indeksOtvoreneZagrade = trenutni;
                if(brojevi[i][trenutni] == ')')
                {
                    indeksZatvoreneZagrade = trenutni;
                    int base = 0;
                    for (int j = indeksOtvoreneZagrade+1; j != indeksZatvoreneZagrade; j++)
                    {
                        base = base * 10 + brojevi[i][j] - '0';
                    }

                    char *number = malloc(indeksOtvoreneZagrade - indeksBroja + 1);
                    strncpy(number, brojevi[i] + indeksBroja, indeksOtvoreneZagrade - indeksBroja);
                    number[indeksOtvoreneZagrade - indeksBroja] = '\0';
                    printf("%.3f", convert(number, base));
                    indeksBroja = trenutni + 2;
                    if(brojevi[i+1] || brojevi[i][indeksBroja]) putchar('\n');
                    free(number);
                }
                trenutni++;
            }
            free(*(brojevi+i));
        }
    }
    free (brojevi);
}