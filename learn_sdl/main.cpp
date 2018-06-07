#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>
#include <cmath>
#include <iomanip>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Obj.h"
#include "ObjLoad.h"
#include "Camera.h"

// Functions Signature
void Init();
void LoadShaders();
void LoadObjects();
void RenderLoop();
void Quit();
void Update(SDL_Window * m_window);
void Draw();

// Variables

unsigned int VBO;
unsigned int VAO;
unsigned int shaderProgram;

static bool m_isClosed = false;

int width = 500;
int height = 500;

SDL_Window *m_window;
SDL_GLContext m_glContext;

float vertices[] = {
        1.0f, 0.0f,  1.0f,
        1.0f, 0.0f, -1.0f,
       -1.0f, 0.0f, -1.0f,
       -1.0f, 0.0f,  1.0f
};

static const GLfloat g_color_buffer_data[] = {
        1.0,  1.0f,  1.0f,
        1.0,  1.0f,  1.0f,
        1.0,  1.0f,  1.0f,
        0.5f,  0.5,  0.5f,
        0.5f,  0.5,  0.5f,
        0.5f,  0.5,  0.5f,
        0.0f,  0.0f,  0.0f
};

// Shaders
const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;"
                                 "layout (location = 1) in vec3 aColor;"
                                 "layout (location = 2) in vec2 aTexCoord;"
                                 "out vec3 ourColor;"
                                 "out vec2 TexCoord;"
                                 "uniform mat4 model;"
                                 "uniform mat4 view;"
                                 /*"mat4 view = mat4(0.0, 0.0, 0.0, 0.0,"
                                 "                 0.0, 0.0, 0.0, 0.0,"
                                 "                 0.0, 0.0, 0.0, 0.0,"
                                 "                 0.0, 0.0, 0.0, 1.0);"*/
                                 "uniform mat4 projection;"
                                 "uniform mat4 combined;"
                                 "void main(){"
                                 "gl_Position = combined * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
                                 "ourColor = vec3(aPos.x, aPos.y, aPos.z);"
                                 "TexCoord = aTexCoord;"
                                 "}";
const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;"
                                   "in vec3 ourColor;"
                                   "in vec2 TexCoord;"
                                   "uniform sampler2D ourTexture;"
                                   "void main(){"
                                   "float ambientStrength = 0.5;"
                                   "vec3 ambient = ambientStrength * vec3(1.0, 1.0, 0.0);"
                                   "vec3 result = ambient * ourColor;"
                                   "FragColor = vec4(result, 1.0);"
                                   "}";

int main(int argc, char *argv[]){

    Init();
    LoadShaders();
    //LoadObjects();
    RenderLoop();
    Quit();

    return 0;
}

void Init(){
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE			 , 8);	// Number of bits used to display shades of red
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE		 , 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE		 , 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE		 , 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE 		 , 32);	// Sum of bits above. Space of a pixel
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER		 , 1);	// Enable double buffer

    SDL_Init (SDL_INIT_EVERYTHING);

    m_window = SDL_CreateWindow ( 	"Titulo",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     width,
                                     height,
                                     SDL_WINDOW_OPENGL
    );

    if(m_window == nullptr){
        std::cout << SDL_GetError();
    }

    m_glContext = SDL_GL_CreateContext(m_window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1;
    }
}

void LoadShaders(){

    // Vertex Shader creation and compiling
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check if its compilation was sucessfull
    int  vertexCompilationSuccess;
    char vertexInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompilationSuccess);
    if(!vertexCompilationSuccess)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexInfoLog << std::endl;
    }

    // Vertex Shader creation and compiling
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check if its compilation was sucessfull
    int  fragmentCompilationSuccess;
    char fragmentInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompilationSuccess);
    if(!fragmentCompilationSuccess)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog << std::endl;
    }

    // Create Program
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if its creation was successfull
    int programSuccess ;
    char shaderProgramInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
    if(!programSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, shaderProgramInfoLog);
    }
}

