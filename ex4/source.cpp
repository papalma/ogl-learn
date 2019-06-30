// Changes in this file
// - Shaders are read from file
// - Use uniform in fragment shader
#include <iostream>
#include <fstream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
};

GLuint indices[] = {
    0, 1, 3,
    1, 2, 3,
};

void shaderComp(GLuint shader, const char* text)
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
    glVertexAttribPointer(0, 3 ,GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

GLuint createShaderProgram()
{
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string source;
    const char* shaderSource = loadFromFile("vertex.glsl", source);

    glShaderSource(vertexShader, 1, &shaderSource, nullptr);
    glCompileShader(vertexShader);
    shaderComp(vertexShader, "Vertex");

    // fragment shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    shaderSource = loadFromFile("fragment.glsl", source);
    glShaderSource(fragShader, 1, &shaderSource, nullptr);
    glCompileShader(fragShader);
    shaderComp(fragShader, "Fragment");

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

int main(int argc, char** argv)
{
    // gl init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // while the book states this is required for mac, it works without it
// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "My Second OGL Window", nullptr, nullptr);
    checkResult(window == nullptr, "glfwCreateWindow");

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad init
    checkResult(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0, "gladLoadGLLoader");

    GLuint vbo, vao, ebo;
    createArrays(vbo, vao, ebo);
    GLuint shaderProgram = createShaderProgram();

    while(!glfwWindowShouldClose(window)) {
        // check pressed input
        processInput(window);

        // fill screen with greenish color
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Get info for Uniform
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

        // let's draw
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.5f, greenValue, 0.0f, 1.0f);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
