#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct element
{
    int id;
    char *vreme;
    char *url;
    struct element *sledeci;
} Element;

Element* formirajListu()
{
    Element *prvi = NULL, *trenutni = NULL, *zadnji = NULL;
    while (1)
    {
        int id;
        char *vreme = NULL;
        char *url = NULL;
        char c;
        if(scanf("%d", &id) != 1) break;
        if(id < 1)
        {
            printf("%s", "ERROR");
            return 0;
        }
        trenutni = malloc (sizeof(Element));
        if(!trenutni)
        {
            printf("%s", "MEM_GRESKA");
            return 0;
        }
        trenutni -> id = id;
        trenutni -> sledeci = NULL;
        c = getchar(); //razmak

        int brojac = 0;
        while(1)
        {
            c = getchar();
            vreme = realloc(vreme, brojac + 1);
            if(!vreme)
            {
                printf("%s", "MEM_GRESKA");
                return 0;
            }
            if(c == ' ')
            {
                vreme[brojac] = '\0';
                break;
            }
            vreme[brojac++] = c;
        }
        trenutni -> vreme = vreme;

        brojac = 0;
        while(1)
        {
            c = getchar();
            url = realloc(url, brojac + 1);
            if(!url)
            {
                printf("%s", "MEM_GRESKA");
                return 0;
            }
            if(c == '\n')
            {
                url[brojac] = '\0';
                break;
            }
            url[brojac++] = c;
        }
        trenutni -> url = url;
        if(!prvi) prvi = trenutni;
        else zadnji -> sledeci = trenutni;
        zadnji = trenutni;
    }
    return prvi;
}

void ispisiListu (Element *lista)
{
    while(lista)
    {
        printf("%s %d %s\n", lista -> vreme, lista -> id, lista -> url);
        lista = lista -> sledeci;
    }
}

int poredjenje (Element *jedan, Element *drugi)
{
    if(strcmp(jedan -> vreme, drugi -> vreme) < 0) return -1;
    if(strcmp(jedan -> vreme, drugi -> vreme) > 0) return 1;
    if(strcmp(jedan -> url, drugi -> url) < 0) return -1;
    if(strcmp(jedan -> url, drugi -> url) > 0) return 1;
    if(jedan -> id < drugi -> id) return -1;
    if(jedan -> id > drugi -> id) return 1;
    return 0;
}

void sortirajListu (Element *lista)
{
    Element *tmp = lista;
    while(tmp)
    {
        Element *minimum = tmp;
        Element *naredni = tmp -> sledeci;

        while(naredni)
        {
            if(poredjenje(minimum, naredni) == 1) minimum = naredni;
            naredni = naredni -> sledeci;
        }
        int id = minimum -> id;
        minimum -> id = tmp -> id;
        tmp -> id = id;

        char *stringovi = minimum -> vreme;
        minimum -> vreme = tmp -> vreme;
        tmp -> vreme = stringovi;

        stringovi = minimum -> url;
        minimum -> url = tmp -> url;
        tmp -> url = stringovi;

        tmp = tmp -> sledeci;
    }
}

char* nadjiPrviURLPoID (Element *lista, int id)
{
    while(lista)
    {
        if(lista -> id == id) return lista -> url;
        lista = lista -> sledeci;
    }
    return 0;
}

void oslobodiListu (Element *prvi)
{
    while(prvi)
    {
        Element *trenutni = prvi;
        prvi = prvi -> sledeci;
        free(trenutni -> vreme);
        free(trenutni -> url);
        free(trenutni);
    }
}

int main() {
    Element *lista = formirajListu();
    if(lista)
    {
        ispisiListu(lista);
        printf("%s\n", "###");
        sortirajListu(lista);
        ispisiListu(lista);
        printf("%s\n", "###");
        for(int i = 0; i<4; i++) i = getchar();
        int id;
        while(scanf("%d", &id) == 1)
        {
            printf("%d\n", id);
            //printf("%s\n", nadjiPrviURLPoID(lista, id));
        }
        oslobodiListu(lista);
    }
    return 0;
}
