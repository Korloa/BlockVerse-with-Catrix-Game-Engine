/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: Game.cpp
 * Description: The detailed logic of the game, this may unit various states of the game.
 */
#include "Game.h"
#include <iostream>

 // 全局访问，用于回调函数
Game* gameInstance = nullptr;

Game::Game() : gameWindow(nullptr), shader(nullptr), camera(nullptr), world(nullptr),
textRender(nullptr), rectRender(nullptr), testButton(nullptr),
deltaTime(0.0f), lastFrame(0.0f), firstMouse(true), lastX(400), lastY(300) {
    gameInstance = this;
}

Game::~Game() {
    cleanup();
}

bool Game::initialize() {
    /// TEMP CODE
    int tempSeed;
    console.info("Please input the seed");
    std::cin >> tempSeed;
    ///

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    gameWindow = glfwCreateWindow(1024, 768, "MyCraft Game Window", NULL, NULL);
    if (gameWindow == NULL) {
        console.info("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(gameWindow);

    glfwSetFramebufferSizeCallback(gameWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(gameWindow, mouse_callback);
    glfwSetScrollCallback(gameWindow, scroll_callback);
    glfwSetMouseButtonCallback(gameWindow, mouse_button_callback); // 设置鼠标点击回调

    glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 改为正常光标以支持 UI

    if (glewInit() != GLEW_OK) {
        console.info("Failed to initialize GLEW");
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    shader = new Shader("render/vertex.glsl", "render/fragment.glsl");
    camera = new Camera(glm::vec3(0.0f, (float)horzion, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
    world = new World(tempSeed);

    // 初始化 TextRenderer 和 RectRenderer
    textRender = new TextRender();
    if (!textRender->initialize("font/pixelFont.ttf", 24)) {
        console.info("Failed to initialize TextRenderer");
        return false;
    }
    rectRender = new RectRender();
    if (!rectRender->initialize()) {
        console.info("Failed to initialize RectRenderer");
        return false;
    }

    // 初始化测试按钮
    float centerX = 1024.0f / 2.0f - 150.0f; // 按钮宽度 300
    testButton = new Button({ centerX, 400.0f }, { 300.0f, 50.0f }, "Test Button",
        [this](Game* game) {
            console.info("Test Button clicked!");
        });

    return true;
}

void Game::run() {
    console.info("Starting the gameLoop...");
    while (!glfwWindowShouldClose(gameWindow)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput();
        update();
        render();
        glfwSwapBuffers(gameWindow);
        glfwPollEvents();
    }
}

void Game::cleanup() {
    console.info("Deleting the sources...");
    delete testButton;
    delete rectRender;
    delete textRender;
    delete world;
    delete camera;
    delete shader;
    glfwTerminate();
}

void Game::processInput() {
    if (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(gameWindow, true);
    if (glfwGetKey(gameWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera->procKey(0, deltaTime);
    if (glfwGetKey(gameWindow, GLFW_KEY_S) == GLFW_PRESS)
        camera->procKey(1, deltaTime);
    if (glfwGetKey(gameWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera->procKey(2, deltaTime);
    if (glfwGetKey(gameWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera->procKey(3, deltaTime);
    if (glfwGetKey(gameWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->procKey(4, deltaTime);
    if (glfwGetKey(gameWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->procKey(5, deltaTime);
}

void Game::update() {
    // update logic
}

void Game::render() {
    // 渲染 3D 场景
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST); // 确保 3D 场景使用深度测试

    shader->use();
    glm::mat4 projection = glm::perspective(glm::radians(camera->cameraZoom), 1024.0f / 768.0f, 0.1f, 1000.0f);
    shader->setMat4("projection", projection);
    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);
    shader->setVec3("lightPos", glm::vec3(0.0f, 256.0f, 0.0f));
    shader->setVec3("viewPos", camera->cameraPosition);
    shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    world->render(*shader);

    // 渲染 2D UI
    glDisable(GL_DEPTH_TEST); // 禁用深度测试以绘制 2D
    double mouseX, mouseY;
    glfwGetCursorPos(gameWindow, &mouseX, &mouseY);
    mouseY = 768.0 - mouseY; // 翻转 Y 坐标
    testButton->render(this, textRender, rectRender, mouseX, mouseY);
    glEnable(GL_DEPTH_TEST); // 恢复深度测试
}

void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (gameInstance->firstMouse) {
        gameInstance->firstMouse = false;
        gameInstance->lastX = xpos;
        gameInstance->lastY = ypos;
    }
    else {
        float xoffset = xpos - gameInstance->lastX;
        float yoffset = ypos - gameInstance->lastY;
        gameInstance->camera->procMouseMove(xoffset, yoffset);
        gameInstance->lastX = xpos;
        gameInstance->lastY = ypos;
    }
}

void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    gameInstance->camera->procMouseScroll(yoffset);
}

void Game::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        mouseY = 768.0 - mouseY; // 翻转 Y 坐标
        if (gameInstance->testButton->isHovered(mouseX, mouseY)) {
            gameInstance->testButton->onClick(gameInstance);
        }
    }
}