#include "render/text/TextRender.h"
#include "render/Shader.h"
#include "debug/Console.h"
//#include <utf8.h> 暂时不加入解析中文字符，否则字节对齐会
//很麻烦
//#include <stdexcept>
//之后要实现动态加入字体

TextRender::TextRender():ft(nullptr),face(nullptr),textShader(nullptr),vao(0),vbo(0){}
TextRender::~TextRender() {
	for (auto& c : characters)
		glDeleteTextures(1, &c.second.textureID);
	// map ,unorderedmap set unorderedset list deque在内存中存储不是连续的
	// vector,array,t[],string都是连续存储的
	characters.clear();
	if (face)
		FT_Done_Face(face);
	if (ft)
		FT_Done_FreeType(ft);
	if (textShader)
		delete textShader;
	if (vao)
		glDeleteVertexArrays(1, &vao);
	if (vbo)
		glDeleteBuffers(1, &vbo);
}

bool TextRender::initialize(const std::string& fontPath, unsigned int fontSize) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//const 防止修改 &可以防止拷贝 提高性能 const A& > A
	// ft库成功返回0，不成功返回非0
	if (FT_Init_FreeType(&ft)) {
		console.error("Failed to initialize FreeType textrender.");
	}
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)){
		FT_Done_FreeType(ft);
		console.error("Failed to load the font.");
	}
	// what about char size??
	// 0 代表自动计算
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	//gl_unpack_alignment
	//这并不会影响效率，因为OpenGL是按快读取的
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//ASCII
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			continue;
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//? 一个状态机，绑定后的所有设置都作用于texture
		glTexImage2D(
			GL_TEXTURE_2D,				//纹理类型
			0,							//纹理级别
			GL_RED,						//数据的占用通道
			face->glyph->bitmap.width,	//纹理宽度
			face->glyph->bitmap.rows,	//纹理高度
			0,							//是否包含边框 0为没有 现代OpenGL必须为0
			GL_RED,						//GPU存储规则
			GL_UNSIGNED_BYTE,			//位图数据格式
			face->glyph->bitmap.buffer	//位图数据类型
		);
		//设置纹理参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//定义纹理坐标超出时要如何处理
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//纹理被缩小时
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//纹理被放大时

		characters[c] = {
			texture,
			glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left,face->glyph->bitmap_top),
			(GLuint)(face->glyph->advance.x)
		};
	}
	//解绑
	glBindTexture(GL_TEXTURE_2D, 0);

	textShader = new Shader("render/text_vertex.glsl", "render/text_fragment.glsl");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);


	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}


// core hard part need to understand soon.
float TextRender::renderText(const std::string& text, float x, float y, float scale, glm::vec3 color, bool render) {
	textShader->use();
	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 0.0f, 768.0f);
	textShader->setMat4("projection", projection);
	textShader->setVec3("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	float width = 0.0f;
	for (unsigned char c : text){
		if (characters.find(c) == characters.end()) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			characters[c] = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				(GLuint)(face->glyph->advance.x)
			};
		}
		Character ch = characters[c];
		width += (ch.advance >> 6) * scale;
		if (render) {
			float xpos = x + ch.bearing.x * scale;
			float ypos = y - (ch.size.y - ch.bearing.y) * scale;
			float w = ch.size.x * scale;
			float h = ch.size.y * scale;
			

			glBindTexture(GL_TEXTURE_2D, ch.textureID);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);



			//GL_TRIANGEL_FAN MODE

			float vertices[] = {
				xpos,     ypos + h, 0.0f, 0.0f,
				xpos,     ypos,     0.0f, 1.0f,
				xpos + w, ypos,     1.0f, 1.0f,
				xpos + w, ypos + h, 1.0f, 0.0f
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);




			//GL TRIANGLES MODE
		//	GLfloat vertices[6][4] = {
		//{ xpos,           ypos + h,   0.0, 0.0 },
		//{ xpos,           ypos,                0.0, 1.0 },
		//{ xpos + w, ypos,                1.0, 1.0 },
		//{ xpos,           ypos + h,   0.0, 0.0 },
		//{ xpos + w, ypos,                1.0, 1.0 },
		//{ xpos + w, ypos + h,   1.0, 0.0 }
		//	};
		//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);




			x += (ch.advance >> 6) * scale;
		}
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return width;
}