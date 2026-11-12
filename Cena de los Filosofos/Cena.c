#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

// Arreglo de semáforos para los tenedores (inicializados en 1)
sem_t forks[NUM_PHILOSOPHERS];

void think(int id) {
    printf("🤔 Filósofo %d está pensando...\n", id);
    sleep(rand() % 3 + 1);
}

void eat(int id) {
    printf("🍝 Filósofo %d está comiendo...\n", id);
    sleep(rand() % 3 + 1);
}

void* philosopher(void* num) {
    int id = *((int*)num);

    while (1) {
        think(id);

        if (id == NUM_PHILOSOPHERS - 1) {
            // Solución asimétrica: El último filósofo (Filósofo 4) toma primero el tenedor de la derecha y luego el de la izquierda
            // Rompe la espera circular especificada en la lámina de referencia
            sem_wait(&forks[0]);         // P(fork[0])
            sem_wait(&forks[id]);        // P(fork[4])
            
            printf("🥢 Filósofo %d (Asimétrico) tomó los tenedores 0 y %d y comenzó a comer.\n", id, id);
            eat(id);
            
            sem_post(&forks[0]);         // V(fork[0])
            sem_post(&forks[id]);        // V(fork[4])
        } else {
            // Filósofos del 0 al 3 toman primero su tenedor izquierdo (i) y luego el derecho (i+1)
            sem_wait(&forks[id]);        // P(fork[i])
            sem_wait(&forks[id + 1]);    // P(fork[i+1])
            
            printf("🥢 Filósofo %d tomó los tenedores %d y %d y comenzó a comer.\n", id, id, id + 1);
            eat(id);
            
            sem_post(&forks[id]);        // V(fork[i])
            sem_post(&forks[id + 1]);    // V(fork[i+1])
        }

        printf("🪑 Filósofo %d terminó de comer y dejó libres los tenedores.\n", id);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    srand(time(NULL));

    // Inicializar cada semáforo de tenedor en 1
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Crear los hilos de los filósofos
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // Esperar a los hilos (simulación infinita, detener con Ctrl+C)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destrucción de semáforos
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}
