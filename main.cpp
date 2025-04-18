#if defined (_APPLE_)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <glm/glm.hpp> //core glm functionality
#include <glm/gtc/matrix_transform.hpp> //glm extension for generating common transformation matrices
#include <glm/gtc/matrix_inverse.hpp> //glm extension for computing inverse matrices
#include <glm/gtc/type_ptr.hpp> //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"
#include "SkyBox.hpp"

#include <iostream>

// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 rataModel;
glm::mat4 balonModel;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;

// shader uniform locations
GLint modelLoc;
GLint viewLoc;
GLint projectionLoc;
GLint normalMatrixLoc;
GLint lightDirLoc;
GLint lightColorLoc;
gps::SkyBox mySkyBox;
gps::Shader skyboxShader;
float yaw = -90.0f;
float pitch = 0.0f;

int glWindowWidth = 800;
int glWindowHeight = 600;
glm::vec3 centru1Position(0.0f, 0.0f, 0.0f);
float moaraRotationAngle = 0.0f;
GLint fogDensityLoc;
GLfloat fogDensity;
GLfloat fogSensitivity = 0.002;

double lastX = glWindowWidth / 2.0;
double lastY = glWindowHeight / 2.0;
bool firstMouse = true;
bool leftMouseButtonPressed = false;
float animationTime = 0.0f;
bool animationActive = true;
bool firstMouseEver = true;
float delta = 0;
glm::vec3 rataPosition(0.0f, 0.0f, 0.0f);
glm::vec3 balonPosition(0.0f, 0.0f, 0.0f);
float rataRotation = 0.0f;
float elapsedTime = 0;
float animationDuration = 10.0f;
bool BalonFloating = false;
enum RenderMode { SOLID, WIREFRAME, POLYGONAL, SMOOTH };
RenderMode currentMode = SOLID;

float movementSpeed = 2;
void updateDelta(double elapsedSeconds) {
    delta = delta + movementSpeed * elapsedSeconds;
}
double lastTimeStamp = glfwGetTime();
// camera
gps::Camera myCamera(
    glm::vec3(0.0f, 2.0f, 3.0f),
    glm::vec3(0.0f, 0.0f, -10.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

GLfloat cameraSpeed = 0.1f;

GLboolean pressedKeys[1024];

// models
gps::Model3D scene;
gps::Model3D rata;
gps::Model3D balon;

GLfloat angle;

// shaders
gps::Shader myBasicShader;




void windowResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 20.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void initSkyBox() {
    std::vector<const GLchar*> faces;
    faces.push_back("skybox/right.tga");
    faces.push_back("skybox/left.tga");
    faces.push_back("skybox/top.tga");
    faces.push_back("skybox/bottom.tga");
    faces.push_back("skybox/back.tga");
    faces.push_back("skybox/front.tga");
    mySkyBox.Load(faces);
}
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        leftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        leftMouseButtonPressed = false;
        firstMouse = true;
    }
}
void updateRataModelMatrix() {
    glm::mat4 rataModel = glm::translate(glm::mat4(1.0f), rataPosition);

    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rataModel));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * rataModel))));
}
void updateBalonModelMatrix() {
    glm::mat4 balonModel = glm::translate(glm::mat4(1.0f), balonPosition);

    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(balonModel));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::inverseTranspose(view * balonModel))));
}


