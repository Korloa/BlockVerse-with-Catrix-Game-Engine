#include"test/Test.h"

#include<iostream>
#include<ft2build.h>

#include FT_FREETYPE_H

Test::Test() {
    FT_Library library;
    if (FT_Init_FreeType(&library)) {
        std::cerr << "无法初始化 FreeType" << std::endl;
        exit(0);
    }

    FT_Face face;
    if (FT_New_Face(library, "font/pixelFont.ttf", 0, &face)) {
        std::cerr << "无法加载字体文件！请确认路径正确" << std::endl;
        exit(0);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    std::cout << "FreeType 初始化成功！字体 '" << face->family_name << "' 已加载。" << std::endl;

    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

Test::~Test() {
    exit(0);
}