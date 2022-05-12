
// Std. Includes
#include <string>
// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// ~Inicialización de la camara~
// Variables de la posición de la cámara
glm::vec3 poscam(5.f, 4.0f, 30.0f);
Camera camera(glm::vec3(poscam.x, poscam.y, poscam.z));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Vectores de spotlight
glm::vec3 slightPosition(5.f, 0.0f, 5.0f);
glm::vec3 slightdirection(10.f, 0.5f, -8.f);

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-2.2f,4.82f, 5.37f),
    /*glm::vec3(8.0f,0.0f, 5.0f),
    glm::vec3(0.0f,10.0f,  0.0f),
    glm::vec3(10.0f,10.0f, 0.0f)*/
};


// Light attributes
glm::vec3 lightPos(0.f, 0.f, 0.f);
float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 90.0f;
bool activanim = false;
float x = 0.0f;
float z = 0.0f;
glm::vec3 Light1 = glm::vec3(0);
//Variables de ventana
float rotv1 = 90;
// 
//~Variables para las animaciones~
//Variables para la animación del tocador
float movetoc1 = 0, movetoc2 = 0, movetoc3 = 0, rottoc =0;
bool movetoc = false;
// Variables para la animación del baúl
float rotbaul =0;
bool activabaul = false, upbaul;
//Variables para la animación del teléfono
float movetel, telcount ;
bool acttel, telmov;
//Variables para la animación de las sillas
float sil1 =0, sil2 = 0 , sil3 = 0;
bool actsillas;