void mouseCallback(GLFWwindow* window, double xpos, double ypos) {

    if (!leftMouseButtonPressed) return;
    if (firstMouseEver && firstMouse) {

        lastX = 0.0f;
        lastY = 0.0f;


        firstMouseEver = false;
    }
    else if (firstMouse) {

        lastX = xpos;
        lastY = ypos;

        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    myCamera.rotate(pitch, yaw);
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

glm::vec3 getPositionFromMatrix(const glm::mat4& transformMatrix) {
    return glm::vec3(transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2]);
}


void updateCameraAnimation(float elapsedTime) {
    const float radius = 5.0f;
    const float baseHeight = 7.0f;

    float scaledElapsedTime = elapsedTime * 0.2f;
    float progress = elapsedTime / animationDuration;

    float angle = progress * glm::pi<float>() * 2.0f;

    glm::vec3 newPosition = glm::vec3(
        radius * cos(angle),
        baseHeight + (sin(angle * 2.0f) * 2.0f),
        radius * sin(angle)
    );

    glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    myCamera.setPosition(newPosition);
    myCamera.setTarget(targetPosition);
}





void updateModelAnimation(float elapsedTime) {
    float rotationSpeed = 0.3f;
    float angle = glm::radians(rotationSpeed * elapsedTime);
    model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
}
void updateBalloonFloating() {
    if (!BalonFloating) {
        return;
    }
    balonPosition.y += cameraSpeed * 0.05;

}



void processMovement() {
    if (pressedKeys[GLFW_KEY_1]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (pressedKeys[GLFW_KEY_2]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (pressedKeys[GLFW_KEY_3]) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    if (pressedKeys[GLFW_KEY_4]) {
        glEnable(GL_SMOOTH);
        glShadeModel(GL_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (pressedKeys[GLFW_KEY_W]) {
        myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_S]) {
        myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }
    if (pressedKeys[GLFW_KEY_A]) {
        myCamera.move(gps::MOVE_LEFT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }
    if (pressedKeys[GLFW_KEY_F]) {
        BalonFloating = true;
        //balonPosition.y += cameraSpeed * 0.05;
    }
    if (pressedKeys[GLFW_KEY_B]) {
        balonPosition.y = 0.0f;
        BalonFloating = false;
    }
    if (pressedKeys[GLFW_KEY_D]) {
        myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }
    if (pressedKeys[GLFW_KEY_K]) {
        lightDir.x -= 0.05f;
    }
    if (pressedKeys[GLFW_KEY_L]) {
        lightDir.x += 0.05f;
    }

    if (pressedKeys[GLFW_KEY_Q]) {
        angle -= 1.0f;
        // update model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
        // update normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_E]) {
        angle += 1.0f;
        // update model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
        // update normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }
    if (pressedKeys[GLFW_KEY_UP]) {
        rataPosition.z += cameraSpeed * 0.05;
    }
    if (pressedKeys[GLFW_KEY_DOWN]) {
        rataPosition.z -= cameraSpeed * 0.05;
    }
    if (pressedKeys[GLFW_KEY_LEFT]) {
        rataPosition.x += cameraSpeed * 0.05f;
        //rataRotation = 90.0f;
    }
    if (pressedKeys[GLFW_KEY_RIGHT]) {
        rataPosition.x -= cameraSpeed * 0.05f;
        //rataRotation = -90.0f;
    }
    if (fabs(rataPosition.x) < 0.001f) {
        rataPosition.x = 0.0f;
    }
    if (pressedKeys[GLFW_KEY_Z]) {
        fogDensity += fogSensitivity;
        if (fogDensity > 0.3f)
            fogDensity = 0.3f;
        myBasicShader.useShaderProgram();
        glUniform1fv(fogDensityLoc, 1, &fogDensity);
    }
    if (pressedKeys[GLFW_KEY_X]) {
        fogDensity -= fogSensitivity;
        if (fogDensity < 0.0f)
            fogDensity = 0.0f;
        myBasicShader.useShaderProgram();
        glUniform1fv(fogDensityLoc, 1, &fogDensity);
    }
    myBasicShader.useShaderProgram();
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));
}

void initOpenGLWindow() {
    myWindow.Create(1024, 768, "OpenGL Project Core");
}

void setWindowCallbacks() {
    glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
    glfwSetMouseButtonCallback(myWindow.getWindow(), mouse_button_callback);
}

void initOpenGLState() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initModels() {
    scene.LoadModel("models/scene3/mapa_proiect2.obj");
    rata.LoadModel("models/rata/rata2.obj");
    balon.LoadModel("models/balon/balon.obj");
    initSkyBox();
}

void initShaders() {
    myBasicShader.loadShader(
        "shaders/basic.vert",
        "shaders/basic.frag");
    skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
    skyboxShader.useShaderProgram();
}

void initUniforms() {
    myBasicShader.useShaderProgram();

    // create model matrix for teapot
    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

    // get view matrix for current camera
    view = myCamera.getViewMatrix();
    viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
    // send view matrix to shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

    //fog
    fogDensityLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fogDensity");
    glUniform1fv(fogDensityLoc, 1, &fogDensity);

    // create projection matrix
    projection = glm::perspective(glm::radians(45.0f),
        (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
        0.1f, 20.0f);
    projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
    // send projection matrix to shader
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //set the light direction (direction towards the light)
    lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
    lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
    // send light dir to shader
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

    //set light color
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
    lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
    // send light color to shader
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
}

void renderTeapot(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    scene.Draw(shader);
    updateRataModelMatrix();
    rata.Draw(shader);
    mySkyBox.Draw(skyboxShader, view, projection);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    mySkyBox.Draw(skyboxShader, view, projection);
    glDepthMask(GL_TRUE);
    myBasicShader.useShaderProgram();

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    scene.Draw(myBasicShader);
    updateBalloonFloating();
    updateBalonModelMatrix();
    balon.Draw(myBasicShader);

    updateRataModelMatrix();
    rata.Draw(myBasicShader);
    // glDepthMask(GL_TRUE); 


}






void cleanup() {
    myWindow.Delete();
    //cleanup code for your own data
}

int main(int argc, const char* argv[]) {

    try {
        initOpenGLWindow();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    initOpenGLState();
    initModels();
    initShaders();
    initUniforms();
    setWindowCallbacks();

    // application loop
    double lastTimeStamp = glfwGetTime();
    while (!glfwWindowShouldClose(myWindow.getWindow())) {
        double currentTime = glfwGetTime();
        elapsedTime = currentTime - lastTimeStamp;
        if (animationActive) {
            updateCameraAnimation(elapsedTime);
            updateModelAnimation(elapsedTime);

            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
            glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        }
        else {
            processMovement();
        }


        renderScene();


        glfwPollEvents();
        glfwSwapBuffers(myWindow.getWindow());
        if (currentTime > animationDuration) {
            animationActive = false;
        }
    }

    cleanup();

    return EXIT_SUCCESS;
}