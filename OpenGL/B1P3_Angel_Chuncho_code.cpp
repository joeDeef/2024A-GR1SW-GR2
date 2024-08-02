/*
    Computacion Grafica
    Practica B1P3: Figures 
    Detalles: Realizar una aplicación que muestre por pantalla el modelo asignado según la tabla adjunta.
    Estudiante: Angel David Chuncho Jimenez
    Fecha: 12/06/2024
*/

// Libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float& xOffset, float& yOffset);
void checkCompileErrors(unsigned int shader, std::string type);

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("shaders/B1P3_Angel_Chuncho_shader.vs", "shaders/B1P3_Angel_Chuncho_shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // Definition of vertices and their colors
    float vertices[] = {
        // Positions            // Colors
        -0.5f,  0.25f, 0.0f,    1.0f, 0.8f, 0.0f,  // T1 - ABC - Gold
         0.5f,  0.25f, 0.0f,    1.0f, 0.8f, 0.0f,
        -0.5f, -0.75f, 0.0f,    1.0f, 0.8f, 0.0f,
         0.5f,  0.25f, 0.0f,    0.2f, 0.8f, 0.4f,  // T2 - BCD - Spring Green
        -0.5f, -0.75f, 0.0f,    0.2f, 0.8f, 0.4f,
         0.5f, -0.75f, 0.0f,    0.2f, 0.8f, 0.4f,
         0.5f,  0.25f, 0.0f,    0.2f, 0.6f, 1.0f,  // T3 - BEF - Dodger Blue
         0.9f, -0.25f, 0.0f,    0.2f, 0.6f, 1.0f,
         0.9f,  0.25f, 0.0f,    0.2f, 0.6f, 1.0f,
         0.9f,  0.25f, 0.0f,    0.6f, 0.8f, 0.2f,  // T4 - FGH - Yellow Green
         0.0f,  0.25f, 0.0f,    0.6f, 0.8f, 0.2f,
         0.5f,  0.75f, 0.0f,    0.6f, 0.8f, 0.2f,
         0.0f,  0.25f, 0.0f,    1.0f, 0.6f, 0.2f,  // T5 - GHI - Dark Orange
         0.5f,  0.75f, 0.0f,    1.0f, 0.6f, 0.2f,
         0.0f,  0.75f, 0.0f,    1.0f, 0.6f, 0.2f,
        -0.5f,  0.25f, 0.0f,    1.0f, 0.4f, 0.4f,  // T6 - AGI - Salmon
         0.0f,  0.25f, 0.0f,    1.0f, 0.4f, 0.4f,
         0.0f,  0.75f, 0.0f,    1.0f, 0.4f, 0.4f,
        -0.5f,  0.25f, 0.0f,    1.0f, 0.4f, 0.4f,  // T7 - AIJ - Salmon
         0.0f,  0.75f, 0.0f,    1.0f, 0.4f, 0.4f,
        -0.5f,  0.75f, 0.0f,    1.0f, 0.4f, 0.4f,
        -0.5f,  0.25f, 0.0f,    0.4f, 0.4f, 0.8f,  // T8 - AKL - Medium Purple
        -0.9f, -0.25f, 0.0f,    0.4f, 0.4f, 0.8f,
        -0.9f,  0.25f, 0.0f,    0.4f, 0.4f, 0.8f,
        -0.5f,  0.25f, 0.0f,    0.4f, 0.4f, 0.8f,  // T9 - AJL - Medium Purple
        -0.5f,  0.75f, 0.0f,    0.4f, 0.4f, 0.8f,
        -0.9f,  0.25f, 0.0f,    0.4f, 0.4f, 0.8f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Dinamyc position
    float xOffset = 0.0f;
    float yOffset = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window, xOffset, yOffset);

        // Color assignment and buffer cleaning
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        ourShader.use();

        // Pass time to the fragment shader
        float timeValue = glfwGetTime();
        ourShader.setFloat("time", timeValue);

        // Pass offset to the vertex shader
        ourShader.setFloat("xOffset", xOffset);
        ourShader.setFloat("yOffset", yOffset);

        // Draw the primitives using the active shaders
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * 6));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

/// <summary>
/// Input processing, allows you to close a window by pressing a specific key, 
/// in this case, pressing esc closes the window.
/// </summary>
/// <param name="window">Desired window</param>
void processInput(GLFWwindow* window, float& xOffset, float& yOffset)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // moviment of the figure using direction keys
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        xOffset += 0.001f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        xOffset -= 0.001f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        yOffset += 0.001f;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        yOffset -= 0.001f;
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