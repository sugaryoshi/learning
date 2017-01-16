/*
 * The Life Game
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NLOOP	(200)
#define N	(8192)
#define M	(8192)
#define ALIVE	(1)
#define DEAD	(0)
#define N_CHANK (64)
typedef int Grid[N + 2][M + 2]; /* グリッドのサイズ */

struct args{
	Grid* p_current;
	Grid* p_next;
	int id;
};

void computeNextGen(void *args)
{
	int i, j;
	struct args *arg = (struct args *)args;
	Grid * pCur = arg->p_current;
	Grid * pNext = arg->p_next;
	int firstIndex = arg->id * N_CHANK;
	//printf("abc: %d\n",firstIndex);

	for (i = firstIndex+1; i <= firstIndex + N_CHANK; i++) {
		for (j = firstIndex+1; j <= firstIndex + N_CHANK; j++) {
			int count = 0;

			/* 周囲のマスの生死チェック */
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
	pthread_t th[(N / N_CHANK)];

	/* グリッドの初期生成 */
	//printf("\033[2J");	/* clear screen */
	for (i = 0; i <= N + 1; ++i) {
		for (j = 0; j <= M + 1; ++j) {
			g[0][i][j] = random() & 1;
		}
		printf("Initializing g[%6d]...\r", i);
	}

	for (n = 0; n < NLOOP; n++) {
		//printf("\033[2J");	/* clear screen */
		printf("n = %d\n", n);

		/* ビットを右にずらして、表示する範囲を制限 */
		for (i = 1; i <= N >> 8; ++i) {
			for (j = 1; j <= M >> 7; ++j) {
				printf("%c", g[cur][i][j] == ALIVE ? '@' : '.');
			}
			printf("\n");
		}

		for(i = 0; i < (N / N_CHANK); i++){
			//printf("wadwada\n");
			struct args tempArg = {&g[cur], &g[(cur + 1) & 1], i};
			pthread_create(&th[i], NULL, (void *)&computeNextGen, (void *)&tempArg);
			pthread_join(th[i], NULL);
		}
        cur = (cur + 1) & 1;
	}

	return 0;
}
