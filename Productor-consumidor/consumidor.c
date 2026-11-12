
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define ITEMS_POR_PRODUCTOR 5

// Variables compartidas
int buf[BUFFER_SIZE];
int front = 0;
int rear = 0;

// Sincronización
sem_t empty;          // Espacios vacíos disponibles
sem_t full;           // Elementos listos para consumir
pthread_mutex_t mutex;// Exclusión mutua para la sección crítica

// Función del hilo Productor
void* producer(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < ITEMS_POR_PRODUCTOR; i++) {
        int item = id * 100 + (i + 1); // Generación del elemento

        // Simular tiempo de producción
        sleep(rand() % 2 + 1);

        // Esperar a que haya espacio disponible en el buffer
        sem_wait(&empty);
        
        // Entrar a la sección crítica protegiendo el acceso a 'rear' y al buffer
        pthread_mutex_lock(&mutex);

        buf[rear] = item;
        printf("[+] Productor %d: insertó el elemento %d en la posición %d\n", id, item, rear);
        rear = (rear + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        
        // Señalizar que hay un nuevo elemento disponible
        sem_post(&full);
    }
    return NULL;
}

// Función del hilo Consumidor
void* consumer(void* arg) {
    int id = *((int*)arg);
    // Como hay 2 productores y 2 consumidores, cada consumidor procesa en total 5 elementos
    for (int i = 0; i < ITEMS_POR_PRODUCTOR; i++) {
        // Esperar a que haya elementos disponibles para consumir
        sem_wait(&full);
        
        // Entrar a la sección crítica protegiendo el acceso a 'front' y al buffer
        pthread_mutex_lock(&mutex);

        int item = buf[front];
        printf("[-] Consumidor %d: extrajo el elemento %d de la posición %d\n", id, item, front);
        front = (front + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        
        // Señalizar que hay un nuevo espacio vacío disponible
        sem_post(&empty);

        // Simular tiempo de consumo
        sleep(rand() % 2 + 1);
    }
    return NULL;
}

int main() {
    pthread_t prod[2], cons[2];
    int prod_ids[1] = {1}, prod_ids_2[1] = {2};
    int cons_ids[1] = {1}, cons_ids_2[1] = {2};

    // Inicializar semáforos y mutex
    sem_init(&empty, 0, BUFFER_SIZE); // Inicializa en el tamaño del buffer
    sem_init(&full, 0, 0);            // Inicializa en 0 (sin elementos)
    pthread_mutex_init(&mutex, NULL);

    // Crear hilos productores (2 hilos)
    pthread_create(&prod[0], NULL, producer, &prod_ids[0]);
    pthread_create(&prod[1], NULL, producer, &prod_ids_2[0]);

    // Crear hilos consumidores (2 hilos)
    pthread_create(&cons[0], NULL, consumer, &cons_ids[0]);
    pthread_create(&cons[1], NULL, consumer, &cons_ids_2[0]);

    // Esperar a que todos los hilos terminen su ejecución
    pthread_join(prod[0], NULL);
    pthread_join(prod[1], NULL);
    pthread_join(cons[0], NULL);
    pthread_join(cons[1], NULL);

    // Destruir recursos de sincronización
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("\nSimulación de productor-consumidor finalizada con éxito.\n");
    return 0;
}
