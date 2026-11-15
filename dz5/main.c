#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct podaci
{
    int broj;
    char naziv[256];
    double lat;
    double lon;
    int zona;
} Podaci;

typedef struct stanica
{
    struct stanica *pret;
    struct podaci *p;
    struct stanica *sled;
} Stanica;

double stepURad (double step)
{
    //konverzija iz stepena u radijane
    return (step * 3.14) / 180;
}

double haversine (double x1, double y1, double x2, double y2)
{
    //racunanje sfernih koordinata
    double t1 = pow((sin((stepURad(x1) - stepURad(x2)) / 2)), 2);
    double t2 = pow((sin((stepURad(y1) - stepURad(y2)) / 2)), 2);
    return 2 * 6371.0 * asin (sqrt(t1 + t2 * cos(stepURad(x1)) * cos(stepURad(x2))));
}

Podaci *procitajJednu(FILE *input)
{
    //ovo je ucitavanje pomocne strukture koja opisuje jednu stanicu, van konteksta liste
    Podaci *p = malloc(sizeof(Podaci));
    if (!p)
    {
        printf("%s", "MEM_GRESKA");
        return NULL;
    }
    if(fscanf(input, "%d!%[^!]!%lf!%lf!%d\n", &p->broj, p->naziv, &p->lat, &p->lon, &p->zona) != 5)
    {
        return NULL;
    }
    return p;
}

Stanica *procitajLiniju(FILE *input)
{
    //ucitavanje liste sa pomocnom f-jom, na nacin obradjen na vezbama
    Stanica *prva = NULL, *temp = NULL, *zadnja = NULL;
    Podaci *jedna = NULL;
    while(1)
    {
        jedna = procitajJednu(input);
        if(!jedna)
        {
            break;
        }
        temp = malloc(sizeof(Stanica));
        temp -> p = jedna;
        temp -> sled = NULL;
        temp -> pret = NULL;
        if(!prva)
        {
            prva = temp;
        }
        else
        {
            temp -> pret = zadnja;
            zadnja -> sled = temp;
        }
        zadnja = temp;
    }
    return prva;
}

int indeksNajmanjeRazdaljine (double x1, double y1, Stanica *lista)
{
    //iteriranjem kroz listu, dobija se indeks onog cvora koji je najblizi datoj tacki (x1, y1)
    double najmanja = haversine (x1, y1, lista->p->lat, lista->p->lon);
    int broj = 0;
    int i = 1;
    lista = lista->sled;
    while(lista)
    {
        double probna = haversine(x1, y1, lista->p->lat, lista->p->lon);
        if(probna <= najmanja)
        {
            broj = i;
            najmanja = probna;
        }
        i++;
        lista = lista->sled;
    }
    return broj;
}

