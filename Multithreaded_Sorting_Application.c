#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// gcc -o msa Multithreaded_Sorting_Application.c -lpthread
#define MAX_NUM 50
int origion_num[MAX_NUM];
int sorted_num[MAX_NUM];
int len;

typedef struct {
    int l, r;
} datas;

void *sort(void *args) {
    datas *sort_data = (datas *)args;
    int start = sort_data->l, end = sort_data->r;
    if (start >= end)
        return NULL;
    int key = origion_num[start];
    int i = start, j = end;
    while (i < j) {
        while (i < j && origion_num[j] >= key) {
            j--;
        }
        origion_num[i] = origion_num[j];
        while (i < j && origion_num[i] <= key) {
            i++;
        }
        origion_num[j] = origion_num[i];
    }
    origion_num[i] = key;
    datas n1 = {start, i - 1};
    datas n2 = {i + 1, end};
    sort(&n1);
    sort(&n2);
    return NULL;
}

void *merge(void *args) {
    int *divide = (int *)args;
    int i = 0, j = *divide, p = *divide + 1, q = len - 1;
    int m = 0;
    while (i <= j && p <= q) {
        if (origion_num[i] <= origion_num[p]) {
            sorted_num[m++] = origion_num[i++];
        } else {
            sorted_num[m++] = origion_num[p++];
        }
    }
    while (i <= j) {
        sorted_num[m++] = origion_num[i++];
    }
    while (p <= q) {
        sorted_num[m++] = origion_num[p++];
    }
    return NULL;
}

int main() {
    pthread_t s1, s2, m;
    int i = 0;
    char c;
    puts("input numbers:");
    do {
        scanf("%d", &origion_num[i++]);
    } while ((c = getchar()) != '\n');
    len = i;
    int divide = len / 2 - 1;
    datas s1_par = {0, divide};
    datas s2_par = {divide + 1, len - 1};

    pthread_create(&s1, NULL, sort, &s1_par);
    pthread_join(s1, NULL);

    pthread_create(&s2, NULL, sort, &s2_par);
    pthread_join(s2, NULL);

    pthread_create(&m, NULL, merge, &divide);
    pthread_join(m, NULL);

    puts("sorted numbers:");
    for (int i = 0; i < len; i++) {
        printf("%d ", sorted_num[i]);
    }
    return 0;
}