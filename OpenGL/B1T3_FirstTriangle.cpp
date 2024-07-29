#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Se carga el codigo del Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// Se carga el codigo del Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // Iniciamos las librearias para la ventana
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // Iniciamos la ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Defaz Joel - CI: 1724915770", NULL, NULL);        //Titulo de la ventana
    if (window == NULL)
    {  
        // Si no inicia la ventan notificamos el error
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //DAmos propiedades de la ventana
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Cargamos las librerias de Glad para asociarlas con OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);                //Creamos el vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);         //Indicamos el codigo
    glCompileShader(vertexShader);                                      //Compilamos el codigo

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //Verficiamos si el codigo del vertex shader se ha compilado correactamente, si no se indica el error producido
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);            //Creamos el fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);     //Indicamos el codigo
    glCompileShader(fragmentShader);                                    //Compialomos el codigo

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //Verififamos si el codigo del fragment shader se ha compilado correctamente, si no e¡se indica el error
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    int shaderProgram = glCreateProgram();                  //Creamos el programa shader
    //Se indica cuales seran el vextes y fragment shader al programma
    glAttachShader(shaderProgram, vertexShader);            
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);                           //enlazamos

    //Verificamos si se han enlazado correctamente, si no se notifica el error
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //Un avez enlazamos podemos eliminarlos para liberar memoria
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Se indica los vertices de cada triangulo para el poligno, en este caso se ha usado 9 tringulos que estan separados
    //Estos puntos se los ha echo con guia de Geogebra para saber cuales vertices se necesitan par cada triangulo por separado
    float vertices[] = {
    0.0f, 0.5f, 0.0f,
    -0.32f, 0.38f, 0.0f,
    0.0f, 0.0f, 0.0f,

    -0.49f, 0.08f, 0.0f,
    -0.32f, 0.38f, 0.0f,
    0.0f, 0.0f, 0.0f,

    -0.43f, -0.25f, 0.0f,
    - 0.49f, 0.08f, 0.0f,
    0.0f, 0.0f, 0.0f,

    -0.17f,-0.46f,0.0f,
    - 0.43f, -0.25f, 0.0f,
    0.0f, 0.0f, 0.0f,

    -0.17f,-0.46f,0.0f,
    0.17f, -0.46f, 0.0f,
    0.0f, 0.0f, 0.0f,

    0.43f, -0.25f, 0.0f,
    0.17f,-0.46f,0.0f,
    0.0f, 0.0f, 0.0f,

    0.49f,0.08f,0.0f,
    0.43f, -0.25f, 0.0f,
    0.0f, 0.0f, 0.0f,

    0.32f, 0.38f, 0.0f,
    0.49f,0.08f,0.0f,
    0.0f, 0.0f, 0.0f,

    0.0f, 0.5f, 0.0f,
    0.32f, 0.38f, 0.0f,
    0.0f, 0.0f, 0.0f,
    };

    unsigned int VBO, VAO;              //Creo los buffers de memoria de la GPU para los vertices
    glGenVertexArrays(1, &VAO);         //Genero un identificador
    glGenBuffers(1, &VBO);              //Genero un identificador para VBO
    glBindVertexArray(VAO);             

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                             //Indicarle que tipo de buffer sera y cual sera
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);      //Pasarle los datos del aCPu  al a GPU

    //Indica como debe leer el arreglo de los datos como en este caso so solov vertices se lo deja asi
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //Iniciamos el programa que se ira repitiendo 
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        //Limpiamos la pantalla/ventana cada vez
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Usamos el programa shader que indicamos antes
        glUseProgram(shaderProgram);
        // Notifica el Vertex Arrays Objects
        glBindVertexArray(VAO); 
        //Dibujamos la imagen, indicando qeu primitiva se usa, desde donde empieza y cuantos vertices existen
        glDrawArrays(GL_TRIANGLES, 0, 27);

        //Mostramos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Al terminar elimianos el array de datos y de configuración para eliminar memoria
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //Terminamos la ejecución 
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