Stanica *formirajListu(Stanica *smerA, Stanica *smerB, double pocetnaLat, double pocetnaLon, double krajnjaLat, double krajnjaLon, char *smer)
{
    //Funkcija formira listu odsecanjem nevaznih cvorova.

    //izracunavanje koordinata
    int najblizaKrajnjojA = indeksNajmanjeRazdaljine(krajnjaLat, krajnjaLon, smerA);
    int najblizaKrajnjojB = indeksNajmanjeRazdaljine(krajnjaLat, krajnjaLon, smerB);
    int najblizaPocetnojA = indeksNajmanjeRazdaljine(pocetnaLat, pocetnaLon, smerA);
    int najblizaPocetnojB = indeksNajmanjeRazdaljine(pocetnaLat, pocetnaLon, smerB);

    if(!smerA || najblizaKrajnjojB >= najblizaPocetnojB)
    {
        //ne postoji smer A ili putanja postoji samo u smeru B
        *smer = 'B'; //pomocno
        int i = 0;
        while(i != najblizaPocetnojB)
        {
            //dolazenje do prvog cvora koji ima da se ubaci i ispise
            i++;
            smerB = smerB -> sled;
        }
        //ovde se ne radi plitka kopija jer bi ostali neizbrisani cvorovi
        Stanica *pocetna = malloc(sizeof(Stanica));
        pocetna -> p = malloc(sizeof(Podaci));
        pocetna -> p->broj = smerB -> p->broj;
        strcpy(pocetna->p->naziv, smerB->p->naziv);
        pocetna -> p->lat = smerB -> p->lat;
        pocetna -> p->lon = smerB -> p->lon;
        pocetna -> p->zona = smerB -> p->zona;

        pocetna -> pret = NULL;
        Stanica *tmp = smerB;
        Stanica *tmppret = pocetna;
        while(i != najblizaKrajnjojB)
        {
            //iteriranje do poslednjeg za ispis
            tmp = tmp -> sled;
            i++;

            Stanica *tmpsad = malloc(sizeof(Stanica));
            tmpsad -> p = malloc(sizeof(Podaci));
            tmpsad -> p->broj = tmp -> p->broj;
            strcpy(tmpsad->p->naziv, tmp->p->naziv);
            tmpsad -> p->lat = tmp -> p->lat;
            tmpsad -> p->lon = tmp -> p->lon;
            tmpsad -> p->zona = tmp -> p->zona;

            tmpsad -> pret = tmppret;
            tmppret -> sled = tmpsad;
            tmpsad -> sled = NULL;

            tmppret = tmppret -> sled;
        }
        return pocetna;
    }
    else
    {
        //ovaj deo je identican sa granom ako je uslov zadovoljen, samo se radi o smeru A
        *smer = 'A';
        int i = 0;
        while(i != najblizaPocetnojA)
        {
            i++;
            smerA = smerA -> sled;
        }
        //ovde se ne radi plitka kopija jer bi ostali neizbrisani cvorovi
        Stanica *pocetna = malloc(sizeof(Stanica));

        pocetna -> p = malloc(sizeof(Podaci));
        pocetna -> p->broj = smerA -> p->broj;
        strcpy(pocetna->p->naziv, smerA->p->naziv);
        pocetna -> p->lat = smerA -> p->lat;
        pocetna -> p->lon = smerA -> p->lon;
        pocetna -> p->zona = smerA -> p->zona;

        pocetna -> pret = NULL;
        Stanica *tmp = smerA;
        Stanica *tmppret = pocetna;
        while(i != najblizaKrajnjojA)
        {
            //iteriranje do poslednjeg za ispis
            tmp = tmp -> sled;
            i++;

            Stanica *tmpsad = malloc(sizeof(Stanica));
            tmpsad -> p = malloc(sizeof(Podaci));
            tmpsad -> p->broj = tmp -> p->broj;
            strcpy(tmpsad->p->naziv, tmp->p->naziv);
            tmpsad -> p->lat = tmp -> p->lat;
            tmpsad -> p->lon = tmp -> p->lon;
            tmpsad -> p->zona = tmp -> p->zona;

            tmpsad -> pret = tmppret;
            tmppret -> sled = tmpsad;
            tmpsad -> sled = NULL;

            tmppret = tmppret -> sled;
        }
        return pocetna;
    }
}

void ispisiListe(int argc, char *izlazFajl, Stanica **liste, char nazivi[][256])
{
    //Funkcija vrsi ispis ulancane liste pocev od head. Geografska sirina (latituda) i duzina (longituda) zaokruzuju se na 6 decimala.
    FILE *izlaz = fopen(izlazFajl, "w");
    if(!izlaz)
    {
        printf("%s","DAT_GRESKA");
        return;
    }
    for(int i = 0; i<argc-6; i++)
    {
        fprintf(izlaz, "%s\n", nazivi[i]); //linija!smer
        Stanica *s = liste[i];
        while(s)
        {
            fprintf(izlaz, "%d!%s!%.6lf!%.6lf!%d\n", s->p->broj, s->p->naziv, s->p->lat, s->p->lon, s->p->zona); //1 stanica u 1 liniji
            s = s -> sled;
        }
    }
    fclose(izlaz);
}

