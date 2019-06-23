// My First OpenGL Window
// 2019/JUN/22

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";

const char* fragShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{ FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
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

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void createArrays(GLuint& vbo, GLuint& vao)
{
    // this store vertex data in video card memory, managed by a vertex buffer object(VBO)
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3 ,GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

GLuint createShaderProgram()
{
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    shaderComp(vertexShader, "Vertex");

    // fragment shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, nullptr);
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
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "My First OGL Window", nullptr, nullptr);
    checkResult(window == nullptr, "glfwCreateWindow");

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad init
    checkResult(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0, "gladLoadGLLoader");

    GLuint vbo, vao;
    createArrays(vbo, vao);
    GLuint shaderProgram = createShaderProgram();

    while(!glfwWindowShouldClose(window)) {
        // check pressed input
        processInput(window);

        // fill screen with greenish color
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // let's draw
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