//Variables para la animacipon de la cochera
float cpart1 = 0, cpart2 = -1.6, cpart3 = -3.2, crot =00;
bool cochera;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto final", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
    Shader lampshader("Shaders/lamp.vs", "Shaders/lamp.frag");
    Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");



    // Load models
    //Primer objeto
    Model comedor((char*)"Models/dinning_room/comedor.obj");
    Model silla1((char*)"Models/dinning_room/silla1.obj");
    Model silla2((char*)"Models/dinning_room/silla2.obj");
    Model silla3((char*)"Models/dinning_room/silla3.obj");
    //Segundo objeto
    Model mueble((char*)"Models/lamp1/mueble.obj");
    Model Octopus((char*)"Models/lamp1/octopuslamp.obj");
    //Tercer objeto
    Model baul((char*)"Models/Baul/Baul_PPG.obj");
    Model tapa((char*)"Models/Baul/tapa.obj");
    //Cuarto objeto
    Model cama((char*)"Models/bed/bed_PPG.obj");
    //Quinto objeto
    Model mesita((char*)"Models/telefono/mesita_t.obj");
    Model caja_tel((char*)"Models/telefono/caja_tel.obj");
    Model telefono((char*)"Models/telefono/telefono.obj");
    Model nariz_tel((char*)"Models/telefono/nariz_tel.obj");
    //Sexto objeto
    Model osito((char*)"Models/lamp2/bear.obj");
    //Sexto objeto
    Model tocador((char*)"Models/dressing_table/tocador.obj");
    Model toc1((char*)"Models/dressing_table/cajon1.obj");
    Model toc2((char*)"Models/dressing_table/cajon2.obj");
    Model toc3((char*)"Models/dressing_table/cajon3.obj");
    //Cochera
    Model cochera1((char*)"Models/house/cochera.obj");
    Model cochera2((char*)"Models/house/cochera.obj");
    Model cochera3((char*)"Models/house/cochera.obj");
    //Ventanas de la fachada
    Model vent1((char*)"Models/house/vent_1.obj");
    //Fachada
    Model house((char*)"Models/house/casita.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);




    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        //glClearColor(0.890f, 0.988f, 0.988f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x + x, lightPos.y + movelightPos, lightPos.z + z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -1.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


        // Point light 1
        glm::vec3 lightColor;
        lightColor.x =  abs(sin(glfwGetTime()* Light1.x));
        lightColor.y =  abs(sin(glfwGetTime()* Light1.y));
        lightColor.z =  abs(sin(glfwGetTime() * Light1.z));


        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 1.8f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), slightPosition.x, slightPosition.y, slightPosition.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), slightdirection.x, slightdirection.y, slightdirection.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 12.0f);



        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));



        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.3f, 0.3f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), .972f, 0.07f, 0.513f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), .5f, 0.7f, 0.67f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.f, 5.1f, 5.1f);



        // Draw the loaded model
      
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.3f, 0.3f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.5f, .50f, 0.5f);//.972f, 0.07f, 0.513f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), .5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.f, 5.1f, 5.1f);

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

       

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.f, 0.8f, 0.f));
        model = glm::scale(model, glm::vec3(0.8f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        comedor.Draw(lightingShader);
        //silla 1
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.f, 0.8f , 0.f + sil1));
        model = glm::scale(model, glm::vec3(0.8f));
        //glBindVertexArray(VAO);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        silla1.Draw(lightingShader);


        //Silla 2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.f + sil2, 0.8f, 0.f));
        model = glm::scale(model, glm::vec3(0.8f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        silla2.Draw(lightingShader);

        //Sillla 3
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.f, 0.8f, 0.f + sil3));
        model = glm::scale(model, glm::vec3(0.8f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        silla3.Draw(lightingShader);


        //Octopus
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(.8f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        Octopus.Draw(lightingShader);
        //Mueble
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(.8f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        mueble.Draw(lightingShader);

        //Baul
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.f, 4.3f, -2.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        baul.Draw(lightingShader);
        //Tapa
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.f, 4.3f, -2.2f));
        model = glm::rotate(model, glm::radians(-rotbaul), glm::vec3(1.f, 0.f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        tapa.Draw(lightingShader);
        //Bed
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-.8f, 0.24f, -4.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        cama.Draw(lightingShader);

        //Mesita
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.9f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        mesita.Draw(lightingShader);
        //* Caja teléfono
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.8f, 4.9f, 5.4f));
        model = glm::scale(model, glm::vec3(.55f, 0.55f, 0.55f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        caja_tel.Draw(lightingShader);
        //* Teléfono
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.8f, 4.9f, 5.4f));
        model = glm::scale(model, glm::vec3(.55f, 0.55f, 0.55f));
        model = glm::rotate(model, glm::radians(movetel), glm::vec3(1.f, 0.f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        telefono.Draw(lightingShader);
        //* nariz teléfono
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.8f, 4.9f, 5.4f));
        model = glm::scale(model, glm::vec3(.55f, 0.55f, 0.55f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        nariz_tel.Draw(lightingShader);
        //Bear
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.5f, -0.1f, -.4f));
        model = glm::scale(model, glm::vec3(0.8f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        osito.Draw(lightingShader);
        //**Tocador
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.f, 3.4f, 4.8f));
        model = glm::scale(model, glm::vec3(0.8f));
        model = glm::rotate(model, glm::radians(-rottoc), glm::vec3(0.f, 1.f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        tocador.Draw(lightingShader);
        //cajon1
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.f, 3.4f, 4.8f - movetoc1));
        model = glm::scale(model, glm::vec3(0.8f));
        model = glm::rotate(model, glm::radians(-rottoc), glm::vec3(0.f, 1.f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        toc1.Draw(lightingShader);
        //cajon2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.f, 3.4f, 4.8f - movetoc2));
        model = glm::scale(model, glm::vec3(0.8f));
        model = glm::rotate(model, glm::radians(-rottoc), glm::vec3(0.f, 1.f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        toc2.Draw(lightingShader);
        //cajon3**
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.f, 3.4f, 4.8f - movetoc3));
        model = glm::scale(model, glm::vec3(0.8f));
        model = glm::rotate(model, glm::radians(-rottoc), glm::vec3(0.f, 1.f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        toc3.Draw(lightingShader);
        //Cochera
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.f, cpart1, 0.f));
        model = glm::rotate(model, glm::radians(-crot), glm::vec3(1.f, .0f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        cochera1.Draw(lightingShader);
        //Cochera
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.f, cpart2, 0.f));
        model = glm::rotate(model, glm::radians(-crot), glm::vec3(1.f, .0f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        cochera2.Draw(lightingShader);
        //Cochera
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.f, cpart3, 0.f));
        model = glm::rotate(model, glm::radians(-crot), glm::vec3(1.f, .0f, 0.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        cochera3.Draw(lightingShader);
        
        //Casa
        model = glm::mat4(1);
        //model = glm::translate(model, glm::vec3(10.f, 0.f, 10.f));
       // model = glm::rotate(model, glm::radians(rot), glm::vec3(1.f, 0.f, 0.f));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.9);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        house.Draw(lightingShader);

        glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Ventana 1
        model = glm::mat4(1);
        //  model = glm::translate(model, glm::vec3(10.f, 0.f, 10.f));
         // model = glm::rotate(model, glm::radians(rot), glm::vec3(1.f, .0f, 0.f));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), .349, .952, .937, 0.4);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        vent1.Draw(lightingShader);
        //
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(10.f, 0.f, 10.f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.75);
        //.Draw(lightingShader);



        /*glBindVertexArray(VAO);
        glBindVertexArray(0);*/

        glDisable(GL_BLEND); //Desactiva canal alfa
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);//Mandar todo a blanco, indispensable
        glBindVertexArray(0);


        //lampshader.Use();
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos + glm::vec3(x, movelightPos,z));
        //model = glm::scale(model, glm::vec3(0.02f));
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glBindVertexArray(0);
        // Also draw the lamp object, again binding the appropriate shader
        lampshader.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(lampshader.Program, "model");
        viewLoc = glGetUniformLocation(lampshader.Program, "view");
        projLoc = glGetUniformLocation(lampshader.Program, "projection");

        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw the light object (using light's vertex attributes)
        /*for (GLuint i = 0; i < 4; i++)
        {*/
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[0]);
            model = glm::scale(model, glm::vec3(0.05f)); // Make it a smaller cube
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        //}
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim) {
        if (rot > -90.0f)
            rot -= 0.1f;
    }
    //Animación de tocador
    if (movetoc) {
        if (movetoc3 < 0.8) {
            movetoc1 += 0.01;
            movetoc2 += 0.02;
            movetoc3 += 0.03;

        }

    }
    else {
        if (movetoc3 > 0) {
            movetoc1 -= 0.01;
            movetoc2 -= 0.02;
            movetoc3 -= 0.03;

        }
    }
    //Animación de baul
    if (activabaul) {
        if (rotbaul < 110)
            rotbaul += 4;
    }
    else {
        if (rotbaul > 0)
            rotbaul -= 2;
    }

    //Animacion del telefono
    if (acttel) {
        Light1 = glm::vec3(1.0f, .0f, .0f);
        if (movetel < 20 && telmov == false)
            movetel += 10;

        else {
            telmov = true;
            if (telcount > 8) {
                movetel = 0;
                telcount = 0;
                acttel = false;
                Light1 = glm::vec3(0);
            }
            if (movetel > -20 && telmov ==true) {
                movetel -= 10;
            }
            else {
                telmov = false;
                telcount += 1;
            }
        }
    }
    //Animación de las sillas
    if (actsillas) {
        if (sil1 < 1) {
            sil1 += .1;
            sil2 -= .1;
            sil3 -= .1;
        }
    }else {
        if (sil1 > 0) {
            sil1 -= .1;
            sil2 += .1;
            sil3 += .1;
        }
    }
    //Animación cochera
    if (cochera) {
        if (cpart3 < cpart2)
            cpart3 += .1;
        else if (cpart2 < cpart1) {
            cpart3 += .1;
            cpart2 += .1;
        }else if (cpart1 < 1) {
            cpart3 += .1;
            cpart2 += .1;
            cpart1 += .1;
            crot += 10;
        }
    }
    else{ 
        if (cpart1 > 0.1) {
            cpart3 -= .1;
            cpart2 -= .1;
            cpart1 -= .1;
            crot -= 10;
        }
        else if (cpart2 > -1.6) {
            cpart3 -= .1;
            cpart2 -= .1;
        }
        else if(cpart3 > -3.2)
            cpart3 -= .1;

    }



 }

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    //Tocador
    if (keys[GLFW_KEY_1])
        movetoc = true;
    if (keys[GLFW_KEY_2])
        movetoc = false;
    //Baul
    if (keys[GLFW_KEY_3])
        activabaul = true;
    if (keys[GLFW_KEY_4])
        activabaul = false;

    //Teléfono
    if (keys[GLFW_KEY_5])
        acttel = true;
    //Sillas
    if (keys[GLFW_KEY_6])
        actsillas = true;
    if (keys[GLFW_KEY_7])
        actsillas = false;
    //Cochera
    if (keys[GLFW_KEY_8])
        cochera = true;
    if (keys[GLFW_KEY_9])
        cochera = false;


    

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}