void oslobodiSve(int argc, Stanica **liste, Stanica *smerA, Stanica *smerB, char *ime)
{
    //Funkcija oslobadja svu dinamicku memoriju. Free nema efekta ako mu je prosledjen null. Brise se niz listi, zatim dve ucitane liste i pomocni string za naziv.
    for(int i = 0; i< argc - 6; i++)
    {
        Stanica *tmp = liste[i];
        while(tmp)
        {
            Stanica *tmp2 = tmp;
            tmp = tmp -> sled;
            free(tmp2->p->naziv);
            free(tmp2->p);
            free(tmp2);
        }
        free(liste[i]);
    }
    free(liste);

    while(smerA)
    {
        Stanica *tmp = smerA;
        smerA = smerA->sled;
        free(tmp);
    }
    while(smerB)
    {
        Stanica *tmp = smerB;
        smerB = smerB->sled;
        free(tmp);
    }

    free(ime);
}

int main(int argc, char* argv[])
{
    if(argc < 7)
    {
        //kraj programa jer nije naveden
        printf("%s","ARG_GRESKA");
        return 0;
    }
    double pocetnaLat = atof(argv[2]);
    double pocetnaLon = atof(argv[3]);
    double krajnjaLat = atof(argv[4]);
    double krajnjaLon = atof(argv[5]);
    if(!pocetnaLat || !pocetnaLon || !krajnjaLat || !krajnjaLon)
        /*
         * svojevrsna provera tacnosti jer atof ne prijavljuje ako je doslo do greske, nego vraca 0.0.
         * Koordinata (0,0) nisu tacka na kopnu Zemlje, tacnije, to je usred Atlantika,
         * a javni gradski prevoz (iz postavke) obicno obuhvata jedino kopneni saobracaj u urbanoj sredini.
         */
    {
        printf("%s","ARG_GRESKA");
        return 0;
    }
    Stanica **liste = malloc((argc-6) * sizeof(Stanica*)); //dinamicki alociran niz ulancanih listi
    char nazivi[argc-6][256]; //linija!smer, npr. 309!B
    for(int i = 6; i<argc; i++)
    {
        Stanica *smerA = NULL;
        Stanica *smerB = NULL;
        char *ime = malloc(strlen(argv[i])+10); //koristim za nazivi[][]
        strcpy(ime, argv[i]);
        strcat(ime, "_dirA.txt");
        FILE *a = fopen(ime, "r");
        if(!a)
        {
            printf("%s", "DAT_GRESKA");
            oslobodiSve(argc, liste, NULL, NULL, ime);
            free(ime);
            return 0;
        }
        smerA = procitajLiniju(a);
        fclose(a);

        ime[strlen(ime)-5] = 'B';
        FILE *b = fopen(ime, "r");
        if(!b)
        {
            printf("%s", "DAT_GRESKA");
            oslobodiSve(argc, liste, smerA, NULL, ime);
            return 0;
        }
        smerB = procitajLiniju(b);
        fclose(b);

        char smer = 1;
        liste[i-6] = formirajListu(smerA, smerB, pocetnaLat, pocetnaLon, krajnjaLat, krajnjaLon, &smer); //kroz ovu funkciju odredjuje se i smer, kroz char
        //liste[...] - jedna ulancana lista
        strcpy(nazivi[i-6], argv[i]);
        strcat(nazivi[i-6], "!");

        nazivi[i-6][strlen(nazivi[i-6])] = smer; //ova i gornje 2 linije sluze za ispis linija!smer
        oslobodiSve(0, NULL, smerA, smerB, ime);
    }

    ispisiListe(argc, argv[1], liste, nazivi);
    oslobodiSve(argc, liste, NULL, NULL, NULL);
    return 0;
}
