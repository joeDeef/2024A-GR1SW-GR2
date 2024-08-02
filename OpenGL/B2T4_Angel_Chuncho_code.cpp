/*
    Computacion Grafica
    Tarea B2T4: The Phong Illumination Model
    Detalles: Utilizar los ejercicios realizados en clase para generar una aplicación 
              con efectos de iluminación en movimiento (fuentes de luz y objetos) 
              utilizando diffuse and specular map.
    Estudiante: Angel David Chuncho Jimenez
    Fecha: 21/07/2024
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

unsigned int loadTexture(const char* path);

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

//lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
    Shader lightingShader("shaders/B2T4_Angel_Chuncho_shader_casters.vs", "shaders/B2T4_Angel_Chuncho_shader_casters.fs");
    Shader lightCubeShader("shaders/B2T4_Angel_Chuncho_shader_lightcube.vs", "shaders/B2T4_Angel_Chuncho_shader_lightcube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        // Container 2
        // -------------------------------------------------------
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,

        // Homer Simpson (Texture 7)
        // -------------------------------------------------------
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.143f, 0.506f, // cara posterior
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.347f, 0.506f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.347f, 0.685f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.347f, 0.685f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.143f, 0.685f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.143f, 0.506f,
                                                  
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.552f, 0.506f, // cara frontal
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.757f, 0.506f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.757f, 0.685f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.757f, 0.685f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.552f, 0.685f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.552f, 0.506f,
                                                  
        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.552f, 0.685f, // oreja izquierda
        -0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.347f, 0.685f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.347f, 0.506f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.347f, 0.506f,
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.552f, 0.506f,
        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.552f, 0.685f,
                                                  
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.757f, 0.685f, // oreja derecha
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.961f, 0.685f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.961f, 0.506f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.961f, 0.506f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.757f, 0.506f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.757f, 0.685f,
                                                  
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.552f, 0.247f, // parte inferior
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.758f, 0.247f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.757f, 0.506f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.757f, 0.506f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.552f, 0.506f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.552f, 0.247f,
                                                  
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.944f, // parte superior
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.757f, 0.944f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.757f, 0.685f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.757f, 0.685f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.685f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.944f,

        // BobaFett
        // -------------------------------------------------------
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.142f, 0.506f, // cara posterior
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.346f, 0.506f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.346f, 0.686f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.346f, 0.686f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.142f, 0.686f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.142f, 0.506f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.551f, 0.506f, // cara frontal
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.756f, 0.506f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.756f, 0.686f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.756f, 0.686f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.551f, 0.686f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.551f, 0.506f,

        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.551f, 0.686f, // oreja izquierda
        -0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.346f, 0.686f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.346f, 0.506f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.346f, 0.506f,
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.551f, 0.506f,
        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.551f, 0.686f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.757f, 0.685f, // oreja derecha
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.961f, 0.685f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.961f, 0.506f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.961f, 0.506f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.757f, 0.506f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.757f, 0.685f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.552f, 0.247f, // parte inferior
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.758f, 0.247f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.757f, 0.506f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.757f, 0.506f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.552f, 0.506f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.552f, 0.247f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.944f, // parte superior
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.757f, 0.944f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.757f, 0.685f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.757f, 0.685f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.685f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.944f,

        // Stormtrooper
        // -------------------------------------------------------
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.145f, 0.510f, // cara posterior
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.350f, 0.510f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.350f, 0.688f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.350f, 0.688f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.145f, 0.688f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.145f, 0.510f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.554f, 0.510f, // cara frontal
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.758f, 0.510f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.758f, 0.688f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.758f, 0.688f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.554f, 0.688f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.554f, 0.510f,

        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.554f, 0.688f, // oreja izquierda 
        -0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.350f, 0.688f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.350f, 0.510f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.350f, 0.510f,
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.554f, 0.510f,
        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.554f, 0.688f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.758f, 0.688f, // oreja derecha 
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.962f, 0.688f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.962f, 0.510f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.962f, 0.510f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.758f, 0.510f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.758f, 0.688f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.554f, 0.253f, // parte inferior
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.758f, 0.253f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.758f, 0.510f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.758f, 0.510f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.554f, 0.510f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.554f, 0.253f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.554f, 0.943f, // parte superior
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.758f, 0.943f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.758f, 0.688f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.758f, 0.688f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.554f, 0.688f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.554f, 0.943f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -6.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-2.8f, -2.0f, -5.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 2.5f, -5.2f, -2.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normals attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture coord attribute
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0);


    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    unsigned int diffuseMap = loadTexture("textures/container2.png");
    unsigned int specularMap = loadTexture("textures/container2_specular.png");
    unsigned int texture1 = loadTexture("textures/Texture7.png");
    unsigned int texture2 = loadTexture("textures/textureBobaFett.png");
    unsigned int texture3 = loadTexture("textures/textureTropper.jpg");
    unsigned int emissionMap = loadTexture("textures/matrix.jpg");
    
    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    lightingShader.setInt("material.emission", 2);

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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        lightingShader.use();

        // implement point lights
        float radius = 5.0f; // Radio de la órbita
        float lightAngle = currentFrame; // Ángulo basado en el tiempo

        // Nueva posición de la luz en órbita
        lightPos.x = radius * cos(lightAngle);
        lightPos.z = radius * sin(lightAngle);

        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // values for atenuation equation
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.09f);
        lightingShader.setFloat("light.quadratic", 0.032f);

        // material properties
        lightingShader.setFloat("material.shininess", 32.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render boxes
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            float time = glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            // Asignar la textura correspondiente a cada cubo
            if (i < 1) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuseMap); // Container 2

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, specularMap);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, emissionMap);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i < 4) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture1); // Homer Simpson

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, 0);

                glActiveTexture(GL_TEXTURE2); // Unbind the emission map for other cubes
                glBindTexture(GL_TEXTURE_2D, 0);

                // Rotacion conforme pasa el tiempo
                angle = 45.0f * sin(time);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
                lightingShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 36, 36);
            }
            else if (i < 7) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture2); // Boba Fett
                // Escala en el tiempo
                float scale = abs(sin(time));
                model = glm::scale(model, glm::vec3(scale, scale, scale));
                lightingShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 72, 36);
            }
            else {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture3); // Storm Tropper
                // Traslacion en el tiempo
                float zoffset = sin(time) * 0.7f;
                glm::vec3 translation = cubePositions[i] + glm::vec3(0.0f, 0.0f, zoffset);
                model = glm::translate(model, translation);
                lightingShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 108, 36);
            }
        }

        // Draw the light cube
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.4f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
