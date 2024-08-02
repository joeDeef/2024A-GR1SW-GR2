/*
    Computacion Grafica
    Practica B2T1: Textures
    Detalles: Dividir el número de triángulos que conforman su modelo en 2 grupos y colocar dos texturas.
    Estudiante: Angel David Chuncho Jimenez
    Fecha: 23/06/2024
*/

// Libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION //Add to use stb_image.h
#include "learnopengl/stb_image.h" //Add to use stb_image.h

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, bool& swapTextures);

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
    Shader ourShader("shaders/B2T1_Angel_Chuncho_shader.vs", "shaders/B2T1_Angel_Chuncho_shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // Definition of vertices and their colors
    float vertices[] = {
        // Positions            // Colors            // Texture Coords
        -0.5f,  0.25f, 0.0f,    1.0f, 0.8f, 0.0f,    00.0f, 1.0f,  // T1 - ABC - Gold - Tex 1
         0.5f,  0.25f, 0.0f,    1.0f, 0.8f, 0.0f,    01.0f, 1.0f,
        -0.5f, -0.75f, 0.0f,    1.0f, 0.8f, 0.0f,    00.0f, 0.0f,
         0.5f,  0.25f, 0.0f,    0.2f, 0.8f, 0.4f,    01.0f, 1.0f,  // T2 - BCD - Spring Green - Text 1
        -0.5f, -0.75f, 0.0f,    0.2f, 0.8f, 0.4f,    00.0f, 0.0f,
         0.5f, -0.75f, 0.0f,    0.2f, 0.8f, 0.4f,    01.0f, 0.0f,
         0.5f,  0.25f, 0.0f,    0.2f, 0.6f, 1.0f,    0.78f, 0.5f,  // T3 - BEF - Dodger Blue - Text 2
         0.9f, -0.25f, 0.0f,    0.2f, 0.6f, 1.0f,    01.0f, 0.0f,
         0.9f,  0.25f, 0.0f,    0.2f, 0.6f, 1.0f,    01.0f, 0.5f,
         0.9f,  0.25f, 0.0f,    0.6f, 0.8f, 0.2f,    01.0f, 0.5f,  // T4 - FGH - Yellow Green - Text 2
         0.0f,  0.25f, 0.0f,    0.6f, 0.8f, 0.2f,    00.5f, 0.5f,
         0.5f,  0.75f, 0.0f,    0.6f, 0.8f, 0.2f,    0.78f, 1.0f,
         0.0f,  0.25f, 0.0f,    1.0f, 0.6f, 0.2f,    00.5f, 0.5f,  // T5 - GHI - Dark Orange - Text 2
         0.5f,  0.75f, 0.0f,    1.0f, 0.6f, 0.2f,    0.78f, 1.0f,
         0.0f,  0.75f, 0.0f,    1.0f, 0.6f, 0.2f,    00.5f, 1.0f,
        -0.5f,  0.25f, 0.0f,    1.0f, 0.4f, 0.4f,    0.22f, 0.5f,  // T6 - AGI - Salmon - Text 2
         0.0f,  0.25f, 0.0f,    1.0f, 0.4f, 0.4f,    00.5f, 0.5f,
         0.0f,  0.75f, 0.0f,    1.0f, 0.4f, 0.4f,    00.5f, 1.0f,
        -0.5f,  0.25f, 0.0f,    1.0f, 0.4f, 0.4f,    0.22f, 0.5f,  // T7 - AIJ - Salmon - Text 2
         0.0f,  0.75f, 0.0f,    1.0f, 0.4f, 0.4f,    00.5f, 1.0f,
        -0.5f,  0.75f, 0.0f,    1.0f, 0.4f, 0.4f,    0.22f, 1.0f,
        -0.5f,  0.25f, 0.0f,    0.4f, 0.4f, 0.8f,    0.22f, 0.5f,  // T8 - AKL - Medium Purple - Text 2
        -0.9f, -0.25f, 0.0f,    0.4f, 0.4f, 0.8f,    00.0f, 0.0f,
        -0.9f,  0.25f, 0.0f,    0.4f, 0.4f, 0.8f,    00.0f, 0.5f,
        -0.5f,  0.25f, 0.0f,    0.4f, 0.4f, 0.8f,    0.22f, 0.5f,  // T9 - AJL - Medium Purple - Text 2
        -0.5f,  0.75f, 0.0f,    0.4f, 0.4f, 0.8f,    0.22f, 1.0f,
        -0.9f,  0.25f, 0.0f,    0.4f, 0.4f, 0.8f,    00.0f, 0.5f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("textures/outerSpace.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 2
    // ----------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // set texture filtering to nearest neighbor to clearly see the texels/pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load("textures/universeStar.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    ourShader.setInt("texture2", 1);

    bool swapTextures = false;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        //processInput(window, setTexture, swap);
        processInput(window, swapTextures);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // render
        ourShader.use();

        // Pass time to the vertex shader
        float timeValue = glfwGetTime();
        ourShader.setFloat("time", timeValue);

        ourShader.setBool("swapTextures", swapTextures);

        // Draw the primitives using the active shaders
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 27);

        /*ourShader.setInt("setTexture", currentTexture1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ourShader.setInt("setTexture", currentTexture2);
        glDrawArrays(GL_TRIANGLES, 6, 21);*/

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
/// Input processing
/// </summary>
/// <param name="window">Desired window</param>
void processInput(GLFWwindow* window, bool& swapTextures)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        swapTextures = false;

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        swapTextures = true;
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