#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int nadjiI (int indeks, int x)
{
    if(x == 2 || x == 3)
    {
        return indeks % 2;
    }
    else
    {
        switch (indeks % 4)
        {
            case 3: return 2;
            case 2: return 3;
            default: return indeks % 4;
        }
    }
}

int nadjiJ (int indeks, int x)
{
    if(x == 2)
    {
        return indeks / 2;
    }
    else if(x == 3)
    {
        switch (indeks / 2)
        {
            case 3: return 2;
            case 2: return 3;
            default: return indeks / 2;
        }
    }
    else
    {
        switch (indeks / 4)
        {
            case 3: return 2;
            case 2: return 3;
            default: return indeks / 4;
        }
    }
}

int main() {
    int x, **mat, i, j;
    scanf("%d", &x);
    if(x < 2 || x > 4) return 0;
    int m, n;
    if(x == 2)
    {
        m = 2;
        n = 2;
    }
    else if (x == 3)
    {
        m = 2;
        n = 4;
    }
    else
    {
        m = 4;
        n = 4;
    }

    mat = (int**) malloc(m * sizeof (int*));
    if(mat == NULL) return 0;
    for(i = 0; i<m; i++)
    {
        *(mat+i) = (int*) malloc(n * sizeof(int*));
        if(mat+i == NULL) return 0;
        for (j = 0; j<n; j++) *(*(mat+i)+j) = 1;
    }

    int goreLevo, doleDesno, goreI, doleI, levoJ, desnoJ;
    while(1)
    {
        if(scanf("%d%d", &goreLevo, &doleDesno) != 2) break;
        if(0 > goreLevo || 0 > doleDesno) return 0;
        if(goreLevo >= pow(2, x) || doleDesno >= pow(2,x)) return 0;
        goreI = nadjiI(goreLevo,x);
        doleI = nadjiI(doleDesno,x);
        levoJ = nadjiJ(goreLevo,x);
        desnoJ = nadjiJ(doleDesno,x);
        for(i = goreI; ; i++)
        {
            if(i == m) i = 0;
            for(j = levoJ; ; j++)
            {
                if(j == n) j = 0;
                *(*(mat+i)+j) = 0;
                if(j == desnoJ) break;
            }
            if(i == doleI) break;
        }
    }

    for(i = 0; i<m; i++)
    {
        for (j = 0; j<n; j++)
        {
            printf("%d", *(*(mat+i)+j));
            if(j < n - 1) putchar(' ');
        }
        if(i < m - 1) putchar('\n');
    }

    for(i = 0; i<m; i++)
    {
        free(mat[i]);
    }
    free(mat);

    return 0;
}