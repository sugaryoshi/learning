/*
 * The Life Game
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NLOOP	(200)
#define N	(8192)
#define M	(8192)
#define ALIVE	(1)
#define DEAD	(0)

typedef int Grid[N + 2][M + 2];

void computeNextGen(const Grid * pCur, Grid * pNext)
{
	int i, j;

	for (i = 1; i <= N; ++i) {
		for (j = 1; j <= M; ++j) {
			int count = 0;

			/* NW neighbor */
			if ((*pCur)[i - 1][j - 1] == ALIVE)
				count++;
			/* N neighbor */
			if ((*pCur)[i - 1][j] == ALIVE)
				count++;
			/* NE neighbor */
			if ((*pCur)[i - 1][j + 1] == ALIVE)
				count++;
			/* W neighbor */
			if ((*pCur)[i][j - 1] == ALIVE)
				count++;
			/* E neighbor */
			if ((*pCur)[i][j + 1] == ALIVE)
				count++;
			/* SW neighbor */
			if ((*pCur)[i + 1][j - 1] == ALIVE)
				count++;
			/* S neighbor */
			if ((*pCur)[i + 1][j] == ALIVE)
				count++;
			/* SE neighbor */
			if ((*pCur)[i + 1][j + 1] == ALIVE)
				count++;

			if (count <= 1 || count >= 4)
				(*pNext)[i][j] = DEAD;
			else if ((*pCur)[i][j] == ALIVE &&
				 (count == 2 || count == 3))
				(*pNext)[i][j] = ALIVE;
			else if ((*pCur)[i][j] == DEAD && count == 3)
				(*pNext)[i][j] = ALIVE;
			else
				(*pNext)[i][j] = DEAD;
		}
	}
}

int main(int argc, char *argv[])
{
	int cur = 0;
	static Grid g[2];
	int i, j, n;

	printf("\033[2J");	/* clear screen */
	for (i = 0; i <= N + 1; ++i) {
		for (j = 0; j <= M + 1; ++j) {
			g[0][i][j] = random() & 1;
		}
		printf("Initializing g[%6d]...\r", i);
	}

	for (n = 0; n < NLOOP; n++) {
		printf("\033[2J");	/* clear screen */
		printf("n = %d\n", n);
		for (i = 1; i <= N >> 8; ++i) {
			for (j = 1; j <= M >> 7; ++j) {
				printf("%c", g[cur][i][j] == ALIVE ? '@' : '.');
			}
			printf("\n");
		}
		computeNextGen(&g[cur], &g[(cur + 1) & 1]);
		cur = (cur + 1) & 1;
	}

	return 0;
}
