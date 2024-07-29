#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int main();
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1050;
const unsigned int SCR_HEIGHT = 850;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//SE ESTABLE EL CODIGO DEL FRAGMENT SHADER PARA CADA COLOR QUE SE VA A USAR
//Rojo
const char* fragmentShaderSourceRed = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.40f, 0.2f, 1.0f);\n"
"}\n\0";

//Amarillo
const char* fragmentShaderSourceYellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.89f, 0.79f, 0.2f, 1.0f);\n"
"}\n\0";

//Verde Oscuro
const char* fragmentShaderSourceDarkGreen = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.27f, 0.85f, 0.45f, 1.0f);\n"
"}\n\0";

//Verde Claro
const char* fragmentShaderSourceLigthGreen = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.58f, 0.85f, 0.27f, 1.0f);\n"
"}\n\0";

//Azul
const char* fragmentShaderSourceBlue = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.26f, 0.58f, 0.87f, 1.0f);\n"
"}\n\0";

//Morado
const char* fragmentShaderSourcePurple = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.43f, 0.38f, 0.93f, 1.0f);\n"
"}\n\0";

//Naranja
const char* fragmentShaderSourceOrange = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.93f, 0.69f, 0.38f, 1.0f);\n"
"}\n\0";

//Metodos para cada accion del programa
void dibujarCuadrado(unsigned int shaderProgram, unsigned int VAO, int numTriangulos);
void dibujarTriangulos(unsigned int shaderProgram, unsigned int VAO, int numTriangulos);
void cargarCompilarFragmentShader(unsigned int fragmentShader, const char* fragmentShaderSource);
void crearPrograma(unsigned int shaderProgram, unsigned int fragmentShader, unsigned int vertexShader);
void configurarDatosParaTriangulos(unsigned int VAO, unsigned int VBO, float* vertices, int size);
void configurarDatosParaCuadrilateros(unsigned int VAO, unsigned int VBO, float* vertices, int size, unsigned int EBO, unsigned int* indices, int sizeIndices);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Defaz Joel - CI: 1724915770", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Creamos y compilamos el codigo del Vertex
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Creanos y compilamos el fragment shader para el color Rojo, ademas de crear un programa para ejecutarlo
    unsigned int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramRed = glCreateProgram();
    cargarCompilarFragmentShader(fragmentShaderRed, fragmentShaderSourceRed);
    crearPrograma(shaderProgramRed, fragmentShaderRed, vertexShader);

    //Color Amarillo
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramYellow = glCreateProgram();
    cargarCompilarFragmentShader(fragmentShaderYellow, fragmentShaderSourceYellow);
    crearPrograma(shaderProgramYellow, fragmentShaderYellow, vertexShader);

    //Color Verde Oscuro
    unsigned int fragmentShaderGreen1 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramGreen1 = glCreateProgram();
    cargarCompilarFragmentShader(fragmentShaderGreen1, fragmentShaderSourceDarkGreen);
    crearPrograma(shaderProgramGreen1, fragmentShaderGreen1, vertexShader);

    //Color Verde Claro
    unsigned int fragmentShaderGreen2 = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramGreen2 = glCreateProgram();
    cargarCompilarFragmentShader(fragmentShaderGreen2, fragmentShaderSourceLigthGreen);
    crearPrograma(shaderProgramGreen2, fragmentShaderGreen2, vertexShader);

    //Color Azul
    unsigned int fragmentShaderBlue = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramBlue = glCreateProgram();
    cargarCompilarFragmentShader(fragmentShaderBlue, fragmentShaderSourceBlue);
    crearPrograma(shaderProgramBlue, fragmentShaderBlue, vertexShader);

    //Color Naranja
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramOrange = glCreateProgram();
    cargarCompilarFragmentShader(fragmentShaderOrange, fragmentShaderSourceOrange);
    crearPrograma(shaderProgramOrange, fragmentShaderOrange, vertexShader);

    //Color Morado
    unsigned int fragmentShaderPurple = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramPurple = glCreateProgram();
    cargarCompilarFragmentShader(fragmentShaderPurple, fragmentShaderSourcePurple);
    crearPrograma(shaderProgramPurple, fragmentShaderPurple, vertexShader);

    //VERTICES PARA LAS FIGURAS

    float cuadradoRojo[] = {
       -0.33f, 0.81f, 0.0f, // A
        -0.38f, 0.56f, 0.0f, // B
        -0.12f, 0.49f, 0.0f,  // C 
        -0.07f, 0.74f, 0.0f   //D
    };

    unsigned int indicesCuadradoRojo[] = {
       0, 1, 2,
       0, 2, 3
    };

    float trianguloAmarillo[] = {
     -0.62f, 0.44f, 0.0f, // E
    -0.21f, 0.1f, 0.0f, // F
    0.14f, 0.51f, 0.0f  // U
    };

    float trianguloVerdeOscuro[] = {
     -0.07f, 0.26f, 0.0f, // G
     -0.44f, -0.14f, 0.0f, // H
     0.3f, -0.08f, 0.0f  // I
    };

    float trianguloVerdeClaro[] = {
     -0.44f, -0.14f, 0.0f, // H
     -0.42f, -0.51f, 0.0f, // J
     -0.05f, -0.105f, 0.0f  // K
    };

    float trianguloAzul[] = {
     -0.42f, -0.51f, 0.0f, // J
     -0.69f, -0.74f, 0.0f, // P
     -0.42f, -0.78f, 0.0f  // Q
    };

    float trianguloNaranja[] = {
     0.38f, -0.2f, 0.0f, // R
     0.68f, -0.20f, 0.0f, // S
     0.7f, -0.51f, 0.0f  // T
    };
    
    float cuadradoMorado[] = {
    0.03f, -0.1f, 0.0f, // L
    0.26f, -0.33f, 0.0f, // M
    0.53f, -0.34f, 0.0f,  // N 
    0.25f, -0.08f, 0.0f   // O
    };

    unsigned int indicesCuadradoMorado[] = {
        0,1,3,
        3,1,2
    };

    unsigned int VBOs[7], VAOs[7], EBOs[2]; //Definimos un arreglo de VAOs, VBOs por cada figura del dibujo y uno de EBOs por cada figura compuesta por primitivas
    glGenVertexArrays(7, VAOs);
    glGenBuffers(7, VBOs);
    glGenBuffers(2, EBOs);

    //Configuramos todos los datos para el cuadrado rojo
    configurarDatosParaCuadrilateros(VAOs[0], VBOs[0], cuadradoRojo, sizeof(cuadradoRojo) ,EBOs[0], indicesCuadradoRojo, sizeof(indicesCuadradoRojo));
    configurarDatosParaTriangulos(VAOs[1], VBOs[1], trianguloAmarillo, sizeof(trianguloAmarillo));          //Configuramos todo para el triangulo amarillo
    configurarDatosParaTriangulos(VAOs[2], VBOs[2], trianguloVerdeOscuro, sizeof(trianguloVerdeOscuro));    //Triangulo verde oscuro
    configurarDatosParaTriangulos(VAOs[3], VBOs[3], trianguloVerdeClaro, sizeof(trianguloVerdeClaro));      //Triangulo verde claro
    configurarDatosParaTriangulos(VAOs[4], VBOs[4], trianguloAzul, sizeof(trianguloAzul));                  //Triangulo azul
    configurarDatosParaTriangulos(VAOs[5], VBOs[5], trianguloNaranja, sizeof(trianguloNaranja));            //Triangulo naranja
    //Configuramos todos los datos para el cuadrado morado
    configurarDatosParaCuadrilateros(VAOs[6], VBOs[6], cuadradoMorado, sizeof(cuadradoMorado), EBOs[1], indicesCuadradoMorado, sizeof(indicesCuadradoMorado));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        dibujarCuadrado(shaderProgramRed, VAOs[0], 9);           //Usamos el programa del cuadrado rojo 
        dibujarTriangulos(shaderProgramYellow, VAOs[1], 3);      //Triangulo amarillo
        dibujarTriangulos(shaderProgramGreen1, VAOs[2], 3);      //Triangulo verde oscuro
        dibujarTriangulos(shaderProgramGreen2, VAOs[3], 3);      //Triangulo verde claro
        dibujarTriangulos(shaderProgramBlue, VAOs[4], 3);        //Triangulo azul
        dibujarTriangulos(shaderProgramOrange, VAOs[5], 3);      //Triangulo naranja
        dibujarCuadrado(shaderProgramPurple, VAOs[6], 9);        //Cuadrado morado

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //Liberamos recursos para la memoria
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//Metodo que presenta en la ejecucion mediante el uso de EBOs para figuras cmpuestas de primitivas
void dibujarCuadrado(unsigned int shaderProgram, unsigned int VAO, int numTriangulos) {
    glUseProgram(shaderProgram);    //Se usa el programa que se creo previamente
    glBindVertexArray(VAO);         //Se indica de donde esta almacenado el VAO
    glDrawElements(GL_TRIANGLES, numTriangulos, GL_UNSIGNED_INT, 0);    //Se indica el numero de triangulos que dibujara
}

