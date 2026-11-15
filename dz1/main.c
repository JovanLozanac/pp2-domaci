#include <stdio.h>
#define MAX 100

void main() {
	//1. I/O
  	int n, niz[MAX], tmp;
	scanf("%d", &n);
	if (n < 1 || n > MAX) return;
	int i = 0;
	for(; i<n; i++)
	{
		scanf("%d", &niz[i]);
	}
	i = 0;
	for(; i<n; i++)
	{
		printf("%d", niz[i]);
		if(i == n-1) putchar('\n');
		else putchar (' ');
	}
	int j = 0;
	i = 0;

	//2. formiranje novog niza
	if(n == 1)
	{
		printf("%d\n", niz[0]);
		printf("%d", niz[0]);
		return;
	}
	for(; i<n; i++)
	{
		if(i == 0)
		{
			if (niz[i] > niz[i+1])
			{
				j++;
			}
		}
		else if (i == n-1)
		{
			if (niz[i] > niz[i-1])
			{
				niz[j++] = niz[i];
			}
		}
		else
		{
			if (niz[i] > niz[i+1] && niz[i] > niz[i-1])
			{
				niz[j++] = niz[i];
			}
		}
	}
	n = j;
	int brNepar = 0;
	for(i = 0; i<n; i++)
	{
		printf("%d", niz[i]);
		if(i == n-1) putchar('\n');
		else putchar (' ');
		if(niz[i] % 2 == 1) brNepar++;
	}
	//3. Sortiranje
	for(i = 0; i<n; i++)
	{
		if(niz[i] % 2 == 1)
		{
			for(j = i; j>0; j--)
			{
				if(niz[j-1] % 2 == 0 || niz[j] < niz[j-1])
				{
					tmp = niz[j];
					niz[j] = niz[j-1];
					niz[j-1] = tmp;
				}
				else break;
			}
		}
	}

	for(i = brNepar; i<n-1; i++)
	{
		int maks = i;
		for(j = i+1; j<n; j++)
		{
			if(niz[j] > niz[maks]) maks = j;
		}
		tmp = niz[maks];
		niz[maks] = niz[i];
		niz[i] = tmp;
	}

	for(i = 0; i<n; i++)
	{
		printf("%d", niz[i]);
		if(i < n-1) putchar(' ');
	}
	return;

}