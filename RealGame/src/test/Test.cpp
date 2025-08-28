//// main.cpp
//#include <GL/glew.h>           // ���� GLEW
//#include <GLFW/glfw3.h>
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"


#include"test/Test.h"

#include<iostream>
#include<ft2build.h>

#include FT_FREETYPE_H

Test::Test() {
    //FT_Library library;
    //if (FT_Init_FreeType(&library)) {
    //    std::cerr << "�޷���ʼ�� FreeType" << std::endl;
    //    exit(0);
    //}

    //FT_Face face;
    //if (FT_New_Face(library, "font/pixelFont.ttf", 0, &face)) {
    //    std::cerr << "�޷����������ļ�����ȷ��·����ȷ" << std::endl;
    //    exit(0);
    //}

    //FT_Set_Pixel_Sizes(face, 0, 48);
    //std::cout << "FreeType ��ʼ���ɹ������� '" << face->family_name << "' �Ѽ��ء�" << std::endl;

    //FT_Done_Face(face);
    //FT_Done_FreeType(library);

    // // ��ʼ�� GLFW
    //glfwInit();
    //// ���� OpenGL �汾������ 3.3��
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui with GLEW", nullptr, nullptr);
    //if (!window) {
    //    fprintf(stderr, "Failed to create GLFW window\n");
    //    glfwTerminate();
    //    return;
    //}
    //glfwMakeContextCurrent(window);

    //// ��ʼ�� GLEW
    //GLenum err = glewInit();
    //if (err != GLEW_OK) {
    //    fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
    //    return;
    //}

    //// ��� OpenGL �汾
    //printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    //// Setup Dear ImGui context
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    //(void)io;
    //ImGui::StyleColorsDark();

    //// Setup Platform/Renderer bindings
    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    //ImGui_ImplOpenGL3_Init("#version 130");  // �� "#version 150" ������� OpenGL 3.3+

    //// ��ѭ��
    //while (!glfwWindowShouldClose(window)) {
    //    glfwPollEvents();

    //    // ��ʼ��֡
    //    ImGui_ImplOpenGL3_NewFrame();
    //    ImGui_ImplGlfw_NewFrame();
    //    ImGui::NewFrame();

    //    // Dear ImGui UI
    //    ImGui::Begin("Hello GLEW");
    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    //    if (ImGui::Button("Click Me")) {
    //        printf("Button clicked!\n");
    //    }
    //    ImGui::End();

    //    // ��Ⱦ
    //    ImGui::Render();
    //    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    //    glClear(GL_COLOR_BUFFER_BIT);
    //    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //    glfwSwapBuffers(window);
    //}

    //// ����
    //ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();

    //glfwTerminate();
}

Test::~Test() {
    exit(0);
}