void LoadObjects(){
    SDL_Surface *pSurface = IMG_Load("data/img.png");
    //SDL_LoadBMP("data/texture.bmp")
    if (pSurface == nullptr) std::cout << "BMP no loaded" << std::endl;


    ObjLoad load("data/Iphone 8.obj");
    load.load();
    Obj *obj = load.getObject();

    if (obj == nullptr) obj = new Obj;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(obj->getNum_vertices() * sizeof(float)), obj->getVertices(), GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    /*GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);*/

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->sizeEBO() * sizeof(float), obj->getEBO(), GL_STATIC_DRAW);


    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (pSurface->pixels) {
        std::cout << "w: " << pSurface->w << " h: " << pSurface->h << std::endl;


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pSurface->w, pSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pSurface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void RenderLoop(){
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    /*glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    glEnable(GL_DEPTH_TEST);
    ObjLoad load("data/cube.obj");
    load.load();
    Obj *obj = load.getObject();
    if (obj == nullptr) obj = new Obj;

    obj->initialize();
    glm::mat4 projection = glm::mat4(1.0f);//glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    Camera camera(glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f));

    /*camera.setPosition(0.0f, 0.0f, 3.0f);
    camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));*/

    const float *ptrr = glm::value_ptr(camera.getView());
    for (int i = 0; i < 16; i++) {
        std::cout << ptrr[i] << " ";
    }
    std::cout << std::endl;

    const float *ptr = glm::value_ptr(camera.getView() * camera.getView());
    for (int i = 0; i < 16; i++) {
        std::cout << ptr[i] << " ";
    }
    camera.setPosition(0.0f, 0.0f, -10.0f);
    //camera.rotate(glm::radians(45.0f), 0.0f, 0.0f, 1.0f);

    float yaw = 0.0f;
    float pitch = 0.0f;
    glm::vec3 front = glm::vec3(0.0, 0.0, 0.0);

    SDL_WarpMouseInWindow(m_window, width / 2, height / 2);
    bool is_warp = true;
    float sensitivity = 0.2f;

    while(!m_isClosed){
        SDL_GL_SwapWindow(m_window);

        SDL_Event e;

        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                m_isClosed = true;
            }

            if (e.type == SDL_MOUSEMOTION){
                if (!is_warp) {
                    SDL_WarpMouseInWindow(m_window, width / 2, height / 2);
                    yaw += e.motion.xrel * sensitivity;
                    pitch += -e.motion.yrel * sensitivity;
                    std::cout << "x: " << e.motion.xrel << std::endl;
                    std::cout << "y: " << e.motion.yrel << std::endl;
                    is_warp = true;
                } else {
                    is_warp = false;
                }
                if (pitch > 89.0f) {
                    pitch = 89.f;
                }

                if (pitch < -89.0f) {
                    pitch = 89.0f;
                }
            }

            if(e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE){
                    m_isClosed = true;
                }

                if (e.key.keysym.sym == SDLK_UP) {
                    pitch++;
                    if (pitch > 89.0f) {
                        pitch = 89.f;
                    }
                    std::cout << "rotate" << std::endl;
                    //camera.rotate(glm::radians(45.0f), -1.0f, 0.0f, 0.0f);
                }

                if (e.key.keysym.sym == SDLK_DOWN) {
                    pitch--;
                    if (pitch < -89.0f) {
                        pitch = 89.0f;
                    }
                    std::cout << "rotate" << std::endl;
                }

                if (e.key.keysym.sym == SDLK_LEFT) {
                    yaw--;
                    std::cout << "rotate" << std::endl;
                }

                if (e.key.keysym.sym == SDLK_RIGHT) {
                    yaw++;
                    std::cout << "rotate" << std::endl;
                }

                if (e.key.keysym.sym == SDLK_w) {
                    //view = glm::translate(view, glm::vec3(0.0f, 0.1f, 0.0f));
                    auto speed = 2.5f;
                    auto dir = camera.getPosition() + camera.getDirection();
                    camera.translate(front.x * speed, front.y * speed, front.z * speed);
                    //camera.setPosition(50, 50, 50);
                    std::cout << "translate" << std::endl;
                }

                if (e.key.keysym.sym == SDLK_s) {
                    auto speed = -2.5f;
                    auto dir = camera.getPosition() + camera.getDirection();
                    camera.translate(front.x * speed, front.y * speed, front.z * speed);
                    std::cout << "translate" << std::endl;
                }


                if (e.key.keysym.sym == SDLK_m) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }

                if (e.key.keysym.sym == SDLK_l) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
            }
        }



        front.x = static_cast<float>(cos(glm::radians(yaw))) * static_cast<float>(cos(glm::radians(pitch)));
        front.y = static_cast<float>(sin(glm::radians(pitch)));
        front.z = static_cast<float>(sin(glm::radians(yaw))) * static_cast<float>(cos(glm::radians(pitch)));
        front = glm::normalize(front);
        camera.lookAt(glm::vec3(camera.getPosition() + front));

        camera.update();

        //view = glm::lookAt(pos, dir, up);
        obj->autoRotate();

        glClearColor(0.2, 0.2, 0.5, 0.0); // Set the clear color. State setting function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// State using funtion
        //Draw();

        float pos[] = {};

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera.getView()));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(camera.getProjection()));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "combined"), 1, GL_FALSE, glm::value_ptr(camera.getCombined()));

        /*glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);*/
        obj->draw(shaderProgram, 0.0f, 0.0f, 0.0f);

        obj->draw(shaderProgram, 0.0f, 4.0f, 0.0f);
        //obj->draw(shaderProgram);
        //Update(m_window);

        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-100.0f, 0.0f, -100.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for (int i = 0; i < 200; i++) {
            for (int j = 0; j < 200; j++) {
                model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

                glBindVertexArray(VAO);
                glDrawArrays(GL_QUADS, 0, 4);
            }
            model = glm::translate(model, glm::vec3(-400.0f, 0.0f, 2.0f));
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }
}

void Quit(){
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}