/*
    Computacion Grafica
    Practica B1P2: Figura Vertices
    Detalles: Realizar una aplicación que muestre por pantalla el modelo asignado según la tabla adjunta.
    Estudiante: Angel David Chuncho Jimenez
    Fecha: 02/06/2024
*/

// Libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkCompileErrors(unsigned int shader, std::string type);

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Definition of configurable parts (Vertex y Fragment Shader)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderGoldSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.8f, 0.0f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderSpringGreenSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.8f, 0.4f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderDodgerBlueSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.6f, 1.f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderYellowGreenSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.6f, 0.8f, 0.2f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderDarkOrangeSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.6f, 0.2f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderSalmonSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.4f, 0.4f, 1.0f);\n"
"}\n\0";
const char* fragmentShaderMediumPurpleSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.4f, 0.4f, 0.8f, 1.0f);\n"
"}\n\0";

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // Window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Angel Chuncho - 0705977866", NULL, NULL);
    // Check if the window was created successfully, if not, terminate the program
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Creation of the VertexShader and verification of correct creation
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // Create Fragment Shaders
    unsigned int fragmentShaderGold = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderGold, 1, &fragmentShaderGoldSource, NULL);
    glCompileShader(fragmentShaderGold);
    checkCompileErrors(fragmentShaderGold, "FRAGMENT");

    unsigned int fragmentShaderSpringGreen = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderSpringGreen, 1, &fragmentShaderSpringGreenSource, NULL);
    glCompileShader(fragmentShaderSpringGreen);
    checkCompileErrors(fragmentShaderSpringGreen, "FRAGMENT");

    unsigned int fragmentShaderDodgerBlue = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderDodgerBlue, 1, &fragmentShaderDodgerBlueSource, NULL);
    glCompileShader(fragmentShaderDodgerBlue);
    checkCompileErrors(fragmentShaderDodgerBlue, "FRAGMENT");

    unsigned int fragmentShaderYellowGreen = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYellowGreen, 1, &fragmentShaderYellowGreenSource, NULL);
    glCompileShader(fragmentShaderYellowGreen);
    checkCompileErrors(fragmentShaderYellowGreen, "FRAGMENT");

    unsigned int fragmentShaderDarkOrange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderDarkOrange, 1, &fragmentShaderDarkOrangeSource, NULL);
    glCompileShader(fragmentShaderDarkOrange);
    checkCompileErrors(fragmentShaderDarkOrange, "FRAGMENT");

    unsigned int fragmentShaderSalmon = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderSalmon, 1, &fragmentShaderSalmonSource, NULL);
    glCompileShader(fragmentShaderSalmon);
    checkCompileErrors(fragmentShaderSalmon, "FRAGMENT");

    unsigned int fragmentShaderMediumPurple = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderMediumPurple, 1, &fragmentShaderMediumPurpleSource, NULL);
    glCompileShader(fragmentShaderMediumPurple);
    checkCompileErrors(fragmentShaderMediumPurple, "FRAGMENT");
    
    // Create Shader Programs and link the programs objects
    unsigned int shaderProgramGold = glCreateProgram();
    glAttachShader(shaderProgramGold, vertexShader);
    glAttachShader(shaderProgramGold, fragmentShaderGold);
    glLinkProgram(shaderProgramGold);
    checkCompileErrors(shaderProgramGold, "PROGRAM");

    unsigned int shaderProgramSpringGreen = glCreateProgram();
    glAttachShader(shaderProgramSpringGreen, vertexShader);
    glAttachShader(shaderProgramSpringGreen, fragmentShaderSpringGreen);
    glLinkProgram(shaderProgramSpringGreen);
    checkCompileErrors(shaderProgramSpringGreen, "PROGRAM");

    unsigned int shaderProgramDodgerBlue = glCreateProgram();
    glAttachShader(shaderProgramDodgerBlue, vertexShader);
    glAttachShader(shaderProgramDodgerBlue, fragmentShaderDodgerBlue);
    glLinkProgram(shaderProgramDodgerBlue);
    checkCompileErrors(shaderProgramDodgerBlue, "PROGRAM");

    unsigned int shaderProgramYellowGreen = glCreateProgram();
    glAttachShader(shaderProgramYellowGreen, vertexShader);
    glAttachShader(shaderProgramYellowGreen, fragmentShaderYellowGreen);
    glLinkProgram(shaderProgramYellowGreen);
    checkCompileErrors(shaderProgramYellowGreen, "PROGRAM");

    unsigned int shaderProgramDarkOrange = glCreateProgram();
    glAttachShader(shaderProgramDarkOrange, vertexShader);
    glAttachShader(shaderProgramDarkOrange, fragmentShaderDarkOrange);
    glLinkProgram(shaderProgramDarkOrange);
    checkCompileErrors(shaderProgramDarkOrange, "PROGRAM");

    unsigned int shaderProgramSalmon = glCreateProgram();
    glAttachShader(shaderProgramSalmon, vertexShader);
    glAttachShader(shaderProgramSalmon, fragmentShaderSalmon);
    glLinkProgram(shaderProgramSalmon);
    checkCompileErrors(shaderProgramSalmon, "PROGRAM");

    unsigned int shaderProgramMediumPurple = glCreateProgram();
    glAttachShader(shaderProgramMediumPurple, vertexShader);
    glAttachShader(shaderProgramMediumPurple, fragmentShaderMediumPurple);
    glLinkProgram(shaderProgramMediumPurple);
    checkCompileErrors(shaderProgramMediumPurple, "PROGRAM");


    //Remove shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderGold);
    glDeleteShader(fragmentShaderSpringGreen);
    glDeleteShader(fragmentShaderDodgerBlue);
    glDeleteShader(fragmentShaderYellowGreen);
    glDeleteShader(fragmentShaderDarkOrange);
    glDeleteShader(fragmentShaderSalmon);
    glDeleteShader(fragmentShaderMediumPurple);

    // Definition of vertices and their indices
    float vertices[] = {
        // Triangle (T1)
        -0.5f,  0.25f, 0.0f, // A 
         0.5f,  0.25f, 0.0f, // B 
        -0.5f, -0.75f, 0.0f, // C
        // Triangle (T2): uses coordinates B and C.
         0.5f, -0.75f, 0.0f, // D
        // Triangle (T3): uses coordinate B.
         0.9f, -0.25f, 0.0f, // E
         0.9f,  0.25f, 0.0f, // F
        // Triangle (T4): uses coordinate F.
         0.0f,  0.25f, 0.0f, // G
         0.5f,  0.75f, 0.0f, // H
        // Triangle (T5): uses coordinates G and H.
         0.0f,  0.75f, 0.0f, // I
        // Polygon (P1): uses coordinates A, G and I for part 1 and coordinates A, I and J for part 2.
        -0.5f,  0.75f, 0.0f, // J
        // Polygon (P2): uses coordinates A, K and L for part 1 and coordinates A, J and L for part 2.
        -0.9f, -0.25f, 0.0f, // K
        -0.9f,  0.25f,0.0f   // L
    };

    unsigned int indices[] = {
        0,  1,  2,  // T1
        1,  2,  3,  // T2
        1,  4,  5,  // T3
        5,  6,  7,  // T4
        6,  7,  8,  // T5
        0,  6,  8,  // P1: part 1
        0,  8,  9,  // P1: part 2
        0, 10, 11,  // P2: part 1
        0,  9, 11   // P2: part 2
    };

    // Create the VBO (Vertex Buffer Object), VAO (Vertex Array Object) and EBO (Element Object Buffer)
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind VAO, bind and set VBO and EBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy the vertices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copy the indices

    // vertex position attributes, identify the components (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Color assignment and buffer cleaning
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Draw the primitives using the active shaders
        glBindVertexArray(VAO);

        // Draw using different shader programs
        glUseProgram(shaderProgramGold);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgramSpringGreen);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

        glUseProgram(shaderProgramDodgerBlue);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));

        glUseProgram(shaderProgramYellowGreen);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(9 * sizeof(unsigned int)));

        glUseProgram(shaderProgramDarkOrange);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));

        glUseProgram(shaderProgramSalmon);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(15 * sizeof(unsigned int)));

        glUseProgram(shaderProgramMediumPurple);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(21 * sizeof(unsigned int)));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

/// <summary>
/// Input processing, allows you to close a window by pressing a specific key, 
/// in this case, pressing esc closes the window.
/// </summary>
/// <param name="window">Desired window</param>
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/// <summary>
/// Adjusts the size of the viewport.
/// </summary>
/// <param name="window">Desired window</param>
/// <param name="width">Width</param>
/// <param name="height">Height</param>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/// <summary>
/// Allows you to check if there are any errors when creating the vertex shader, 
/// fragment shader and shader programs
/// </summary>
/// <param name="shader">shader</param>
/// <param name="type">type of shader</param>
void checkCompileErrors(unsigned int shader, std::string type) 
{
    int success;
    char infoLog[512];
    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAM::LINKING_FAILED of type: " << type << "\n" << infoLog << std::endl;
        }
    }
    else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED of type: " << type << "\n" << infoLog << std::endl;
        }
    }
}