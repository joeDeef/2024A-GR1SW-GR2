/*
    Computacion Grafica
    Tarea B2T3: 3D Transformation and Camera
    Detalles: Diseñar e implementar una escena tipo MINECRAFT. 
              Utilizar camera class e implementar un sistema de cámara de la escena.
    Estudiante: Angel David Chuncho Jimenez
    Fecha: 10/07/2024
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Angel Chuncho - 0705977866", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // configure global opengl state
    glEnable(GL_DEPTH_TEST);


    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("shaders/B2T3_Angel_Chuncho_shader.vs", "shaders/B2T3_Angel_Chuncho_shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // texture coords
        // Homer Simpson (Texture 7)
        // ----------------------------------------------------
        -0.5f, -0.5f, -0.5f,  0.143f, 0.506f, // cara posterior
         0.5f, -0.5f, -0.5f,  0.347f, 0.506f,
         0.5f,  0.5f, -0.5f,  0.347f, 0.685f,
         0.5f,  0.5f, -0.5f,  0.347f, 0.685f,
        -0.5f,  0.5f, -0.5f,  0.143f, 0.685f,
        -0.5f, -0.5f, -0.5f,  0.143f, 0.506f,

        -0.5f, -0.5f,  0.5f,  0.552f, 0.506f, // cara frontal
         0.5f, -0.5f,  0.5f,  0.757f, 0.506f,
         0.5f,  0.5f,  0.5f,  0.757f, 0.685f,
         0.5f,  0.5f,  0.5f,  0.757f, 0.685f,
        -0.5f,  0.5f,  0.5f,  0.552f, 0.685f,
        -0.5f, -0.5f,  0.5f,  0.552f, 0.506f,

        -0.5f,  0.5f,  0.5f,  0.552f, 0.685f, // oreja izquierda
        -0.5f,  0.5f, -0.5f,  0.347f, 0.685f,
        -0.5f, -0.5f, -0.5f,  0.347f, 0.506f,
        -0.5f, -0.5f, -0.5f,  0.347f, 0.506f,
        -0.5f, -0.5f,  0.5f,  0.552f, 0.506f,
        -0.5f,  0.5f,  0.5f,  0.552f, 0.685f,
        
         0.5f,  0.5f,  0.5f,  0.757f, 0.685f, // oreja derecha f
         0.5f,  0.5f, -0.5f,  0.961f, 0.685f,
         0.5f, -0.5f, -0.5f,  0.961f, 0.506f,
         0.5f, -0.5f, -0.5f,  0.961f, 0.506f,
         0.5f, -0.5f,  0.5f,  0.757f, 0.506f,
         0.5f,  0.5f,  0.5f,  0.757f, 0.685f,
        
        -0.5f, -0.5f, -0.5f,  0.552f, 0.247f, // parte inferior c
         0.5f, -0.5f, -0.5f,  0.758f, 0.247f,
         0.5f, -0.5f,  0.5f,  0.757f, 0.506f,
         0.5f, -0.5f,  0.5f,  0.757f, 0.506f,
        -0.5f, -0.5f,  0.5f,  0.552f, 0.506f,
        -0.5f, -0.5f, -0.5f,  0.552f, 0.247f,

        -0.5f,  0.5f, -0.5f,  0.552f, 0.944f, // parte superior
         0.5f,  0.5f, -0.5f,  0.757f, 0.944f,
         0.5f,  0.5f,  0.5f,  0.757f, 0.685f,
         0.5f,  0.5f,  0.5f,  0.757f, 0.685f,
        -0.5f,  0.5f,  0.5f,  0.552f, 0.685f,
        -0.5f,  0.5f, -0.5f,  0.552f, 0.944f,

        // BobaFett
        // ----------------------------------------------------
        -0.5f, -0.5f, -0.5f,  0.142f, 0.506f, // cara posterior
         0.5f, -0.5f, -0.5f,  0.346f, 0.506f,
         0.5f,  0.5f, -0.5f,  0.346f, 0.686f,
         0.5f,  0.5f, -0.5f,  0.346f, 0.686f,
        -0.5f,  0.5f, -0.5f,  0.142f, 0.686f,
        -0.5f, -0.5f, -0.5f,  0.142f, 0.506f,

        -0.5f, -0.5f,  0.5f,  0.551f, 0.506f, // cara frontal
         0.5f, -0.5f,  0.5f,  0.756f, 0.506f, 
         0.5f,  0.5f,  0.5f,  0.756f, 0.686f,
         0.5f,  0.5f,  0.5f,  0.756f, 0.686f,
        -0.5f,  0.5f,  0.5f,  0.551f, 0.686f,
        -0.5f, -0.5f,  0.5f,  0.551f, 0.506f,

        -0.5f,  0.5f,  0.5f,  0.551f, 0.686f, // oreja izquierda
        -0.5f,  0.5f, -0.5f,  0.346f, 0.686f,
        -0.5f, -0.5f, -0.5f,  0.346f, 0.506f,
        -0.5f, -0.5f, -0.5f,  0.346f, 0.506f,
        -0.5f, -0.5f,  0.5f,  0.551f, 0.506f,
        -0.5f,  0.5f,  0.5f,  0.551f, 0.686f,

         0.5f,  0.5f,  0.5f,  0.757f, 0.685f, // oreja derecha
         0.5f,  0.5f, -0.5f,  0.961f, 0.685f,
         0.5f, -0.5f, -0.5f,  0.961f, 0.506f,
         0.5f, -0.5f, -0.5f,  0.961f, 0.506f,
         0.5f, -0.5f,  0.5f,  0.757f, 0.506f, 
         0.5f,  0.5f,  0.5f,  0.757f, 0.685f,

        -0.5f, -0.5f, -0.5f,  0.552f, 0.247f, // parte inferior
         0.5f, -0.5f, -0.5f,  0.758f, 0.247f,
         0.5f, -0.5f,  0.5f,  0.757f, 0.506f,
         0.5f, -0.5f,  0.5f,  0.757f, 0.506f,
        -0.5f, -0.5f,  0.5f,  0.552f, 0.506f,
        -0.5f, -0.5f, -0.5f,  0.552f, 0.247f,

        -0.5f,  0.5f, -0.5f,  0.552f, 0.944f, // parte superior
         0.5f,  0.5f, -0.5f,  0.757f, 0.944f,
         0.5f,  0.5f,  0.5f,  0.757f, 0.685f,
         0.5f,  0.5f,  0.5f,  0.757f, 0.685f,
        -0.5f,  0.5f,  0.5f,  0.552f, 0.685f,
        -0.5f,  0.5f, -0.5f,  0.552f, 0.944f,

        // Stormtrooper
        // -------------------------------------------------
        -0.5f, -0.5f, -0.5f,  0.145f, 0.510f, // cara posterior
         0.5f, -0.5f, -0.5f,  0.350f, 0.510f,
         0.5f,  0.5f, -0.5f,  0.350f, 0.688f,
         0.5f,  0.5f, -0.5f,  0.350f, 0.688f,
        -0.5f,  0.5f, -0.5f,  0.145f, 0.688f,
        -0.5f, -0.5f, -0.5f,  0.145f, 0.510f,

        -0.5f, -0.5f,  0.5f,  0.554f, 0.510f, // cara frontal
         0.5f, -0.5f,  0.5f,  0.758f, 0.510f, 
         0.5f,  0.5f,  0.5f,  0.758f, 0.688f,
         0.5f,  0.5f,  0.5f,  0.758f, 0.688f,
        -0.5f,  0.5f,  0.5f,  0.554f, 0.688f,
        -0.5f, -0.5f,  0.5f,  0.554f, 0.510f,

        -0.5f,  0.5f,  0.5f,  0.554f, 0.688f, // oreja izquierda 
        -0.5f,  0.5f, -0.5f,  0.350f, 0.688f,
        -0.5f, -0.5f, -0.5f,  0.350f, 0.510f,
        -0.5f, -0.5f, -0.5f,  0.350f, 0.510f,
        -0.5f, -0.5f,  0.5f,  0.554f, 0.510f,
        -0.5f,  0.5f,  0.5f,  0.554f, 0.688f,

         0.5f,  0.5f,  0.5f,  0.758f, 0.688f, // oreja derecha 
         0.5f,  0.5f, -0.5f,  0.962f, 0.688f,
         0.5f, -0.5f, -0.5f,  0.962f, 0.510f,
         0.5f, -0.5f, -0.5f,  0.962f, 0.510f,
         0.5f, -0.5f,  0.5f,  0.758f, 0.510f, 
         0.5f,  0.5f,  0.5f,  0.758f, 0.688f,

        -0.5f, -0.5f, -0.5f,  0.554f, 0.253f, // parte inferior
         0.5f, -0.5f, -0.5f,  0.758f, 0.253f,
         0.5f, -0.5f,  0.5f,  0.758f, 0.510f, 
         0.5f, -0.5f,  0.5f,  0.758f, 0.510f, 
        -0.5f, -0.5f,  0.5f,  0.554f, 0.510f,
        -0.5f, -0.5f, -0.5f,  0.554f, 0.253f,

        -0.5f,  0.5f, -0.5f,  0.554f, 0.943f, // parte superior
         0.5f,  0.5f, -0.5f,  0.758f, 0.943f,
         0.5f,  0.5f,  0.5f,  0.758f, 0.688f,
         0.5f,  0.5f,  0.5f,  0.758f, 0.688f,
        -0.5f,  0.5f,  0.5f,  0.554f, 0.688f,
        -0.5f,  0.5f, -0.5f,  0.554f, 0.943f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2, texture3;
    // texture 1
    // ---------
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
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("textures/Texture7.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("textures/textureBobaFett.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 3
    // ---------
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("textures/textureTropper.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setInt("texture3", 2);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            float time = glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            // Asignar la textura correspondiente a cada cubo
            if (i < 4) {
                ourShader.setInt("setTexture", 0);
                // Rotacion conforme pasa el tiempo
                angle = 45.0f * sin(time);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
                ourShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i < 7) {
                ourShader.setInt("setTexture", 1);
                // Escala en el tiempo
                float scale = abs(sin(time));
                model = glm::scale(model, glm::vec3(scale, scale, scale));
                ourShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 36, 36);
            }
            else {
                ourShader.setInt("setTexture", 2);
                // Traslacion en el tiempo
                float zoffset = sin(time) * 0.7f;
                glm::vec3 translation = cubePositions[i] + glm::vec3(0.0f, 0.0f, zoffset);
                model = glm::translate(model, translation);
                ourShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 72, 36);
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
