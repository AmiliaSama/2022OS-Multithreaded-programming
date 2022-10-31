#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// gcc -o ssv Sudoku_Solution_Validator.c -lpthread
#define show 1
typedef struct {
    int x, y;
} pos; // 块的左上角坐标，默认块的大小是3*3的

int sudoku[9][9] = {{6, 2, 4, 5, 3, 9, 1, 8, 7}, {5, 1, 9, 7, 2, 8, 6, 3, 4},
                    {8, 3, 7, 6, 1, 4, 2, 9, 5}, {1, 4, 3, 8, 6, 5, 7, 2, 9},
                    {9, 5, 8, 2, 4, 7, 3, 6, 1}, {7, 6, 2, 3, 9, 1, 4, 5, 8},
                    {3, 7, 1, 9, 5, 6, 8, 4, 2}, {4, 9, 6, 1, 8, 2, 5, 7, 3},
                    {2, 8, 5, 4, 7, 3, 9, 1, 6}};
int flag[3][9]; // 分别对应行、列、块的9个flag，共计9行9列9个块
int check[10];  // 数字范围是0-9
void *checkX(void *value);
void *checkY(void *value);
void *checkSub(void *value);

void *checkX(void *value) {
    memset(check, 0, sizeof(check));
    int x = *(int *)value;
    for (int i = 0; i < 9; i++) {
        if (!check[sudoku[x][i]]) {
            check[sudoku[x][i]] = 1;
        } else {
            flag[0][x] = 1;
        }
    }
    return NULL;
}
void *checkY(void *value) {
    memset(check, 0, sizeof(check));
    int y = *(int *)value;
    for (int i = 0; i < 9; i++) {
        if (!check[sudoku[i][y]]) {
            check[sudoku[i][y]] = 1;
        } else {
            flag[1][y] = 1;
        }
    }
    return NULL;
}
void *checkSub(void *value) {
    memset(check, 0, sizeof(check));
    pos p = *(pos *)value;
    int x = p.x, y = p.y;
    for (int i = x; i < x + 3; i++) {
        for (int j = y; j < y + 3; j++) {
            if (!check[sudoku[i][j]]) {
                check[sudoku[i][j]] = 1;
            } else {
                flag[2][x + y / 3] = 1;
            }
        }
    }
}

void printinfo() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", flag[i][j]);
        }
        putchar('\n');
    }
}
int main() {
    int OK = 1;
    pthread_t id[3][9];
    for (int i = 0; i < 9; i++) {
        pthread_create(&id[0][i], NULL, checkX, &i);
        pthread_join(id[0][i], NULL);
    }
    for (int i = 0; i < 9; i++) {
        pthread_create(&id[1][i], NULL, checkY, &i);
        pthread_join(id[1][i], NULL);
    }
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            pos tmp = {i, j};
            pthread_create(&id[2][i + j / 3], NULL, checkSub, &tmp);
            pthread_join(id[2][i + j / 3], NULL);
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            if (flag[i][j]) {
                OK = 0;
                break;
            }
        }
    }
    // printinfo();
    puts(OK ? "valid" : "invalid");
    return 0;
}
