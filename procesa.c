#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Imprime UNA línea completa con una sola llamada al sistema write().
   No usa el buffer de stdio, así que nada se queda a medias ni se
   mezcla con lo que escribe el otro proceso. */
static void imprimir_linea(const char *formato, long valor) {
    char linea[128];
    int n = snprintf(linea, sizeof(linea), formato, valor);
    write(STDOUT_FILENO, linea, n);
}

static void imprimir_texto(const char *texto) {
    write(STDOUT_FILENO, texto, strlen(texto));
}

int main() {
    pid_t pid;

    // Crear un nuevo proceso
    pid = fork();

    // Verificar si ocurrió un error
    if (pid < 0) {
        perror("Error al crear el proceso");
        return 1;
    }

    // Proceso hijo
    if (pid == 0) {
        imprimir_texto("=== PROCESO HIJO ===\n");
        imprimir_linea("PID del hijo: %ld\n", (long)getpid());

        for (int i = 10000; i >= 1; i--) {
            imprimir_linea("%ld\n", i);
        }
        _exit(0);
    }

    // Proceso padre
    imprimir_texto("=== PROCESO PADRE ===\n");
    imprimir_linea("PID del padre: %ld\n", (long)getpid());

    for (int i = 1; i <= 10000; i++) {
        imprimir_linea("%ld\n", i);
    }

    // Esperar a que termine el hijo
    wait(NULL);

    imprimir_texto("=== EL PROCESO HIJO HA TERMINADO ===\n");
    return 0;
}
