#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> 

#define SUMSIZE 100
#define BUFSIZE 8

int buffer[BUFSIZE];

sem_t llenos;
sem_t vacios;

static void *productor(void *arg1) {
    int i, entra, producir;
    entra = 0;

    for (i = 1; i <= SUMSIZE; i++) {
        while ((producir = getchar()) == '\n');
        sem_wait(&vacios);
        buffer[entra] = producir;
        entra = (entra + 1) % BUFSIZE;
        printf("productor --> \t %c\n", producir);
        sem_post(&llenos);
    }
    return NULL;
}

static void *consumidor(void *arg2) {
    int i, sale, consumir;
    sale = 0;
    for (i = 1; i <= SUMSIZE; i++) {
        sem_wait(&llenos);
        sleep(1);
        consumir = buffer[sale];
        sale = (sale + 1) % BUFSIZE;
        printf(" \t%c\tConsumidor -->\n", consumir);
        sem_post(&vacios);
    }
    return NULL;
}

int main() {
    pthread_t prodtid;
    pthread_t constid;

    sem_init(&llenos, 0, 0);
    sem_init(&vacios, 0, BUFSIZE);

    pthread_create(&prodtid, NULL, productor, NULL);
    pthread_create(&constid, NULL, consumidor, NULL);

    pthread_join(prodtid, NULL);
    pthread_join(constid, NULL);

    printf("terminado\n");

    return 0; // Debes devolver un valor desde la función main
}
