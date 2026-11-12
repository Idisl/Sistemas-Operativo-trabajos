#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define LIMITE 10000

sem_t semaforo;

void *contar(void *arg)
{
    int numero = *(int *)arg;
    int i;

    for (i = 1; i <= LIMITE; i++)
    {
        /*
         * Antes de entrar a la sección crítica,
         * el hilo obtiene el semáforo.
         *
         * Fuera de la sección crítica, cada
         * impresión también utiliza el semáforo.
         */
        if (i == 4000 || i < 4000 || i > 6000)
        {
            sem_wait(&semaforo);
        }

        if (i == 4000)
        {
            printf("\n====================================\n");
            printf("HILO %d ENTRA A LA SECCION CRITICA\n", numero);
            printf("====================================\n");
        }

        printf("Hilo %d: %d\n", numero, i);

        if (i == 6000)
        {
            printf("====================================\n");
            printf("HILO %d SALE DE LA SECCION CRITICA\n", numero);
            printf("====================================\n\n");

            sem_post(&semaforo);
        }
        else if (i < 4000 || i > 6000)
        {
            sem_post(&semaforo);
        }
    }

    return NULL;
}

int main()
{
    pthread_t hilo1, hilo2;

    int id1 = 1;
    int id2 = 2;

    if (sem_init(&semaforo, 0, 1) != 0)
    {
        perror("Error al inicializar el semaforo");
        return 1;
    }

    printf("INICIANDO LOS DOS HILOS...\n\n");

    if (pthread_create(&hilo1, NULL, contar, &id1) != 0)
    {
        perror("Error al crear hilo 1");
        sem_destroy(&semaforo);
        return 1;
    }

    if (pthread_create(&hilo2, NULL, contar, &id2) != 0)
    {
        perror("Error al crear hilo 2");
        pthread_cancel(hilo1);
        pthread_join(hilo1, NULL);
        sem_destroy(&semaforo);
        return 1;
    }

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    sem_destroy(&semaforo);

    printf("\nLOS DOS HILOS TERMINARON CORRECTAMENTE.\n");

    return 0;
}
