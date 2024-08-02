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

// settings
const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 700;
glm::vec3 lightPos(0.0f, 15.0f, 0.0f);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 posicionNave = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

struct CameraSettings {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
};

CameraSettings cameraSettings[] = {
    {glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // Inicial
    {glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // Lado Izquierdo
    {glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)}  // Arriba
};

float transitionDuration = 5.0f; // Duración de la transición en segundos
float currentTime = 0.0f;

void cambiarLugarCamara(Camera camera, float x, float y, float z);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercise 16 Task 3", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
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
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("shaders/shader_exercise16_mloading.vs", "shaders/shader_exercise16_mloading.fs");
    Shader lightingShader("shaders/shader_exercise14t4_materials.vs", "shaders/shader_exercise14t4_materials.fs");

    // load models
    // -----------
    Model models[] = {
        Model("C:/Users/User/Documents/VisualStudio2022/OpenGL/OpenGL/model/immortalDemonic/demonio.obj"),
        Model("C:/Users/User/Documents/VisualStudio2022/OpenGL/OpenGL/model/ovni/ovni.obj")
    };

    //Posicion Inicial de los modelos
    glm::vec3 movimiento[] = {
        glm::vec3(0.9f,-0.8f,0.0f),
        glm::vec3(0.0f,0.0f,0.0f)
    };    

    camera.MovementSpeed = 10; //Optional. Modify the speed of the camera

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        cambiarLugarCamara(camera, 0.0f, 0.0f, 0.0f);
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Azul cielo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        currentTime += 0.0001f; // Incrementa el tiempo de transición
        float t = currentTime / transitionDuration; // Normaliza el tiempo

        t = glm::clamp(t, 0.0f, 1.0f);

        cameraPos = glm::mix(cameraSettings[0].position, cameraSettings[2].position, t);
        glm::vec3 cameraFront = glm::mix(cameraSettings[0].front, cameraSettings[2].front, t);
        glm::vec3 cameraUp = glm::mix(cameraSettings[0].up, cameraSettings[2].up, t);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        // Configurar la vista
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        //std::cout << "Pos Camera: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;


        //Movimiento de la Nave
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, posicionNave);
        model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0.0f, 2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        ourShader.setMat4("model", model);
        models[1].Draw(ourShader);
        
        //Posoicon del Jefe
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,sin(glfwGetTime())/0.5 +0.5,0.0f));
        model = glm::translate(model, movimiento[0]);
        model = glm::translate(model, movimiento[1]);
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
        ourShader.setMat4("model", model);
        models[0].Draw(ourShader);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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
            posicionNave.y += 0.001;
        }
        else if (cameraPos == cameraSettings[2].position) {
            posicionNave.z -= 0.001;
        }
        else {
            posicionNave.y += 0.001;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (cameraPos == cameraSettings[1].position) {
            posicionNave.y -= 0.001;
        }
        else if (cameraPos == cameraSettings[2].position) {
            posicionNave.z += 0.001;
        }
        else {
            posicionNave.y -= 0.001;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //camera.ProcessKeyboard(LEFT, deltaTime);
        if (cameraPos == cameraSettings[1].position) {
            posicionNave.z -= 0.001;
        }
        else if (cameraPos == cameraSettings[2].position) {
            posicionNave.x -= 0.001;
        }
        else {
            posicionNave.x -= 0.001;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //camera.ProcessKeyboard(RIGHT, deltaTime);
        if (cameraPos == cameraSettings[1].position) {
            posicionNave.z += 0.001;
        }
        else if (cameraPos == cameraSettings[2].position) {
            posicionNave.x += 0.001;
        }
        else {
            posicionNave.x += 0.001;
        }
    }
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

void cambiarLugarCamara(Camera camera, float x, float y, float z) {

}