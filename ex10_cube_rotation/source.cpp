// What's new:
// Enhance previous example to rotate a cube.

// It is remarkable that no changes are required to the shader. Just to
// vertices and minor changes to the code used to draw on screen .

#include <iostream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// math lib
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

GLfloat vertices[] = {
    // positions          // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

void checkShaderCompilationStatus(GLuint shader, const char* text)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
    char infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << text << " shader compilation error: " << infoLog << '\n';
        exit(EXIT_FAILURE);
    }
}

void checkResult(bool result, const char* text)
{
    if( result ) {
        std::cerr << "Error: " << text <<'\n';
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width, height);
}

const char* loadFromFile(const std::string& pathToFile, std::string& content)
{
    content = "";
    // std::string content;
    std::ifstream fileStream(pathToFile, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << pathToFile << std::endl;
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    return content.c_str();
}

void processInput(GLFWwindow* window)
{
    // press ESC to quit
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // ugly but works
    static bool wireframe = false;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        wireframe = !wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }
}

void createArrays(GLuint& vbo, GLuint& vao, GLuint& ebo)
{
    // this store vertex data in video card memory, managed by a vertex buffer object(VBO)
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Stride was updated attributes before texture coordinates.
    glVertexAttribPointer(0, 3 ,GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0) ;
    glEnableVertexAttribArray(0);

    // texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

GLuint createShaderProgram()
{
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string source;
    const char* shaderSource = loadFromFile("shader_vertex.glsl", source);

    glShaderSource(vertexShader, 1, &shaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderCompilationStatus(vertexShader, "Vertex");

    // fragment shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    shaderSource = loadFromFile("shader_fragment.glsl", source);
    glShaderSource(fragShader, 1, &shaderSource, nullptr);
    glCompileShader(fragShader);
    checkShaderCompilationStatus(fragShader, "Fragment");

    // shader program linking
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader program creation error: " << infoLog << '\n';
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    return shaderProgram;
}

// This is new!
GLuint createTextures(int vao, const std::string& texFilename, GLenum format = GL_RGB) {
    GLuint texture;
    int width, height, nrChannels;
    unsigned char* data = stbi_load( texFilename.c_str(), &width, &height, &nrChannels, 0);

    if(data) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // free input image memory
        stbi_image_free(data);
        return texture;
    }
    else {
        std::cout << "couldn't load texture: " << texFilename << std::endl;
    }
    return 0;
}


int main(int argc, char** argv)
{
    // gl init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My OGL Example Window", nullptr, nullptr);
    checkResult(window == nullptr, "glfwCreateWindow");

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad init
    checkResult(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0, "gladLoadGLLoader");

    GLuint vbo, vao, ebo;
    createArrays(vbo, vao, ebo);
    GLuint shaderProgram = createShaderProgram();


    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // load textures
    GLuint texture1 = createTextures(vao, "../res/container.jpg");
    GLuint texture2 = createTextures(vao, "../res/awesomeface.png", GL_RGBA);
    if(texture1==0 || texture2 == 0) {
        return EXIT_FAILURE;
    }
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);


    int uniMatrixModel = glGetUniformLocation(shaderProgram, "model");
    int uniMatrixView = glGetUniformLocation(shaderProgram, "view");
    int uniMatrixProj = glGetUniformLocation(shaderProgram, "projection");

    glm::mat4 model = glm::mat4(1.0f);
    while(!glfwWindowShouldClose(window)) {
        // check pressed input
        processInput(window);

        model =  glm::rotate(model, (float)glfwGetTime() * glm::radians(0.05f), glm::vec3(0.5f, 1.0f, 0.0f));

        // fill screen with greenish color
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUniformMatrix4fv(uniMatrixModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniMatrixView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(uniMatrixProj, 1, GL_FALSE, glm::value_ptr(projection));

        // let's draw
        glUseProgram(shaderProgram);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        // Now use 6 vertices
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
