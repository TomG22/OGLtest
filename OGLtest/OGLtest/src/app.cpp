/* Author: Tom G.
 * Program: OGLtest
 * Description: OpenGL renderer framework
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main() {
    //Init window
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "glewInit: GLEW_OK false" << std::endl;
    }
    glfwMaximizeWindow(window);
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    {//Outside scope to fix GLClear error on close
        //Vertex Array
        float positions[] = {
            0.0f, 0.0f, 0.0f, 0.0f, //pos2, texcoord2
            100.0f, 0.0f, 1.0f, 0.0f,
            100.0f, 100.0f, 1.0f, 1.0f,
            0.0f, 100.0f, 0.0f, 1.0f
        };
        //Index Buffer
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        //Rendering
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);
        //Matrix Math
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, -1.5f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/logo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        glm::vec3 translationA(0, 0, 0);
        glm::vec3 translationB(100, 100, 0);

        float r = 0.0f;
        float increment = 0.01f;
        while (!glfwWindowShouldClose(window)) {
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            
            shader.Bind();
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
             
                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                
                renderer.Draw(va, ib, shader);
            }

            

            if (r > 1.0f)
                increment = -0.001f;
            else if (r < 0.0f)
                increment = 0.001f;
            r += increment;

            {
                static float f = 0.0f;
                ImGui::SliderFloat3("tA", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("tB", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    };
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}