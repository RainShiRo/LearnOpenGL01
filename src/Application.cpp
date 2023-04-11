#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//����һ���ص�������֤���ڴ�С������ʱ����
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

    //����һ��800��600�Ĵ��ڶ��󣬱���ΪLearnOpenGL
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //֪ͨGLFW�����ڵ�����������Ϊ��ǰ�̵߳���������
    glfwMakeContextCurrent(window);

    //��ʼ��glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    //��֪OpenGL��Ⱦ���ڵĴ�СΪ800��600�����½ǵ�λ��Ϊ0��0
    glViewport(0, 0, 800, 600);
    //��֪OpenGL�������ڴ�С
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //����һ��VAO���󣬽�֮��Ķ������Դ��������VAO��
    unsigned int VAO;
    
    //unsigned int& shaderProgram = draw_rec(VAO);
    unsigned int& shaderProgram = draw_tri(VAO);
    //��Ⱦѭ��,glfwWindowShouldClose��������glfw��Ҫ���˳�ʱ����true
    while (!glfwWindowShouldClose(window))
    {
        //���������¼�
        processInput(window);

        //����һ��������ɫ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //ִ������������ѡ�õĲ���ֻ�������ɫ����
        glClear(GL_COLOR_BUFFER_BIT);
        
        //��Ⱦ����
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        
        //������ɫ����
        glfwSwapBuffers(window);
        //��鴥���¼�
        glfwPollEvents();
    }

    //ѭ���������ͷ���Դ
    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    //���������밴��esc����glfw�ᱻ��֪�˳�
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


unsigned int& draw_rec(unsigned int& VAO) {

    float vertices[] = {
    0.5f, 0.5f, 0.0f,   // ���Ͻ�
    0.5f, -0.5f, 0.0f,  // ���½�
    -0.5f, -0.5f, 0.0f, // ���½�
    -0.5f, 0.5f, 0.0f   // ���Ͻ�
    };
       
    unsigned int indices[] = {
        0, 1, 3, // ��һ��������
        1, 2, 3  // �ڶ���������
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //����һ��VBO���󣬽����ݷŽ�������ڴ���
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //����һ��EBO���󣬽������Ž�������ڴ���
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*����OpenGL��ν�����������
    * ��һ������Ϊλ��ֵ
    * �ڶ�������Ϊ�������ԵĴ�С
    * ����������Ϊ������������
    * ���ĸ�����Ϊ�Ƿ����ݱ�׼��
    * ���������Ϊ������Ĳ���������������float�ĳ��ȣ�
    * ���һ��������ƫ����
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //��ʼ��������ɫ����ƬԪ��ɫ��
    unsigned int& vertexShader = vertex_shader();
    unsigned int& fragmentShader = fragment_shader();

    //����һ����ɫ������
    static unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    //��������ɫ�����ڳ�����
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //������ɫ������
    glLinkProgram(shaderProgram);
    
    //���������ɫ�������Ƿ�ʧ��
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::Program::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //������ɺ�ɾ����ɫ��
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

//������ɫ��
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

//ƬԪ��ɫ��
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