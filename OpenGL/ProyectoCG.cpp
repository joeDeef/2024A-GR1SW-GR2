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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void setDisparoYPosicion();
void setPointLight(GLuint shaderID, int index, glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic);

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(2.6f, 10.0f, -5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cambioCamara = false;
float desplazamiento = 0.003;
int j = 0;

// Sin decidir todavia
bool disparo = false;
bool posicionDisparo = false;
int habilidad = 1;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Posiciones de las cosas
glm::vec3 posicionNave = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 ultimaPosicion = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

//Configuracion de la camara
struct CameraSettings {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
};

//Confiracion para las diferentes posicone sy perspectivas de la camara
CameraSettings cameraSettings[] = {
    {glm::vec3(0.0f, 1.5f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // Inicial
    {glm::vec3(-5.0f, 1.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // Lado Izquierdo
    {glm::vec3(0.0f, 10.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)}  // Arriba
};

float transitionDuration = 5.0f; // Duración de la transición en segundos
float currentTime = 0.0f;

int main()
{
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
        Model("C:/Users/User/Documents/VisualStudio2022/OpenGL/OpenGL/model/escenario/escenario.obj")
    };

    //Posicion Inicial de los modelos
    glm::vec3 posicionModelos[] = {
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(1.0f,1.5f,0.0f),
        glm::vec3(0.0f,-2.0f,0.0f),
    };    

    glm::vec3 posicionPuntosLuz[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    glm::vec3 posicionColoresLuz[] = {
        glm::vec3(1.0f, 0.6f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0, 0.0),
        glm::vec3(0.2f, 0.2f, 1.0f)
    };

    camera.MovementSpeed = 10;

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

        lightingShader.use();

        // SpotLight/*
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.position"), posicionNave.x, posicionNave.y, posicionNave.z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.direction"), 0.0, -1.0, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.quadratic"), 0.032);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.diffuse"), 0.05f, 0.05f, 0.05);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.specular"), 0.2f, 0.2f, 0.2f);

        //Indicar los diferentes puntos de luz para la escena
        for (int i = 0; i < 4; ++i) {
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

        currentTime += 0.001f; // Incrementa el tiempo de transición
        float t = currentTime / transitionDuration; // Normaliza el tiempo

        t = glm::clamp(t, 0.0f, 1.0f);
        //Cambio de perspetiva de la camara
        if (cambioCamara) {
            j = 1;
            currentTime = 0;
            cambioCamara = false;
        }
        cameraPos = glm::mix(cameraSettings[j].position, cameraSettings[j+1].position, t);
        glm::vec3 cameraFront = glm::mix(cameraSettings[j].front, cameraSettings[j+1].front, t);
        glm::vec3 cameraUp = glm::mix(cameraSettings[j].up, cameraSettings[j+1].up, t);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        lightingShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);

        //Movimiento de la Nave
        model = glm::translate(model, posicionNave);
        model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.0f, 2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        ourShader.setMat4("model", model);
        models[1].Draw(ourShader);

        //movimiento Nave -> Por ver si se aplica
        float angulo = sin(glfwGetTime());
        if (disparo && habilidad > 0) {
            model = glm::mat4(1.0f);
            if (!posicionDisparo) {
                ultimaPosicion = glm::vec3(float(glfwGetTime()), float(glfwGetTime()), 0.0f);
                model = glm::translate(model, ultimaPosicion);
                model = glm::translate(model, glm::vec3(-6.0f, -6.0f, 0.0f));
                model = glm::rotate(model, angulo, glm::vec3(0.5f, 0.0f, 0.0f));
            }
            else {
                model = glm::translate(model, glm::vec3(float(glfwGetTime()), 0.0f, 0.0f));
                model = glm::translate(model, ultimaPosicion);
                setDisparoYPosicion();
            }
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
            ourShader.setMat4("model", model);
            models[2].Draw(ourShader);
        }

        //Posicion del Boss
        model = glm::mat4(1.0f);
        model = glm::translate(model, posicionModelos[1]);
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        ourShader.setMat4("model", model);
        models[0].Draw(ourShader);
        
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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        disparo = true;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        posicionDisparo = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        cambioCamara = true;
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

bool tomoUnaHabilidad() {
    return false;
}

void setDisparoYPosicion() {
    if (true) {
        disparo = false;
        posicionDisparo = false;
    }
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