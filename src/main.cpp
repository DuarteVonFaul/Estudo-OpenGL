#include <GLFW/glfw3.h>
#include <list>
#include <string>
#include <iostream>
#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui_impl_glfw.h"
#include "lib/imgui/imgui_impl_opengl3.h"
#include <cmath>

// Definição de um tipo de ponteiro para função que recebe e retorna um inteiro

struct Event{

};

struct arender{

};



typedef void (*FuncaoCallback)(Event, arender);

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


// Classe base abstrata Control
class Control {  
    public:
        bool frame = false;
        Control(bool r) : frame(r){};
        virtual void Draw() = 0; // Método virtual puro
        virtual void Draw(ImVec2 p, ImVec2 s) = 0;
};

// Classe ImGuiWindowManager que implementa a interface Control
class ImGuiWindowManager{
    private:
        std::list<Control*> items;
        ImVec2 pos;
        ImVec2 size;
        const char* title;

    public:

        ImGuiWindowManager(const char* title){
            this->title = title;
            this->pos = ImVec2(0,0);
            this->size = ImVec2(0,0);
        }

        void add(Control* item) {
            this->items.push_back(item);
        }

        void Draw(ImVec2 p, ImVec2 s) {
            this->pos = p;
            this->size = s;
            bool aqui = true;
            ImGui::SetNextWindowPos(pos);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin(title,nullptr,ImGuiWindowFlags_NoResize);
            for (Control* item : this->items) {
                if(item->frame){
                    item->Draw(this->pos, this->size);
                }else{
                    item->Draw();
                }
                 // Chama o método Draw de cada item
            }
            ImGui::End();
        }

};

// Classe TextArea que implementa a interface Control
class TextArea : public Control {
private:
    char textTitle[100];
    char textBuffer[16512];
    ImVec2 pos;
    ImVec2 size;

public:
    TextArea(const char* title, ImVec2 p, ImVec2 s,bool fr = false) : pos(p), size(s), Control(fr) {
        strcpy(textBuffer, "");; // Inicializa o buffer de texto vazio
        strcpy(textTitle, title);
    }

    void Draw() override {
        // size_t sizeBuffer = textBuffer.size() + IM_ARRAYSIZE(textTitle);
        ImGui::InputTextMultiline(textTitle, textBuffer, + IM_ARRAYSIZE(textBuffer), ImVec2(size.x, size.y));
    }

    void Draw(ImVec2 p, ImVec2 s) override {
        // size_t sizeBuffer = textBuffer.size() + IM_ARRAYSIZE(textTitle);
        ImGui::InputTextMultiline(textTitle, textBuffer, + IM_ARRAYSIZE(textBuffer), ImVec2(s.x * size.x, s.y * size.y));
    }

    const char* GetText() const {
        return textBuffer;
    }

    // void setText(const char text) {
    //     textBuffer = text;
    // }
};

// Classe Button que implementa a interface Control
class Button : public Control {
private:
    const char* text;
    ImVec2 size;
    bool onClick = false;

public:
    Button(const char* t, ImVec2 s, bool fr = false) : text(t), size(s), Control(fr)  {};

    void Draw() override {
        this->onClick = false;
        if(ImGui::Button(text, size)){
            this->onClick = true;
        }
    }

    void Draw(ImVec2 p, ImVec2 s){
        if(ImGui::Button(text, s)){
            this->onClick = true;
        }
    }

    bool is_pressed()
    {
       return this->onClick;
    }

};



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
    TextArea textArea("Sem nome",ImVec2(0, 0), ImVec2(1,0.9f),true);
    Button button("Salvar",ImVec2(60,30),false);
    ImGuiWindowManager codigo("Sem nome");
    ImGuiWindowManager Pasta("Arquivos");
    ImGuiWindowManager Items("Explorar");
    codigo.add(&textArea);
    codigo.add(&button);
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

                    Items.Draw(ImVec2(0,0), ImVec2(display_w *0.25,display_h*0.5));
                    Pasta.Draw(ImVec2(0,display_h*0.5), ImVec2(display_w *0.25,display_h*0.5));
                    codigo.Draw(ImVec2((display_w *0.5) * 0.5f, 0), ImVec2(display_w *0.5,display_h));
                    if(button.is_pressed()){
                        
                    }
                    
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