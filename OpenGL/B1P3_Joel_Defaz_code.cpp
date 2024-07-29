#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

//Variables para el movimiento de la figura
float mov_x,mov_y;

void configurarDatosParaTriangulos(unsigned int VAO, unsigned int VBO, float* vertices, int size);
void dibujarTriangulo(unsigned int VAO, int numTriangulos);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Defaz Joel - CI:1724915770", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    Shader ourShader("shaders/B1P3_Joel_Defaz.vs", "shaders/B1P3_Joel_Defaz.fs");

    //Vertices para cada triangulo de la figura con su respectivo color
    float trianguloRojo1[] = {
       -0.33f, 0.81f, 0.0f, 1.0f, 0.40f, 0.2f, // A
        -0.38f, 0.56f, 0.0f, 1.0f, 0.40f, 0.2f, // B
        -0.12f, 0.49f, 0.0f, 1.0f, 0.40f, 0.2f, // C 
    };

    float trianguloRojo2[] = {
    -0.33f, 0.81f, 0.0f, 1.0f, 0.40f, 0.2f, // A
    -0.12f, 0.49f, 0.0f, 1.0f, 0.40f, 0.2f, // C 
    -0.07f, 0.74f, 0.0f, 1.0f, 0.40f, 0.2f   //D
    };

    float trianguloAmarillo[] = {
     -0.62f, 0.44f, 0.0f, 0.89f, 0.79f, 0.2f,// E
    -0.21f, 0.1f, 0.0f, 0.89f, 0.79f, 0.2f,// F
    0.14f, 0.51f, 0.0f, 0.89f, 0.79f, 0.2f  // U
    };

    float trianguloVerdeOscuro[] = {
     -0.07f, 0.26f, 0.0f, 0.27f, 0.85f, 0.45f,// G
     -0.44f, -0.14f, 0.0f, 0.27f, 0.85f, 0.45f,// H
     0.3f, -0.08f, 0.0f,0.27f, 0.85f, 0.45f  // I
    };

    float trianguloVerdeClaro[] = {
     -0.44f, -0.14f, 0.0f, 0.58f, 0.85f, 0.27f,// H
     -0.42f, -0.51f, 0.0f, 0.58f, 0.85f, 0.27f,// J
     -0.05f, -0.105f, 0.0f ,0.58f, 0.85f, 0.27f // K
    };

    float trianguloAzul[] = {
     -0.42f, -0.51f, 0.0f,0.26f, 0.58f, 0.87f, // J
     -0.69f, -0.74f, 0.0f, 0.26f, 0.58f, 0.87f,// P
     -0.42f, -0.78f, 0.0f ,0.26f, 0.58f, 0.87f // Q
    };

    float trianguloNaranja[] = {
     0.38f, -0.2f, 0.0f, 0.93f, 0.69f, 0.38f, // R
     0.68f, -0.20f, 0.0f,0.93f, 0.69f, 0.38f, // S
     0.7f, -0.51f, 0.0f, 0.93f, 0.69f, 0.38f  // T
    };

    float trianguloMorado1[] = {
    0.03f, -0.1f, 0.0f, 0.43f, 0.38f, 0.93f, // L
    0.26f, -0.33f, 0.0f, 0.43f, 0.38f, 0.93f, // M
    0.25f, -0.08f, 0.0f ,0.43f, 0.38f, 0.93f  // O
    };

    float trianguloMorado2[] = {
    0.26f, -0.33f, 0.0f, 0.43f, 0.38f, 0.93f, // M
    0.53f, -0.34f, 0.0f, 0.43f, 0.38f, 0.93f,  // N 
    0.25f, -0.08f, 0.0f, 0.43f, 0.38f, 0.93f   // O
    };

    unsigned int VBOs[9], VAOs[9];
    glGenVertexArrays(9, VAOs);
    glGenBuffers(9, VBOs);

    configurarDatosParaTriangulos(VAOs[0], VBOs[0], trianguloRojo1, sizeof(trianguloRojo1));
    configurarDatosParaTriangulos(VAOs[1], VBOs[1], trianguloRojo2, sizeof(trianguloRojo2));
    configurarDatosParaTriangulos(VAOs[2], VBOs[2], trianguloAmarillo, sizeof(trianguloAmarillo));
    configurarDatosParaTriangulos(VAOs[3], VBOs[3], trianguloVerdeOscuro, sizeof(trianguloVerdeOscuro));
    configurarDatosParaTriangulos(VAOs[4], VBOs[4], trianguloVerdeClaro, sizeof(trianguloVerdeClaro));
    configurarDatosParaTriangulos(VAOs[5], VBOs[5], trianguloAzul, sizeof(trianguloAzul));
    configurarDatosParaTriangulos(VAOs[6], VBOs[6], trianguloNaranja, sizeof(trianguloNaranja));
    configurarDatosParaTriangulos(VAOs[7], VBOs[7], trianguloMorado1, sizeof(trianguloMorado1));
    configurarDatosParaTriangulos(VAOs[8], VBOs[8], trianguloMorado2, sizeof(trianguloMorado2));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // render the triangle
        ourShader.use();
        
        //Se dibuja cada figura
        dibujarTriangulo(VAOs[0], 3);
        dibujarTriangulo(VAOs[1], 3);
        dibujarTriangulo(VAOs[2], 3);
        dibujarTriangulo(VAOs[3], 3);
        dibujarTriangulo(VAOs[4], 3);
        dibujarTriangulo(VAOs[5], 3);
        dibujarTriangulo(VAOs[6], 3);
        dibujarTriangulo(VAOs[7], 3);
        dibujarTriangulo(VAOs[8], 3);
        
        //Se envian los valores para el movimiento
        ourShader.setFloat("mov_x", mov_x);
        ourShader.setFloat("mov_y", mov_y);
        
        //Se envia la variable del tiempo para el cambio de color
        float timeValue = glfwGetTime();
        int timeLocation = glGetUniformLocation(ourShader.ID, "tiempo");
        glUniform1f(timeLocation, timeValue);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Liberamos recursosº
    int tamano = sizeof(VAOs) / sizeof(VAOs[0]);
    for (int i = 0; i < tamano; i++) {
        glDeleteVertexArrays(1, &VAOs[0]);
        glDeleteBuffers(1, &VBOs[0]);
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)          //Aumenta la componente en y 
            mov_y = mov_y + 0.0001f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)        //Disminuye la componente en y 
            mov_y = mov_y - 0.0001f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)        //Aumenta la componente en x
            mov_x = mov_x - 0.0001f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)       //Disminuye la componente en x 
            mov_x = mov_x + 0.0001f;
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {         //Reinicia la figura a la posición inicial
            mov_x = 0.0f;
            mov_y = 0.0f;
        }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//Metodo qye configura la forma en que s leen los datos y de donde obtener los datos para dibujar utilizando primitivas
void configurarDatosParaTriangulos(unsigned int VAO, unsigned int VBO, float* vertices, int size) {
    glBindVertexArray(VAO);                 //Se indica el VAO  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     //Se indica el VBO
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);      //Se pasan los datos de los vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);                       //Se indica como leerlos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));                       //Se indica como leerlos
    glEnableVertexAttribArray(1);
}

//Metodo que presenta en la ejecucion mediante el uso de triangulos
void dibujarTriangulo(unsigned int VAO, int numTriangulos) {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, numTriangulos);
}