
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../learnopengl/shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <SOIL.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // 1. 初始化 GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH, SCR_HEIGHT, "Two Textures", nullptr, nullptr
    );

    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 2. 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


    glEnable(GL_BLEND);

    // 设置透明混合方式
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 3. 编译着色器
    Shader ourShader(
        "demos/4.4.shader.vs",
        "demos/4.4.shader.fs"
    );

    // 4. 顶点数据
    float vertices[] = {
        // 位置              // 颜色             // 纹理坐标
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    // 颜色属性
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // 纹理坐标属性
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // 5. 创建第一张纹理
    unsigned int texture1, texture2;
    int width, height;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = SOIL_load_image(
        "resources/textures/logoTV.png",
        &width, &height, 0, SOIL_LOAD_RGBA
    );

    if (data)
    {
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Texture1 loaded successfully\n";
    }
    else
    {
        std::cout << "Texture1 failed: "
                  << SOIL_last_result() << '\n';
    }

    SOIL_free_image_data(data);

    
    // 7. 配置着色器采样器（只设置一次）
    ourShader.use();
    ourShader.setInt("texture1", 0);
    
    

    float originx = 0.0f;
    float originy = 0.0f;

    float velocityX = 0.3f;
    float velocityY = 0.2f;
    float lastTime = 0.0f;
    unsigned int colorLoc = glGetUniformLocation(ourShader.ID,"Color");
    glUniform4f(colorLoc,1.0f,1.0f,1.0f,1.0f);
    // 8. 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        
        // trans = glm::translate(trans,glm::vec3(0.5f,-0.5f,0.0f));
        // trans = glm::rotate(trans,(GLfloat)glfwGetTime() * 2.0f,glm::vec3(0.0f,0.0f,1.0f));
        unsigned int transformloc = glGetUniformLocation(ourShader.ID,"transform");
        
        // glUniformMatrix4fv(transformloc,1,GL_FALSE,glm::value_ptr(trans));
        // 将第一张纹理绑定到纹理单元 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glBindVertexArray(VAO);
        

        float currentTime = glfwGetTime();
        float delateTime = currentTime - lastTime;
        lastTime = currentTime;
        originx += velocityX * delateTime;
        originy += velocityY * delateTime;

        
        //检测是否x接触左边界
        if(originx < -0.5f){
            originx = -0.5f;
            velocityX = -velocityX;
            glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
            
        }
        if(originx > 0.5f){
            originx = 0.5f;
            velocityX = -velocityX;
            glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
        }
        //检测y是否接触上边界
        if(originy > 0.725f){
            originy = 0.725f;
            velocityY = -velocityY;
            glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);

        }
        if(originy < -0.725f){
            originy = -0.725f;
            velocityY = -velocityY;
            glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
        }

        


        
        glm::mat4 trans(1.0f);
        trans = glm::translate(trans,glm::vec3(originx,originy,0.0f));
        trans = glm::scale(trans,glm::vec3(1.0f,0.55f,1.0f));
        
        glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(trans));

        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 9. 释放资源
    glDeleteTextures(1, &texture1);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}                                                                                                                                  

void framebuffer_size_callback(
    GLFWwindow* window, int width, int height
)
{
    glViewport(0, 0, width, height);
}
