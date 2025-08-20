#include "render/text/TextRender.h"
#include "render/Shader.h"
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
			static_cast<unsigned int>(face->glyph->advance.x)
		};
	}
	//解绑
	glBindTexture(GL_TEXTURE_2D, 0);

	textShader = new Shader("text_vertex.glsl", "text_fragment.glsl");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}