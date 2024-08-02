/*
    Computacion Grafica
    Estudiante: Angel David Chuncho Jimenez
*/

// Incluir la biblioteca de GLFW ya que proporciona todas las constantes, tipos y prototipos de funciones
#include <GLFW/glfw3.h>

int main(void)
{
    // Puntero que representa la ventana
    GLFWwindow* window;

    // Se inicializa la biblioteca GLFW
    // Si la inicializacion falla, se termina el programa
    if (!glfwInit())
        return -1;

    // Creacion de la ventana y su contexto OpenGL
    // Los parámetros son: ancho de la ventana, alto de la ventana, titulo de la ventana, monitor y compartir recursos con otra ventana
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    // Si la ventana no se pudo crear, termina GLFW y el programa retorna -1
    if (!window)
    {
        glfwTerminate(); // metodo para terminar GLFW
        return -1;
    }

    // Actualiza el contexto de OpenGL de la ventana
    glfwMakeContextCurrent(window);

    // Bucle que se ejecuta hasta que el usuario cierra la ventana
    // Cada ventana tiene una bandera que indica si la ventana debe cerrarse.
    while (!glfwWindowShouldClose(window))
    {
        // Inicia la renderizacion
        // En este caso, se limpia el buffer de color
        glClear(GL_COLOR_BUFFER_BIT);

        // Intercambia los buffers frontal y posterior de la ventana especificada cuando se renderiza con OpenGL
        glfwSwapBuffers(window);

        // Procesa eventos que ya están en la cola de eventos y luego regresa inmediatamente.
        glfwPollEvents();
    }

    // Termina GLFW, liberando todos los recursos asignados por GLFW
    glfwTerminate();
    return 0;
}