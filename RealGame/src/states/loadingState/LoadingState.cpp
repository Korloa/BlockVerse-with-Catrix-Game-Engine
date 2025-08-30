/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "states/loadingState/LoadingState.h"
#include <cmath>
LoadingState::LoadingState():visible(false),progress(0.0f),message("Loading..."),cubeShader(nullptr)
                            ,rotateAngle(0.0f),rotateSpeed(100.0f),cubeVAO(0),cubeEBO(0),cubeVBO(0){
    cubeVertices = {
        // (-Z)
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
        //  (+Z)
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        //  (-X)
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
        // (+X)
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         // (-Y)
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
          1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
         -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
         // (+Y)
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
          1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
          1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f
    };

    cubeIndices = {
        0,1,2, 0,2,3,
        4,6,5, 4,7,6,
        8,9,10, 8,10,11,
        12,14,13, 12,15,14,
        16,17,18, 16,18,19,
        20,22,21, 20,23,22
    };
	console.info("Loading state installed.");
}

LoadingState::~LoadingState() {
	console.info("Loading state uninstalled.");
}



void LoadingState::update(GameController* game, float deltaTime) {
	if (!visible)
		return;
    sliderOffset += 300.0f * deltaTime;
	rotateAngle += rotateSpeed*deltaTime;
	if (rotateAngle >= 100000000.0f)
		rotateAngle -= 100000000.0f;
}

void LoadingState::render(GameController* game) {
	if (!visible)
		return;

    glClearColor(1.0f, 1.0f, 1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetWindowSize(game->getWindowInstance(), &width, &height);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);

    cubeShader->use();

    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        (float)1920.0f / (float)1080.0f,
        0.1f,
        100.0f
    );
    cubeShader->setMat4("projection", projection);

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 30.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    cubeShader->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotateAngle), glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    cubeShader->setMat4("model", model);

    cubeShader->setVec3("lightPos", glm::vec3(2.0f, 2.0f, 2.0f));
    cubeShader->setVec3("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));
    cubeShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);  
    glBindVertexArray(0);


    glDisable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);

    //glm::mat4 ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    //textRender->setProjection(ortho);
    //rectRender->setProjection(ortho);
    float barX = 1920.0f/2 - progressWidth * 0.5f;
    game->rectRender->renderRect({ barX, progressY }, { progressWidth, progressHeight}, { 0.2f, 0.2f, 0.2f });

    if (progress >= 0.0f) {
        float fillWidth = progressWidth * progress;
        game->rectRender->renderRect({ barX, progressY }, { fillWidth, progressHeight }, {0.0f, 0.5f, 1.0f});
    }
    else {
        const float period = progressWidth + slidingButtonWidth;

        // 使用 fmodf 保证平滑循环
        float pos = fmodf(sliderOffset, period);
        if (pos < 0) pos += period;

        // 画两个滑块：一个主，一个在左边周期位置
        float mainX = barX - slidingButtonWidth + pos;
        float prevX = mainX - period;

        // 只要滑块与进度条区域有交集就绘制
        auto drawIfVisible = [&](float x) {
            if (x + slidingButtonWidth > barX && x < barX + progressWidth) {
                float drawX = std::max(x, barX);
                float drawW = std::min(x + slidingButtonWidth, barX + progressWidth) - drawX;
                if (drawW > 0) {
                    game->rectRender->renderRect({ drawX, progressY }, { drawW, progressHeight }, { 0.0f, 0.7f, 1.0f });
                }
            }
            };

        drawIfVisible(mainX);
        drawIfVisible(prevX);
    }
    float textWidth = game->textRender->getTextWidth(message,1.0f);
        game->textRender->renderText(message,
        1920.0f/2 - textWidth * 0.5f,
        messageY,
        1.0f,
        glm::vec3(0.4f),true);
}

//API

bool LoadingState::enter(GameController* game) {
	visible = true;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size()*sizeof(float), &cubeVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size()*sizeof(unsigned int), &cubeIndices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    cubeShader = new Shader("render/vertex.glsl", "render/fragment.glsl");
 
    if (!game->rectRender->initialize() || !game->textRender->initialize("font/pixelFont.ttf", 30)) {
        console.error("Fatal Error:cound not initialize rectRender in the LoadingState.");
        game->kill();
        return false;
    }

    return true;
}

bool LoadingState::exit(GameController* game) {
	visible = false;
    return true;
}

void LoadingState::setProgress(float num) {
	progress = num;
}

void LoadingState::setText(const std::string& msg) {
	message = msg;
}

