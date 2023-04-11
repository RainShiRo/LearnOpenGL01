#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//创建一个回调函数保证窗口大小被调整时调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int& draw_tri(unsigned int& VAO);
unsigned int& draw_rec(unsigned int& VAO);
unsigned int& vertex_shader();
unsigned int& fragment_shader();


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //创建一个800×600的窗口对象，标题为LearnOpenGL
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //通知GLFW将窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    //初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    //告知OpenGL渲染窗口的大小为800×600，左下角的位置为0，0
    glViewport(0, 0, 800, 600);
    //告知OpenGL调整窗口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //创建一个VAO对象，将之后的顶点属性储存在这个VAO中
    unsigned int VAO;
    
    //unsigned int& shaderProgram = draw_rec(VAO);
    unsigned int& shaderProgram = draw_tri(VAO);
    //渲染循环,glfwWindowShouldClose函数会在glfw被要求退出时返回true
    while (!glfwWindowShouldClose(window))
    {
        //按键输入事件
        processInput(window);

        //设置一个清屏颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //执行清屏，这里选用的参数只会清除颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT);
        
        //渲染部分
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        
        //交换颜色缓冲
        glfwSwapBuffers(window);
        //检查触发事件
        glfwPollEvents();
    }

    //循环结束后释放资源
    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    //当我们输入按下esc键后glfw会被告知退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


unsigned int& draw_rec(unsigned int& VAO) {

    float vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
    };
       
    unsigned int indices[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //生成一个VBO对象，将数据放进缓冲的内存中
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //生成一个EBO对象，将索引放进缓冲的内存中
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*告诉OpenGL如何解析顶点数据
    * 第一个参数为位置值
    * 第二个参数为顶点属性的大小
    * 第三个参数为顶点数据类型
    * 第四个参数为是否将数据标准化
    * 第五个参数为顶点组的步长（这里是三个float的长度）
    * 最后一个参数是偏移量
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //初始化顶点着色器和片元着色器
    unsigned int& vertexShader = vertex_shader();
    unsigned int& fragmentShader = fragment_shader();

    //创建一个着色器程序
    static unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    //将两个着色器绑定在程序上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //链接着色器程序
    glLinkProgram(shaderProgram);
    
    //检测链接着色器程序是否失败
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::Program::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //链接完成后删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
       
    return shaderProgram;
}

unsigned int& draw_tri(unsigned int& VAO) {
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int&  vertexShader = vertex_shader();
    unsigned int& fragmentShader = fragment_shader();

    static unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

//顶点着色器
unsigned int& vertex_shader() {
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    static unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return vertexShader;
}

//片元着色器
unsigned int& fragment_shader() {
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    static unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
 
    int  success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    return fragmentShader;
}