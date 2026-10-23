
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LIMITE 10000

// Funcion que ejecutara el hilo secundario
void *contar_descendente(void *arg)
{
    for (int i = LIMITE; i >= 1; i--)
    {
        printf("Hilo secundario: %d\n", i);
    }

    return NULL;
}

int main(void)
{
    pthread_t hilo;

    // Crear el hilo secundario
    if (pthread_create(&hilo, NULL, contar_descendente, NULL) != 0)
    {
        perror("Error al crear el hilo");
        return EXIT_FAILURE;
    }

    // El proceso principal cuenta del 1 al 10000
    for (int i = 1; i <= LIMITE; i++)
    {
        printf("Proceso principal: %d\n", i);
    }

    // Esperar a que termine el hilo secundario
    if (pthread_join(hilo, NULL) != 0)
    {
        perror("Error al esperar al hilo");
        return EXIT_FAILURE;
    }

    printf("\nEjecucion finalizada correctamente.\n");

    return EXIT_SUCCESS;
}

