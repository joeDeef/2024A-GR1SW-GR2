#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>
bool keyPressedP = false; // Estado anterior de la tecla 'P'

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void setDisparoYPosicion();
void setPointLight(GLuint shaderID, int index, glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic);
void setSpotLight(GLuint shaderID, glm::vec3 position, float constant, float linear, float quadratic);
void setDirectionalLight(GLuint shaderID, glm::vec3 direcction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
void cambiarPerspectivaCamara();
// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(2.6f, 10.0f, -5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cambioCamara = false;
int transicion = 0;
float desplazamiento = 0.005;
int j = 0;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Posiciones de las cosas
glm::vec3 posicionNave = glm::vec3(-0.5f, 1.5f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 poscionLuna = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

//Configuracion de la camara
struct CameraSettings {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
};

//Confiracion para las diferentes posicone sy perspectivas de la camara
CameraSettings cameraSettings[] = {
    {glm::vec3(0.0f, 1.5f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // Inicial
    {glm::vec3(-5.0f, 1.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // Lado Izquierdo
    {glm::vec3(0.0f, 10.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)}  // Arriba
};


float transitionDuration1 = 5.0f; // Duración de la transición en segundos
float transitionDuration2 = 10.0f; // Duración de la transición en segundos

float currentTime = 0.0f;

int main()
{
    camera.Position = cameraSettings[0].position;
    camera.Front = cameraSettings[0].front;
    camera.Up = cameraSettings[0].up;

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercise 16 Task 3", NULL, NULL);
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

    // build and compile shaders
    Shader ourShader("shaders/shader_exercise16_mloading.vs", "shaders/shader_exercise16_mloading.fs");
    Shader lightingShader("shaders/shader_exercise15t5_casters.vs", "shaders/shader_exercise15t5_casters.fs");

    // Cargamos los modelos
    Model models[] = {
        Model("C:/Users/User/Documents/VisualStudio2022/OpenGL/OpenGL/model/immortalDemonic/demonio.obj"),
        Model("C:/Users/User/Documents/VisualStudio2022/OpenGL/OpenGL/model/ovni/ovni.obj"),
        Model("C:/Users/User/Documents/VisualStudio2022/OpenGL/OpenGL/model/nave/nave.obj"),
        Model("C:/Users/User/Documents/VisualStudio2022/OpenGL/OpenGL/model/escenario/escenario.obj"),
        Model("C:/Users/User/Documents/VisualStudio2022/OpenGL/OpenGL/model/calavera/calavera.obj"),
        Model("C:/Users/User/Documents/VisualStudio2022/OpenGL/OpenGL/model/moon/moon.obj"),
    };


    //Posicion Inicial de los modelos
    glm::vec3 posicionModelos[] = {
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(1.0f,1.5f,0.0f),
        glm::vec3(0.0f,-2.0f,0.0f),
    };    

    poscionLuna = posicionModelos[1] + glm::vec3(50.0f, 40.0f, 0.0f);

    glm::vec3 posicionPuntosLuz[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f),
        poscionLuna + glm::vec3(-10.0f,0.0f,0.0f)
    };

    glm::vec3 posicionColoresLuz[] = {
        glm::vec3(1.0f, 0.6f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0, 0.0),
        glm::vec3(0.2f, 0.2f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
    };

    glm::vec3 posicionCalaveras[] = {
        glm::vec3(1.0f, -0.1f, -5.0f),
        glm::vec3(-6.0f, -0.1f, -2.3f),
        glm::vec3(3.0f, -0.1f, 2.0),
        glm::vec3(2.2f, -0.1f, 6.4f),
        glm::vec3(-2.2f, -0.1f, -10.0f),
        glm::vec3(0.5f, -0.1f, 10.0f),
        glm::vec3(-8.5f, -0.1f, 4.0),
        glm::vec3(-10.0f, -0.1f, -4.0f)
    };
    camera.MovementSpeed = 12;

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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Configuración de las luces
        //----------------------------------------------------------------
        lightingShader.use();
        setSpotLight(lightingShader.ID, posicionNave, 1, 0.09, 0.032);          // SpotLight
        setDirectionalLight(lightingShader.ID, glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.2f, 0.2f, 0.2f));                                                  // Directional light

        //Indicar los diferentes puntos de luz para la escena
        for (int i = 0; i < 5; ++i) {
            if (i == 2) {
                setPointLight(lightingShader.ID, i,
                    posicionPuntosLuz[i],
                    posicionColoresLuz[i],
                    1.0f, 0.14f, 0.07f);  // Atenuación constante, lineal y cuadrática
            }
            else {
                setPointLight(lightingShader.ID, i,
                    posicionPuntosLuz[i],
                    posicionColoresLuz[i],
                    1.0f, 0.22f, 0.2f);
            }
        }
        
        //Para la transicion de la cámara

        //Configuración de la cámara
        //---------------------------
        cameraPos = camera.Position;
        glm::vec3 cameraFront = camera.Front;
        glm::vec3 cameraUp = camera.Up;

        std::cout << currentTime << std::endl;

        //Cambio de perspetiva de la camara
        //-----------------------------------
        float t = (transicion == 0) ? (currentTime / transitionDuration1) : (currentTime / transitionDuration2);
        t = glm::clamp(t, 0.0f, 1.0f);
        if (cambioCamara) {
            currentTime += 0.001f; // Incrementa el tiempo de transición
            if (transicion == 0) {
                j = 0;
            }
            else {
                j = 1;
            }
            if (transicion == 0 && currentTime >= 5) {
                cambioCamara = false;
                transicion = 1;
            }
            else if (transicion == 1 && currentTime >= 10) {
                cambioCamara = false;
            }
            cameraPos = glm::mix(cameraSettings[j].position, cameraSettings[j + 1].position, t);
            camera.Position = cameraSettings[j + 1].position;
            cameraFront = glm::mix(cameraSettings[j].front, cameraSettings[j + 1].front, t);
            camera.Front = cameraSettings[j + 1].front;
            cameraUp = glm::mix(cameraSettings[j].up, cameraSettings[j + 1].up, t);
            camera.Up = cameraSettings[j + 1].up;
        }

        // view/projection transformations
        //----------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        lightingShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);

        //Posicion del ovni
        model = glm::mat4(1.0f);
        model = glm::translate(model, posicionNave);
        model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        ourShader.setMat4("model", model);
        models[1].Draw(ourShader);

        //Posicion del Boss
        model = glm::mat4(1.0f);
        model = glm::translate(model, posicionModelos[1]);
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        ourShader.setMat4("model", model);
        models[0].Draw(ourShader);

        //Posicion del Moon
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,-float(glfwGetTime()),0.0f));
        model = glm::translate(model, poscionLuna);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        ourShader.setMat4("model", model);
        models[5].Draw(ourShader);

        //Posicion del Calaveras
        for (int i = 0; i < 8; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f,sin(glfwGetTime())/2,0.0f));
            model = glm::translate(model, posicionCalaveras[i]);
            model = glm::rotate(model, glm::radians(70.0f), glm::vec3(-1.0f, 0.0f, 2.5f));
            model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
            ourShader.setMat4("model", model);
            models[4].Draw(ourShader);
        }

        //Posicion del Escenario
        model = glm::mat4(1.0f);
        model = glm::translate(model, posicionModelos[0]);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", model);
        models[3].Draw(ourShader);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //camera.ProcessKeyboard(FORWARD, deltaTime);
        if (cameraPos == cameraSettings[1].position) {
            posicionNave.y += desplazamiento;
        }
        else if (cameraPos == cameraSettings[2].position) {
            posicionNave.z -= desplazamiento;
        }
        else {
            posicionNave.y += desplazamiento;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (cameraPos == cameraSettings[1].position) {
            posicionNave.y -= desplazamiento;
        }
        else if (cameraPos == cameraSettings[2].position) {
            posicionNave.z += desplazamiento;
        }
        else {
            posicionNave.y -= desplazamiento;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //camera.ProcessKeyboard(LEFT, deltaTime);
        if (cameraPos == cameraSettings[1].position) {
            posicionNave.z -= desplazamiento;
        }
        else if (cameraPos == cameraSettings[2].position) {
            posicionNave.x -= desplazamiento;
        }
        else {
            posicionNave.x -= desplazamiento;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //camera.ProcessKeyboard(RIGHT, deltaTime);
        if (cameraPos == cameraSettings[1].position) {
            posicionNave.z += desplazamiento;
        }
        else if (cameraPos == cameraSettings[2].position) {
            posicionNave.x += desplazamiento;
        }
        else {
            posicionNave.x += desplazamiento;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!keyPressedP) { // Si la tecla no estaba presionada anteriormente
            cambioCamara = true; // Ejecutar la acción una vez
            keyPressedP = true;  // Actualizar el estado de la tecla
        }
    }
    else {
        keyPressedP = false; // Resetear el estado cuando la tecla no esté presionada
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
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

//Configura los shaders para cada luz en la escena
void setPointLight(GLuint shaderID, int index, glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic) {
    std::string baseName = "pointLights[" + std::to_string(index) + "]";
    glUniform3f(glGetUniformLocation(shaderID, (baseName + ".position").c_str()), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(shaderID, (baseName + ".ambient").c_str()), color.x * 0.1f, color.y * 0.1f, color.z * 0.1f);
    glUniform3f(glGetUniformLocation(shaderID, (baseName + ".diffuse").c_str()), color.x, color.y, color.z);
    glUniform3f(glGetUniformLocation(shaderID, (baseName + ".specular").c_str()), color.x, color.y, color.z);
    glUniform1f(glGetUniformLocation(shaderID, (baseName + ".constant").c_str()), constant);
    glUniform1f(glGetUniformLocation(shaderID, (baseName + ".linear").c_str()), linear);
    glUniform1f(glGetUniformLocation(shaderID, (baseName + ".quadratic").c_str()), quadratic);
}

void setSpotLight(GLuint shaderID, glm::vec3 position,  float constant, float linear, float quadratic) {
    glUniform3f(glGetUniformLocation(shaderID, "spotLight.position"), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(shaderID, "spotLight.direction"), 0.0, -1.0, 0.0f);
    glUniform3f(glGetUniformLocation(shaderID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(shaderID, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaderID, "spotLight.constant"), constant);
    glUniform1f(glGetUniformLocation(shaderID, "spotLight.linear"), linear);
    glUniform1f(glGetUniformLocation(shaderID, "spotLight.quadratic"), quadratic);
    glUniform1f(glGetUniformLocation(shaderID, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
    glUniform1f(glGetUniformLocation(shaderID, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
}

void setDirectionalLight(GLuint shaderID, glm::vec3 direcction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    glUniform3f(glGetUniformLocation(shaderID, "dirLight.direction"), direcction.x,direcction.y, direcction.z);
    glUniform3f(glGetUniformLocation(shaderID, "dirLight.ambient"), ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(shaderID, "dirLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(shaderID, "dirLight.specular"), specular.x, specular.y, specular.z);
}

        //Para la transicion de la cámara
void cambiarPerspectivaCamara() {

}