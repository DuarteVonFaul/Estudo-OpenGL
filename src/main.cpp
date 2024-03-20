#include <GLFW/glfw3.h>
#include <list>
#include <iostream>
#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui_impl_glfw.h"
#include "lib/imgui/imgui_impl_opengl3.h"
#include <cmath>

bool validateWindowInitialize(std::list<GLFWwindow*>& windows){
    for (GLFWwindow* window : windows){
        if(!window){
            glfwTerminate();
            return false;
        }
    }
    return true;
}

void MakeContextCurrent(std::list<GLFWwindow*>& windows){
    for (GLFWwindow* window : windows){
        glfwMakeContextCurrent(window);
    }
}


class TextArea {
private:
    char textTitle[1024];
    char textBuffer[1024];
    ImVec2 pos;
    ImVec2 size;

public:
    TextArea(){
        // Inicializa o buffer de texto
        this->pos = 0;
        this->size = 0;
        textBuffer[0] = '\0'; // Coloca o caractere nulo no início para indicar uma string vazia
        textTitle[0] = '\0';

        
    }

    void Draw(const ImVec2& p, const ImVec2& s){
        this->pos = p;
        this->size = s;
        // Define a posição e tamanho da próxima janela que será criada
        ImGui::SetNextWindowPos(ImVec2((pos.x - size.x) * 0.5f, 0));
        ImGui::SetNextWindowSize(size);
        
        // Cria a janela de texto
        ImGui::Begin("Codigo");
        ImGui::InputText("##Title",textTitle, IM_ARRAYSIZE(textTitle));
        // Cria o campo de texto multiline do ImGui
        ImGui::InputTextMultiline("##textarea", textBuffer, IM_ARRAYSIZE(textBuffer), ImVec2(size.x,size.y-90));
        ImGui::Button("Salve",ImVec2(size.x * 0.25, 20));
        ImGui::End();
    }

    const char* GetText() const {
        return textBuffer;
    }
};


// void TextArea(ImVec2 pos, ImVec2 size){
//     char textBuffer[1024] = { 0 };
//     ImGui::SetNextWindowPos(ImVec2((pos.x - size.x) * 0.5f, 0));
//     ImGui::SetNextWindowSize(size);
//     ImGui::Begin("codigo");
//     ImGui::InputTextMultiline("##textarea", textBuffer, IM_ARRAYSIZE(textBuffer),
//     ImVec2(size));

//     ImGui::End();
// }

int main(void) {
    std::list<GLFWwindow*> windows;
    /* Initialize the library */
    if (!glfwInit()) return -1;


            
    // Decide GL+GLSL versions
    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    #elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    #else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif



   


    /* Create windows and add them to the list */
    GLFWwindow* window1 = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    GLFWwindow* window2 = glfwCreateWindow(1280, 720, "Hello World2", NULL, NULL);



    windows.push_back(window1);
    windows.push_back(window2);

    /* Validate window creation */
    if (!validateWindowInitialize(windows)) return -1;

    /* Make the windows' contexts current */
    MakeContextCurrent(windows);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window2, true);
    #ifdef __EMSCRIPTEN__
        ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
    #endif
        ImGui_ImplOpenGL3_Init(glsl_version);


    
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    TextArea textArea();
    /* Loop until all windows are closed */
    while (!windows.empty()) {
        for (auto it = windows.begin(); it != windows.end(); ) {
            GLFWwindow* window = *it;
            if (glfwWindowShouldClose(window)) {
                glfwDestroyWindow(window);
                
                it = windows.erase(it);
            } else {

                // glfwMakeContextCurrent(window);
                glfwPollEvents();

                if(window == window2){
                    int display_w, display_h;
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplGlfw_NewFrame();
                    ImGui::NewFrame();
                    
                    textArea.Draw(ImVec2(display_w, display_h), ImVec2(800,display_h));
                    
                    
                    // Rendering
                    ImGui::Render();
                    
                    glfwGetFramebufferSize(window, &display_w, &display_h);
                    glViewport(0, 0, display_w, display_h);
                    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
                    glClear(GL_COLOR_BUFFER_BIT);
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                    glfwSwapBuffers(window);
                }else{
                    glClear(GL_COLOR_BUFFER_BIT);
                    glfwSwapBuffers(window);
                }

                ++it;
                }

            }
        }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}