//Metodo que presenta en la ejecucion mediante el uso de triangulos
void dibujarTriangulos(unsigned int shaderProgram, unsigned int VAO, int numTriangulos) {
    glUseProgram(shaderProgram);        //Se usa el programa creado previamente
    glBindVertexArray(VAO);             //Se indica el VAO
    glDrawArrays(GL_TRIANGLES, 0, numTriangulos);   //Se indica el numero de triangulos que dibujara
}

//Metodo que carga el codigo fuente del fragment shader y lo compila
void cargarCompilarFragmentShader(unsigned int fragmentShader, const char* fragmentShaderSource) {
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);     //Lo caarga
    glCompileShader(fragmentShader);                                    //Lo compila
}

//Crea el programa para cada fragment shader
void crearPrograma(unsigned int shaderProgram, unsigned int fragmentShader, unsigned int vertexShader) {
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
}

//Metodo qye configura la forma en que s leen los datos y de donde obtener los datos para dibujar utilizando primitivas
void configurarDatosParaTriangulos(unsigned int VAO, unsigned int VBO, float* vertices, int size) {
    glBindVertexArray(VAO);                 //Se indica el VAO  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     //Se indica el VBO
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);      //Se pasan los datos de los vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);                       //Se indica como leerlos
    glEnableVertexAttribArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

//Metodo qye configura la forma en que s leen los datos y de donde obtener los datos para dibujar cuadrilateros compuestos de primitivas
void configurarDatosParaCuadrilateros(unsigned int VAO, unsigned int VBO, float* vertices, int size, unsigned int EBO, unsigned int* indices, int sizeIndices) {
    glBindVertexArray(VAO);             // se indice el VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // se indica el VBO
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);      //Se pasan los datos de los vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                                         //Se indica el EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices * sizeof(float), indices, GL_STATIC_DRAW);    //Se pasan los indices de los vertices para que sepa como armar la figura
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);       //Se indica como leerlos
    glEnableVertexAttribArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}