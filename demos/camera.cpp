
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../learnopengl/shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <SOIL.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 camerapos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 camerafront = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraup = glm::vec3(0.0f,1.0f,0.0f);

bool keys[1024];
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
    glfwSetKeyCallback(window, key_callback);
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

    // 3. 编译着色器
    Shader ourShader(
        "demos/5.4.shader.vs",
        "demos/5.4.shader.fs"
    );

    // 4. 顶点数据
    GLfloat vertices[] = {
    // ---------- 前面 (-Z) ----------
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    // ---------- 后面 (+Z) ----------
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // ---------- 左面 (-X) ----------
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    // ---------- 右面 (+X) ----------
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    // ---------- 下面 (-Y) ----------
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    // ---------- 上面 (+Y) ----------
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
        5 * sizeof(GLfloat), (void*)0
    );
    glEnableVertexAttribArray(0);

    // 颜色属性
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);

    // 纹理坐标属性
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))
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
        "resources/textures/container.jpg",
        &width, &height, 0, SOIL_LOAD_RGB
    );

    if (data)
    {
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB,
            width, height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, data
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

    // 6. 创建第二张纹理
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* image = SOIL_load_image(
        "resources/textures/awesomeface.png",
        &width, &height, 0, SOIL_LOAD_RGBA
    );

    if (image)
    {
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, image
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Texture2 loaded successfully\n";
    }
    else
    {
        std::cout << "Texture2 failed: "
                  << SOIL_last_result() << '\n';
    }

    SOIL_free_image_data(image);

    // 7. 配置着色器采样器（只设置一次）
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    

    

    
    unsigned int colorLoc = glGetUniformLocation(ourShader.ID,"Color");
    glUniform4f(colorLoc,1.0f,1.0f,1.0f,1.0f);
    
    

    // 8. 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        
        
        // 将第一张纹理绑定到纹理单元 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // 将第二张纹理绑定到纹理单元 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // glBindVertexArray(VAO);
        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f), 
            glm::vec3( 2.0f,  5.0f, -15.0f), 
            glm::vec3(-1.5f, -2.2f, -2.5f),  
            glm::vec3(-3.8f, -2.0f, -12.3f),  
            glm::vec3( 2.4f, -0.4f, -3.5f),  
            glm::vec3(-1.7f,  3.0f, -7.5f),  
            glm::vec3( 1.3f, -2.0f, -2.5f),  
            glm::vec3( 1.5f,  2.0f, -2.5f), 
            glm::vec3( 1.5f,  0.2f, -1.5f), 
            glm::vec3(-1.3f,  1.0f, -1.5f)  
            };
        
        glBindVertexArray(VAO);
        do_movement();
        


        glm::mat4 view= glm::mat4(1.0f);;
        view = glm::lookAt(camerapos,camerapos+camerafront,cameraup);
        glm ::mat4 projection= glm::mat4(1.0f);;
        projection = glm::perspective(glm::radians(45.0f),(GLfloat)width/(GLfloat)height,0.5f,100.0f);
        
        unsigned int viewloc = glGetUniformLocation(ourShader.ID,"view");
        glUniformMatrix4fv(viewloc,1,GL_FALSE,glm::value_ptr(view));
        unsigned int projectionloc = glGetUniformLocation(ourShader.ID,"projection");
        glUniformMatrix4fv(projectionloc,1,GL_FALSE,glm::value_ptr(projection));
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);
        for(GLuint i = 0;i<10;i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            GLfloat angle = 10.0f*i;
            model = glm::translate(model,cubePositions[i]);
            model = glm::rotate(model,angle,glm::vec3(1.0f,0.3f,0.5f));
            unsigned int modelloc = glGetUniformLocation(ourShader.ID,"model");
            
            glUniformMatrix4fv(modelloc,1,GL_FALSE,glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES,0,36);

        }
        


      

        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 9. 释放资源
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    
    
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}                                                                                                                                 

void do_movement(){
    GLfloat cameraspeed = 0.01f;
    if(keys[GLFW_KEY_W])
        camerapos += cameraspeed * camerafront;
    if(keys[GLFW_KEY_S])
        camerapos -= cameraspeed * camerafront;
    if(keys[GLFW_KEY_A])
        camerapos -= cameraspeed * glm::normalize(glm::cross(camerafront,cameraup));
    if(keys[GLFW_KEY_D])
        camerapos += cameraspeed * glm::normalize(glm::cross(camerafront,cameraup));
}
void framebuffer_size_callback(
    GLFWwindow* window, int width, int height
)
{
    glViewport(0, 0, width, height);
}
